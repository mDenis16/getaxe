#include "../features.hpp"


namespace legit_bot {

	data_t m_data;

	player_t * get_closest_target ( ) {
		player_t * closest = nullptr; float min_fov = variables::legit_bot::fov;

		vec3_t viewangle = vec3_t ( ); interfaces::engine->get_view_angles ( viewangle );
		for ( size_t i = 1; i < interfaces::globals->max_clients; i++ ) {
			auto entity = static_cast< player_t * >( interfaces::entity_list->get_client_entity ( i ) );

			if ( !entity )
				continue;
			if ( entity == csgo::local_player )
				continue;

			if ( entity->health ( ) <= 0 )
				continue;

			if ( !entity->is_enemy ( ) )
				continue;

			auto angle = math::calc_angle ( csgo::local_player->get_eye_pos ( ), entity->get_eye_pos ( ) );
			auto fov = math::get_fov ( viewangle, angle );
			if ( fov < min_fov ) {
				min_fov = fov;
				closest = entity;
			}
		}
		return closest;
	}

	void validate_target ( ) {
		if ( m_data.current_target ) {
			if ( m_data.current_target->health ( ) < 0 )
				m_data.current_target = nullptr;
		}
	}

	void run ( c_usercmd * cmd ) {
		if ( !variables::legit_bot::enable )
			return;
		m_data.current_angle = cmd->viewangles;

		validate_target ( );

		if ( !m_data.current_target ) {
			m_data.current_target = get_closest_target ( );
			m_data.shoot_before = false;
			m_data.hit_random_angle = false;
		}
		if ( !( cmd->buttons & in_attack ) ) {
			m_data.shoot_before = false;
			m_data.hit_random_angle = false;
			return;
		
		}


		if ( m_data.current_target ) {
			calculate_hitbox ( );
			calculate_angle ( );
			apply_angle ( cmd );
		}
	
	}
	void linear_smooth ( ) {

		vec3_t delta;
		delta.x = m_data.current_angle.x - m_data.target_angle.x;
		delta.y = m_data.current_angle.y - m_data.target_angle.y;
		delta.angle_normalize ( );
		delta.angle_clamp ( );
		auto smooth_x = variables::legit_bot::smooth;
		auto smooth_y = variables::legit_bot::smooth;
		auto fov = math::get_fov ( m_data.current_angle, m_data.target_angle );
		if ( variables::legit_bot::curve_factor > 1.0f && fov > 3.0 ) {
			smooth_x /= (variables::legit_bot::curve_factor / 100.f);
		}
		m_data.aimbot_angle.x = m_data.current_angle.x - delta.x / ( 5.f * smooth_x );
		m_data.aimbot_angle.y = m_data.current_angle.y - delta.y / ( 5.f * smooth_y );
	
		m_data.aimbot_angle.angle_normalize ( );
		m_data.aimbot_angle.angle_clamp ( );
		
	}
	void calculate_angle ( ) {
	
		m_data.target_angle = m_data.hitbox_angle;
		
	

		linear_smooth ( );
	}
	void apply_rcs ( ) {
		if ( !variables::legit_bot::recoil )
			return;

		float recoil_value_x = ( 2.0 / 100 ) * variables::legit_bot::recoil_x;
		float recoil_value_y = ( 2.0 / 100 ) * variables::legit_bot::recoil_y;

		vec3_t current_punch = csgo::local_player->aim_punch_angle ( );

		if ( csgo::local_player->shots_fired ( ) > 0 ) {
			vec3_t new_punch = { current_punch.x - m_data.last_punch.x, current_punch.y - m_data.last_punch.y, 0 };
			m_data.aimbot_angle.x -= new_punch.x * recoil_value_x;
			m_data.aimbot_angle.y -= new_punch.y * recoil_value_y;
		}
		else {
			m_data.aimbot_angle.x -= current_punch.x * recoil_value_x;
			m_data.aimbot_angle.y -= current_punch.y * recoil_value_y;
		}
   	m_data.last_punch = current_punch;
	}
	void calculate_hitbox ( ) {
		m_data.hitbox_position = m_data.current_target->get_hitbox_position ( hitbox_head );
		m_data.hitbox_angle = math::calc_angle ( engine_prediction::unpredicted_eye, m_data.hitbox_position );

		float recoil_value_x = ( 2.0 / 100 ) * variables::legit_bot::recoil_x;
		float recoil_value_y = ( 2.0 / 100 ) * variables::legit_bot::recoil_y;

		vec3_t current_punch = csgo::local_player->aim_punch_angle ( );
		m_data.hitbox_angle.x -= current_punch.x * recoil_value_x;
		m_data.hitbox_angle.y -= current_punch.y * recoil_value_y;
		m_data.hitbox_angle.angle_normalize ( );
		m_data.hitbox_angle.angle_clamp ( );



		m_data.random_angle =  vec3_t( m_data.hitbox_angle.x + m_data.random_x, m_data.hitbox_angle.y + m_data.random_y , 0);
		
	}
	void apply_angle ( c_usercmd * cmd ) {
		
		cmd->viewangles = m_data.aimbot_angle;
		interfaces::engine->set_view_angles ( cmd->viewangles );
	}
}
