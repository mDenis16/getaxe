#include "signatures.h"
#include "../../dependencies/utilities/utilities.hpp"
std::uint32_t Sigantures::GetPattern(std::uint32_t hash) {

	return SigList[hash];
}


std::uint32_t Sigantures::GetPattern(std::uint32_t hash, const char* dll, const char* pattern) {
	
	auto address = utilities::pattern_scan(dll, pattern);
	SigList[hash] = (std::uint32_t)address;

	return SigList[hash];
}

