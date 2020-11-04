#include "../../features.hpp"

void misc::hitmarker::think( ) {
//	if ( !variables::visuals::hitmarker || !interfaces::engine->is_connected( ) && !interfaces::engine->is_in_game( ) )
	//	return;

	//int width, height;
	//interfaces::engine->get_screen_size( width, height );

	if ( hitmarker_alpha > 0 ) {
		/*render::draw_line( width / 2.f - 8.f, height / 2.f - 8.f, width / 2.f - 3.f, height / 2.f - 3.f, color( 255, 255, 255, hitmarker_alpha ) );
		render::draw_line( width / 2.f - 8.f, height / 2.f + 8.f, width / 2.f - 3.f, height / 2.f + 3.f, color( 255, 255, 255, hitmarker_alpha ) );
		render::draw_line( width / 2.f + 8.f, height / 2.f - 8.f, width / 2.f + 3.f, height / 2.f - 3.f, color( 255, 255, 255, hitmarker_alpha ) );
		render::draw_line( width / 2.f + 8.f, height / 2.f + 8.f, width / 2.f + 3.f, height / 2.f + 3.f, color( 255, 255, 255, hitmarker_alpha ) );

		hitmarker_alpha -= 1.f / 0.5f * interfaces::globals->frame_time;*/
	}
}

void misc::hitmarker::event( i_game_event* event ) {
	if ( !event )
		return;

	auto attacker = interfaces::entity_list->get_client_entity( interfaces::engine->get_player_for_user_id( event->get_int( "attacker" ) ) );
	if ( !attacker )
		return;

	if ( attacker == csgo::local_player ) {
	//	if ( variables::visuals::hitmarker_sound )
		//	interfaces::surface->play_sound( "buttons\\arena_switch_press_02.wav" );

		hitmarker_alpha = 255;
	}
}