#include "../features.hpp"
#include "../../menu/render/menu_render.hpp"
#include "../../menu/ImGui/imgui.h"
#include "../../menu/ImGui/imgui_internal.h"
#include "../../features/nade prediction/nade_prediction.hpp"

visuals::data visuals::m_data;//ragebot::hitscan_points
visuals::local_data visuals::m_local;
constexpr auto units2meters ( float units ) noexcept {
	return units * 0.0254f;
}

void visuals::draw_debug_points ( ) {
	


}

bool visuals::get_playerbox ( entity_t * ent, visuals::box & in) {
	matrix3x4_t & tran_frame = ent->coord_frame ( );

	const vec3_t min = ent->mins ( );
	const vec3_t max = ent->maxs ( );

	vec3_t screen_boxes [ 8 ];

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

	for ( int i = 0; i <= 7; i++ )
		if ( !interfaces::debug_overlay->world_to_screen ( math::vector_transform ( points [ i ], tran_frame ), screen_boxes [ i ] ) )
			return false;

	vec3_t box_array [ ] = {
		screen_boxes [ 3 ], // fl
		screen_boxes [ 5 ], // br
		screen_boxes [ 0 ], // bl
		screen_boxes [ 4 ], // fr
		screen_boxes [ 2 ], // fr
		screen_boxes [ 1 ], // br
		screen_boxes [ 6 ], // bl
		screen_boxes [ 7 ] // fl
	};

	float left = screen_boxes [ 3 ].x, bottom = screen_boxes [ 3 ].y, right = screen_boxes [ 3 ].x, top = screen_boxes [ 3 ].y;

	for ( int i = 0; i <= 7; i++ ) {
		if ( left > box_array [ i ].x )
			left = box_array [ i ].x;

		if ( bottom < box_array [ i ].y )
			bottom = box_array [ i ].y;

		if ( right < box_array [ i ].x )
			right = box_array [ i ].x;

		if ( top > box_array [ i ].y )
			top = box_array [ i ].y;
	}

	in.x = left;
	in.y = top;
	in.w = right - left;
	in.h = bottom - top;
	return true;
}
void visuals::draw_noscope ( ) {
	if ( !config.visuals_world_removals [ 4 ] )
		return;
   const  auto color_1 = ImColor ( 153, 102, 204, 255 );
   const  auto color_2 = ImColor ( 153, 102, 204, 0 );
    c_menu::get ( ).draw->AddRectFilled ( ImVec2 ( static_cast< float >( csgo::screen_width) / 2.f - 0.5f, 0 ), ImVec2 ( static_cast< float >( csgo::screen_width) / 2.f + 0.5f, static_cast< float >( csgo::screen_height)), color_1 );
    c_menu::get ( ).draw->AddRectFilled ( ImVec2(0, static_cast<float>(csgo::screen_height) / 2.f - 0.5f), ImVec2( static_cast< float >( csgo::screen_width ), static_cast< float >( csgo::screen_height ) / 2.f + 0.5f), color_1);

}
void visuals::autowall_crosshair ( ) {
	if ( !config.visuals_world_autowall_crosshair )
		return;

	if ( m_local.autowall_crosshair.points.size() > 0 )
	m_local.autowall_crosshair.points.clear ( );

	if ( !local_player::m_data.pointer )
		return;

	if ( !local_player::m_data.pointer->is_alive() )
		return;

	if ( local_player::m_data.pointer->health ( ) <= 0 )
		return;
	vec3_t eye_angles;
	interfaces::engine->get_view_angles ( eye_angles );

	

	vec3_t dst, forward;
	auto start = local_player::m_data.pointer->get_eye_pos ( );

	forward = math::angle_vector ( eye_angles );
	dst = start + ( forward * 8196.f );

	trace_t tr;
	ray_t ray;
	trace_filter filter;
	filter.skip = local_player::m_data.pointer;

	
	auto dir = ( dst - start ).normalized ( );

	ray.initialize ( start, dst );
	interfaces::trace_ray->trace_ray ( ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr );



	auto center = tr.end;
	
	vec3_t size = vec3_t ( 4, 4, 0 );

	float anglez = tr.plane.normal.dot_product ( vec3_t ( 0, 0, 1 )) ;
	float invanglez = tr.plane.normal.dot_product ( vec3_t ( 0, 0, -1 ) );
	float angley = tr.plane.normal.dot_product ( vec3_t ( 0, 1, 0 ) );
		float invangley = tr.plane.normal.dot_product ( vec3_t ( 0, -1, 0 ) );
	float anglex = tr.plane.normal.dot_product ( vec3_t ( 1, 0, 0 ) );
	float invanglex = tr.plane.normal.dot_product ( vec3_t ( -1, 0, 0 ) );

	vec3_t add_top_left, add_top_right,
		add_bot_left, add_bot_right;


	int angle = 0;


	if ( anglez > 0.5 || invanglez > 0.5 )
		angle = 0;

	else if ( angley > 0.5 || invangley > 0.5 )
		angle = 1;

	else if ( anglex > 0.5 || invanglex > 0.5 )
		angle = 2;


	switch ( angle ) {
	case 0: // Z-AXIS
		add_top_left = vec3_t ( -size.x, -size.y, 0 );
		add_top_right = vec3_t ( size.x, -size.y, 0 );
		//
		add_bot_left = vec3_t ( -size.x, size.y, 0 );
		add_bot_right = vec3_t ( size.x, size.y, 0 );

		break;
	case 1: // Y-AXIS
		add_top_left = vec3_t ( -size.x, 0, -size.y );
		add_top_right = vec3_t ( size.x, 0, -size.y );
		//
		add_bot_left = vec3_t ( -size.x, 0, size.y );
		add_bot_right = vec3_t ( size.x, 0, size.y );

		break;
	case 2: // X-AXIS
		add_top_left = vec3_t ( 0, -size.y, -size.x );
		add_top_right = vec3_t ( 0, -size.y, size.x );
		//
		add_bot_left = vec3_t ( 0, size.y, -size.x );
		add_bot_right = vec3_t ( 0, size.y, size.x );

		break;
	}

	//top line
	vec3_t c_top_left = center + add_top_left;
	vec3_t c_top_right = center + add_top_right;

	//bottom line
	vec3_t c_bot_left = center + add_bot_left;
	vec3_t c_bot_right = center + add_bot_right;

	vec3_t top_left, top_right, bot_left, bot_right;

	if ( interfaces::debug_overlay->world_to_screen ( c_top_left, top_left ) &&
		interfaces::debug_overlay->world_to_screen ( c_top_right, top_right ) &&
		interfaces::debug_overlay->world_to_screen ( c_bot_left, bot_left ) &&
		interfaces::debug_overlay->world_to_screen ( c_bot_right, bot_right ) ) {
		m_local.autowall_crosshair.points.push_back ( ImVec2 ( top_left.x, top_left.y ) );
		m_local.autowall_crosshair.points.push_back ( ImVec2 ( bot_left.x, bot_left.y ) );
		m_local.autowall_crosshair.points.push_back ( ImVec2 ( bot_right.x, bot_right.y ) );
		m_local.autowall_crosshair.points.push_back ( ImVec2 ( top_right.x, top_right.y ) );
	}
	
}
void visuals::draw_texture_polygon ( ) {

}
ImVec2 visuals::render_text ( const ImFont * font, int size, const float textCfg [ 4 ], const char * text, const ImVec2 & pos, bool centered , bool adjustHeight  ) noexcept {


	ImGuiWindow * window = ImGui::GetCurrentWindow ( );


	const auto textSize = ImGui::CalcTextSize ( text );

	const auto horizontalOffset = centered ? textSize.x / 2 : 0.0f;
	const auto verticalOffset = adjustHeight ? textSize.y : 0.0f;

	
	window->DrawList->AddText ( font, static_cast<float>(size), { pos.x + horizontalOffset + 1.0f, pos.y - verticalOffset + 1.0f }, ImColor ( textCfg [ 0 ], textCfg [ 1 ], textCfg [ 2 ], textCfg [ 3 ] ) & IM_COL32_A_MASK, text );
	window->DrawList->AddText ( font, static_cast<float>(size), { pos.x + horizontalOffset, pos.y - verticalOffset }, ImColor ( textCfg [ 0 ], textCfg [ 1 ], textCfg [ 2 ], textCfg [ 3 ] ), text );

	return textSize;
}
void visuals::local_esp_think ( ) {
    if ( local_player::m_data.pointer && local_player::m_data.pointer->health ( ) > 0 ) {
        if ( local_player::m_data.pointer->active_weapon ( ) )
            m_local.scoped = local_player::m_data.pointer->is_scoped ( );
        else
            m_local.scoped = false;
    }
    else {
        m_local.scoped = false;
    }
	//autowall_crosshair ( );

	

   
}
void visuals::capsule_overlay ( aimbot::target target, float duration, color color ) {
	if ( !target.player )
		return;

	for ( int i = 0; i < target.hitbox_set->hitbox_count; i++ ) {
		auto pHitbox = target.hitbox_set->hitbox ( i );
		if ( !pHitbox )
			continue;

		vec3_t mins = math::vector_transform ( pHitbox->mins, target.aimbot.record.bone [ pHitbox->bone ] ); 
        vec3_t maxs = math::vector_transform ( pHitbox->maxs, target.aimbot.record.bone [ pHitbox->bone ] );

		if ( pHitbox->radius > -1 ) {
			interfaces::debug_overlay->capsule_overlay ( mins, maxs, pHitbox->radius, color.r, color.g, color.b, color.a, duration );
		}
	}
}
void visuals::local_esp ( ) {

	if ( m_local.scoped )
		draw_noscope ( );

	nade_pred.draw ( );

	//c_menu::get ( ).draw->AddConvexPolyFilled ( m_local.autowall_crosshair.points.data ( ), m_local.autowall_crosshair.points.size ( ), m_local.autowall_crosshair.dmg > 1.f ? ImColor( 87, 173, 21, 200) : ImColor( 88, 19, 20,  200 ));
	//c_menu::get ( ).draw->AddPolyline ( m_local.autowall_crosshair.points.data ( ), m_local.autowall_crosshair.points.size ( ), m_local.autowall_crosshair.dmg > 1.f ? ImColor ( 87, 173, 21, 255 ) : ImColor ( 88, 19, 20, 255 ), true, 1.2f );

	aimbot::render ( );
	shot_processor::draw_shots ( );
	visuals::projectiles::grenades::proximity::draw ( );
	const float center_x = csgo::screen_width / 2.f;
	const float center_y = csgo::screen_height / 2.f;

	if (localdata.force_safe_point_on_key )
	    c_menu::get ( ).draw->AddText ( ImVec2( center_x + 15, center_y - 35 ), ImColor ( 255, 0, 255, 255 ), "SAFEPOINT");

	if ( localdata.force_min_dmg_on_key )
		c_menu::get ( ).draw->AddText ( ImVec2 ( center_x + 15, center_y - 75 ), ImColor ( 255, 0, 255, 255 ), "MIN DMG 15" );

	if ( localdata.force_low_delta_on_key )
		c_menu::get ( ).draw->AddText ( ImVec2 ( center_x + 15, center_y - 105 ), ImColor ( 255, 0, 0, 255 ), "dodge enemy" );

	if ( localdata.force_invert_resolver_on_key )
		c_menu::get ( ).draw->AddText ( ImVec2 ( center_x + 15, center_y - 145 ), ImColor ( 255, 0, 0, 255 ), "invert resolver" );

	if ( localdata.force_invert_doubletap_on_key )
		c_menu::get ( ).draw->AddText ( ImVec2 ( center_x + 15, center_y - 185 ), ImColor ( 255, 0, 0, 255 ), "doubletap" );

	//ImVec2 scr;
	//if ( visuals::world_to_screen ( engine_prediction::unpredicted_eye, scr ) )
	//	c_menu::get ( ).draw->AddCircleFilled ( scr, 8.f, ImColor ( 255, 255, 255, 255 ) );
	/*for ( auto point : anti_aim::points ) {
		ImVec2 point2d = ImVec2 ( );
		if ( visuals::world_to_screen ( point.point, point2d ) ) {
			c_menu::get ( ).draw->AddText ( point2d, ImColor(255,255,255,255), std::to_string(point.dmg).c_str() );
		}
	}*/
}

