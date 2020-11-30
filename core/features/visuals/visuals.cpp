#include "../features.hpp"
#include "../../menu/render/menu_render.hpp"
#include "../../menu/ImGui/imgui.h"
#include "../../menu/ImGui/imgui_internal.h"
#include "../../features/nade prediction/nade_prediction.hpp"

visuals::data visuals::m_data;//ragebot::hitscan_points
visuals::local_data visuals::m_local;
void visuals::draw_debug_points ( ) {
	


}
float visuals::draw_text ( ImFont * pFont, const std::string text, const ImVec2 & pos, float size, float const * color, float shadow, bool center, bool bold ) {
	ImGuiWindow * window = ImGui::GetCurrentWindow ( );


	std::stringstream stream ( text );
	std::string line;

	float y = 0.0f;
	int i = 0;


	while ( std::getline ( stream, line ) && line.length ( ) != 0 ) {
		ImVec2 textSize = pFont->CalcTextSizeA ( size, FLT_MAX, 0.0f, line.c_str ( ) );

		if ( center ) {
			if ( bold ) {
				window->DrawList->AddText ( pFont, size, ImVec2 ( ( pos.x - textSize.x / 2.0f ) + shadow, ( pos.y + textSize.y * i ) + shadow ), ImColor ( 0, 0, 0, ( int ) color [ 3 ] ), line.c_str ( ) );
				window->DrawList->AddText ( pFont, size, ImVec2 ( ( pos.x - textSize.x / 2.0f ) - shadow, ( pos.y + textSize.y * i ) - shadow ), ImColor ( 0, 0, 0, ( int ) color [ 3 ] ), line.c_str ( ) );
				window->DrawList->AddText ( pFont, size, ImVec2 ( ( pos.x - textSize.x / 2.0f ) + shadow, ( pos.y + textSize.y * i ) - shadow ), ImColor ( 0, 0, 0, ( int ) color [ 3 ] ), line.c_str ( ) );
				window->DrawList->AddText ( pFont, size, ImVec2 ( ( pos.x - textSize.x / 2.0f ) - shadow, ( pos.y + textSize.y * i ) + shadow ), ImColor ( 0, 0, 0, ( int ) color [ 3 ] ), line.c_str ( ) );

			}
			window->DrawList->AddText ( pFont, size, ImVec2 ( pos.x - textSize.x / 2.0f, pos.y + textSize.y * i ), ImColor ( color [ 0 ], color [ 1 ], color [ 2 ], color [ 3 ] ), line.c_str ( ) );
		}
		else {
			if ( bold ) {
				window->DrawList->AddText ( pFont, size, ImVec2 ( ( pos.x ) + 1, ( pos.y + textSize.y * i ) + shadow ), ImColor ( 0, 0, 0, ( int ) color [ 3 ] ), line.c_str ( ) );
				window->DrawList->AddText ( pFont, size, ImVec2 ( ( pos.x ) - 1, ( pos.y + textSize.y * i ) - shadow ), ImColor ( 0, 0, 0, ( int ) color [ 3 ] ), line.c_str ( ) );
				window->DrawList->AddText ( pFont, size, ImVec2 ( ( pos.x ) + 1, ( pos.y + textSize.y * i ) - shadow ), ImColor ( 0, 0, 0, ( int ) color [ 3 ] ), line.c_str ( ) );
				window->DrawList->AddText ( pFont, size, ImVec2 ( ( pos.x ) - 1, ( pos.y + textSize.y * i ) + shadow ), ImColor ( 0, 0, 0, ( int ) color [ 3 ] ), line.c_str ( ) );
			}
			window->DrawList->AddText ( pFont, size, ImVec2 ( pos.x, pos.y + textSize.y * i ), ImColor ( color [ 0 ], color [ 1 ], color [ 2 ], color [ 3 ] ), line.c_str ( ) );
		}

		y = pos.y + textSize.y * ( i + 1 );
		i++;
	}

	return y;
}
bool visuals::get_playerbox ( entity_t * ent, visuals::box & in) {
	vec3_t origin, min, max, flb, brt, blb, frt, frb, brb, blt, flt;
	float left, top, right, bottom;

	origin = ent->get_absolute_origin( );
	min = ent->collideable( )->mins( ) + origin;
	max = ent->collideable( )->maxs( ) + origin;

	vec3_t points [ ] = {
		vec3_t ( min.x, min.y, min.z ),
		vec3_t ( min.x, max.y, min.z ),
		vec3_t ( max.x, max.y, min.z ),
		vec3_t ( max.x, min.y, min.z ),
		vec3_t ( max.x, max.y, max.z ),
		vec3_t ( min.x, max.y, max.z ),
		vec3_t ( min.x, min.y, max.z ),
		vec3_t ( max.x, min.y, max.z )
	};

	if ( !interfaces::debug_overlay->world_to_screen ( points [ 3 ], flb ) || !interfaces::debug_overlay->world_to_screen ( points [ 5 ], brt )
		|| !interfaces::debug_overlay->world_to_screen ( points [ 0 ], blb ) || !interfaces::debug_overlay->world_to_screen ( points [ 4 ], frt )
		|| !interfaces::debug_overlay->world_to_screen ( points [ 2 ], frb ) || !interfaces::debug_overlay->world_to_screen ( points [ 1 ], brb )
		|| !interfaces::debug_overlay->world_to_screen ( points [ 6 ], blt ) || !interfaces::debug_overlay->world_to_screen ( points [ 7 ], flt ) )
		return false;

	vec3_t arr [ ] = { flb, brt, blb, frt, frb, brb, blt, flt };

	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	for ( int i = 1; i < 8; i++ ) {
		if ( left > arr [ i ].x )
			left = arr [ i ].x;
		if ( bottom < arr [ i ].y )
			bottom = arr [ i ].y;
		if ( right < arr [ i ].x )
			right = arr [ i ].x;
		if ( top > arr [ i ].y )
			top = arr [ i ].y;
	}

	in.x = ( int ) left;
	in.y = ( int ) top;
	in.w = int ( right - left );
	in.h = int ( bottom - top );

	return true;
}
void visuals::draw_noscope ( ) {
    const float screen_size[2] = { csgo::screen_width, csgo::screen_height };
    auto width = 2;
    auto color_1 = ImColor ( 153, 102, 204, 255 );
    auto color_2 = ImColor ( 153, 102, 204, 0 );
    c_menu::get ( ).draw->AddRectFilled ( ImVec2 ( csgo::screen_width / 2.f - 0.5f, 0 ), ImVec2 ( csgo::screen_width / 2.f + 0.5f, csgo::screen_height), color_1 );
    c_menu::get ( ).draw->AddRectFilled ( ImVec2(0, csgo::screen_height / 2.f - 0.5f), ImVec2(csgo::screen_width, csgo::screen_height / 2.f + 0.5f), color_1);
}
void visuals::local_esp_think ( ) {
    if ( csgo::local_player && csgo::local_player->health ( ) > 0 ) {
        if ( csgo::local_player->active_weapon ( ) )
            m_local.scoped = csgo::local_player->is_scoped ( );
        else
            m_local.scoped = false;
    }
    else {
        m_local.scoped = false;
    }

   
}/*
void visuals::capsule_overlay ( aimbot::target target, float duration, color color, matrix_t pBoneToWorldOut [ 128 ] ) {
	if ( !target.player )
		return;

	for ( int i = 0; i < target.hitbox_set->hitbox_count; i++ ) {
		auto pHitbox = target.hitbox_set->hitbox ( i );
		if ( !pHitbox )
			continue;

		vec3_t mins = math::vector_transform ( pHitbox->mins, pBoneToWorldOut [ pHitbox->bone ] ); 
        vec3_t maxs = math::vector_transform ( pHitbox->maxs, pBoneToWorldOut [ pHitbox->bone ] );

		if ( pHitbox->radius > -1 ) {
			interfaces::debug_overlay->capsule_overlay ( mins, maxs, pHitbox->radius, color.r, color.g, color.b, color.a, duration );
		}
	}
}*/
void visuals::local_esp ( ) {

    if ( m_local.scoped )
        draw_noscope ( );

   // vec3_t w2s = vec3_t ( );
  //  if ( (int)autopeek::m_data.state > 0 && interfaces::debug_overlay->world_to_screen ( autopeek::m_data.forward_position, w2s ) ) /{
  //      c_menu::get ( ).draw->AddCircleFilled ( ImVec2 ( w2s.x, w2s.y ), 15, ImColor ( 255, 0, 0, 200 ) );
  //  }
	aimbot::render ( );
    nade_pred.draw ( );
}
