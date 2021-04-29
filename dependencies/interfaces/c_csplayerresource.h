#pragma once
#include "../utilities/crypt_str.h"
#include "../utilities/netvars/netvars.hpp"
#define OFFSET(type, name, offset)\
type &name##() const\
{\
	return *(type*)(uintptr_t(this) + offset);\
}

class CPlayerResource {

};

class c_csplayer_resource : public CPlayerResource {
public:
	int get_ping ( int index ) {
		static auto m_iPing = netvar_manager::get_net_var ( fnv::hash ( "CCSPlayerResource" ), fnv::hash ( "m_iPing" ) );
		return *( int * ) ( ( uintptr_t ) this + m_iPing + index * sizeof ( int ) );
	}

	int GetKills ( int index ) {
		static auto m_iKills = netvar_manager::get_net_var ( fnv::hash ( "CCSPlayerResource" ), fnv::hash ( "m_iKills" ) );
		return *( int * ) ( ( uintptr_t ) this + m_iKills + index * sizeof ( int ) );
	}

	int GetAssists ( int index ) {
		static auto m_iAssists = netvar_manager::get_net_var ( fnv::hash ( "CCSPlayerResource" ), fnv::hash ( "m_iAssists" ) );
		return *( int * ) ( ( uintptr_t ) this + m_iAssists + index * sizeof ( int ) );
	}

	int GetDeaths ( int index ) {
		static auto m_iDeaths = netvar_manager::get_net_var ( fnv::hash ( "CCSPlayerResource" ), fnv::hash ( "m_iDeaths" ) );
		return *( int * ) ( ( uintptr_t ) this + m_iDeaths + index * sizeof ( int ) );
	}

	bool GetConnected ( int index ) {
		static auto m_bConnected = netvar_manager::get_net_var ( fnv::hash ( "CCSPlayerResource" ), fnv::hash ( "m_bConnected" ) );
		return *( bool * ) ( ( uintptr_t ) this + m_bConnected + index * sizeof ( bool ) );
	}
};

class cs_game_rules_proxy {
public:
	OFFSET ( int, m_iRoundTime, 0x44 );
	OFFSET ( float, m_fRoundStartTime, 0x4C );
	OFFSET ( bool, m_bIsValveDS, 0x75 );
};