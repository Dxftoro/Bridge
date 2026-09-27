#pragma once

namespace igca {

	template <typename T>
	constexpr size_t index(T value) { return static_cast<size_t>(value); }

	template <typename T>
	constexpr T member(size_t index) { return static_cast<T>(index); }

	template <typename T>
	consteval size_t length() { return static_cast<size_t>(T::NONE); }

}