bool visuals::world_to_screen ( const vec3_t & origin, ImVec2 & screen ) {
	vec3_t screen_game = vec3_t ( );
	const auto screen_transform = [ &origin, &screen ] ( ) -> bool {
		static std::uint8_t * matrix;
		if ( !matrix ) {
			matrix = static_cast< std::uint8_t * >( utilities::pattern_scan ( "client.dll", "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9" ) );
			matrix += 3;
			matrix = *reinterpret_cast< std::uint8_t ** >( matrix );
			matrix += 176;
		}

		const matrix3x4_t & w2s_matrix = *reinterpret_cast< matrix3x4_t * >( matrix );
		screen.x = w2s_matrix.m_flMatVal [ 0 ][ 0 ] * origin.x + w2s_matrix.m_flMatVal [ 0 ][ 1 ] * origin.y + w2s_matrix.m_flMatVal [ 0 ][ 2 ] * origin.z + w2s_matrix.m_flMatVal [ 0 ][ 3 ];
		screen.y = w2s_matrix.m_flMatVal [ 1 ][ 0 ] * origin.x + w2s_matrix.m_flMatVal [ 1 ][ 1 ] * origin.y + w2s_matrix.m_flMatVal [ 1 ][ 2 ] * origin.z + w2s_matrix.m_flMatVal [ 1 ][ 3 ];


		float w = w2s_matrix.m_flMatVal [ 3 ][ 0 ] * origin.x + w2s_matrix.m_flMatVal [ 3 ][ 1 ] * origin.y + w2s_matrix.m_flMatVal [ 3 ][ 2 ] * origin.z + w2s_matrix.m_flMatVal [ 3 ][ 3 ];

		if ( w < 0.001f ) {
			screen.x *= 100000;
			screen.y *= 100000;
			return true;
		}

		float invw = 1.f / w;
		screen.x *= invw;
		screen.y *= invw;

		return false;
	};

	if ( !screen_transform ( ) ) {
		int screen_width, screen_height;
		interfaces::engine->get_screen_size ( screen_width, screen_height );

		screen.x = ( screen_width * 0.5f ) + ( screen.x * screen_width ) * 0.5f;
		screen.y = ( screen_height * 0.5f ) - ( screen.y * screen_height ) * 0.5f;

		return true;
	}

	return false;
}