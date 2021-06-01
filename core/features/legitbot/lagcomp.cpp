#include "../features.hpp"

void c_legitbot::manipulate_lagcomp ( ) {
	if ( current_cmd->buttons & in_attack ) {
		if ( settings->lagcompensation ) {

			//auto target = active_target;

		//	if ( !target ) {
			//	target = get_closest_target ( true );
		//	}

			//if ( target ) {
			//	auto best_record = find_bestlagcomp ( target );

		//		if ( best_record )
		//			best_record->apply_tickcount ( );
//
		//	}

		}
	}
}
void c_legitbot::store_lagcompensation ( ) {
	if ( settings->lagcompensation ) {
		for ( size_t i = 1; i < interfaces::globals->max_clients; i++ ) {
			auto player = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( i ) );

			if ( !player || !player->is_alive ( ) ) {

				if ( !records [ i ].empty ( ) ) {
					for ( auto & rec : records [ i ] )
						delete rec;

					records [ i ].clear ( );
				}
				continue;
			}

			if ( records [ i ].size ( ) && ( records [ i ].back ( )->simulation_time == player->simulation_time ( ) ) )
				continue;

			records [ i ].reserve ( 32 );


			auto rec = new c_record_t ( );
			rec->log_data ( player );


			records [ i ].push_back ( rec );


			while ( records [ i ].size ( ) > 3 && records [ i ].size ( ) > static_cast< size_t >( math::time_to_ticks ( static_cast< float >( settings->lag_compensation_time ) / 1000.f ) ) ) {
				delete records [ i ].at ( 0 );
				records [ i ].erase ( records [ i ].begin ( ) );
			}

		}
	}
}

void c_record_t::log_data ( player_t * pl ) {
	entity = pl;
	simulation_time = pl->simulation_time ( );
	mins = pl->mins ( );
	maxs = pl->maxs ( );
	memcpy ( bone, pl->m_CachedBoneData ( ).base ( ), pl->m_CachedBoneData ( ).count ( ) * sizeof ( matrix3x4_t ) );


}
void c_record_t::apply_tickcount ( ) {

	if ( legitbot && legitbot->current_cmd ) {
		legitbot->current_cmd->tick_count = math::time_to_ticks ( simulation_time );
	}

}
c_record_t * c_legitbot::find_bestlagcomp ( player_t * entity ) {

	float min_fov = 180.f; c_record_t * best = nullptr;
	vec3_t viewangle = vec3_t ( ); interfaces::engine->get_view_angles ( viewangle );
	for ( auto & rec : records [ entity->index ( ) ] ) {
		auto angle = math::calc_angle ( localdata.eye_position, entity->get_hitbox_position ( hitbox_head, rec->bone ) );
		auto fov = math::get_fov ( viewangle, angle, localdata.eye_position.distance_to ( entity->origin ( ) ) );

		if ( fov < min_fov ) {
			min_fov = fov;
			best = rec;
		}

	}

	return best;
}