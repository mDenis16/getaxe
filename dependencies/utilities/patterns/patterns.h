#pragma once
#include <string>
#include <map>
#include "../literal_string.hpp"
#include "../fnv.hpp"
#include "../utilities.hpp"

#ifdef PRODUCTION_LOCAL
#include <Windows.h>
#endif

namespace patterns_manager
{
#ifdef PRODUCTION_LOCAL
	/// <summary>
	/// This should be only on PRODUCTION_LOCAL and needs to be deployed to the server.
	/// </summary>
	struct PatternServer
	{
		std::string _module;
		std::string pattern;
		std::uint32_t address;
	};

	inline std::map<std::size_t, PatternServer> SERVER_PATTERN_LIST;
#endif
	  

	inline std::map<std::size_t, std::uint32_t> PATTERN_ADDRESES;
	void simulate_server_computing();
	void on_server_receive(std::map<std::size_t, std::uint32_t> received);


	template<LiteralString _module, LiteralString _pattern>
	inline constexpr static uint32_t GET_PATTERN_HASH() {

		constexpr auto hash = fnv::hash(_module) + fnv::hash(_pattern);

#ifdef PRODUCTION_LOCAL
		auto found_pattern = utilities::pattern_scan(_module, _pattern);

		const auto module_handle = GetModuleHandleA(_module);
		PATTERN_ADDRESES[hash] = (std::uint32_t)found_pattern;

		found_pattern -= (std::uint32_t)module_handle;

		/*auto tempmod = std::string{ _module };
		auto temppat = std::string{ _pattern };*/


	//	SERVER_PATTERN_LIST[hash] = PatternServer{ tempmod, temppat, found_pattern };
	
#endif
		/// <summary>
		/// Upload this data to server.
		///	Address needs to be calculated based on module address  for each inject.
		/// </summary>
		/// <returns></returns>

		return hash;
	}

}



#define FETCH_PATTERN( mod, pattern) patterns_manager::PATTERN_ADDRESES[patterns_manager::GET_PATTERN_HASH<mod, pattern>()]