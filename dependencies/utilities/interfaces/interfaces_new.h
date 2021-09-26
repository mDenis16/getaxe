#pragma once
#include "../literal_string.hpp"
#include "../fnv.hpp"
#include "../obf.h"
#include <iostream>
#include <map>
#include <string>
#include <iostream>
#include <vector>


namespace interfaces_manager
{



	/// <summary>
	/// Server part
	/// </summary>
	struct InterfaceObject
	{
		std::string mod;
		std::string inter;
		std::size_t addr;
	};
	inline std::map<std::size_t, InterfaceObject> INTERFACES_LIST_SERVER;
	/// <summary>
	/// 
	/// </summary>


	template<LiteralString _STR>
	inline constexpr static uint32_t GET_INTERFACE_HASH() {

		constexpr auto hash = fnv::hash(_STR);
		return hash;
	}


	/// <summary>
	/// Client part
	/// </summary>

	inline std::map<std::size_t, std::uint32_t> INTERFACES_ADDRESS;

	void on_server_receive(std::map < std::size_t, std::uint32_t> data);

	void simulate_server_computing();
	void init_prop_table();
}


#define FETCH_INTERFACE(type, ret, interf)  \
	OBF_BEGIN \
auto type##_samp = []()  { \
bool obscuj = false; \
int szeqrk = 2292; \
bool xvylcisvfmiik = true; \
int wihpdsdya = 4558; \
bool ycudh = true; \
if (true != true) { \
int uoof; \
for (uoof=93; uoof > 0; uoof--) { \
continue; \
}  \
} \
if (true == true) { \
int ekvx; \
for (ekvx=58; ekvx > 0; ekvx--) { \
continue; \
}  \
} \
if (__COUNTER__ * 12 != __COUNTER__ * 2) { \
int aqvitfwy; \
for (aqvitfwy=__COUNTER__ * 23; aqvitfwy > 0; aqvitfwy--) { \
continue; \
}  \
} \
  auto hash =  reinterpret_cast<ret*>(interfaces_manager::INTERFACES_ADDRESS[interfaces_manager::GET_INTERFACE_HASH<interf>()]); \
if (false != false) { \
int mltsl; \
for (mltsl=45; mltsl > 0; mltsl--) { \
continue; \
}  \
} \
if (4558 != 4558) { \
int qy; \
   return hash; \
for (qy=43; qy > 0; qy--) { \
continue; \
}  \
} \
     return hash; \
}; \
type = type##_samp(); \
OBF_END \


#define asd(table, prop, func_name, type) \
	type& func_name( ) { \
      static uintptr_t offset = 0; \
      static constexpr auto table_hash = fnv::hash( table ); \
      static constexpr auto prop_hash = fnv::hash( prop ); \
      if(!offset) \
      { offset = netvar_manager::get_net_var(table_hash, prop_hash ); } \
	  \
      return *reinterpret_cast< type* >( uintptr_t( this ) + offset ); \
    }