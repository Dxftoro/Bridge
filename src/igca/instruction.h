#pragma once
#include <stdint.h>

#include "enums/opcode.h"
#include "enums/register.h"

namespace igca {

	template <typename T>
	struct RegValue {
		Register reg;
		T value;
	};

	template <typename T>
	struct Value {
		T value;
	};

	struct RegReg {
		Register destination;
		Register source;
	};

	template <typename T>
	struct Instruction {
	public:
		Opcode opcode;

		union Args {
			RegValue<T> rv;
			Value<T> v;
			RegReg rr;
		} args;

		Instruction(Opcode _opcode, Register reg, T value) : opcode(_opcode) {
			args.rv.reg = reg;
			args.rv.value = value;
		}

		Instruction(Opcode _opcode, T value) : opcode(_opcode) {
			args.v.value = value;
		}

		Instruction(Opcode _opcode, Register destination, Register source) : opcode(_opcode) {
			args.rr.destination = destination;
			args.rr.source = source;
		}
	};

	using Instruction32 = Instruction<uint32_t>;
	using Instruction64 = Instruction<uint64_t>;

}