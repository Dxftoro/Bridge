#pragma once
#include <stdint.h>

#include "enums/register.h"
#include "enums/enum_size.h"
#include "util/memory.h"

namespace igca {

	template <typename T>
	class RegisterBank {
	private:
		T buffer[length<Register>()];

	public:
		RegisterBank() {
			reset();
		}

		void store(Register reg, T value) { buffer[index(reg)] = value; }
		T load(Register reg) const { return buffer[index(reg)]; }
		void reset() { igca::memset(buffer, length<Register>(), 0U); }
	};

	using RegisterBank32 = RegisterBank<uint32_t>;
	using RegisterBank64 = RegisterBank<uint64_t>;

}