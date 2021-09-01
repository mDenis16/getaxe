#pragma once
#include <map>
#include "../../dependencies/utilities/fnv.hpp"
		
class Sigantures {

	std::map<std::uint32_t, std::uint32_t> SigList;

public:
	Sigantures() {}
	~Sigantures() {}


	std::uint32_t GetPattern(std::uint32_t hash, const char* dll, const char* pattern);
	std::uint32_t GetPattern(std::uint32_t hash);

};
#define SIG_HASH_OFFSET 0x35223345
inline Sigantures* signatures = new Sigantures();
#ifdef PRODUCTION_RELEASE
#define PATTERN_SCAN(mod, sig) signatures->GetPattern(fnv::hash(mod, SIG_HASH_OFFSET) + fnv::hash(sig, SIG_HASH_OFFSET))
#else
#define PATTERN_SCAN(mod, sig) signatures->GetPattern(fnv::hash(mod, SIG_HASH_OFFSET) + fnv::hash(sig, SIG_HASH_OFFSET), mod, sig)
#endif