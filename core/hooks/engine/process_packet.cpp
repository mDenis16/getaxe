
#include "../hooks.hpp"


namespace hooks::callback {
	void __cdecl process_packet ( void * packet, bool header ) {
		struct event_t {
		public:
			uint8_t uchPad0 [ 4 ];
			float delay;
			uint8_t uchPad2 [ 48 ];
			event_t * next;
		};


		if ( !interfaces::engine->get_net_channel ( ) )
			return process_packet_original ( packet, header );

		/* get this from CL_FireEvents string "Failed to execute event for classId" in engine.dll */
		auto event = *( event_t ** ) ( uintptr_t ( interfaces::clientstate ) + 0x4E6C );
		while ( event ) {
			event->delay = 0.0f;
			event = event->next;
		}

		/* game events are actually fired in OnRenderStart which is WAY later after they are received */
		/* effective delay by lerp time, now we call them right after theyre received (all receive proxies are invoked without delay). */
		interfaces::engine->fire_game_event ( );
	}
}