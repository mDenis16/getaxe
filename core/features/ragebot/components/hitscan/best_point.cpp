#include "../../../features.hpp"

namespace aimbot {
	bestpoint best_point ( target & entity, player_manager::lagcomp_t & record ) {
		bool should_override = record.simtime == entity.player->simulation_time();
		
			
	
		autowall::FireBulletData_t awall = { };
		bestpoint data = {};
		bool found_fatal_hit = false;
		bool is_newest_record = record.simtime == entity.player->simulation_time ( );
	

		float min_dmg = config.ragebot_min_dmg;

		if ( config.ragebot_double_tap || entity.player->is_in_air ( ) )
			min_dmg = local_player::m_data.weapon_data->iDamage / 2 + 15.f;

		for ( auto hitbox : hitscan_list ) { /*loop through our hitscan list*/

			auto points = multipoint ( );
			multi_point ( entity, hitbox, record, points, !is_newest_record );
	
			for ( auto point : points.points ) {

				if ( !point.safe )
					continue;

			
				int dmg = autowall::GetDamage ( local_player::m_data.pointer, point.pos, awall );
			
				found_fatal_hit = ( dmg >= entity.player->health ( ) + 5 ) && hitbox != hitbox_head;
			

				if ( ( dmg >= data.dmg && ( dmg >= min_dmg ) )  || found_fatal_hit ) {
					data.position = point.pos;
					data.dmg = dmg;
					data.hitbox = hitbox;
					data.center = points.center;
			
					if ( found_fatal_hit ) {
						break;
					}
				}

			}
			if ( found_fatal_hit )
				break;
		}


		return data;
	}
}