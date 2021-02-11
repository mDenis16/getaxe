#include "../features.hpp"
#include "../../menu/render/menu_render.hpp"
#include "../../menu/ImGui/imgui.h"
#include "../../menu/ImGui/imgui_internal.h"

static int last_id = 0;
std::deque<shot_processor::shot_data> shot_processor::shots;
std::deque<shot_processor::shot_data> current_shots;
std::deque<shot_processor::shot_data> unapproved_shots;
void shot_processor::weapon_fire( i_game_event* event ) {
	if ( !event )
		return;

	if ( !interfaces::engine->is_connected( ) && !interfaces::engine->is_in_game( ) )
		return;

	auto local_player = reinterpret_cast< player_t* >( interfaces::entity_list->get_client_entity( interfaces::engine->get_local_player( ) ) );
	if ( !local_player )
		return;

	auto entity = interfaces::engine->get_player_for_user_id( event->get_int( "userid" ) );
	if ( !entity )
		return;

	if ( entity != interfaces::engine->get_local_player( ) )
		return;
	

	if ( !aimbot::last_target.player )
		return;

	if ( aimbot::last_target.index == -1 )
		return;

	shot_data shot = {};

	auto target = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( aimbot::last_target.index ));
	if ( !target )
		return;
	aimbot::last_target.player = target;
	shot.tick = interfaces::globals->tick_count;
	shot.target = aimbot::last_target;
	shot.enemy_index = aimbot::last_target.player->index ( ); /*crash after map change should reset it ;)*/
	shot.shotpos = engine_prediction::unpredicted_eye;
	shot.curtime = interfaces::globals->cur_time;
	shots.push_back ( shot );
	aimbot::last_target = { };
}
void shot_processor::hurt_event( i_game_event* event ) {
	if ( !event )
		return;

	if ( !interfaces::engine->is_connected( ) && !interfaces::engine->is_in_game( ) )
		return;

	auto local_player = reinterpret_cast< player_t* >( interfaces::entity_list->get_client_entity( interfaces::engine->get_local_player( ) ) );
	if ( !local_player )
		return;

	auto victim = interfaces::engine->get_player_for_user_id( event->get_int( "userid" ) );
	if ( !victim )
		return;

	auto attacker = interfaces::entity_list->get_client_entity( interfaces::engine->get_player_for_user_id( event->get_int( "attacker" ) ) );
	if ( !attacker )
		return;

	if ( attacker == local_player ) {
		auto entity = reinterpret_cast< player_t* >( interfaces::entity_list->get_client_entity( victim ) );
		if ( !entity ) {
			return;
		}

		player_info_t info;
		interfaces::engine->get_player_info( entity->index( ), &info );

		auto hitbox = event->get_int( "hitgroup" );
		if ( !hitbox )
			return;

		auto damage = event->get_int( "dmg_health" );
		if ( !damage )
			return;

		auto health = event->get_int( "health" );

		if ( !health && health != 0 )
			return;

		if ( entity->team( ) == local_player->team( ) )
			return;

	
		auto shot = closest_shot( interfaces::globals->tick_count );
		if ( shot ) {
			shot->hurt = true;
			shot->hit_info.damage = damage;
			shot->hit_info.hitgroup = hitbox;
			shot->hit_info.victim = victim;
		}

		
	}

}

void shot_processor::bullet_impact ( i_game_event * event ) {
	auto userid = event->get_int ( "userid" );
	if ( !userid )
		return;

	if ( !interfaces::engine->is_connected ( ) && !interfaces::engine->is_in_game ( ) )
		return;

	auto engine_userid = interfaces::engine->get_player_for_user_id ( userid );

	if ( !engine_userid )
		return;

	if ( engine_userid != interfaces::engine->get_local_player ( ) )
		return;




	vec3_t bullet_impact = vec3_t ( event->get_float ( "x" ), event->get_float ( "y" ), event->get_float ( "z" ) );

	auto shot = closest_shot ( interfaces::globals->tick_count );
	if ( shot ) {
		shot->hitpos = bullet_impact;
		shot->approved_bullet = true;
		auto entity = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( shot->enemy_index ) );

		if ( !entity )
			return;
		if ( !entity->is_alive ( ) )
			return;
		if ( entity->dormant ( ) )
			return;


		if ( auto intersection = aimbot::get_intersect_point ( shot->shotpos, bullet_impact, shot->target.aimbot.best_point.col.mins, shot->target.aimbot.best_point.col.maxs, shot->target.aimbot.best_point.col.radius); intersection ) {
			shot->hit = true;
			shot->hitpos = *intersection;
		}
	}
}


