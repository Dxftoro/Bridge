#pragma once
#include <stdint.h>

#include "enums/flag_register.h"

namespace igca {
	
	class FlagBank {
	private:
		uint32_t flags;

	public:
		FlagBank() : flags(0) {}

		void store(FlagRegister reg, bool value);
		bool load(FlagRegister reg) const;
		void reset();
	};

}