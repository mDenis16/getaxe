#include "../features.hpp"
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

	shot_data shot;
	shot.tick = interfaces::globals->tick_count;
	shot.target = aimbot::last_target;
	shot.enemy_index = aimbot::last_target.player->index ( ); /*crash after map change should reset it ;)*/
	shot.shotpos = engine_prediction::unpredicted_eye;
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
			shot->hit = true;
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
		if ( shot->hit )
			return;
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

			auto entity = reinterpret_cast< player_t* >( interfaces::entity_list->get_client_entity( shot->enemy_index ) );

			if ( !entity )
				return;

			std::string name = entity->get_info( ).name;

			if ( name.length( ) > 17 )
				name = name.substr( 0, 17 ) + "...";

			std::transform( name.begin( ), name.end( ), name.begin( ), tolower );
			auto hitgroup = hitgroup_text( shot->hit_info.hitgroup );
			auto hitbox = hitbox_text ( ( hitboxes ) shot->target.aimbot.best_point.hitbox );
			if ( shot->hit && !shot->hurt )
			{


					std::stringstream ss;
					resolver::resolver_data [ shot->enemy_index ].missed_shots++;
					resolver::resolver_data [ shot->enemy_index ].side = static_cast<resolver::desync_side>(resolver::resolver_data [ shot->enemy_index ].missed_shots % 3);
					ss << "Missed  " << name << " in " << hitbox << " damage due to resolver.";
			
					visuals::notifications::add ( ss.str ( ) );
					

				
			}
			else if ( !shot->hit )
			{
				std::stringstream ss;
				ss << "Missed  " << name << " in " << hitbox << " damage due to spread.";
				visuals::notifications::add ( ss.str ( ) );
				//visuals::event_.push_front( visuals::loginfo( ss.str ( ), color( 255, 255, 255 ), interfaces::globals->cur_time ) );
			}
			else if ( shot->hurt && shot->hit )
			{

				std::stringstream ss;

				ss << "Hit  " << name << " in " << hitgroup << " for " << shot->hit_info.damage << " damage.  bt (  "  << math::time_to_ticks ( shot->target.player->simulation_time() - shot->target.aimbot.record.simtime ) << " ) " << std::endl;
				
				visuals::notifications::add ( ss.str ( ) );
			
				
			}

			shot->approved = true;
		}
		if ( (interfaces::globals->cur_time - shot->curtime) > 5.f )
			shots.erase( shots.begin( ) + i );
		
	}
	
}