#include "../../features.hpp"
#include "../../../helpers/helpers.h"


namespace aimbot::autostop {


	
	void clamp_movement_speed ( c_usercmd * cmd, float speed ) {
		float final_speed = speed;

		cmd->buttons |= in_speed;

		float squirt = std::sqrtf ( ( cmd->forwardmove * cmd->forwardmove ) + ( cmd->sidemove * cmd->sidemove ) );

		if ( squirt > speed ) {
			float squirt2 = std::sqrtf ( ( cmd->forwardmove * cmd->forwardmove ) + ( cmd->sidemove * cmd->sidemove ) );

			float cock1 = cmd->forwardmove / squirt2;
			float cock2 = cmd->sidemove / squirt2;

			auto Velocity = local_player::m_data.pointer->velocity ( ).Length2D ( );

			if ( final_speed + 1.0 <= Velocity ) {
				cmd->forwardmove = 0;
				cmd->sidemove = 0;
			}
			else {
				cmd->forwardmove = cock1 * final_speed;
				cmd->sidemove = cock2 * final_speed;
			}
		}
	}

	void slow_walk ( c_usercmd * cmd, bool override ) {

		if ( m_data.slow_walk_this_tick )
			return;

		if ( interfaces::inputsystem->is_button_down ( button_code_t::KEY_LSHIFT ) )
			override = true;

		m_data.slow_walk_this_tick = true;
		if ( override && local_player::m_data.pointer && local_player::m_data.active_weapon && local_player::m_data.weapon_data) {
			// get the max possible speed whilest we are still accurate.

			float flMaxSpeed = local_player::m_data.pointer->is_scoped ( ) > 0 ? local_player::m_data.weapon_data->flMaxSpeedAlt : local_player::m_data.weapon_data->flMaxSpeed;
			float flDesiredSpeed = ( flMaxSpeed * 0.33000001 );
			cmd->buttons &= ~( int ) in_walk;
			clamp_movement_speed ( cmd, flDesiredSpeed );
		}
	}

	auto_data m_data;
	void run ( c_usercmd* cmd ) {

		if ( !aimbot::targets.size ( ) )
			return;

		m_data.target = aimbot::targets.front( ).player;
		m_data.ticks_to_stop = math::ticks_to_stop ( local_player::m_data.pointer->velocity ( ) );
		m_data.futute_shot_position = local_player::m_data.pointer->origin ( ) + local_player::m_data.pointer->view_offset ( ) +
			( local_player::m_data.pointer->velocity ( ) * interfaces::globals->interval_per_tick * m_data.ticks_to_stop );

		early_stop ( cmd );

	}


	void minimal_stop ( c_usercmd * cmd ) {
		if ( !config.ragebot_autostop [ 1 ] )
			return;
		slow_walk ( cmd, true );
	}
	void between_shots(c_usercmd* cmd ){
		if ( !config.ragebot_autostop [ 2 ] )
			return;
		clamp_movement_speed ( cmd, 0 );
	}

	void rotate_movement ( c_usercmd * cmd, float yaw ) {
		vec3_t viewangles;
		interfaces::engine->get_view_angles ( viewangles );

		float rotation = DEG2RAD ( viewangles.y - yaw );

		float cos_rot = cos ( rotation );
		float sin_rot = sin ( rotation );

		float new_forwardmove = ( cos_rot * cmd->forwardmove ) - ( sin_rot * cmd->sidemove );
		float new_sidemove = ( sin_rot * cmd->forwardmove ) + ( cos_rot * cmd->sidemove );

		cmd->forwardmove = new_forwardmove;
		cmd->sidemove = new_sidemove;
	}


	void early_stop ( c_usercmd * cmd ) {
		if ( !config.ragebot_autostop [ 0 ] )
			return;

	   	 autowall::FireBulletData_t awall = { };
	     int dmg = autowall::GetDamage ( local_player::m_data.pointer, m_data.target->get_bone_position(8, player_manager::records[m_data.target->index()].back().bone), awall );

		 if ( dmg >= config.ragebot_min_dmg ) {
			 cmd->sidemove = 0;
			 cmd->forwardmove = local_player::m_data.pointer->velocity ( ).Length2D ( ) > 13.f ? 450.f : 0.f;

			 rotate_movement ( cmd, math::calc_angle ( vec3_t ( 0, 0, 0 ), local_player::m_data.pointer->velocity ( ) ).y + 180.f );
		 }
	}



}

