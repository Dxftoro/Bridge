#include "virtual_machine.h"
#include <print>

namespace igca {

	void compare(FlagBank& flags, uint32_t a, uint32_t b) {
		uint32_t diff = a - b;
		flags.store(FlagRegister::FZ, diff == 0);
		flags.store(FlagRegister::FN, (diff >> sizeof(diff) - 1) & 1);
		flags.store(FlagRegister::FC, a < b);
		flags.store(FlagRegister::FO, ((a ^ b) & (a & diff) & (1U << sizeof(diff))) != 0);
	}

	VirtualMachine::VirtualMachine(size_t stackSize) : stack(stackSize) {
		setupHandlers();
	}

	void VirtualMachine::setupHandlers() {
		handlers[index(Opcode::ADDRR)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			uint32_t result = registers.load(instr->args.rr.destination) + registers.load(instr->args.rr.source);
			registers.store(instr->args.rr.destination, result);
			return ExecCode::SUCCESS;
		};

		handlers[index(Opcode::ADDRV)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			uint32_t result = registers.load(instr->args.rv.reg) + instr->args.rv.value;
			registers.store(instr->args.rv.reg, result);
			return ExecCode::SUCCESS;
		};

		/* ==================== AND instructions ==================== */
		handlers[index(Opcode::ANDRR)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			uint32_t result = registers.load(instr->args.rr.destination) & registers.load(instr->args.rr.source);
			registers.store(instr->args.rr.destination, result);
			return ExecCode::SUCCESS;
		};

		handlers[index(Opcode::ANDRV)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			uint32_t result = registers.load(instr->args.rv.reg) & instr->args.rv.value;
			registers.store(instr->args.rv.reg, result);
			return ExecCode::SUCCESS;
		};

		/* ==================== Compare instructions ==================== */
		handlers[index(Opcode::CMPRR)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			FlagBank& flags = vm->getFlagBank();

			compare(flags, registers.load(instr->args.rr.destination), registers.load(instr->args.rr.source));

			return ExecCode::SUCCESS;
		};
		
		handlers[index(Opcode::CMPRV)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			FlagBank& flags = vm->getFlagBank();

			compare(flags, registers.load(instr->args.rv.reg), instr->args.rv.value);

			return ExecCode::SUCCESS;
		};

		/* ==================== Division instructions ==================== */
		handlers[index(Opcode::DIVRR)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			uint32_t source = registers.load(instr->args.rr.source);

			if (!source) {
				vm->getFlagBank().store(FlagRegister::FZD, 1);
				return ExecCode::DIVISION_BY_ZERO;
			}

			uint32_t result = registers.load(instr->args.rr.destination) / source;
			registers.store(instr->args.rr.destination, result);

			return ExecCode::SUCCESS;
		};

		handlers[index(Opcode::DIVRV)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			uint32_t result = registers.load(instr->args.rv.reg) / instr->args.rv.value;
			registers.store(instr->args.rv.reg, result);
			return ExecCode::SUCCESS;
		};

		/* ==================== Increment/Decrement ==================== */
		handlers[index(Opcode::DEC)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			registers.store(instr->args.r.reg, registers.load(instr->args.r.reg) - 1);
			return ExecCode::SUCCESS;
		};

		handlers[index(Opcode::INC)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			registers.store(instr->args.r.reg, registers.load(instr->args.r.reg) + 1);
			return ExecCode::SUCCESS;
		};

		/* ==================== Bunny-hop ==================== */
		handlers[index(Opcode::JMP)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			registers.store(Register::RPC, instr->args.v.value);
			return ExecCode::SUCCESS;
		};

		handlers[index(Opcode::JE)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			FlagBank& flags = vm->getFlagBank();

			if (flags.load(FlagRegister::FZ)) registers.store(Register::RPC, instr->args.v.value);
			
			return ExecCode::SUCCESS;
		};

		handlers[index(Opcode::JG)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			FlagBank& flags = vm->getFlagBank();

			if (!flags.load(FlagRegister::FN) && !flags.load(FlagRegister::FZ)) 
				registers.store(Register::RPC, instr->args.v.value);

			return ExecCode::SUCCESS;
		};

		handlers[index(Opcode::JGE)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			FlagBank& flags = vm->getFlagBank();

			if (!flags.load(FlagRegister::FN)) registers.store(Register::RPC, instr->args.v.value);

			return ExecCode::SUCCESS;
		};

		handlers[index(Opcode::JL)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			FlagBank& flags = vm->getFlagBank();

			if (flags.load(FlagRegister::FN) && !flags.load(FlagRegister::FZ)) registers.store(Register::RPC, instr->args.v.value);

			return ExecCode::SUCCESS;
		};

		handlers[index(Opcode::JLE)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			FlagBank& flags = vm->getFlagBank();

			if (flags.load(FlagRegister::FN)) registers.store(Register::RPC, instr->args.v.value);

			return ExecCode::SUCCESS;
		};

		handlers[index(Opcode::JNE)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			FlagBank& flags = vm->getFlagBank();

			if (!flags.load(FlagRegister::FZ)) registers.store(Register::RPC, instr->args.v.value);

			return ExecCode::SUCCESS;
		};

		handlers[index(Opcode::JZ)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			FlagBank& flags = vm->getFlagBank();

			if (flags.load(FlagRegister::FZ)) registers.store(Register::RPC, instr->args.v.value);

			return ExecCode::SUCCESS;
		};

		handlers[index(Opcode::LOOP)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			FlagBank& flags = vm->getFlagBank();

			uint32_t value = registers.load(Register::RCX);
			registers.store(Register::RCX, value - 1);

			if (registers.load(Register::RCX)) registers.store(Register::RPC, instr->args.v.value);

			return ExecCode::SUCCESS;
		};

		/* ==================== Modulo instructions ==================== */
		handlers[index(Opcode::MODRR)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			uint32_t result = registers.load(instr->args.rr.destination) % registers.load(instr->args.rr.source);
			registers.store(instr->args.rr.destination, result);
			return ExecCode::SUCCESS;
		};

		handlers[index(Opcode::MODRV)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			uint32_t result = registers.load(instr->args.rv.reg) % instr->args.rv.value;
			registers.store(instr->args.rv.reg, result);
			return ExecCode::SUCCESS;
		};

		/* ==================== Multiplication instructions ==================== */
		handlers[index(Opcode::MULRR)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			uint32_t result = registers.load(instr->args.rr.destination) * registers.load(instr->args.rr.source);
			registers.store(instr->args.rr.destination, result);
			return ExecCode::SUCCESS;
		};

		handlers[index(Opcode::MULRV)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			uint32_t result = registers.load(instr->args.rv.reg) * instr->args.rv.value;
			registers.store(instr->args.rv.reg, result);
			return ExecCode::SUCCESS;
		};

		/* ==================== Movement instructions ==================== */
		handlers[index(Opcode::MOVRR)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			registers.store(instr->args.rr.destination, registers.load(instr->args.rr.source));
			return ExecCode::SUCCESS;
		};

		handlers[index(Opcode::MOVRV)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			registers.store(instr->args.rv.reg, instr->args.rv.value);
			return ExecCode::SUCCESS;
		};

		/* ==================== OR instructions ==================== */
		handlers[index(Opcode::ORRR)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			uint32_t result = registers.load(instr->args.rr.destination) | registers.load(instr->args.rr.source);
			registers.store(instr->args.rr.destination, result);
			return ExecCode::SUCCESS;
		};

		handlers[index(Opcode::ORRV)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			uint32_t result = registers.load(instr->args.rv.reg) | instr->args.rv.value;
			registers.store(instr->args.rv.reg, result);
			return ExecCode::SUCCESS;
		};

		/* ==================== (REMOVE PLS) Pin instructions ==================== */
		handlers[index(Opcode::PINW)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			uint32_t result = registers.load(instr->args.rr.destination);
			std::println("PIN: {}", result);
			return ExecCode::SUCCESS;
		};

		/* ==================== Pushes ==================== */
		handlers[index(Opcode::PUSHR)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			Stack32& stack = vm->getStack();

			stack.push(registers.load(instr->args.r.reg));

			return ExecCode::SUCCESS;
		};

		handlers[index(Opcode::PUSHV)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			Stack32& stack = vm->getStack();

			stack.push(instr->args.v.value);

			return ExecCode::SUCCESS;
		};

		/* ==================== Pop ==================== */
		handlers[index(Opcode::POP)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			Stack32& stack = vm->getStack();

			registers.store(instr->args.r.reg, stack.top());
			stack.pop();

			return ExecCode::SUCCESS;
		};

		/* ==================== Left shifts ==================== */
		handlers[index(Opcode::SHLRR)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			uint32_t result = registers.load(instr->args.rr.destination) << registers.load(instr->args.rr.source);
			registers.store(instr->args.rr.destination, result);
			return ExecCode::SUCCESS;
		};

		handlers[index(Opcode::SHLRV)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			uint32_t result = registers.load(instr->args.rv.reg) << instr->args.rv.value;
			registers.store(instr->args.rv.reg, result);
			return ExecCode::SUCCESS;
		};

		/* ==================== Right shifts ==================== */
		handlers[index(Opcode::SHLRR)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			uint32_t result = registers.load(instr->args.rr.destination) >> registers.load(instr->args.rr.source);
			registers.store(instr->args.rr.destination, result);
			return ExecCode::SUCCESS;
		};

		handlers[index(Opcode::SHLRV)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			uint32_t result = registers.load(instr->args.rv.reg) >> instr->args.rv.value;
			registers.store(instr->args.rv.reg, result);
			return ExecCode::SUCCESS;
		};

		/* ==================== Subtraction instructions ==================== */
		handlers[index(Opcode::SUBRR)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			uint32_t result = registers.load(instr->args.rr.destination) - registers.load(instr->args.rr.source);
			registers.store(instr->args.rr.destination, result);
			return ExecCode::SUCCESS;
		};

		handlers[index(Opcode::SUBRV)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			uint32_t result = registers.load(instr->args.rv.reg) - instr->args.rv.value;
			registers.store(instr->args.rv.reg, result);
			return ExecCode::SUCCESS;
		};

		/* ==================== XOR instructions ==================== */
		handlers[index(Opcode::XORRR)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			uint32_t result = registers.load(instr->args.rr.destination) ^ registers.load(instr->args.rr.source);
			registers.store(instr->args.rr.destination, result);
			return ExecCode::SUCCESS;
		};

		handlers[index(Opcode::XORRV)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			uint32_t result = registers.load(instr->args.rv.reg) ^ instr->args.rv.value;
			registers.store(instr->args.rv.reg, result);
			return ExecCode::SUCCESS;
		};
	}

	ExecCode VirtualMachine::execute(Instruction32* instructions, uint32_t count) {
		registers.store(Register::RPC, 0);

		for (; registers.load(Register::RPC) < count;) {
			uint32_t i = registers.load(Register::RPC);
			registers.store(Register::RPC, registers.load(Register::RPC) + 1);
			ExecCode code = handlers[index(instructions[i].opcode)](this, &instructions[i]);
			if (code != ExecCode::SUCCESS) return code;
		}

		return ExecCode::SUCCESS;
	}

	void VirtualMachine::reset() {
		registers.reset();
		flags.reset();
		stack.reset();
	}

}