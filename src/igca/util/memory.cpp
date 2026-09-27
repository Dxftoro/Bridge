#include "memory.h"

namespace igca {

	void memcpy(char* destination, char* source, size_t bytes) {
		for (size_t i = 0; i < bytes; i++) {
			destination[i] = source[i];
		}
	}

}