#include "../features.hpp"
void event_manager::round_prestart ( i_game_event * event ) {
	if ( !event )
		return;

	if ( !interfaces::engine->is_connected ( ) && !interfaces::engine->is_in_game ( ) )
		return;

	for ( int i = 1; i < 64; i++ ) {
		auto  data = &visuals::player::m_data [ i ];
		auto res_data = &resolver::resolver_data [ i ];
		data->alive = false;
		data->on_screen = false;
		data->valid = false;


		res_data->missed_shots = 0;
		res_data->brute_side = 0;
		res_data->safety = 0;
		res_data->safe_points = 0;
		res_data->first_shoot_correction = 0.f;
	}

}
void event_manager::round_end ( i_game_event * event ) {
	if ( !event )
		return;

	if ( !interfaces::engine->is_connected ( ) && !interfaces::engine->is_in_game ( ) )
		return;
	for ( int i = 1; i < 64; i++ ) {
		auto  data = &visuals::player::m_data [ i ];
		auto res_data = &resolver::resolver_data [ i ];
		data->alive = false;
		data->on_screen = false;
		data->valid = false;


		res_data->missed_shots = 0;
		res_data->brute_side = 0;
		res_data->safety = 0;
		res_data->safe_points = 0;
		res_data->first_shoot_correction = 0.f;
	}

	
}
