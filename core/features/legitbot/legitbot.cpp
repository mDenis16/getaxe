#include "../features.hpp"


namespace legit_bot {
	
	data_t m_data;

	bool can_see_player_pos ( player_t * player ) {
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
	player_t * get_closest_target ( ) {
		player_t * closest = nullptr; float min_fov = config.legitbot_fov;
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

	player_manager::lagcomp_t* get_best_record ( player_t * player ) {
		player_manager::lagcomp_t* record = nullptr;
		if ( player_manager::records [ player->index ( ) ].size ( ) )
			return record;

		float min_fov = 0.f;
		
		for ( auto rec : player_manager::records [ player->index ( ) ] ) {
			float fov = math::get_fov ( local_player::m_data.orig_viewangle, player->get_bone_position ( 8, rec.bone ) );
		    
			if ( min_fov < fov ) {
				record = &rec;
				min_fov = fov;
			}
		}

		return record;
	}
	void validate_target ( ) {
		if ( m_data.current_target ) {
			if ( m_data.current_target->health ( ) < 0 )
				m_data.current_target = nullptr;
		}
	}


	void run ( c_usercmd * cmd ) {

		if ( !( config.triggerbot_enable || config.legitbot_enable ) )
			return;


		m_data.current_angle = cmd->viewangles;

		

	

		
			m_data.current_target = get_closest_target ( );

			validate_target ( );


			if ( !m_data.current_target )
				return;

			m_data.shoot_before = false;
			m_data.hit_random_angle = false;
		
			if ( config.triggerbot_enable )
				trigger_bot::run ( cmd );

	
		if ( !( cmd->buttons & in_attack ) ) {
			m_data.shoot_before = false;
			m_data.hit_random_angle = false;
			return;
		
		}


		if ( m_data.current_target && config.legitbot_enable ) {
			calculate_hitbox ( );

			if ( m_data.hitbox_position.is_zero ( ) )
				return;

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
		auto smooth_x = config.legitbot_smooth;
		auto smooth_y = config.legitbot_smooth;
		auto fov = math::get_fov ( m_data.current_angle, m_data.target_angle );
		if ( config.legitbot_curve_factor > 1.0f && fov > 3.0 ) {
			smooth_x /= (config.legitbot_curve_factor / 100.f);
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
		if ( !config.legitbot_recoil )
			return;

		float recoil_value_x = ( 2.0f / 100.f ) * config.legitbot_recoil_x;
		float recoil_value_y = ( 2.0f / 100.f ) * config.legitbot_recoil_y;

		vec3_t current_punch = local_player::m_data.pointer->aim_punch_angle ( );

		if ( local_player::m_data.pointer->shots_fired ( ) > 0 ) {
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
		matrix3x4_t bone_matrix [ MAXSTUDIOBONES ];

		memcpy ( bone_matrix, m_data.current_target->m_CachedBoneData ( ).Base ( ), m_data.current_target->m_CachedBoneData ( ).Count ( ) * sizeof ( matrix3x4_t ) );
		m_data.hitbox_position = m_data.current_target->get_hitbox_position ( hitbox_head, bone_matrix );
		m_data.hitbox_angle = math::calc_angle ( engine_prediction::unpredicted_eye, m_data.hitbox_position );

		float recoil_value_x = ( 2.0f / 100.f ) * config.legitbot_recoil_x;
		float recoil_value_y = ( 2.0f / 100.f ) * config.legitbot_recoil_y;

		vec3_t current_punch = local_player::m_data.pointer->aim_punch_angle ( );
		m_data.hitbox_angle.x -= current_punch.x * recoil_value_x;
		m_data.hitbox_angle.y -= current_punch.y * recoil_value_y;
		m_data.hitbox_angle.angle_normalize ( );
		m_data.hitbox_angle.angle_clamp ( );



		m_data.random_angle =  vec3_t( m_data.hitbox_angle.x + m_data.random_x, m_data.hitbox_angle.y + m_data.random_y , 0);
		
	}
	void apply_angle ( c_usercmd * cmd ) {
		
		cmd->viewangles = m_data.aimbot_angle;
//
	//	cmd->tick_count = math::time_to_ticks(get_best_record ( m_data.current_target ).simtime);
		interfaces::engine->set_view_angles ( cmd->viewangles );
	}
}
