#pragma once
#include "flag_bank.h"
#include "register_bank.h"
#include "stack.h"
#include "instruction.h"

namespace igca {

	class VirtualMachine {
		using Handler = void(*)(VirtualMachine*, Instruction32*);
	private:
		RegisterBank32 registers;
		FlagBank flags;
		Stack32 stack;
		Handler handlers[length<Opcode>()];

		void setupHandlers();

	public:
		VirtualMachine(size_t stackSize);

		int execute(Instruction32* instructions, size_t count);

		RegisterBank32& getRegisterBank() { return registers; }
		FlagBank& getFlagBank() { return flags; }
		Stack32& getStack() { return stack; }
	};

}