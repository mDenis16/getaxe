#include "../features.hpp"

    c_legitbot * legitbot = new c_legitbot ( );

	bool c_legitbot::can_see_player_pos ( player_t * player ) {
		trace_t tr;
		ray_t ray;
		trace_filter filter;
		filter.skip = local_player::m_data.pointer;

		auto start = local_player::m_data.pointer->get_eye_pos ( );
		auto dir = ( player->get_eye_pos() - start ).normalized ( );

		ray.initialize ( start, player->get_eye_pos ( ) );
		interfaces::trace_ray->trace_ray ( ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr );

		return tr.entity == player || tr.flFraction > 0.97f;
	}


	player_t * c_legitbot::get_closest_target ( ) {
		player_t * closest = nullptr; float min_fov = settings->minimum_fov;
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
		
			if ( !can_see_player_pos(entity) )
				continue;

			auto angle = math::calc_angle ( local_player::m_data.pointer->get_eye_pos ( ), entity->get_eye_pos ( ) );
			auto fov = math::get_fov ( viewangle, angle );
			if ( fov < min_fov ) {
				min_fov = fov;
				closest = entity;
			}
		}
		return closest;
	}

	void c_legitbot::target_check ( ) {


		// check if target is still valid 

		if ( valid_target ) {
			if ( active_target && !active_target->is_alive ( ) ) {
				running_curve = false;
				dt_progress = 0.f;
				last_target_tick = interfaces::globals->tick_count;
				current_target_shots_fired = 0;
				last_shots_fired = 0;
				valid_target = false;
				active_target = nullptr;
			}
		}


	}

	
	void c_legitbot::find_target ( ) {
	
		if ( !valid_target ) {
 
			if ( ( interfaces::globals->tick_count - last_target_tick ) > settings->target_delay ) {
				active_target = get_closest_target ( );
				valid_target = true;

				
			}

		}
		if ( !active_target )
			valid_target = false;

	}

	vec3_t bezier ( float t, vec3_t & a, vec3_t & b, vec3_t & c, vec3_t & d ) {
		vec3_t dst;

		dst.x =
			a.x * ( 1.f - t ) * ( 1.f - t ) * ( 1.f - t ) +
			b.x * 3.f * t * ( 1.f - t ) * ( 1.f - t ) +
			c.x * 3.f * t * t * ( 1.f - t ) +
			d.x * t * t * t;
		dst.y =
			a.y * ( 1.f - t ) * ( 1.f - t ) * ( 1.f - t ) +
			b.y * 3.f * t * ( 1.f - t ) * ( 1.f - t ) +
			c.y * 3.f * t * t * ( 1 - t ) +
			d.y * t * t * t;
		dst.z =
			a.z * ( 1.f - t ) * ( 1.f - t ) * ( 1.f - t ) +
			b.z * 3.f * t * ( 1.f - t ) * ( 1 - t ) +
			c.z * 3.f * t * t * ( 1.f - t ) +
			d.z * t * t * t;

		return dst;

	}
	void c_legitbot::control_rcs ( vec3_t & position ) {
		float recoil_value_x = ( 2.0f / 100.f ) * settings->recoil_control_x;
		float recoil_value_y = ( 2.0f / 100.f ) * settings->recoil_control_y;

		vec3_t current_punch = local_player::m_data.pointer->aim_punch_angle ( );
		position.x -= current_punch.x * recoil_value_x;
		position.y -= current_punch.y * recoil_value_y;
		position.angle_normalize ( );
		position.angle_clamp ( );
	}
	void c_legitbot::aim_at ( vec3_t & position ) {
		aim_angles = math::calc_angle ( local_pointer->eye_pos ( ), position );
		aim_angles.angle_normalize ( );
		aim_angles.angle_clamp ( );
		control_rcs ( aim_angles );
	//	if ( dt_progress < 1.f ) {
			//current_cmd->viewangles = aim_angles;

			

	//	}
	//	else {
		

			vec3_t delta;
			delta.x = current_cmd->viewangles.x - aim_angles.x;
			delta.y = current_cmd->viewangles.y - aim_angles.y;
			delta.angle_normalize ( );
			delta.angle_clamp ( );
			auto smooth_x = settings->aim_speed * 10.f;
			auto smooth_y = settings->aim_speed * 10.f;
			
			

			current_cmd->viewangles.x = current_cmd->viewangles.x - delta.x / ( 5.f * smooth_x );
			current_cmd->viewangles.y = current_cmd->viewangles.y - delta.y / ( 5.f * smooth_y );
			
		

			current_cmd->viewangles.angle_normalize ( );
			current_cmd->viewangles.angle_clamp ( );
	//	}
		interfaces::engine->set_view_angles ( current_cmd->viewangles );

	}

	void c_legitbot::compute_hitscan ( ) {

	
		return;

		hitscan.clear ( );
		hitscan.reserve ( 5 );

		


		hitscan.push_back ( target_hitbox );

		if ( settings->hitscan.at ( 0 ) == 1 ) {
			hitscan.push_back ( ( int ) hitboxes::hitbox_head );
		}

		if ( settings->hitscan.at ( 1 ) == 1 ) {
			hitscan.push_back ( ( int ) hitboxes::hitbox_upper_chest );
			hitscan.push_back ( ( int ) hitboxes::hitbox_chest );

		}

		if ( settings->hitscan.at ( 0 ) == 2 ) {
			hitscan.push_back ( ( int ) hitboxes::hitbox_pelvis );
		}


		for ( size_t i = hitscan.size ( ) - 1; i >= 0; i-- ) {
			if ( hitscan.at ( i ) == settings->hitbox ) {
				hitscan.erase ( hitscan.begin ( ) + i );
				break;
			}
		}
	}
	void c_legitbot::calc_target_hitbox ( ) {

		switch ( settings->hitbox ) {
		case 0:
			target_hitbox = hitbox_head;
			break;
		case 1:
			target_hitbox = hitbox_body;
			break;
		case 2:
			target_hitbox = hitbox_pelvis;
			break;
		default:
			target_hitbox = hitbox_head;
			break;
		}

		
	}
	int c_legitbot::best_hitbox ( ) {

		if ( settings->closest_bone ) {

			int best_hitbox = 0;


			float closest = INT_MIN;
			vec3_t va;

			for ( auto & bone : hitscan ) {
				float fov = math::get_fov ( va, local_eye, active_target->get_hitbox_position ( bone ) );

				if ( fov < closest ) {
					closest = fov;
					best_hitbox = bone;
				}
			}

			return best_hitbox;
		}

		calc_target_hitbox ( );
		compute_hitscan ( );

		if ( hitscan.size ( ) == 0 )
			return target_hitbox;

		
		

		if ( current_target_shots_fired <= 0) 
			return settings->hitbox;

		if ( current_target_shots_fired > hitscan.size ( ) - 1 )
			current_target_shots_fired = 0;


		return target_hitbox;
	//	return hitscan [ current_target_shots_fired % (hitscan.size ( ) - 1) ];


	}

	void c_legitbot::run ( c_usercmd * cmd ) {

		current_cmd = cmd;
		settings = &config.weapon_groups [ 0 ];


		target_check ( );

		if ( cmd->buttons & in_attack ) {

			

			find_target ( );

			if ( valid_target && active_target ) {
				hitbox = active_target->get_hitbox_position ( best_hitbox() );

				if ( !hitbox.is_zero ( ) ) {
					auto aim_pos = calculate_trail ( );

					aim_at ( aim_pos );
				}

				if ( last_shots_fired != local_pointer->shots_fired ( ) ) {

					current_target_shots_fired++;

					last_shots_fired = local_pointer->shots_fired ( );
				}
			}

		

		}
		else {
			running_curve = false;
			dt_progress = 0.f;
			last_target_tick = 0;
			current_target_shots_fired = 0;
			last_shots_fired = 0;
			valid_target = false;
			active_target = nullptr;
		}

		did_attack_before =  current_cmd->buttons & in_attack;

		if ( valid_target && active_target && current_cmd->buttons & in_attack && dt_progress < settings->shoot_delay / 100.f )
			current_cmd->buttons &= ~in_attack;

		//if ( cmd->buttons & in_attack ) 
		//  cmd->buttons &= ~in_attack;
	}

	vec3_t c_legitbot::calculate_trail ( ) {
		

		vec3_t view_angles;


		interfaces::engine->get_view_angles ( view_angles );


		vec3_t head_start = local_pointer->eye_pos ( );

		float distance_to_hitbox = head_start.distance_to ( hitbox );

		vec3_t forward_hitbox_dirrection = math::angle_vector ( math::calc_angle ( head_start, hitbox ) );

		vec3_t forward_hitbox = head_start + ( forward_hitbox_dirrection * distance_to_hitbox );


		vec3_t forward_dirrection = math::angle_vector ( view_angles );

		vec3_t forward = head_start + ( forward_dirrection * distance_to_hitbox );


		if ( current_cmd->buttons & in_attack && !did_attack_before ) 			{
			did_attack_before = true;
			running_curve = true;
			start_position = forward;
		}

		
		
		
			ImVec2 start2d, end2d;


			float a1 = settings->bezier_curve [ 0 ];
			float a2 = settings->bezier_curve [ 1 ];
			float a3 = settings->bezier_curve [ 2 ];
			float a4 = settings->bezier_curve [ 3 ];


			vec3_t a = start_position;
			vec3_t d = forward_hitbox;

			vec3_t b, c;

			b.x = std::lerp ( start_position.x, forward_hitbox.x, a2 );
			b.y = std::lerp ( start_position.y, forward_hitbox.y, 1.f - a1 );
			b.z = std::lerp ( forward_hitbox.z, start_position.z, a2 );



			c.x = std::lerp ( forward_hitbox.x, start_position.x, a2 );
			c.y = std::lerp ( forward_hitbox.y, start_position.y, a3 );
			c.z = std::lerp ( forward_hitbox.z, start_position.z, a4 );


	

			vec3_t p = bezier ( dt_progress, a, b, c, d );

			if ( !current_cmd->command_number )
			   dt_progress += interfaces::globals->interval_per_tick * settings->aim_speed * 2.f;




			if ( dt_progress > 1.f ) {
				dt_progress = 1.f;
			}
			
		

			return p;
	}
	void c_legitbot::draw_debug ( ImDrawList* render ) {
	
		if ( local_pointer && local_pointer->is_alive ( ) ) {
			vec3_t view_angles, forward;
			vec3_t local = local_pointer->eye_pos ( );

			interfaces::engine->get_view_angles ( view_angles );
			view_angles.angle_normalize ( ); view_angles.angle_clamp ( );

			math::angle_vectors ( view_angles, forward );

			auto hitbox_ang = math::calc_angle ( local, hitbox );
			hitbox_ang.angle_normalize ( ); hitbox_ang.angle_normalize ( );

			vec3_t hitbox_forward;
			math::angle_vectors ( hitbox_ang, hitbox_forward );

			float dist = hitbox.distance_to ( local );

			vec3_t start = local + ( forward * 8192.f );

			vec3_t end  = local + ( hitbox_forward * 8192.f );


			ImVec2 from, to;

			if ( visuals::world_to_screen ( start, from ) && visuals::world_to_screen ( end, to ) ) {

				render->AddLine ( from, to, ImColor ( 255, 0, 0, 255 ), 3.f );

			}
		}

		if ( valid_target && !hitbox.is_zero ( )  && local_pointer  && local_pointer->is_alive()) {

			vec3_t view_angles, forward;
			vec3_t local = local_pointer->eye_pos ( );

			interfaces::engine->get_view_angles ( view_angles );

			math::angle_vectors ( view_angles, forward );

			vec3_t start = local + ( forward * (local.distance_to(hitbox)) );

			ImVec2 start2d, end2d;
			
			auto point_value = ( hitbox - start ) / static_cast< float >(  10 );
			
			 std::vector<vec3_t> points;
			 points.reserve ( 10 );
			 std::vector<ImVec2> points2s;
			 points2s.reserve ( 10 );

			ImVec2 pp;
			
			float a1 = settings->bezier_curve[0];
			float a2 = settings->bezier_curve [ 1 ];
			float a3 = settings->bezier_curve [ 2 ];
			float a4 = settings->bezier_curve [ 3 ];


			points.push_back ( start );
            points.push_back ( hitbox );

			vec3_t a = start;
			vec3_t d = hitbox;

			vec3_t b, c;
 
			b.x = std::lerp ( start.x, hitbox.x,  a2 );
			b.y = std::lerp ( start.y, hitbox.y,  1.f - a1 );
			b.z = std::lerp ( hitbox.z, start.z, a2 );



			c.x = std::lerp ( hitbox.x, start.x,  a2 );
			c.y = std::lerp ( hitbox.y, start.y, a3 );
			c.z = std::lerp ( hitbox.z, start.z, a4 );



			points.push_back ( c );
			points.push_back ( b );
			
			float ddt = 1.0f / 20;

			for ( float dt = 0; dt < 1; dt += ddt ) {
				vec3_t p = bezier ( dt, a, b, c, d );
				points.push_back ( p );
			}

			auto& tran_frame = active_target->coord_frame ( );

			for ( auto & p : points ) {
				
				if ( visuals::world_to_screen (p, pp ) )
					points2s.push_back ( pp );
			}
		//	}
			int i = 1;

			for ( auto & p : points2s ) {
				i += 50;

				render->AddCircleFilled ( p, 3.f, ImColor ( i, i, 255, 255 ) );
			}

		}
	}