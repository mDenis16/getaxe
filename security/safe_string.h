//--------------------------------------------------------------------------------
//-- XorCompileTime.hpp
//
// Author: frk
// Date: 12.12.2015
//
//--------------------------------------------------------------------------------

#pragma once
#include <string>
#include <array>
#include <cstdarg>
#include <functional> //for std::function
#include <algorithm>  //for std::generate_n

#define BEGIN_NAMESPACE( x ) namespace x {
#define END_NAMESPACE }

BEGIN_NAMESPACE(XorCompileTime)

constexpr auto time = __TIME__;
constexpr auto seed = static_cast<int>(time[7]) + static_cast<int>(time[6]) * 10 + static_cast<int>(time[4]) * 60 + static_cast<int>(time[3]) * 600 + static_cast<int>(time[1]) * 3600 + static_cast<int>(time[0]) * 36000;

// 1988, Stephen Park and Keith Miller
// "Random Number Generators: Good Ones Are Hard To Find", considered as "minimal standard"
// Park-Miller 31 bit pseudo-random number generator, implemented with G. Carta's optimisation:
// with 32-bit math and without division

template < int N >
struct RandomGenerator {
private:
	static constexpr unsigned a = 16807; // 7^5
	static constexpr unsigned m = 2147483647; // 2^31 - 1

	static constexpr unsigned s = RandomGenerator< N - 1 >::value;
	static constexpr unsigned lo = a * (s & 0xFFFF); // Multiply lower 16 bits by 16807
	static constexpr unsigned hi = a * (s >> 16); // Multiply higher 16 bits by 16807
	static constexpr unsigned lo2 = lo + ((hi & 0x7FFF) << 16); // Combine lower 15 bits of hi with lo's upper bits
	static constexpr unsigned hi2 = hi >> 15; // Discard lower 15 bits of hi
	static constexpr unsigned lo3 = lo2 + hi;

public:
	static constexpr unsigned max = m;
	static constexpr unsigned value = lo3 > m ? lo3 - m : lo3;
};

template <>
struct RandomGenerator< 0 > {
	static constexpr unsigned value = seed;
};

template < int N, int M >
struct RandomInt {
	static constexpr auto value = RandomGenerator< N + 1 >::value % M;
};

template < int N >
struct RandomChar {
	static const char value = static_cast<char>(1 + RandomInt< N, 0x7F - 1 >::value);
};

template < size_t N, int K >
struct XorString {
public:
	const char _key;
	std::array< char, N + 1 > _encrypted;

	constexpr char enc(char c) const {
		return c ^ 2;
	}

	char dec(char c) const {
		return c ^ 2;
	}

public:
	template < size_t... Is >
	constexpr __forceinline XorString(const char* str, std::index_sequence< Is... >) : _key(RandomChar< K >::value), _encrypted{ enc(str[Is])... } {
	}

	__forceinline decltype(auto) decrypt(void) {
		for (size_t i = 0; i < N; ++i) {
			_encrypted[i] = dec(_encrypted[i]);
		}
		_encrypted[N] = '\0';
		return _encrypted.data();
	}
};

//--------------------------------------------------------------------------------
//-- Note: XorStr will __NOT__ work directly with functions like printf.
//         To work with them you need a wrapper function that takes a const char*
//         as parameter and passes it to printf and alike.
//
//         The Microsoft Compiler/Linker is not working correctly with variadic 
//         templates!
//  
//         Use the functions below or use std::cout (and similar)!
//--------------------------------------------------------------------------------



#ifdef NDEBUG
#define XorStr( s ) ( XorCompileTime::XorString< sizeof( s ) - 1, __COUNTER__ >( s, std::make_index_sequence< sizeof( s ) - 1>() )._encrypted.data() )
#else
#define XorStr( s ) ( s )
#endif

END_NAMESPACE


class SafeString {
public:
	SafeString() {
		auto randchar = []() -> char {
			const char charset[] =
				"0123456789"
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"abcdefghijklmnopqrstuvwxyz";
			const size_t max_index = (sizeof(charset) - 1);
			return charset[rand() % max_index];
		};
		//raw.resize ( 13 );
		raw = "undefined";

		//std::generate_n ( raw.begin ( ), 13, randchar );
	}
	SafeString(char* str) {
		raw = std::string{ str };

	}




	std::string raw;

	void display() {
		for (auto& c : raw)
			c ^= 2;
	}

	void end_display() {
		for (auto& c : raw)
			c ^= 2;
	}
};