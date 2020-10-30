#include "events.hpp"

event_listener events;

void event_listener::fire_game_event( i_game_event* event ) {

	if ( !strcmp( event->get_name( ), "player_hurt" ) ) {
		shot_processor::hurt_event( event );
	}
	else if ( strstr( event->get_name( ), "bullet_impact" ) ) {
		player_manager::event_logs::bullet_impact( event );
	
		shot_processor::bullet_impact( event );
		anti_aim::event_logs::bullet_impact( event );
	}
	else if ( strstr( event->get_name( ), "weapon_fire" ) ) {
		shot_processor::weapon_fire( event );

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

	if ( !interfaces::event_manager->find_listener( this, "weapon_fire" ) || !interfaces::event_manager->find_listener( this, "bullet_impact" ) ||
		!interfaces::event_manager->find_listener( this, "player_hurt" ) ) {
		throw std::runtime_error( "failed to initialize event listeners." );
		return;
	}

}

void event_listener::release( ) {
	interfaces::event_manager->remove_listener( this );
}