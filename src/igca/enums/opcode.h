#pragma once

namespace igca {

	enum class Opcode : unsigned char {
		ADDRR, ADDRV,
		CMPRR, CMPRV,
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