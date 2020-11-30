#include "../../helpers/helpers.h"

#include "../features.hpp"

namespace aimbot {
	target  best_target;
	void create_move ( c_usercmd * cmd ){
		if ( !variables::ragebot::enabled )
			return;
		best_target = {};
		if ( !local_player::m_data.pointer )
			return;
		if ( !local_player::m_data.active_weapon )
			return;
		if ( !local_player::m_data.weapon_data )
			return;

		populate_hitscan ( );
		
		select_targets ( );

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

		if (hitchance(best_target ) )
		{
		
		
			if ( variables::ragebot::auto_fire && csgo::local_player->can_shoot ( variables::ragebot::double_tap ) )
				cmd->buttons |= ( int ) in_attack;
			else if ( variables::ragebot::auto_fire && !csgo::local_player->can_shoot ( variables::ragebot::double_tap ) )
				cmd->buttons &= ~( int ) in_attack;

			if ( cmd->buttons & ( int ) in_attack ) {

				cmd->tick_count = math::time_to_ticks ( best_target.aimbot.record.simtime + player_manager::get_lerp_time ( ) );
				cmd->viewangles = best_target.aimbot.angle;
				cmd->viewangles -= csgo::local_player->aim_punch_angle ( ) * 2.f;
			}
		}
	}
	

}