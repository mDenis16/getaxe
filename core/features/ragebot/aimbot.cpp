#include "../../helpers/helpers.h"

#include "../features.hpp"

namespace aimbot {

	int sleep_ticks = 0;
	bool last_tick_stop = false;
	bool m_shoot_next_tick = false;
	bool should_teleport_next_tick = false;
	void create_move ( c_usercmd * cmd, bool & send_packet ) {
		if ( should_teleport_next_tick ) {
			tickbase_system::m_shift_data.m_prepare_recharge = true;
			tickbase_system::m_shift_data.m_should_be_ready = false;
			should_teleport_next_tick = false;
		}
		if ( !tickbase_system::m_shift_data.m_did_shift_before && !tickbase_system::m_shift_data.m_should_be_ready )
			m_shoot_next_tick = false;

		if ( !config.ragebot_enabled )
			return;




		best_target = { };

		if ( !local_player::m_data.pointer )
			return;
		if ( !local_player::m_data.active_weapon )
			return;
		if ( !local_player::m_data.weapon_data )
			return;

		select_targets ( );

		autostop::run ( cmd );


		populate_hitscan ( );




		for ( auto & target : targets ) {
			scan ( target );
			if ( target.aimbot.best_point.dmg >= best_target.aimbot.best_point.dmg )
				best_target = target;
		}

		if ( !best_target.player ) /*we failed */
			return;

		if ( best_target.aimbot.best_point.position.is_zero ( ) )
			return;


		best_target.aimbot.angle = math::calc_angle ( engine_prediction::unpredicted_eye, best_target.aimbot.best_point.position );

		if ( last_tick_stop )
			autostop::minimal_stop ( cmd );

		if ( !hitchance ( best_target ) && !m_shoot_next_tick ) {
			autostop::minimal_stop ( cmd );
			last_tick_stop = true;
			return;
		}

		sleep_ticks++;

		if ( !tickbase_system::m_shift_data.m_should_attempt_shift || ( ( /*wait for charge !g_cfg [ XOR ( "rage_exploit_charged" ) ].get<bool> ( ) ||*/ csgo::m_goal_shift == config.ragebot_double_tap_ticks || tickbase_system::m_shift_data.m_should_disable ) && tickbase_system::m_shift_data.m_should_attempt_shift ) || ( /*wait for charge g_cfg [ XOR ( "rage_exploit_charged" ) ].get<bool> ( ) &&*/ csgo::m_goal_shift == 7 && tickbase_system::m_shift_data.m_should_attempt_shift && !( tickbase_system::m_shift_data.m_prepare_recharge || tickbase_system::m_shift_data.m_did_shift_before && !tickbase_system::m_shift_data.m_should_be_ready ) ) ) {
			if ( config.ragebot_auto_fire )
				cmd->buttons |= in_attack;

			if ( cmd->buttons & ( int ) in_attack ) {

				cmd->tick_count = math::time_to_ticks ( best_target.aimbot.record.simtime + player_manager::get_lerp_time ( ) );
				cmd->viewangles = best_target.aimbot.angle;
				cmd->viewangles -= local_player::m_data.pointer->aim_punch_angle ( ) * 2.f;
				send_packet = true;
				if ( m_shoot_next_tick ) {
					m_shoot_next_tick = false;
					should_teleport_next_tick = true;
				}
				last_target = best_target;

				last_tick_stop = false;
				if ( !m_shoot_next_tick && csgo::m_goal_shift == config.ragebot_double_tap_ticks && tickbase_system::m_shift_data.m_should_attempt_shift && !( tickbase_system::m_shift_data.m_prepare_recharge || tickbase_system::m_shift_data.m_did_shift_before && !tickbase_system::m_shift_data.m_should_be_ready ) ) {
						m_shoot_next_tick = true;
				}
			}
		}
	}
}