shot_processor::shot_data* shot_processor::closest_shot( int tickcount )
{
	shot_data* closest_shot = nullptr;
	auto closest_diff = 64;
	for ( auto& shot : shots )
	{
		const auto diff = abs( tickcount - shot.tick );
		if ( diff <= closest_diff )
		{
		//	interfaces::console->console_printf( "DIF TICKCOUNT %i \n", diff );
			closest_shot = &shot;
			closest_diff = diff;
		}
	}

	return closest_shot;
}//resolver::resolver_data [ shot->enemy_index ].last_hit_angle_related_to_me.push_back ( dif );
const char * shot_processor::hitbox_text ( hitboxes hitbox ) {
	switch ( hitbox ) {
	case hitbox_head:
	case hitbox_neck:
		return "head";
		break;
	case hitbox_pelvis:
	case hitbox_body:
	case hitbox_thorax:
	case hitbox_chest:
	case hitbox_upper_chest:
		return "body";
		break;
	case hitbox_right_thigh:
	case hitbox_left_thigh:
	case hitbox_right_calf:
	case hitbox_left_calf:
	case hitbox_right_foot:
	case hitbox_left_foot:
		return "feet";
		break;
	case hitbox_right_hand:
	case hitbox_left_hand:
		return "hand";
		break;
	case hitbox_right_upper_arm:
	case hitbox_right_forearm:
	case hitbox_left_upper_arm:
	case hitbox_left_forearm:
		return "arm";
		break;
	case hitbox_max:
		return "generic";
		break;
	default:
		return "generic";
		break;
	}
}
const char* hitgroup_text( int hitgroup ) {
	switch ( hitgroup ) {
	case hitgroup_head:
		return "head";
	case hitgroup_stomach:
		return "stomach";
	case hitgroup_leftarm:
	case hitgroup_rightarm:
		return "arm";
	case hitgroup_leftleg:
	case hitgroup_rightleg:
		return "leg";
	default:
		return "body";
	}
}
void shot_processor::manage_shots( ) {
	for ( size_t i = 0; i < shots.size(); i++ )
	{
		auto shot = &shots.at( i );
		if ( !shot->approved  && abs(shot->tick - interfaces::globals->tick_count) < 64 ) {

			if ( !shot->approved_bullet && ( interfaces::globals->cur_time - shot->curtime ) < 2.f)
				continue;

			auto entity = reinterpret_cast< player_t* >( interfaces::entity_list->get_client_entity( shot->enemy_index ) );

			if ( !entity )
				return;

			std::string name = entity->get_info( ).name;

			if ( name.length( ) > 17 )
				name = name.substr( 0, 17 ) + "...";


			std::transform ( name.begin ( ), name.end ( ), name.begin ( ), ::tolower );
		
			auto hitgroup = hitgroup_text( shot->hit_info.hitgroup );
			auto hitbox = hitbox_text ( ( hitboxes ) shot->target.aimbot.best_point.hitbox );
		
			std::stringstream ss = {};
			//interfaces::console->console_printf ( "SHOT [ HITBOX : %i, EXTRAPOLATED: %i, PREDICTED: %s ] \n", shot->target.aimbot.best_point.hitbox, shot->target.aimbot.record.choked, shot->target.aimbot.record.predicted ? "TRUE" : "FALSE" );

			if ( !shot->approved_bullet ) {
				ss << "Hit " << name << " in " << hitbox << " due to unregister shot.";
			}
			else if ( shot->hit &&  !shot->hurt )
			{
					resolver::resolver_data [ shot->enemy_index ].missed_shots++;
					resolver::resolver_data [ shot->enemy_index ].side = static_cast<resolver::desync_side>(resolver::resolver_data [ shot->enemy_index ].missed_shots % 3);
					ss << "Missed  " << name << " in " << hitbox << " damage due to resolver.";
			
			}
			else if ( shot->hurt && !shot->hit ) {
				
				ss << "Hit " << name << " in " << hitbox << " due to spread.";
				
			}
			else if ( !shot->hit )
			{
				ss << "Missed  " << name << " in " << hitbox << " damage due to spread.";
			}	
			else if ( shot->hurt && shot->hit )
			{
	
				ss << "Hit  " << name << " in " << hitgroup << " for " << shot->hit_info.damage << " damage.  bt (  "  << math::time_to_ticks ( shot->target.player->get_old_simulation_time() - shot->target.aimbot.record.simtime ) << " ) shot  " << (shot->target.aimbot.record.shoot ? "yes" : "no") << std::endl;
				
			
				if ( shot->target.aimbot.record.resolved ) {
					connection::telemetry send_data = { };
					send_data.lby = shot->target.aimbot.record.lby;
					send_data.autowall_side = shot->target.aimbot.record.side;
					send_data.goal_feet = shot->target.aimbot.record.goal_feet;
					send_data.left_yaw = shot->target.aimbot.record.eye_angles.y - 58.f;
					send_data.yaw = shot->target.aimbot.record.eye_angles.y;
					send_data.right_yaw = shot->target.aimbot.record.eye_angles.y + 58.f;
					send_data.map = interfaces::engine->get_level_name ( );
					send_data.hit_side = shot->target.aimbot.record.side;
					send_data.max_desync_delta = shot->target.aimbot.record.max_delta;
					send_data.missed = false;
					send_data.position = shot->target.aimbot.record.origin;
					send_data.send ( );
				}

				visuals::player::hit_chams hitchams;
				hitchams.curtime = interfaces::globals->cur_time;
				std::memcpy ( hitchams.bones, shot->target.aimbot.record.bone, sizeof ( shot->target.aimbot.record.bone ) );
				visuals::player::chams_log.at ( shot->target.index ).push_back ( hitchams );


			}

			visuals::notifications::add ( ss.str ( ) );
			shot->approved = true;
		}
		if ( (interfaces::globals->cur_time - shot->curtime) > 15.f )
			shots.erase( shots.begin( ) + i );
		
	}
	
}

