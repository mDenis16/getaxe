#include "../../../features.hpp"

namespace aimbot {
	bestpoint best_point ( target & entity, player_manager::lagcomp_t & record ) {

		

		autowall::FireBulletData_t awall = { };
		awall.target = entity.player;

		bestpoint data = {};
		bool found_fatal_hit = false;
		bool is_newest_record = record.simtime == entity.player->simulation_time ( );
		
		auto dist = math::calc_distance ( local_pointer->origin ( ), entity.player->origin ( ), false );

		//interfaces::console->console_printf ( "distance %f \n", dist );

		bool high_dist = dist > 1000.f;
		if ( !localdata.force_min_dmg_on_key && dist > 500.f ) {
			auto bone = entity.player->get_hitbox_position ( hitbox_body, record.bone );

			autowall::get_damage ( local_pointer, localdata.eye_position, bone, awall );

			int pen_limit = high_dist ? 6 : 8;

			if ( awall.penetrationTry > pen_limit ) /* dont scan far enemies */
				return data;
		}

		float min_dmg = localdata.force_min_dmg_on_key ? 15 : config.ragebot_min_dmg;

		if ( min_dmg > local_player::m_data.weapon_data->iDamage )
			min_dmg = local_player::m_data.weapon_data->iDamage - 1.f;

		for ( auto hitbox : hitscan_list ) { /*loop through our hitscan list*/
	
			if (  hitbox == hitbox_head && ( entity.player->health ( ) < 30))
				continue;
			if ( hitbox == hitbox_head && record.resolve_info.side <= player_manager::predicted_side::UNKNOWN)
				continue;

			auto points = multipoint ( );
			multi_point ( entity, hitbox, record, points, false );
	


			for ( auto point : points.points ) {
			
		
				if ( !point.safe )
					continue;

			
				int dmg = autowall::get_damage ( local_pointer, localdata.eye_position, point.pos, awall );

			
				found_fatal_hit = ( dmg >= entity.player->health ( ) + 10 ) && hitbox != hitbox_head;
			

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