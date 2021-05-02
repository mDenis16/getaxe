#include "../../features/features.hpp"
#include "../hooks.hpp"


namespace hooks::callback {
	void __stdcall frame_stage_notify ( client_frame_stage_t stage ) {

		if ( !interfaces::engine->is_in_game ( ) )
			return reinterpret_cast< void ( __stdcall * )( client_frame_stage_t ) >( list.at ( hook_index::frame_stage_notify )->original )( stage );

		if ( !interfaces::engine->is_connected ( ) )
			return reinterpret_cast< void ( __stdcall * )( client_frame_stage_t ) >( list.at ( hook_index::frame_stage_notify )->original )( stage );

		local_player::m_data.pointer = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( interfaces::engine->get_local_player ( ) ) );

		if ( stage == FRAME_START ) {
			visuals::handler->intialization ( );
		}


		
		csgo::local_player = local_player::m_data.pointer;

		
		if ( localdata.alive && local_pointer && local_pointer->is_alive ( ) ) {
			animations::update_local_update_start ( stage );
		}




		reinterpret_cast< void ( __stdcall * )( client_frame_stage_t ) >( list.at ( hook_index::frame_stage_notify )->original )( stage ); /*original function call*/

		if ( local_player::available ( ) ) { /*update animations here to get interpolated origin by game. ps: it uses uninterpolated data*/
			animations::update_animations_update_end ( stage );


			//animations::update_fake_animation ( );
		}

		if ( stage == client_frame_stage_t::FRAME_RENDER_START ) {
			visuals::handler->on_update ( );
		}




	}
}