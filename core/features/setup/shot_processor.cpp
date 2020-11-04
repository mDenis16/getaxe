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
	
	if ( ragebot::last_target_index == -1 )
		return;
	if ( !ragebot::get_last_target.entity )
		return;
	

	shot_processor::add_shot( csgo::local_player->get_eye_pos( ), ragebot::last_target_index, ragebot::get_last_target.backtrack, ragebot::get_last_target.record );
	ragebot::get_last_target.entity = nullptr;
	ragebot::last_target_index = -1;
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

void shot_processor::bullet_impact( i_game_event* event ) {
	auto userid = event->get_int( "userid" );
	if ( !userid )
		return;

	if ( !interfaces::engine->is_connected( ) && !interfaces::engine->is_in_game( ) )
		return;

	auto engine_userid = interfaces::engine->get_player_for_user_id( userid );
	if ( !engine_userid )
		return;
	if ( engine_userid != interfaces::engine->get_local_player( ) )
		return;




	vec3_t bullet_impact = vec3_t( event->get_float( "x" ), event->get_float( "y" ), event->get_float( "z" ) );

	auto shot = closest_shot( interfaces::globals->tick_count );
	if ( shot ) {
		shot->hitpos = bullet_impact;
		if ( shot->hit )
			return;
		auto entity = reinterpret_cast< player_t* >( interfaces::entity_list->get_client_entity( shot->enemy_index ) );

		if ( !entity )
			return;
		if ( !entity->is_alive( ) )
			return;
		if ( entity->dormant( ) )
			return;
		if ( shot->is_backtrack )
			player_manager::restore_record(entity, shot->backtrack );
		trace_t tr;
		ray_t ray;
		ray.initialize( shot->shotpos, bullet_impact );
		interfaces::trace_ray->clip_ray_to_entity( ray, MASK_SHOT_HULL | CONTENTS_HITBOX, entity, &tr );

		if ( tr.entity == entity )
		{
			shot->hit = true;
			shot->hitpos = tr.end;
		}
		if ( shot->is_backtrack )
			player_manager::restore_player(entity  );
	}

}

void shot_processor::add_shot( const vec3_t shotpos, const int& entity_index, bool backtrack, player_manager::backtrack_records_t record) {
	
		shot_data shot;
		shot.backtrack = record; shot.enemy_index = entity_index; shot.hit = false; shot.is_backtrack = backtrack; shot.tick = interfaces::globals->tick_count; shot.hurt = false;
		shot.shotpos = shotpos;
		shot.approved = false;
		shot.curtime = interfaces::globals->cur_time;
		
		shots.push_back( shot );
	
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

			if ( shot->hit && !shot->hurt )
			{
				if ( shot->is_backtrack ) {
					interfaces::console->console_printf( "[lua debug] Missed shot due resolver. " );
					std::stringstream ss;
					ss << "Missed  " << name << " in " << hitgroup << " for " << shot->hit_info.damage << " damage due to bad lagcomp [ " << math::time_to_ticks(entity->simulation_time() - shot->backtrack.simtime) << " ] "   << "curtime " << interfaces::globals->cur_time;
				//	visuals::event_.push_front( visuals::loginfo( ss.str( ), color( 255, 255, 255 ), interfaces::globals->cur_time ) );
					visuals::notifications::add ( ss.str ( ));
				}
				else {
					interfaces::console->console_printf( "[lua debug] Missed shot due resolver. " );
					std::stringstream ss;
					ss << "Missed  " << name << " in " << hitgroup << " for " << shot->hit_info.damage << " damage due to resolver.";
					//visuals::event_.push_front( visuals::/loginfo( ss.str( ), color( 255, 255, 255 ), interfaces::globals->cur_time ) );
					visuals::notifications::add ( ss.str ( ) );
					resolver::resolver_data [ shot->enemy_index ].missed_shots++;
				}
				
			}
			else if ( !shot->hit )
			{
				std::stringstream ss;
				ss << "Missed  " << name << " in " << hitgroup << " for " << shot->hit_info.damage << " damage due to spread.";
				visuals::notifications::add ( ss.str ( ) );
				//visuals::event_.push_front( visuals::loginfo( ss.str( ), color( 255, 255, 255 ), interfaces::globals->cur_time ) );
			}
			else if ( shot->hurt && shot->hit )
			{

				interfaces::console->console_printf( "[lua debug] Hit entity. " );
				std::stringstream ss;
				if ( shot->is_backtrack ) {
					ss << "Hit  backtrack " << name << " in " << hitgroup << " for " << shot->hit_info.damage << " damage. ticks [ " << math::time_to_ticks( entity->simulation_time( ) - shot->backtrack.simtime ) << " ]" << "curtime " << interfaces::globals->cur_time;
				}else
					ss << "Hit  " << name << " in " << hitgroup << " for " << shot->hit_info.damage << " damage.";
				
				visuals::notifications::add ( ss.str ( ) );
				//visuals::event_.push_front( visuals::loginfo( ss.str( ), color( 255, 255, 255 ), interfaces::globals->cur_time ) );
			}
		//	interfaces::debug_overlay->add_line_overlay( shot->shotpos, shot->hitpos, 255, 0, 0, false, 0.5f );
			shot->approved = true;
		}
		if ( (interfaces::globals->cur_time - shot->curtime) > 5.f )
			shots.erase( shots.begin( ) + i );
		
	}
	
}