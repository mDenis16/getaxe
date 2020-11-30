#include "events.hpp"

event_listener events;

void event_listener::fire_game_event( i_game_event* event ) {
	switch ( fnv::hash (event->get_name() ) ) {
	case fnv::hash("player_hurt" ):
		shot_processor::hurt_event ( event );
		resolver::event_logs::player_hurt ( event );
		break;
	case fnv::hash ( "bullet_impact" ):
		anti_aim::event_logs::bullet_impact ( event );
		shot_processor::bullet_impact ( event );
		resolver::event_logs::bullet_impact ( event );
		break;
	case fnv::hash ( "weapon_fire" ):
		shot_processor::weapon_fire ( event );
		resolver::event_logs::weapon_fire ( event );
		break;
	case fnv::hash ( "player_death" ):
		visuals::player::player_death ( event );
		break;
	case fnv::hash ( "round_prestart" ):
		event_manager::round_prestart ( event );
		break;
	case fnv::hash ( "round_end" ):
		event_manager::round_end ( event );
		break;
	default:
		break;
	}
	
}

int event_listener::get_event_debug_id( ) {
	return EVENT_DEBUG_ID_INIT;
}

void event_listener::setup( ) {
	g_event_listener = new event_listener( );
	if ( !g_event_listener )
		return;

	interfaces::event_manager->add_listener( this, "player_hurt", false );
	interfaces::event_manager->add_listener( this, "bullet_impact", false );
	interfaces::event_manager->add_listener( this, "weapon_fire", false );
	interfaces::event_manager->add_listener ( this, "player_death", false );
	interfaces::event_manager->add_listener ( this, "round_prestart", false );
	interfaces::event_manager->add_listener ( this, "round_end", false );
	if ( !interfaces::event_manager->find_listener( this, "weapon_fire" ) || !interfaces::event_manager->find_listener( this, "bullet_impact" ) ||
		!interfaces::event_manager->find_listener( this, "player_hurt" ) ) {
		throw std::runtime_error( "failed to initialize event listeners." );
		return;
	}

}

void event_listener::release( ) {
	interfaces::event_manager->remove_listener( this );
}