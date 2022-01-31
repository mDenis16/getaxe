#pragma once
#include "../../helpers/helpers.h"


#include <renderlib/imgui/imgui.h>



#include "../visuals.h"
#include <UIFramework/framework_internal.h>
#include "../../overlay.hpp"
#include "../../../render/d3dfont.h"



namespace visuals {
	void visual_player::render_range ( ) {
 
		if ( !( taze || is_knife ) ) return;

		
		if ( taze && !cfg->taser_range )
			return;

		if ( is_knife && !cfg->knife_range )
			return;



			float step = M_PI * 2.0 / 32;
			const float radius = is_knife ? 48.f : 70.f;
			const vec3_t position = player->abs_origin ( );
			ImVec2 screen; vec3_t location;
			location.z = position.z;

			for ( float a = 0; a < ( M_PI * 2.0 ); a += step ) {
				location.x = radius * cos ( a ) + position.x; location.y = radius * sin ( a ) + position.y; 

				if ( visuals::world_to_screen ( location, screen ) )
					warning_range.push_back ( screen );
			}


			render->AddShadowConvexPoly ( warning_range.data ( ), warning_range.size ( ), is_knife  ? cfg->knife_range_color : cfg->taser_range_color, 30.f, ImVec2(0,0) );
			warning_range.clear ( );
	}
	void visual_player::render_name ( ) {




		std::string print ( player_info.fakeplayer ? std::string ( "bot " ).append ( player_info.name ).c_str ( ) : player_info.name );

		std::transform ( print.begin ( ), print.end ( ), print.begin ( ), ::tolower );

		//primitive_string ( true, fonts [ ESP ], box_data.top_center.x, box_data.top_center.y - 15.f, config.player_visual [ type ].name_color, font_center, print.data ( ) );

		float name_size = 0.f;
		if ( cfg->dynamic_name_size ) {
			auto box_size = ( box_data.maxs.x - box_data.mins.x );


			name_size = box_size / cfg->name_size;

		}

		if ( name_size < cfg->name_size - 2 )
			name_size = cfg->name_size - 2;


		auto text_size = ImGui::CalcTextSize ( print.c_str ( ), cfg->dynamic_name_size ? name_size : cfg->name_size, visuals::esp_font );
		auto center_x = box_data.top_center.x - text_size.x / 2.f;
		auto center_y = box_data.top_center.y - 2.f - text_size.y;

		float offset_x = 1.1f;
		float offset_y = 1.1f;



		/*if (cfg->shadow_name )
		render->AddText ( visuals::esp_font, cfg->dynamic_name_size ? name_size : cfg->name_size, ImVec2 ( center_x + offset_x, center_y + offset_y ), ImColor(0,0, 0, 255), print.c_str ( ) );

		render->AddText ( visuals::esp_font, cfg->dynamic_name_size ? name_size : cfg->name_size, ImVec2( center_x, center_y ), config.player_visual [ type ].name_color, print.c_str ( ) );*/
		if ( cfg->name_render_type == 1 ) {
			std::wstring wsTmp ( print.begin ( ), print.end ( ) );
			int flags = c_font::centered_x;

			if ( cfg->shadow_name )
				flags |= c_font::drop_shadow;

			overlay::text ( ImVec2 ( box_data.top_center.x, box_data.top_center.y - 16 ), wsTmp, config.player_visual [ type ].name_color, overlay::fonts_ns::esp, flags );
		}
		else {
		//this->renderer->AddText ( this->icon_font, this->icon_size, ImVec2 ( center_X - text_size.x / 2.f + 2.f, middle_y - text_size.y - 1.175f + offset_y + 2.f), ImColor ( 0, 0, 0, alpha ), this->icon_text.data ( ) );
			if ( cfg->shadow_name )
				render->AddText ( visuals::esp_font, cfg->dynamic_name_size ? name_size : cfg->name_size, ImVec2 ( center_x + 1.f, center_y + 1.f ), ImColor ( 0, 0, 0, 255 ), print.c_str ( ) );

			render->AddText ( visuals::esp_font, cfg->dynamic_name_size ? name_size : cfg->name_size, ImVec2 ( center_x, center_y ), config.player_visual [ type ].name_color, print.c_str ( ) );
		}



	}
	void visual_player::render_box ( ) {

		if ( !player ) {
			return;
		}

		switch ( cfg->box_type ) {
		case NORMAL:
		{

			int old_flags = render->Flags;
			render->Flags = ImDrawListFlags_AntiAliasedLines | ImDrawListFlags_AntiAliasedLines;

			render->AddRect ( this->box_data.mins, this->box_data.maxs, ImColor ( 0, 0, 0, 125 ), cfg->bound_box_border_rounding, 0, cfg->bound_box_border_thickness );
			render->AddRect ( this->box_data.mins, this->box_data.maxs, config.player_visual [ type ].bound_box_color, cfg->bound_box_border_rounding, 0, cfg->bound_box_thickness );

			render->Flags = old_flags;
		}
		break;
		case CORNERED:
		{
			static auto RectangleD = [ ] ( float x, float y, float x1, float y1, ImColor color ) {
				render->AddRectFilled ( ImVec2 ( x, y ), ImVec2 ( x1, y1 ), color );

			};



			int VertLine = ( int ) ( box_data.w * 0.33f );
			int HorzLine = ( int ) ( box_data.h * 0.33f );
			int squareLine = min ( VertLine, HorzLine );

			float x = box_data.x;
			float y = box_data.y;
			float h = box_data.h;
			float w = box_data.w;

			// top-left corner / ImColor
			RectangleD ( x, y, x + squareLine, y + 1, cfg->bound_box_color );
			RectangleD ( x, y, x + 1, y + squareLine, cfg->bound_box_color );

			// top-left corner / outer outline
			RectangleD ( x - 1, y - 1, x + squareLine, y, ImColor ( 10, 10, 10, 190 ) );
			RectangleD ( x - 1, y, x, y + squareLine, ImColor ( 10, 10, 10, 190 ) );

			// top-left corner / inner outline
			RectangleD ( x + 1, y + 1, x + squareLine, y + 2, ImColor ( 10, 10, 10, 190 ) );
			RectangleD ( x + 1, y + 2, x + 2, y + squareLine, ImColor ( 10, 10, 10, 190 ) );

			// top-left corner / missing edges
			RectangleD ( x + squareLine, y - 1, x + squareLine + 1, y + 2, ImColor ( 10, 10, 10, 190 ) );
			RectangleD ( x - 1, y + squareLine, x + 2, y + squareLine + 1, ImColor ( 10, 10, 10, 190 ) );


			// top-right corner / ImColor
			RectangleD ( x + w - squareLine, y, x + w, y + 1, cfg->bound_box_color );
			RectangleD ( x + w - 1, y, x + w, y + squareLine, cfg->bound_box_color );

			// top-right corner / outer outline
			RectangleD ( x + w - squareLine, y - 1, x + w + 1, y, ImColor ( 10, 10, 10, 190 ) );
			RectangleD ( x + w, y, x + w + 1, y + squareLine, ImColor ( 10, 10, 10, 190 ) );

			// top-right corner / inner outline
			RectangleD ( x + w - squareLine, y + 1, x + w - 1, y + 2, ImColor ( 10, 10, 10, 190 ) );
			RectangleD ( x + w - 2, y + 2, x + w - 1, y + squareLine, ImColor ( 10, 10, 10, 190 ) );

			// top-right corner / missing edges
			RectangleD ( x + w - squareLine - 1, y - 1, x + w - squareLine, y + 2, ImColor ( 10, 10, 10, 190 ) );
			RectangleD ( x + w - 2, y + squareLine, x + w + 1, y + squareLine + 1, ImColor ( 10, 10, 10, 190 ) );


			// bottom-left corner / ImColor
			RectangleD ( x, y + h - 1, x + squareLine, y + h, cfg->bound_box_color );
			RectangleD ( x, y + h - squareLine, x + 1, y + h, cfg->bound_box_color );

			// bottom-left corner / outer outline
			RectangleD ( x - 1, y + h, x + squareLine, y + h + 1, ImColor ( 10, 10, 10, 190 ) );
			RectangleD ( x - 1, y + h - squareLine, x, y + h, ImColor ( 10, 10, 10, 190 ) );

			// bottom-left corner / inner outline
			RectangleD ( x + 1, y + h - 2, x + squareLine, y + h - 1, ImColor ( 10, 10, 10, 190 ) );
			RectangleD ( x + 1, y + h - squareLine, x + 2, y + h - 2, ImColor ( 10, 10, 10, 190 ) );

			// bottom-left corner / missing edges
			RectangleD ( x + squareLine, y + h - 2, x + squareLine + 1, y + h + 1, ImColor ( 10, 10, 10, 190 ) );
			RectangleD ( x - 1, y + h - squareLine - 1, x + 2, y + h - squareLine, ImColor ( 10, 10, 10, 190 ) );


			// bottom-right corner / ImColor
			RectangleD ( x + w - squareLine, y + h - 1, x + w, y + h, cfg->bound_box_color );
			RectangleD ( x + w - 1, y + h - squareLine, x + w, y + h, cfg->bound_box_color );

			// bottom-right corner / outer outline
			RectangleD ( x + w - squareLine, y + h, x + w + 1, y + h + 1, ImColor ( 10, 10, 10, 190 ) );
			RectangleD ( x + w, y + h - squareLine, x + w + 1, y + h + 1, ImColor ( 10, 10, 10, 190 ) );

			// bottom-right corner / inner outline
			RectangleD ( x + w - squareLine, y + h - 2, x + w - 1, y + h - 1, ImColor ( 10, 10, 10, 190 ) );
			RectangleD ( x + w - 2, y + h - squareLine, x + w - 1, y + h - 2, ImColor ( 10, 10, 10, 190 ) );

			// bottom-right corner / missing edges
			RectangleD ( x + w - squareLine, y + h - 2, x + w - squareLine + 1, y + h + 1, ImColor ( 10, 10, 10, 190 ) );
			RectangleD ( x + w - 2, y + h - squareLine - 1, x + w + 1, y + h - squareLine, ImColor ( 10, 10, 10, 190 ) );
		}
		break;
		case THREE_DIMENSIONAL:
		{
			vec3_t vOrigin = origin;

			vec3_t min = mins + vOrigin;
			vec3_t max = maxs + vOrigin;

			vec3_t points [ ] = { vec3_t ( min.x, min.y, min.z ),
								vec3_t ( min.x, max.y, min.z ),
								vec3_t ( max.x, max.y, min.z ),
								vec3_t ( max.x, min.y, min.z ),
								vec3_t ( min.x, min.y, max.z ),
								vec3_t ( min.x, max.y, max.z ),
								vec3_t ( max.x, max.y, max.z ),
								vec3_t ( max.x, min.y, max.z ) };

			int edges [ 12 ][ 2 ] = { { 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 0 },
								 { 4, 5 }, { 5, 6 }, { 6, 7 }, { 7, 4 },
								 { 0, 4 }, { 1, 5 }, { 2, 6 }, { 3, 7 }, };

			ImVec2 p1 = ImVec2 ( ); ImVec2 p2 = ImVec2 ( );

			std::vector<ImVec2> points_Arr;

			for ( auto & it : edges ) {

				if ( !world_to_screen ( points [ it [ 0 ] ], p1 ) || !world_to_screen ( points [ it [ 1 ] ], p2 ) )
					return;


				render->AddLine ( p1, p2, cfg->bound_box_color );
			}

		}
		break;
		default:
			break;
		}





	}
	void visual_player::render_flags ( ) {

		auto _x = box_data.x + box_data.w + 8, _y = box_data.y + 3;

		auto & flags_cfg = cfg->flags_input;

		for ( size_t i = 0; i < flags_list.size ( ); i++ ) {

			if ( flags_cfg.at ( i ) == 1 && flags_list.at ( i ).flag.size ( ) > 0 ) {
				primitive_string ( true, fonts [ FLAGS ], _x, _y, flags_list.at ( i ).color, ImColor ( 0, 0, 0, 255 ), visuals::font_left, flags_list.at ( i ).flag.c_str ( ) );
				_y += 8;
			}

		}

	}
	void visual_player::render_ammo ( ) {

		clip = std::clamp ( clip, 0, 1000 );
		max_clip = std::clamp ( max_clip, 1, 1000 );

		float percent = 0.f;

		if ( clip > 0 ) {
			percent = (float)clip / (float)max_clip;

		}
		render->AddRect ( ImVec2 ( box_data.mins.x, box_data.maxs.y + 3 ), ImVec2 ( box_data.maxs.x, box_data.maxs.y + 6 ), ImColor ( 0, 0, 0, 125 ), 3.f );
		render->AddRectFilled ( ImVec2 ( box_data.mins.x + 1, box_data.maxs.y + 4 ), ImVec2 ( box_data.mins.x  + (box_data.maxs.x - box_data.mins.x ) * percent - 1, box_data.maxs.y + 5 ), ImColor ( 0, 0, 200, 255 ), 3.f );



	}
	double easeOutBack ( double t ) {
		return 1 + ( --t ) * t * ( 2.70158 * t + 1.70158 );
	}

