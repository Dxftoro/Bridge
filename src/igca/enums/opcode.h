#pragma once

namespace igca {

	enum class Opcode : unsigned char {
		ADDRR, ADDRV,
		DIVRR, DIVRV,
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