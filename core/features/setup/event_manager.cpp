#include "../features.hpp"
void event_manager::round_prestart ( i_game_event *  ) {
	

	if ( !interfaces::engine->is_connected ( ) && !interfaces::engine->is_in_game ( ) )
		return;

	

}
void event_manager::round_end ( i_game_event *  ) {

	//aimbot::best_target = { };
//	aimbot::last_target = { };
	
}
