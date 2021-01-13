#include "../../features.hpp"
/*
const char* hitgroup_name( int hitgroup ) {
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

void misc::event_logs::player_hurt( i_game_event* event ) {
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

		auto hitgroup = hitgroup_name( hitbox );
		std::string player_name = info.name;
		std::transform( player_name.begin( ), player_name.end( ), player_name.begin( ), ::tolower );

		if ( entity->team( ) == local_player->team( ) )
			return;

		if ( interfaces::chat_element ) {
			interfaces::chat_element->chat_print( 0, 0, " ""\x01""[""\x03""amtlib""\x01""]""\x01"" hit %s in %s for %d \n", player_name.c_str( ), hitgroup, damage );
		}
	}
}

void misc::event_logs::item_purchase( i_game_event* event ) {
	auto userid = event->get_int( "userid" );
	if ( !userid )
		return;

	if ( !interfaces::engine->is_connected( ) && !interfaces::engine->is_in_game( ) )
		return;

	auto engine_userid = interfaces::engine->get_player_for_user_id( userid );
	if ( !engine_userid )
		return;

	player_info_t info;
	interfaces::engine->get_player_info( engine_userid, &info );

	auto local_player = reinterpret_cast< player_t* >( interfaces::entity_list->get_client_entity( interfaces::engine->get_local_player( ) ) );
	auto entity = reinterpret_cast< player_t* >( interfaces::entity_list->get_client_entity( engine_userid ) );

	if ( !local_player || !entity )
		return;

	if ( entity->team( ) == local_player->team( ) )
		return;

	std::string player_name = info.name;
	std::transform( player_name.begin( ), player_name.end( ), player_name.begin( ), ::tolower );

	std::string gun = event->get_string( "weapon" );

	if ( interfaces::chat_element ) {
		gun.erase( gun.find( "weapon_" ), 7 );
		interfaces::chat_element->chat_print( 0, 0, " ""\x01""[""\x03""amtlib""\x01""]""\x01"" %s bought %s \n", player_name.c_str( ), gun.c_str( ) );
	}
}*/