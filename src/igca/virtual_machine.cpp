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

		/* ==================== (REMOVE PLS) Pin instructions ==================== */
		handlers[index(Opcode::PINW)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			uint32_t result = registers.load(instr->args.rr.destination);
			std::println("PIN: {}", result);
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

}