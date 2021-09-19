#pragma once

#include <array>
#include <string>
#include <iostream>
#include <fstream>
#include <ostream>
#include <vector>
#include <map>
#include <string_view>
#include "fnv.hpp"


template<unsigned N>
struct FixedString {

	unsigned size;
	char buf[N + 1]{};
	constexpr FixedString(char const* s) {


		size = N;

		for (size_t i = 0; i < N; ++i)
			buf[i] = s[i];// ^ 0x811C9DC5;

		//buf[N] = '';

	}
	constexpr operator char const* () const { return buf; }
};
template<unsigned N> FixedString(char const (&)[N])->FixedString<N - 1>;


class SecureString
{
public:

	std::vector<char> str;
	SecureString(const char* _s, int size) {
		str.resize(size + 1);

		for (size_t i = 0; i < size; ++i)
			str[i] = _s[i];


		//str[size] = '\0';




	}
	const char* data() {
		return this->str.data();
	}

	void encrypt() {


		//for (size_t i = 0; i < str.size() - 1; ++i)
		//	str[i] ^= 0x811C9DC5;

		str[str.size()] = '\0';
	}
	void decrypt() {
		//for (size_t i = 0; i < str.size() - 1; ++i)
			//str[i] ^= 0x811C9DC5;

		str[str.size()] = '\0';
	}

	SecureString() {

	};
	~SecureString() {

	}

private:

};


inline std::map<size_t, SecureString> SECURE_STRINGS;

template<FixedString _STR>
inline constexpr static uint32_t GET_STR_HASH() {

	constexpr auto hash = fnv::hash(_STR);
	SECURE_STRINGS[hash] = SecureString(_STR, _STR.size);
	return hash;
}

#define GET_STR(str) { SECURE_STRINGS[GET_STR_HASH<str>()] }
