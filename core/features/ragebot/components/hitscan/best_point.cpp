#include "../../../features.hpp"
#include "../../../../helpers/helpers.h"

namespace aimbot {
	bestpoint best_point ( target & entity, player_manager::lagcomp_t & record ) {
		bool should_override = record.simtime == entity.player->simulation_time();
		
			
			
		autowall::FireBulletData_t awall = { };
		bestpoint data = {};
		bool found_fatal_hit = false;
		bool is_record_same_spot = record.simtime == player_manager::records [ entity.index ].back ( ).simtime && math::calc_distance ( record.origin, player_manager::records [ entity.index ].back ( ).origin, false ) < 0.2f;
		for ( auto hitbox : hitscan_list ) { /*loop through our hitscan list*/
			auto points = multipoint ( );
			multi_point ( entity, hitbox, record, points, is_record_same_spot );
	
			for ( auto point : points.points ) {

				if ( !point.safe )
					continue;

				player_manager::restore_record ( entity.player, record );
				int dmg = autowall::GetDamage ( csgo::local_player, point.pos, awall );
				player_manager::restore_player ( entity.player );

				found_fatal_hit = ( dmg >= entity.player->health ( ) + 5 ) && hitbox != hitbox_head;
				float min_dmg = variables::ragebot::min_dmg;

				if ( variables::ragebot::double_tap || entity.player->is_in_air ( ) )
					min_dmg = local_player::m_data.weapon_data->iDamage / 2 + 15.f;

				if ( ( dmg >= data.dmg && ( dmg >= min_dmg ) ) ) {
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