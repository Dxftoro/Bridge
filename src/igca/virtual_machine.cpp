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
		};

		handlers[index(Opcode::ADDRV)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			uint32_t result = registers.load(instr->args.rv.reg) + instr->args.rv.value;
			registers.store(instr->args.rv.reg, result);
		};

		handlers[index(Opcode::DIVRR)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			uint32_t source = registers.load(instr->args.rr.source);

			if (!source) {
				vm->getFlagBank().store(FlagRegister::FZD, 1);
				return;
			}

			uint32_t result = registers.load(instr->args.rr.destination) / source;
			registers.store(instr->args.rr.destination, result);
		};

		handlers[index(Opcode::DIVRV)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			uint32_t result = registers.load(instr->args.rv.reg) / instr->args.rv.value;
			registers.store(instr->args.rv.reg, result);
		};

		handlers[index(Opcode::MODRR)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			uint32_t result = registers.load(instr->args.rr.destination) % registers.load(instr->args.rr.source);
			registers.store(instr->args.rr.destination, result);
		};

		handlers[index(Opcode::MODRV)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			uint32_t result = registers.load(instr->args.rv.reg) % instr->args.rv.value;
			registers.store(instr->args.rv.reg, result);
		};

		handlers[index(Opcode::MULRR)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			uint32_t result = registers.load(instr->args.rr.destination) * registers.load(instr->args.rr.source);
			registers.store(instr->args.rr.destination, result);
		};

		handlers[index(Opcode::MULRV)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			uint32_t result = registers.load(instr->args.rv.reg) * instr->args.rv.value;
			registers.store(instr->args.rv.reg, result);
		};

		handlers[index(Opcode::MOVRR)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			registers.store(instr->args.rr.destination, registers.load(instr->args.rr.source));
		};

		handlers[index(Opcode::MOVRV)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			registers.store(instr->args.rv.reg, instr->args.rv.value);
		};

		handlers[index(Opcode::PINW)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			uint32_t result = registers.load(instr->args.rr.destination);
			std::println("PIN: {}", result);
		};

		handlers[index(Opcode::SUBRR)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			uint32_t result = registers.load(instr->args.rr.destination) - registers.load(instr->args.rr.source);
			registers.store(instr->args.rr.destination, result);
		};

		handlers[index(Opcode::SUBRV)] = [](VirtualMachine* vm, Instruction32* instr) {
			RegisterBank32& registers = vm->getRegisterBank();
			uint32_t result = registers.load(instr->args.rv.reg) - instr->args.rv.value;
			registers.store(instr->args.rv.reg, result);
		};
	}

	ExecCodes VirtualMachine::execute(Instruction32* instructions, size_t count) {
		for (size_t i = 0; i < count; i++) {
			handlers[index(instructions[i].opcode)](this, &instructions[i]);
			if (flags.load(FlagRegister::FZD)) return ExecCodes::DIVISION_BY_ZERO;
		}

		return ExecCodes::SUCCESS;
	}

}