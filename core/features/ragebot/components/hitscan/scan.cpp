#include "../../../features.hpp"
#include "../../../../helpers/helpers.h"

namespace aimbot {
	std::deque< player_manager::lagcomp_t > filter_records ( target & entity ) {
		std::deque< player_manager::lagcomp_t > best_records;
		player_manager::lagcomp_t best_record;
		float min_speed = 9999.f;
		float dmg = 0.f;
		int min_points = 0;
		float min_delta = 999.f;
		autowall::FireBulletData_t awall = { };
	
		bool fatal_found = false;

	//	if ( !variables::ragebot::prioritize_record )
		//	return best_records;

		for ( auto record : player_manager::records [ entity.index ] ) {
			if ( math::time_to_ticks ( record.simtime ) == math::time_to_ticks ( entity.player->simulation_time ( ) ) )
				continue; /*skip current tick*/

			if ( record.shoot ) {
				best_record = record;
				break;
			}

			switch ( variables::ragebot::prioritize_record ) {
			case 1:
			{
				if ( variables::ragebot::prioritize_hitbox == hitbox_head ) {
					auto points = multipoint ( );
					multi_point ( entity, hitbox_head, record, points );
					if ( points.points.size ( ) >= min_points ) {
						best_record = record;
						min_points = points.points.size ( );
					}
				}
				else {
					if ( record.resolver.max_desync_delta <= min_delta ) {
						best_record = record;
						min_delta = record.resolver.max_desync_delta;
					}
				}
				break;
			}
			case 2:
			{
				if ( record.speed <= min_speed ) {
					min_speed = record.speed;
					best_record = record;
				}
				break;
			}
			case 3:
			{
				player_manager::restore_record ( entity.player, record );

				auto cur_damage = autowall::GetDamage ( csgo::local_player, record.bone [ 8 ].get_origin ( ), awall );

				if ( cur_damage > dmg ) {
					best_record = record;
					dmg = cur_damage;
				}

				if ( dmg >= entity.player->health ( ) ) {
					best_record = record;
					fatal_found = true;
				}
				if ( !fatal_found ) {
					cur_damage = autowall::GetDamage ( csgo::local_player, record.bone [ 0 ].get_origin ( ), awall );

					if ( cur_damage > dmg ) {
						best_record = record;
						dmg = cur_damage;
					}
				}
				player_manager::restore_player ( entity.player );
			}
			break;
			default:
				break;
			}

			if ( fatal_found )
				break; /*quit the loop if we already found a fatal one*/

		}

		if ( best_record.shoot ) {
			best_records.push_back ( best_record );
			best_records.push_back ( player_manager::records[entity.index].back() );
		}
		else {
			best_records.push_back ( player_manager::records [ entity.index ].back ( ) );
			best_records.push_back ( best_record.simtime == 0.f ? player_manager::records[entity.index].front() : best_record );
		}
		return best_records;
	}
	void scan ( target & entity ) {
		for ( auto record : filter_records ( entity ) ) {

			bestpoint data = best_point ( entity, record );
			
			if ( data.dmg >= (entity.aimbot.best_point.dmg + 5.f) ) {
				entity.aimbot.best_point = data;
				entity.aimbot.record = record;
				if ( record.shoot )
					break;
			}
		}
	
	}

}