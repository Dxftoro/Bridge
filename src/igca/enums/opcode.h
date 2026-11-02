#pragma once

namespace igca {

	enum class Opcode : unsigned char {
		ADDRR, ADDRV,
		ANDRR, ANDRV,
		CMPRR, CMPRV,
		DEC,
		DIVRR, DIVRV,
		INC,
		JE, JG, JGE, JL, JLE, JNE, JMP, JZ,
		LOOP,
		MODRR, MODRV,
		MOVRR, MOVRV,
		MULRR, MULRV,
		ORRR, ORRV,
		PINW,
		POP,
		PUSHR, PUSHV,
		SHLRR, SHLRV,
		SHRRR, SHRRV,
		SUBRR, SUBRV,
		XORRR, XORRV,
		NONE
	};

}