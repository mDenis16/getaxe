#pragma once
#include <cstdint>
#include "../../source-sdk/math/vector3d.hpp"

class i_client_entity_listener {
public:
	virtual void on_entity_created ( void * pEntity ) { }
	virtual void on_entity_deleted ( void * pEntity ) { }
};

class i_client_entity_list {
public:
	virtual i_client_networkable * get_client_networkable ( int entnum ) = 0;
	virtual void * vtablepad0x1 ( ) = 0;
	virtual void * vtablepad0x2 ( ) = 0;
	virtual void * get_client_entity ( int entNum ) = 0;
	virtual void * get_client_entity_handle ( uintptr_t hEnt ) = 0;
	virtual int number_of_entities ( bool bIncludeNonNetworkable ) = 0;
	virtual int get_highest_entity_index ( ) = 0;
	virtual void set_max_entities ( int maxEnts ) = 0;
	virtual int get_max_entities ( ) = 0;

	template< class T >
	T * get ( int index ) {
		return reinterpret_cast< T * >( get_client_entity ( index ) );
	}

	template< class T >
	T * get ( void* hEnt ) {
		return reinterpret_cast< T * >( get_client_entity_handle ( hEnt ) );
	}

	void add_listener_entity ( i_client_entity_listener * pListener ) {
		m_entity_listeners.add_to_tail ( pListener );
	}

	void remove_listener_entity ( i_client_entity_listener * pListener ) {
		m_entity_listeners.find_and_remove ( pListener );
	}

private:
	c_utl_vector< i_client_entity_listener * > m_entity_listeners;
};