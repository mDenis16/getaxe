#pragma once
#include "../../features.hpp"

class event_listener : public i_game_event_listener2
{
public:
	void fire_game_event( i_game_event* event );
	void setup( );
	void release( );
	int get_event_debug_id( );
private:
	event_listener* g_event_listener = nullptr;
};

extern event_listener events;