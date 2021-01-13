#pragma once
#include "../../utilities/csgo.hpp"

namespace console {
	void initialize(const char* title);
	void release();

	template <typename ... Args>
	void log(char const* const format, Args const& ... args) {

		printf(format, args ...);

	}

}