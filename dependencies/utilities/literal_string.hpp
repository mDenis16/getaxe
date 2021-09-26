#pragma once

template<unsigned N>
struct LiteralString {

	unsigned size;
	char buf[N + 1]{};
	constexpr LiteralString(char const* s) {
		size = N;

		for (size_t i = 0; i < N; ++i)
			buf[i] = s[i];



	}
	constexpr operator char const* () const { return buf; }
};
template<unsigned N> LiteralString(char const (&)[N])->LiteralString<N - 1>;