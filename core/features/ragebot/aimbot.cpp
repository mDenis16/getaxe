#include "../../helpers/helpers.h"

#include "../features.hpp"

namespace aimbot {

	int sleep_ticks = 0;
	bool last_tick_stop = false;
	bool m_shoot_next_tick = false;
	bool should_teleport_next_tick = false;
	void create_move ( c_usercmd * cmd, bool & send_packet ) {
		

		if ( !config.ragebot_enabled )
			return;

		if ( !local_player::available ( ) )
			return;


		best_target = { };

		if ( !local_player::m_data.pointer )
			return;
		if ( !local_player::m_data.active_weapon )
			return;

		if ( local_player::m_data.active_weapon->is_non_aim ( ) )
			return;

		if ( !local_player::m_data.weapon_data )
			return;


 
		    populate_hitscan ( );

			select_targets ( );
		


			for ( auto & target : targets ) {
				if ( !target.player )
					continue;
				if ( target.player->dormant() )
					continue;
				if ( !target.player->is_alive() )
					continue;
				scan ( target );
				if ( target.aimbot.best_point.dmg >= best_target.aimbot.best_point.dmg )
					best_target = target;
			}
		

			if ( !best_target.player ) /*we failed */
				return;

			if ( best_target.aimbot.best_point.position.is_zero ( ) )
				return;
		
	


		best_target.aimbot.angle = math::calc_angle ( engine_prediction::unpredicted_eye, best_target.aimbot.best_point.position );

		if ( !misc::can_fire (local_player::m_data.active_weapon, true) )
			return;
	 
	//	interfaces::console->console_printf ( "innacuracy level %f \n", get_innacuracy_level ( ) );
		if ( !hitchance ( best_target )) {
			int ticks_to_stop = math::ticks_to_stop ( engine_prediction::unpredicted_velocity );
			if ( !autostop::m_data.failed_hitchance ) {
				autostop::m_data.target_hitchance = ticks_to_stop;
				autostop::m_data.failed_hitchance = autostop::m_data.target_hitchance;
			}
			return;
		}

		if ( cmd->buttons & in_attack )
			cmd->buttons &= ~in_attack;
		

		
			if (  config.ragebot_auto_fire )
				cmd->buttons |= in_attack;
			if ( cmd->buttons & in_attack ) {
				local_player::m_data.aimbot_working = true;
				local_player::m_data.last_aimbot_shot = interfaces::globals->tick_count;


				cmd->tick_count = math::time_to_ticks ( best_target.aimbot.record.simtime + player_manager::get_lerp_time ( ) );

				cmd->viewangles = best_target.aimbot.angle;
				cmd->viewangles -= local_player::m_data.pointer->aim_punch_angle ( ) * 2.f;
				if ( !localdata.fakeducking ) {
					send_packet = true;
					anti_aim::choke_next_tick = true; 
				}

				last_target = best_target;
			}
	
	}
}