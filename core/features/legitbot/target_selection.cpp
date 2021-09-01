#include "../features.hpp"


int c_legitbot::best_target ( bool no_fov ) {
	int closest = -1; float min_fov = settings->minimum_fov;
	
	if ( no_fov )
		min_fov = 180.f;

	ray_t ray;
	trace_t trace;
	trace_filter filter;

	vec3_t viewangle = vec3_t ( ); interfaces::engine->get_view_angles ( viewangle );
	for ( int i = 1; i < interfaces::globals->max_clients; i++ ) {
		auto entity = static_cast< player_t * >( interfaces::entity_list->get_client_entity ( i ) );

		if ( !entity )
			continue;
		if ( entity == local_player::m_data.pointer )
			continue;


		if ( entity->health ( ) <= 0 )
			continue;

		if ( !entity->is_enemy ( ) )
			continue;

		vec3_t entity_eye = entity->eye_pos ( );

		if ( !local_pointer->can_see_player_pos(entity, localdata.eye_position, entity_eye) )
			continue;



		auto angle = math::calc_angle ( localdata.eye_position, entity_eye ); angle.angle_normalize ( ); angle.angle_clamp ( );
		auto fov = math::get_fov ( viewangle, localdata.eye_position, entity_eye );
		if ( fov < min_fov ) {
			min_fov = fov;
			closest = entity->index();
		}
	}
	return closest;
}

void c_legitbot::find_target ( ) {
	/*we can make lock target here*/

	if (settings->target_delay > 0 && (interfaces::globals->tick_count - target_change_tick ) < settings->target_delay ) {
		valid_target = false;
		active_target = nullptr;
		return;
	}
	if ( target_index == -1 ) { /*there is no target at the moment find one*/
		target_index = best_target ( );
		active_target = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( target_index ) );
		valid_target = active_target != nullptr; /* check if there is no target in this tick*/

	}
	else /*check if target is still valid, if not search one in the same tick*/ {
	 
		active_target = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( target_index ) );

		if ( active_target == nullptr || ( active_target && !active_target->is_alive ( ) ) ) /*reset target if is no longer available*/ {
			reset_target ( );
			if ( settings->lock_target ) {
				in_aiming = false;
				reset_target();
				return;
			}
			target_index = best_target ( ); // find new one in the same tick
			active_target = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( target_index ) );
			valid_target = active_target != nullptr;
		}
		else {
			valid_target = true; /*that means old target its still valid*/
		}


	}
	

	if ( target_index != -1 && last_target_index != -1 && target_index != last_target_index ) {
		dt_progress = 0.f;
		changed_target = true;
		target_change_tick = interfaces::globals->tick_count;

		std::cout << "changed target " << std::endl;
	}

	last_target_index = target_index;
}
