#include "../../features.hpp"
#include "../../../render/gui/gui.h"


damage_log::damage_log ( const int & _dmg, const vec3_t & _pos ) {
	dmg = _dmg;
	position = _pos;
	time = ImGui::GetTime();
}

void damage_indicator::addDmg ( damage_log * log ) {
	logs.push_back ( log );
}


double easeInBack ( double t ) {
	return t * t * ( 2.70158 * t - 1.70158 );
}
void damage_indicator::on_render ( ) {
	if ( !visuals::render ) return;
	//to add settings for dmg indicator

	for ( auto & log : logs ) {

		float progress = ImGui::GetTime ( ) - log->time; 
		ImVec2 screen;

		vec3_t new_pos = vec3_t ( log->position.x, log->position.y, std::lerp( log->position.z + 30.f, log->position.z + 220.f, (float) easeInBack ( std::clamp ( progress * 0.6f, 0.f, 1.f ) )));
		
		if ( visuals::world_to_screen ( new_pos, screen ) ) {
			int alpha = std::lerp ( 1, 255, 1.f - std::clamp ( progress * 0.55f, 0.f, 1.f ) );

			ImVec2 size = ImGui::CalcTextSize ( std::to_string ( log->dmg ).c_str ( ), 13.f, ui::font_widgets );

			visuals::render->AddText ( ui::font_widgets, 13.f, ImVec2 ( screen.x + 2.f - size.x / 2.f, screen.y + 2.f ), ImColor ( 0, 0, 0, alpha ), std::to_string ( log->dmg ).c_str ( ) );
			visuals::render->AddText ( ui::font_widgets, 13.f, ImVec2 ( screen.x - size.x / 2.f, screen.y  + 2.f ), ImColor ( 255, 255, 255, alpha ), std::to_string( log->dmg).c_str() );
		}
	}
}

damage_indicator * dmg_indicator = new damage_indicator ( );