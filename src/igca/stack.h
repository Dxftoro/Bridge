#pragma once
#include <stdint.h>
#include <assert.h>

namespace igca {

	template <typename T>
	class Stack {
	private:
		T* buffer;
		size_t current;
		size_t stackSize;

		size_t count(size_t bytes) const {
			return bytes / sizeof(T);
		}

	public:
		Stack(size_t sizeBytes) : current(0), stackSize(count(sizeBytes)) {
			assert(stackSize > 0 && stackSize % sizeof(T) == 0 && "Invalid stack size given!");
			buffer = new T[stackSize];
		}

		~Stack() {
			delete buffer;
		}

		void push(T value) {
			assert(current < stackSize && "Stack overflow!");
			buffer[current] = value;
			current++;
		}

		void pop() { current--; }

		T top() const { return buffer[current]; }
	};

	using Stack32 = Stack<uint32_t>;
	using Stack64 = Stack<uint64_t>;

}