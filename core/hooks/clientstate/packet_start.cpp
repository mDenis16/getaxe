#include "../hooks.hpp"


namespace hooks::callback {
	void __fastcall packet_start ( void * ecx, void *, int incoming, int outgoing ) {
		/*runtime convention error*/
		/*local_player::ptr() = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( interfaces::engine->get_local_player ( ) ) );

		if ( !local_player::available ( ) )
			return packet_start_original ( ecx, incoming, outgoing );

		if ( !local_player::ptr()->is_alive ( ) )
			return packet_start_original ( ecx, incoming, outgoing );

		if ( local_player::data().commands.empty ( ) )
			return packet_start_original ( ecx, incoming, outgoing );

	

		for ( auto it = local_player::data().commands.rbegin ( ); it != local_player::data().commands.rend ( ); ++it ) {
			if ( !it->is_outgoing )
				continue;

			if ( it->command_number == outgoing || outgoing > it->command_number && ( !it->is_used || it->previous_command_number == outgoing ) ) {
				it->previous_command_number = outgoing;
				it->is_used = true;
				packet_start_original ( ecx, incoming, outgoing );
				break;
			}
		}

		auto result = false;

		for ( auto it = local_player::data().commands.begin ( ); it != local_player::data().commands.end ( );) {
			if ( outgoing == it->command_number || outgoing == it->previous_command_number )
				result = true;

			if ( outgoing > it->command_number && outgoing > it->previous_command_number )
				it = local_player::data().commands.erase ( it );
			else
				++it;
		}

		if ( !result )*/


			packet_start_original ( ecx, incoming, outgoing );
	}
}