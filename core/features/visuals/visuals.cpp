#include "../features.hpp"
#include "../../menu/render/menu_render.hpp"
#include "../../menu/ImGui/imgui.h"
#include "../../menu/ImGui/imgui_internal.h"

visuals::data visuals::m_data;//ragebot::hitscan_points
void visuals::draw_debug_points ( ) {
	for ( auto point : ragebot::hitscan_points ) {
		vec3_t w2s;
		if ( interfaces::debug_overlay->world_to_screen ( point.point, w2s ) ) {
			auto color = point.is_safe ? ImColor ( 50, 168, 82, 255 ) : ImColor ( 189, 43, 26, 255 );
			c_menu::get ( ).draw->AddCircleFilled ( ImVec2 ( w2s.x, w2s.y ), 2.5f, color, 30 );
		}
	}
}