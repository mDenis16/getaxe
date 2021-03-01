#include "../../../features.hpp"


namespace aimbot {
	std::vector< player_manager::lagcomp_t > filter_records ( target & entity ) {

		auto index = entity.player->index ( );

		std::vector< player_manager::lagcomp_t > best_records;
		best_records.resize ( 2 );

		if ( !player_manager::records [ index ].size ( ) )
			return best_records;

		autowall::FireBulletData_t awall = { };

		entity.is_current_record_hidden = autowall::get_damage ( local_pointer, engine_prediction::unpredicted_eye, player_manager::records [ index ].back ( ).bone [ 8 ].GetOrigin ( ), awall ) < 10;


		player_manager::lagcomp_t best_record = {};
		float min_speed = 9999.f;
		int dmg = 0;
		std::size_t  min_points = 0;
		float min_delta = 999.f;


		bool fatal_found = false;



		for ( auto record : player_manager::records [ index ] ) {
			if ( record.invalid )
				continue;

			if ( record.shoot ) {
				best_record = record;
				break;
			}

			switch ( config.ragebot_record_selection ) {
			case 1:
			{
				/*if ( config.ragebot_prioritize_hitbox == hitbox_head ) {
					auto points = multipoint ( );
					multi_point ( entity, hitbox_head, record, points );
					if ( points.points.size ( ) >= min_points ) {
						best_record = record;
						min_points = points.points.size ( );
					}
				}
				else {*/
				if ( record.max_delta <= min_delta ) {
					best_record = record;
					min_delta = record.max_delta;
				}
				//}
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
				/*
					auto cur_damage = autowall::GetDamage ( local_player::m_data.pointer, record.bone [ 8 ].GetOrigin ( ), awall );

					if ( cur_damage > dmg ) {
						best_record = record;
						dmg = cur_damage;
					}

					if ( dmg >= entity.player->health ( ) ) {
						best_record = record;
						fatal_found = true;
					}
					if ( !fatal_found ) {
						cur_damage = autowall::GetDamage ( local_player::m_data.pointer, record.bone [ 0 ].GetOrigin ( ), awall );

						if ( cur_damage > dmg ) {
							best_record = record;
							dmg = cur_damage;
						}
					}
					player_manager::restore_record ( entity.player, player_manager::records[entity.index].front() );
				*/
			}
			break;
			default:
				break;
			}

			if ( fatal_found )
				break; /*quit the loop if we already found a fatal one*/

		}


		if ( best_record.shoot || entity.is_current_record_hidden ) {
			best_records [ 1 ] = player_manager::records [ index ].back ( );
			best_records [ 0 ] = best_record;
		}
		else {
			best_records [ 0 ] = player_manager::records [ index ].back ( );
			best_records [ 1 ] = best_record;
		}
		return best_records;
	}


	void scan ( target & entity ) {


		for ( auto & record : filter_records ( entity ) ) {
			if ( player_manager::records [ entity.index ].empty ( ) ) {
				interfaces::console->console_printf ( "EMPTY RECORDS \n" );
				return;
			}

			record.apply ( entity.player );
			bestpoint data = data = best_point ( entity, record );
			record.restore ( entity.player );

			interfaces::console->console_printf ( "best_point dmg %i | hitbox %i \n", data.dmg, data.hitbox );

			if ( data.dmg >= ( entity.aimbot.best_point.dmg + 5.f ) ) {
				entity.aimbot.best_point = data;
				entity.aimbot.record = record;
				if ( record.shoot )
					break;
			}
		}
	}
}