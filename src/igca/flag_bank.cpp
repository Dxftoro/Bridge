#include "flag_bank.h"
#include "enums/enum_size.h"

namespace igca {

	void FlagBank::store(FlagRegister reg, bool value) {
		flags |= static_cast<uint32_t>(value) << index(reg);
	}

	bool FlagBank::load(FlagRegister reg) const {
		return flags & (1 << index(reg));
	}

	void FlagBank::reset() { flags = 0; }

}