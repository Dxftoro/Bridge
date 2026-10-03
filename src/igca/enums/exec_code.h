#pragma once

namespace igca {

	enum class ExecCode : int {
		SUCCESS,
		DIVISION_BY_ZERO,
		NONE
	};

	constexpr const char* execCodeToString(ExecCode execCode) {
		switch (execCode) {
		case ExecCode::SUCCESS: return "SUCCESS";
		case ExecCode::DIVISION_BY_ZERO: return "DIVISION_BY_ZERO";
		case ExecCode::NONE: return "NONE (fix it)";
		default: return "UNKNOWN (fix it)";
		}
	}

}