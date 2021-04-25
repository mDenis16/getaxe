#include "../../features/features.hpp"
#include "../hooks.hpp"


namespace hooks::callback {
	void __fastcall fire_event ( void * ecx, void * ) {
	
		if ( !local_player::m_data.pointer || !interfaces::engine->is_in_game ( ) || !interfaces::engine->is_connected ( ) )
			return fire_event_original ( ecx );


		if ( !local_player::m_data.pointer || !local_player::m_data.pointer->is_alive ( ))
			return fire_event_original ( ecx );


		struct event_t {
		public:
			uint8_t uchPad0 [ 4 ];
			float delay;
			uint8_t uchPad2 [ 48 ];
			event_t * next;
		};

		auto ei = interfaces::clientstate->m_Events;

		event_info * next = nullptr;

		if ( !ei )
			return fire_event_original ( ecx );

		do {
			next = ei->pNext;
			ei->flDelay = 0.f;
			ei = next;
		} while ( next );

		return fire_event_original ( ecx );
	}
}