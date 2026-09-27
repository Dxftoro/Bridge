#pragma once

namespace igca {

	void memcpy(char* destination, char* source, size_t bytes);

	template <typename T>
	void memset(T* destination, size_t size, T value) {
		for (size_t i = 0; i < size; i++) {
			destination[i] = value;
		}
	}

	template <typename T>
	void fill(T* destination, T* source, size_t size) {
		memcpy(static_cast<char*>(destination), static_cast<char*>(source), size * sizeof(T));
	}

}