	void visual_player::render_health ( ) {
		static auto get_health_clr = [ & ] ( int health ) {
			return ImColor ( static_cast< int >( 255 - ( health * 2.55f ) ), static_cast< int >( health * 2.55f ), 0, 100 );
		};

		static auto get_health_clr_1 = [ ] ( int health ) {
			auto clr = get_health_clr ( health );
			clr.get_rgb ( );

			clr.rgb.x -= 75;
			clr.rgb.y -= 75;
			clr.rgb.z -= 75;

			if ( clr.rgb.x < 0 )
				clr.rgb.x = 0;

			if ( clr.rgb.y < 0 )
				clr.rgb.y = 0;

			if ( clr.rgb.z < 0 )
				clr.rgb.z = 0;

			if ( clr.rgb.w < 0 )
				clr.rgb.w = 0;

			return clr;
		};


		static auto filled_box_outlined = [ ] ( const float x, const float y, const float w, const float h, const ImColor color, const ImColor outline, const float thickness ) {
			render->AddRectFilled ( ImVec2 ( x, y ), ImVec2 ( x + w, y + h ), color );
			render->AddRect ( ImVec2 ( x, y ), ImVec2 ( x + w, y + h ), outline, 0.f, 15, thickness );
		};
		static auto filled_box = [ ] ( const float x, const float y, const float w, const float h, const ImColor color ) {
			render->AddRectFilled ( ImVec2 ( x, y ), ImVec2 ( x + w, y + h ), color );
		};

		int elements = 0;

		health = std::clamp ( health, 0, 100 );

		auto multiplier = health / 100.f;

		multiplier = std::clamp ( multiplier, 0.f, 1.f );

		const auto height = ( box_data.h - 2 ) * multiplier;

		float progress_height = 0.f;
		
		if ( animating_health ) {
			progress_height = ImGui::GetTime ( ) - this->health_animation_start;  progress_height = easeOutBack( std::clamp ( progress_height * 1.5f, 0.f, 1.f ));
			if ( progress_height >= 1.f )
				animating_health = false;
		}
		else {
			progress_height = health * 0.01f;
		}

		float progress_health = 1.f; 
		if ( animating_health ) 
			progress_health = std::lerp ( health_on_change, health, progress_height );
		else
			progress_health = 	std::lerp ( 0.f, 100.f, progress_height );


		const int red = static_cast< int >( 255 - health * 2.55f );
		const int green = static_cast< int >( health * 2.55f );




		//render->Flags = 0;

		//render->Flags = ImDrawListFlags_AntiAliasedLines || ImDrawListFlags_AntiAliasedLinesUseTex || ImDrawListFlags_AntiAliasedFill;

		render->AddRectFilled ( ImVec2 ( box_data.mins.x - 5, box_data.mins.y ), ImVec2 ( box_data.mins.x - 2, box_data.maxs.y ), ImColor ( 0, 0, 0, 125 ) );
		render->AddRectFilled ( ImVec2 ( box_data.mins.x - 4, box_data.mins.y + 1 ), ImVec2 ( box_data.mins.x - 3,  box_data.mins.y + 1 + ( box_data.maxs.y - box_data.mins.y ) * progress_health * 0.01f - 2 ), config.player_visual [ type ].health_color );

		//filled_box ( box_data.x - 5.7f, box_data.y + box_data.h - height - 1, 1.0f, height - 1.5f, ImColor ( red, green, 0, static_cast< int >( 255 ) ) );
		//primitive_string ( true, fonts [ FLAGS ],  rend_pos.x, rend_pos.y, ImColor(0, 0, 0, 255), ImColor ( 255, 255, 255, 255 ), font_center, std::to_string ( health ).c_str ( ) );

	}
	void visual_player::render_weapon ( ) {
		//dsdsDrawText ( c_menu::get ( ).weapon_icons, _data.weapon_icon, ImVec2 ( ( _data.box_data.x + ( ( _data.box_data.x + _data.box_data.w ) - _data.box_data.x ) / 2.0f ), _data.box_data.y + _data.box_data.h + 5 ), 8.f, clr_h, 0.1f, true, true );


		auto text_size = ImGui::CalcTextSize ( weapon_icon.c_str ( ), 11.f, weapon_font );


		float add_offset = 0.f;
		if ( config.player_visual [ type ].ammo )
			add_offset += 2.f;


		render->AddText ( visuals::weapon_font, 11.f, ImVec2 ( box_data.bottom_center.x - text_size.x / 2.f + 1.f, box_data.bottom_center.y + add_offset + ( text_size.y / 2.f ) + 1.f ), ImColor ( 0, 0, 0, 125 ), weapon_icon.c_str ( ) );
		render->AddText ( visuals::weapon_font, 11.f, ImVec2 ( box_data.bottom_center.x - text_size.x / 2.f, box_data.bottom_center.y + add_offset + ( text_size.y / 2.f ) ), config.player_visual [ type ].weapon_color, weapon_icon.c_str ( ) );

	}
	void visual_player::render_skeleton ( ) {

		if ( bones.size ( ) > 0 ) {
			ImVec2 child, parent;
			for ( auto & bone : bones ) {
				if ( world_to_screen ( bone.child, child ) && world_to_screen ( bone.parent, parent ) )
					render->AddLine ( parent, child, config.player_visual [ type ].skeleton_color );
			}
		}
	}
	void visual_player::render_barrel ( ) {

		if ( world_to_screen ( barrel_start, barrel_start_w2s ) && world_to_screen ( barrel_end, barrel_end_w2s ) ) {
			render->AddLine ( barrel_start_w2s, barrel_end_w2s, config.player_visual [ type ].view_barrel_color );
			render->AddCircleFilled ( barrel_end_w2s, 3.f, ImColor ( 255, 0, 0, 125 ) );
			render->AddCircle ( barrel_end_w2s, 3.f, ImColor ( 255, 255, 255, 125 ) );
		}
	}
	void visual_player::on_render ( ) {

		if ( !cfg )
			return;

		if ( dormant && !cfg->dormant )
			return;


		if ( health > 1.f && on_screen ) {



			animate ( );


			//	if (in_animation )
				  // render->PushClipRect ( animated_clip_mins, animated_clip_maxs, false );

			if ( cfg->bounding_box )
				render_box ( );
			if ( cfg->name )
				render_name ( );
			if ( cfg->health )
				render_health ( );
			if ( cfg->ammo )
				render_ammo ( );
			if ( cfg->weapon )
				render_weapon ( );
			if ( cfg->flags )
				render_flags ( );
			if ( cfg->skeleton )
				render_skeleton ( );
			if ( cfg->knife_range || cfg->taser_range )
				render_range ( );

			if ( type <= 1 && static_cast< player_visual * >( cfg )->view_barrel )
				render_barrel ( );

			//if ( in_animation )
			  //render->PopClipRect ( );

		//	visibility_check ( );

		}

		if ( cfg->out_of_pov )
			render_offscreen ( );

	}
	void visual_player::on_update (  ) {

		
		if ( !entity )
			return;


		player = reinterpret_cast < player_t * >( entity );

		type = player->is_teammate ( ) ? 0 : 1;

		if ( player == local_player::ptr() )
			type = 2;

		if ( type > 1 )
			cfg = &config.local_visual;
		else
			cfg = &config.player_visual [ type ];

		dormant = player->dormant ( );
		enemy = player->is_enemy ( );
		health = player->health ( );
		index = player->index ( );
		origin = player->abs_origin ( );
		interfaces::engine->get_player_info ( player->index ( ), &player_info );
		mins = player->mins ( );
		maxs = player->maxs ( );
		type = player->is_enemy ( ) ? 1 : 0;
		valid = true;
		if ( player->spotted ( ) && !spotted ) {
			spot_time = ImGui::GetTime ( );
		}

		spotted = player->spotted ( );

		is_knife = false;
		taze = false;

		if ( !animating_health && std::fabs ( health - old_health ) > 1 ) {
			health_animation_start = ImGui::GetTime ( );
			animating_health = true;
			health_on_change = old_health;
		}

		old_health = health;
		
		on_screen = calculate_box ( );
		distance = local_player::data().eye_position.distance_to ( player->abs_origin ( ) );

		if ( never_seen ) {
			in_animation = true;
			animated_clip_maxs = box_data.mins;

			never_seen = false;
		}
		
		const auto weap = player->active_weapon ( );
		if ( weap ) {
			weapon_icon = weapon_icons [ weap->item_definition_index ( ) ];
			clip = weap->clip1_count ( );
			max_clip = weap->get_weapon_data ( )->iMaxClip1;
			is_knife = weap->is_knife ( );
 
			taze = weap->item_definition_index ( ) == weapon_taser;
		}
		if ( config.player_visual [ type ].skeleton && on_screen && !dormant && player && player->is_alive ( ) ) {
			auto p_studio_hdr = interfaces::model_info->get_studio_model ( player->model ( ) );
			if ( !p_studio_hdr )
				return;

			if ( bones.size ( ) < p_studio_hdr->bones_count )
				bones.resize ( p_studio_hdr->bones_count );

			for ( int i = 0; i < p_studio_hdr->bones_count; i++ ) {
				studio_bone_t * bone = p_studio_hdr->bone ( i );


				if ( !bone )
					continue;

				if ( bone && ( bone->flags & BONE_USED_BY_HITBOX ) && ( bone->parent != -1 ) ) {
					bones.at ( i ).child = player->get_bone_position ( i );
					bones.at ( i ).parent = player->get_bone_position ( bone->parent );
				}
			}
		}
		if ( config.player_visual [ type ].view_barrel && on_screen && !dormant && player && player->is_alive ( ) && player->active_weapon ( ) && player->active_weapon ( )->get_weapon_data ( ) ) {


			barrel_start = player->eye_pos ( );

			vec3_t  dst, forward;
			trace_t tr;
			ray_t ray;
			trace_filter filter;

			math::angle_vectors ( player->eye_angles ( ), forward );
			filter.skip = player;

			barrel_end = barrel_start + ( forward * 4091.f );

			ray.initialize ( barrel_start, barrel_end );

			interfaces::trace_ray->trace_ray ( ray, MASK_SHOT, &filter, &tr );

			barrel_end = tr.end;

		}

		handle_flags ( );

	}
	void visual_player::visibility_check ( ) {
		return;

		matrix3x4_t & tran_frame = player->coord_frame ( );


		const vec3_t min = player->mins ( );
		const vec3_t max = player->maxs ( );

		ImVec2 screen_boxes [ 8 ];

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
			if ( !world_to_screen ( math::vector_transform ( points [ i ], tran_frame ), screen_boxes [ i ] ) )
				return;

		ImVec2 box_array [ ] = {
			screen_boxes [ 3 ], // fl
			screen_boxes [ 5 ], // br
			screen_boxes [ 0 ], // bl
			screen_boxes [ 4 ], // fr
			screen_boxes [ 2 ], // fr
			screen_boxes [ 1 ], // br
			screen_boxes [ 6 ], // bl
			screen_boxes [ 7 ] // fl
		};


		//render->AddCircleFilled ( p, 5.f, ImColor ( 255, 0, 0, 255 ) );


	}
	void visual_player::animate ( ) {
		if ( in_animation ) {
			animated_clip_mins.x = box_data.mins.x - 50;
			animated_clip_maxs.x = box_data.maxs.x + 50;
			animated_clip_mins.y = box_data.mins.y - 50;

			animated_clip_maxs.y += ( 1000.0 / ( double ) ImGui::GetIO ( ).Framerate ) / 2.0;

			if ( animated_clip_maxs.y > box_data.maxs.y + 20 ) {
				animated_clip_maxs.y = box_data.maxs.y + 20;
				in_animation = false;
			}
		}
	}

