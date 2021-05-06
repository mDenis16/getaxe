#pragma once

#include "../../../../dependencies/imgui/imgui.h"

#include "../../../../dependencies/imgui/imgui_internal.h"
#include "../../../render/gui/gui.h"

#include "../../features.hpp"

namespace visuals {
	void visual_weapon_t::render_name ( ) {
		auto text_size = ImGui::CalcTextSize ( weapon_icon.c_str ( ), 11.f, weapon_font );


		float add_offset = 0.f;
		//if ( cfg->ammo )
		//	add_offset += 2.f;


		render->AddText ( visuals::weapon_font, 11.f, ImVec2 ( box_data.bottom_center.x - text_size.x / 2.f + 1.f, box_data.bottom_center.y + add_offset + ( text_size.y / 2.f ) + 1.f ), ImColor ( 0, 0, 0, 125 ), weapon_icon.c_str ( ) );
		render->AddText ( visuals::weapon_font, 11.f, ImVec2 ( box_data.bottom_center.x - text_size.x / 2.f, box_data.bottom_center.y + add_offset + ( text_size.y / 2.f ) ), cfg->name_color, weapon_icon.c_str ( ) );

	}
	void visual_weapon_t::render_box ( ) {

		if ( !weapon ) {
			return;
		}

		switch ( cfg->box_type ) {
		case NORMAL:
		{

			int old_flags = render->Flags;
			render->Flags = ImDrawListFlags_AntiAliasedLines | ImDrawListFlags_AntiAliasedLines;

			render->AddRect ( this->box_data.mins, this->box_data.maxs, ImColor ( 0, 0, 0, 125 ), cfg->bound_box_border_rounding, 0, cfg->bound_box_border_thickness );
			render->AddRect ( this->box_data.mins, this->box_data.maxs, cfg->bound_box_color, cfg->bound_box_border_rounding, 0, cfg->bound_box_thickness );

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
			int squareLine = std::min ( VertLine, HorzLine );

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

	void visual_weapon_t::render_ammo ( ) {

		clip = std::clamp ( clip, 0, 1000 );
		max_clip = std::clamp ( max_clip, 1, 1000 );

		render->AddRect ( ImVec2 ( box_data.mins.x, box_data.maxs.y + 3 ), ImVec2 ( box_data.maxs.x, box_data.maxs.y + 6 ), ImColor ( 0, 0, 0, 125 ), 3.f );
		render->AddRectFilled ( ImVec2 ( box_data.mins.x + 1, box_data.maxs.y + 4 ), ImVec2 ( box_data.maxs.x - 1, box_data.maxs.y + 5 ), ImColor ( 0, 0, 200, 255 ), 3.f );



	}




	void visual_weapon_t::on_render ( ) {

		if ( !cfg )
			return;


		if ( on_screen ) {



			if ( cfg->bounding_box )
				render_box ( );
			//if ( cfg->ammo )
			//	render_ammo ( );
			if ( cfg->name )
				render_name ( );



		}



	}
	void visual_weapon_t::on_update (  ) {


		weapon = reinterpret_cast < weapon_t * >( entity );


		cfg = &config.weapons_visual;



		dormant = weapon->dormant ( );

		index = weapon->index ( );
		origin = weapon->origin ( );

		mins = weapon->mins ( );
		maxs = weapon->maxs ( );

		valid = true;

		on_screen = calculate_box ( );
		distance = localdata.eye_position.distance_to ( weapon->origin ( ) );

		auto weapon_index = weapon->item_definition_index ( );

		if ( weapon->item_definition_index ( ) <= 64 )
			weapon_icon = weapon_icons [ weapon->item_definition_index ( ) ];


	}




}