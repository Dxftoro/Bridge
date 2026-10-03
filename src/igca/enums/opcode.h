#pragma once

namespace igca {

	enum class Opcode : unsigned char {
		ADDRR, ADDRV,
		CMPEQRR, CMPEQRV,	// a == b
		CMPLRR, CMPLRV,		// a < b
		CMPLERR, CMPLERV,	// a <= b
		DIVRR, DIVRV,
		JE, JG, JGE, JL, JLE, JNE, JMP, JZ,
		MODRR, MODRV,
		MOVRR, MOVRV,
		MULRR, MULRV,
		PINL, PINR, PINU, PINW,
		POP,
		PUSHR, PUSHV,
		SUBRR, SUBRV,
		NONE
	};

}