	void visual_player::handle_flags ( ) {

		if ( !cfg->flags )
			return;

		if ( flags_list.size ( ) < FLAGS_MAX )
			flags_list.resize ( FLAGS_MAX );

		if ( last_flags_update_tickcount != interfaces::globals->tick_count ) {
			last_flags_update_tickcount = interfaces::globals->tick_count;
			auto & flags = cfg->flags_input;

			for ( size_t i = 0; i < flags_list.size ( ); i++ ) {
				if ( flags.at ( i ) == 0 )
					continue;

				auto & flag_object = flags_list.at ( i );
				flag_object.color = cfg->flags_color;

				auto & flag = flag_object.flag;

				flag.clear ( );

				switch ( i ) {
				case FLAGS_MONEY:
					flag = "$ ";
					flag += std::to_string ( player->money ( ) );
					break;
				case FLAGS_ARMOR:
					if ( player->armor ( ) > 0 && player->has_helmet ( ) )
						flag = "HK";
					else if ( player->armor ( ) > 0 )
						flag = "K";
					break;
				case FLAGS_KIT:
					if ( player->has_defuser ( ) )
						flag = "KIT";
					break;
				case FLAGS_SCOPED:
					if ( player->is_scoped ( ) )
						flag = "SCOPED";
					break;
				case FLAGS_FLASHED:
					if ( player->flash_duration ( ) > 0.f )
						flag = "FLASHED";
					break;
				case FLAGS_FAKEDUCKING:
				{
					auto animstate = player->get_anim_state ( );


					auto fakeducking = [ & ] ( ) -> bool {
						static auto stored_tick = 0;
						static int crouched_ticks [ 65 ];

						if ( animstate->m_fDuckAmount ) //-V550
						{
							if ( animstate->m_fDuckAmount < 0.9f && animstate->m_fDuckAmount > 0.5f ) //-V550
							{
								if ( stored_tick != interfaces::globals->tick_count ) {
									crouched_ticks [ player->index ( ) ]++;
									stored_tick = interfaces::globals->tick_count;
								}

								return crouched_ticks [ player->index ( ) ] > 16;
							}
							else
								crouched_ticks [ player->index ( ) ] = 0;
						}

						return false;
					};

					if ( fakeducking ( ) && player->flags ( ) & fl_onground && !animstate->m_bOnGround )
						flag = "fakeduck";


				}
				break;
				case FLAGS_C4:
					//not implemented yet

					break;
				case FLAGS_LC:
					//not impelemnt yet
					break;
				case FLAGS_TASER:
				{
					UINT * hWeapons = player->get_weapons ( );

					if ( !hWeapons )
						return;

					for ( int nIndex = 0; hWeapons [ nIndex ]; nIndex++ ) {
						weapon_t * pWeapon = ( weapon_t * ) interfaces::entity_list->get_client_entity_handle ( hWeapons [ nIndex ] );

						if ( !pWeapon )
							continue;
						auto item_Def = pWeapon->item_definition_index ( );

						if ( item_Def == weapon_taser ) {
							flag = "TASER";
							break;
						}
					}
				}
				break;
				case FLAGS_HIT:
					//rage implement
					break;
				case FLAGS_EXPLOIT:
					//rage implement
					break;
				case FLAGS_PING:
				{
					auto latency = interfaces::player_resource->get_ping ( player->index ( ) );
					flag = std::to_string ( latency ) + "MS";

					auto green_factor = ( int ) std::clamp ( 255.0f - ( float ) latency * 225.0f / 200.0f, .0f, 255.0f );
					flag_object.color = ImColor ( 150, ( int ) green_factor, 0 );
				}
				break;
				case FLAGS_HOSTAGE:
					break;
				case FLAGS_DEFUSING:
					break;
				case FLAGS_RELOAD:
				{
					animationlayer * layer1 = &player->get_animoverlays ( ) [ 1 ];

					// check if reload animation is going on.
					if ( layer1->flWeight != 0.f && player->get_sequence_act ( layer1->nSequence) == 967 /* ACT_CSGO_RELOAD */ ) {
						flag = "RELOAD";
						flag_object.color = ImColor ( 60, 180, 225, 255 );
					}
				}
				break;
				case FLAGS_DORMANT:
					if ( player->dormant ( ) )
						flag = "dormant";
					break;
				case FLAGS_DISTANCE:
					flag = "distance ";
					flag += std::to_string ( ( int ) origin.distance_to ( local_player::data().eye_position ) );
					break;
				case FLAGS_MAX:
					break;
				default:
					break;
				}
			}



		}



	}

	
	void visual_player::render_offscreen ( ) {
		auto isOnScreen = [ ] ( vec3_t origin, ImVec2 & screen ) -> bool {
			if ( !world_to_screen ( origin, screen ) )
				return false;

			static int iScreenWidth, iScreenHeight;
			interfaces::engine->get_screen_size ( iScreenWidth, iScreenHeight );

			auto xOk = iScreenWidth > screen.x;
			auto yOk = iScreenHeight > screen.y;

			return xOk && yOk;
		};


		if ( interfaces::globals->tick_count != last_tick ) {
			last_tick = interfaces::globals->tick_count;

			ImVec2 screenPos;

			if ( isOnScreen ( origin, screenPos ) )
				return;

			vec3_t viewAngles;
			interfaces::engine->get_view_angles ( viewAngles );

			static int width, height;
			interfaces::engine->get_screen_size ( width, height );

			auto screenCenter = ImVec2 ( width * 0.5f, height * 0.5f );
			auto angleYawRad = DEG2RAD ( viewAngles.y - math::calc_angle ( local_player::data().eye_position, origin ).y - 90.0f );


			base_pos.x = screenCenter.x + ( ( ( ( width - ( cfg->out_of_pov_base_size * 3 ) ) * 0.5f ) * ( cfg->out_of_pov_radius / 100.0f ) ) * cos ( angleYawRad ) ) + ( int ) ( 6.0f * ( ( ( float ) cfg->out_of_pov_base_size - 4.0f ) / 16.0f ) );
			base_pos.y = screenCenter.y + ( ( ( ( height - ( cfg->out_of_pov_base_size * 3 ) ) * 0.5f ) * ( cfg->out_of_pov_radius / 100.0f ) ) * sin ( angleYawRad ) );

			if ( config.player_visual [ type ].foot_steps )
				base_pos = ImVec2 ( width * 0.7f, height * 0.7f );

			if ( base_pos.x < 40 )
				base_pos.x = 40;
			if ( base_pos.y < 40 )
				base_pos.y = 40;

			if ( base_pos.x > width - 40 )
				base_pos.x = width - 40;
			if ( base_pos.y > height - 40 )
				base_pos.y = height - 40;


			int rounding_error = 16;
			float step = ( M_PI / rounding_error );
			double a = -step * 3.0;

			if ( offscreen_points.size ( ) < 24 )
				offscreen_points.resize ( 24 );

			int count = 0;
			while ( a <= M_PI + ( step * 4.0 ) ) {
				offscreen_points.at ( count ) = ( ImVec2 ( base_pos.x + cfg->out_of_pov_base_size * cos ( a ), base_pos.y + cfg->out_of_pov_base_size * sin ( a ) ) );
				a += step;
				count++;
			}

			offscreen_points.at ( count ) = ( ImVec2 ( base_pos.x, base_pos.y - cfg->out_of_pov_base_size * 2.f ) );

			auto rot = viewAngles.y - math::calc_angle ( local_player::data().eye_position, origin ).y;// -90.0f;

			rot = math::normalize_yaw ( rot );


			while ( rot < 0.f )
				rot += 360.f;
			while ( rot > 360.f )
				rot -= 180.f;


			rotate_circle ( offscreen_points, base_pos, rot );
		}


		if ( offscreen_points.size ( ) > 0 && !on_screen ) {
			render->AddConvexPolyFilled ( offscreen_points.data ( ), offscreen_points.size ( ), config.player_visual [ type ].out_of_pov_color );
			render->AddPolyline ( offscreen_points.data ( ), offscreen_points.size ( ), ImColor ( 255, 255, 255, 125 ), ImDrawListFlags_AntiAliasedLines, 1.f );


			auto text_size = ImGui::CalcTextSize ( "e", cfg->out_of_pov_base_size, ui::font_icons );

			ImVec2 original_pos = base_pos;
			original_pos.x -= text_size.x / 2.f;
			original_pos.y -= text_size.y / 2.f;


			render->AddText ( ui::font_icons, cfg->out_of_pov_base_size, original_pos, ImColor ( 255, 255, 255, 255 ), &config.player_types.at ( type ).second [ 0 ] );
		}
	}
}