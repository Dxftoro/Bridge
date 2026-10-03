#pragma once
#include "flag_bank.h"
#include "register_bank.h"
#include "stack.h"
#include "instruction.h"
#include "enums/exec_code.h"
#include "util/pair.h"

namespace igca {

	class VirtualMachine {
		using Handler = ExecCode(*)(VirtualMachine*, Instruction32*);
	private:
		RegisterBank32 registers;
		FlagBank flags;
		Stack32 stack;
		Handler handlers[length<Opcode>()];

		void setupHandlers();

	public:
		VirtualMachine(size_t stackSize);

		ExecCode execute(Instruction32* instructions, uint32_t count);

		RegisterBank32& getRegisterBank() { return registers; }
		FlagBank& getFlagBank() { return flags; }
		Stack32& getStack() { return stack; }
	};

}