void shot_processor::draw_shots ( ) {
	for ( size_t i = 0; i < shots.size ( ); i++ ) {
		auto shot = shots.at ( i );
		if ( shot.approved ) {
			ImVec2 from = ImVec2 ( ); ImVec2 to = ImVec2();
			if ( visuals::world_to_screen ( shot.shotpos, from ) && visuals::world_to_screen ( shot.hitpos, to ) ) {
				int pLineSize = 6;

				auto color = shot.hurt ? ImColor ( 255, 255, 255 ) : ImColor(255,0,0);

				if ( shot.hit ) {
					c_menu::get ( ).draw->AddLine ( ImVec2 ( to.x - pLineSize / 2, to.y - pLineSize / 2 ), ImVec2 ( to.x - ( pLineSize ), to.y - ( pLineSize ) ), color );
					c_menu::get ( ).draw->AddLine ( ImVec2 ( to.x - pLineSize / 2, to.y + pLineSize / 2 ), ImVec2 ( to.x - ( pLineSize ), to.y + ( pLineSize ) ), color );
					c_menu::get ( ).draw->AddLine ( ImVec2 ( to.x + pLineSize / 2, to.y + pLineSize / 2 ), ImVec2 ( to.x + ( pLineSize ), to.y + ( pLineSize ) ), color );
					c_menu::get ( ).draw->AddLine ( ImVec2 ( to.x + pLineSize / 2, to.y - pLineSize / 2 ), ImVec2 ( to.x + ( pLineSize ), to.y - ( pLineSize ) ), color );
				}
				else {
					c_menu::get ( ).draw->AddCircle ( to, 30, 15, color );
				}

				c_menu::get ( ).draw->AddLine ( from, to, color );
			
				
			}
		}
	}
	/*for ( size_t i = 0; i < resolver::shots.size ( ); i++ ) {
		resolver::resolve_shot shot = resolver::shots.at ( i );
		if ( shot.approved ) {
			ImVec2 from = ImVec2 ( ); ImVec2 to = ImVec2 ( );
			if ( visuals::world_to_screen ( shot.shotpos, from ) && visuals::world_to_screen ( shot.hitpos, to ) ) {
				int pLineSize = 6;

				auto color = shot.hurt ? ImColor ( 255, 255, 255 ) : ImColor ( 255, 0, 0 );

				if ( shot.hit ) {
					c_menu::get ( ).draw->AddLine ( ImVec2 ( to.x - pLineSize / 2, to.y - pLineSize / 2 ), ImVec2 ( to.x - ( pLineSize ), to.y - ( pLineSize ) ), color );
					c_menu::get ( ).draw->AddLine ( ImVec2 ( to.x - pLineSize / 2, to.y + pLineSize / 2 ), ImVec2 ( to.x - ( pLineSize ), to.y + ( pLineSize ) ), color );
					c_menu::get ( ).draw->AddLine ( ImVec2 ( to.x + pLineSize / 2, to.y + pLineSize / 2 ), ImVec2 ( to.x + ( pLineSize ), to.y + ( pLineSize ) ), color );
					c_menu::get ( ).draw->AddLine ( ImVec2 ( to.x + pLineSize / 2, to.y - pLineSize / 2 ), ImVec2 ( to.x + ( pLineSize ), to.y - ( pLineSize ) ), color );
				}
				else {
					c_menu::get ( ).draw->AddCircle ( to, 30, 15, color );
				}

				c_menu::get ( ).draw->AddLine ( from, to, color );


			}
		}
	}*/
}