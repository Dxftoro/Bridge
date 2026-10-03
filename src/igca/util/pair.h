#pragma once

namespace igca {

	template <typename A, typename B>
	class Pair {
	private:
		A first;
		B second;

	public:
		constexpr Pair(A _first, B _second) : first(_first), second(_second) {}
		constexpr Pair(const Pair& other) : first(other.first), second(other.second) {}

		A& getFirst() { return first; }
		B& getSecond() { return second; }

		void setFirst(const A& first) { this->first = first; }
		void setSecond(const B& second) { this->second = second; }
	};

}