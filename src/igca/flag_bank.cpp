#include "flag_bank.h"
#include "enums/enum_size.h"

namespace igca {

	void FlagBank::store(FlagRegister reg, bool value) {
		uint32_t mask = 1 << index(reg);
		flags = (flags & ~mask) | (-value & mask);
	}

	bool FlagBank::load(FlagRegister reg) const {
		return static_cast<bool>(flags & (1 << index(reg)));
	}

	void FlagBank::reset() { flags = 0; }

}