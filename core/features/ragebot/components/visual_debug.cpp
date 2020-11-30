#include "../../features.hpp"
#include "../../../helpers/helpers.h"
#include "../../../menu/render/menu_render.hpp"
namespace aimbot {
	visual_debug m_visual_debug;

	void render ( ) {
		if ( interfaces::debug_overlay->world_to_screen ( m_visual_debug.from, m_visual_debug.from_2d ) && interfaces::debug_overlay->world_to_screen ( m_visual_debug.end, m_visual_debug.end_2d ) ) {
			c_menu::get ( ).draw->AddLine ( ImVec2 ( m_visual_debug.from_2d.x, m_visual_debug.from_2d.y ), ImVec2 ( m_visual_debug.end_2d.x, m_visual_debug.end_2d.y ), ImColor ( 255, 0, 0, 255 ), 1.2f );
		}
		if ( interfaces::debug_overlay->world_to_screen ( m_visual_debug.aimbot_hit, m_visual_debug.aimbot_hit_2d ) ) {
			c_menu::get ( ).draw->AddCircle ( ImVec2 ( m_visual_debug.aimbot_hit_2d.x, m_visual_debug.aimbot_hit_2d.y ), 4.f, ImColor ( 255, 221, 24, 255 ) );
		}
	}
	void think ( ) {
		

	}
}