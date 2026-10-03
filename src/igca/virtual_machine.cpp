#include "virtual_machine.h"
#include <print>

namespace igca {

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

		handlers[index(Opcode::PINW)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			uint32_t result = registers.load(instr->args.rr.destination);
			std::println("PIN: {}", result);
			return ExecCode::SUCCESS;
		};

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

		for (; registers.load(Register::RPC) < count; registers.store(Register::RPC, registers.load(Register::RPC) + 1)) {
			uint32_t i = registers.load(Register::RPC);
			ExecCode code = handlers[index(instructions[i].opcode)](this, &instructions[i]);
			if (code != ExecCode::SUCCESS) return code;
		}

		return ExecCode::SUCCESS;
	}

}