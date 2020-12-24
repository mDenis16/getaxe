#include "../features.hpp"
void event_manager::round_prestart ( i_game_event * event ) {
	if ( !event )
		return;

	if ( !interfaces::engine->is_connected ( ) && !interfaces::engine->is_in_game ( ) )
		return;

	for ( int i = 1; i < 64; i++ ) {
		auto  data = &visuals::player::m_data [ i ];
	


	
	}

}
void event_manager::round_end ( i_game_event * event ) {

	aimbot::best_target = { };
	aimbot::last_target = { };
	
}
