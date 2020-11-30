#include "helpers.h"
#include "../features/features.hpp"

namespace local_player {
	data m_data;
	void begin_tick ( c_usercmd * cmd ) {
		m_data.orig_viewangle = cmd->viewangles;
		m_data.pointer = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( interfaces::engine->get_local_player ( ) ) );
		m_data.alive = m_data.pointer->health ( ) > 0;
		if ( m_data.alive ) {
			m_data.active_weapon = m_data.pointer->active_weapon ( );
			if ( m_data.active_weapon ) {
				m_data.weapon_data = m_data.active_weapon->get_weapon_data ( );
				m_data.have_weapon = true;
			}
			m_data.eye_position = engine_prediction::unpredicted_eye;
			m_data.velocity = engine_prediction::unpredicted_velocity;
		}
	}

	void end_tick ( c_usercmd * cmd ) {
	
	}
}