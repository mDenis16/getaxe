#pragma once

#include "../../../../dependencies/imgui/imgui.h"

#include "../../../../dependencies/imgui/imgui_internal.h"
#include "../../../render/gui/gui.h"

#include "../../features.hpp"

namespace visuals {
	/*
	const auto is_grenade = [ ] ( const int id ) {
			return  id == 114 || id == 157 || id == 48 || id == 9 || id == 100 || id == 156;
		};
	*/

	std::string get_grenade_icon ( grenade_t* prjectile) {
		switch ( prjectile->client_class()->class_id ) {
		case CBaseCSGrenadeProjectile:
		{
			if ( const auto model = prjectile->model ( ); model && strstr ( model->name, "flashbang" ) )
				return "k";
			else
				return "l";
			break;
		}
		case CDecoyProjectile:
			return "o";
			break;
		case CMolotovProjectile:
			return "n";
			break;
		case CSmokeGrenadeProjectile:
			return "m";
			break;
		default:
			return "?";
			break;
		}
	}

	void visual_projectile::render_name ( ) {
		auto text_size = ImGui::CalcTextSize ( grenade_icon.c_str ( ), 11.f, weapon_font );


		float add_offset = 0.f;
		//if ( cfg->ammo )s
		//	add_offset += 2.f;


		render->AddText ( visuals::weapon_font, 11.f, ImVec2 ( box_data.bottom_center.x - text_size.x / 2.f + 1.f, box_data.bottom_center.y + add_offset + ( text_size.y / 2.f ) + 1.f ), ImColor ( 0, 0, 0, 125 ), grenade_icon.c_str ( ) );
		render->AddText ( visuals::weapon_font, 11.f, ImVec2 ( box_data.bottom_center.x - text_size.x / 2.f, box_data.bottom_center.y + add_offset + ( text_size.y / 2.f ) ), cfg->name_color, grenade_icon.c_str ( ) );

	}
	void visual_projectile::render_box ( ) {

		if ( !projectile ) {
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


	void visual_projectile::on_render ( ) {

		if ( !cfg )
			return;


		if ( should_animate_alpha ) {

			alpha += ( 1000.0 / ( double ) ImGui::GetIO ( ).Framerate ) / 1.0;
			if ( alpha > 255 ) {
				alpha = 255;
				should_animate_alpha = false;
			}
		}

		if ( on_screen ) {



			if ( cfg->bounding_box )
				render_box ( );
			if ( cfg->name )
				render_name ( );
			if ( cfg->grenade_warning ) {
				render_warning ( );
			
			}
			if ( cfg->grenade_path )
				render_path ( );


		}



	}

	void visual_projectile::on_update ( ) {


		projectile = reinterpret_cast < grenade_t * >( entity );


		cfg = &config.projectiles_visual;


	

		dormant = projectile->dormant ( );

		index = projectile->index ( );
		origin = projectile->origin ( );

		mins = projectile->mins ( );
		maxs = projectile->maxs ( );

		valid = true;

		on_screen = calculate_box ( );
		distance = localdata.eye_position.distance_to ( projectile->origin ( ) );

		

		if ( (cfg->grenade_path || cfg->grenade_warning ) && path.empty ( ) ) {
		
			grenade_icon = get_grenade_icon ( projectile );

			if ( projectile->owner_handle ( ) ) {
				auto h_owner = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity_handle ( projectile->owner_handle ( ) ));
			
				if ( h_owner ) {
					owner_type = h_owner->is_teammate ( ) ? 0 : 1;

					if ( h_owner == local_pointer )
						owner_type = 2;
				}
			}
			entity_t * temp = nullptr;

			grenade_prediction::Simulate ( temp, projectile, path, 0.f, projectile->client_class ( )->class_id, &projectile->origin ( ) );
		}

		int tick = ( interfaces::globals->cur_time - spawn_curtime ) / 1.5f * path.size ( );
		progress = 1.f - ( interfaces::globals->cur_time - spawn_curtime ) / 1.5f;
		
		if ( progress > 1.f )
			progress = 1.f;
		else if ( progress < 0.01f )
			progress = 0.01f;


		std::cout << "projectile tick " << tick << std::endl;
		std::cout << "projectile progress " << progress << std::endl;
		if ( tick > path.size ( ) )
			tick = path.size ( );

		//progress = 1.0f - tick / path.size ( );



		if ( projectile->client_class()->class_id == CSmokeGrenadeProjectile && projectile->m_nSmokeEffectTickBegin ( ) > 0 || ( projectile->client_class ( )->class_id == CBaseCSGrenadeProjectile && projectile->m_nExplodeEffectTickBegin ( ) > 0 )) {
			this->entity->m_bDidSmokeEffect ( ) = false;
			this->delete_entity( );
		}


	}

	void visual_projectile::render_warning ( ) {

		if ( !path.empty ( ) ) {

			if ( pulse ) {
				pulse_value += ( ( 1000.0 / ( double ) ImGui::GetIO ( ).Framerate ) / 15.f );
				if ( pulse_value > 25.f ) {
					pulse = false;
				}
			}
			else {
				pulse_value -= ( ( 1000.0 / ( double ) ImGui::GetIO ( ).Framerate ) / 15.f );
				if ( pulse_value < 1.f ) {
					pulse = true;
				}
			}

			ImVec2 base_pos;

			if ( world_to_screen ( path.back ( ), base_pos ) ) {
			
				


				render->AddShadowCircle ( base_pos, radius, ImColor ( 0, 0, 0, ( int ) alpha ), pulse_value, ImVec2 ( 0, 0 ), 0, 30 );
				render->PathClear ( );
				render->PathLineTo ( base_pos );
				render->PathArcTo ( base_pos, radius, DEG2RAD ( -90.f ), DEG2RAD ( (  progress * 360.f ) - 90.f ), 32 );
				render->AddShadowConvexPoly ( render->_Path.Data, render->_Path.Size, ImColor ( 255, 0, 0, ( int ) alpha ), pulse_value, ImVec2 ( 0, 0 ), 0 );
				render->_Path.Size = 0;

				render->AddCircle ( base_pos, radius, ImColor ( 255, 255, 255, ( int ) ( ( alpha / 255.f ) * 100.f ) ), 30, 2.f );

				auto text_size = ImGui::CalcTextSize ( grenade_icon.c_str(), radius, ui::weapon_icons );

				auto original_pos = base_pos;
				base_pos.x -= text_size.x / 2.f;
				base_pos.y -= text_size.y / 4.f;

				render->AddText ( ui::weapon_icons, radius, base_pos, ImColor ( 255, 255, 255, ( int ) ( alpha ) ), grenade_icon.c_str() );

				text_size = ImGui::CalcTextSize ( "6 hp", 0.4f * radius, ui::font_widgets );

				original_pos.x -= text_size.x / 2.f;
				original_pos.y -= text_size.y * 2.f - 0.4f * radius / 2.f;

				render->AddText ( ui::font_title, 0.4f * radius, original_pos, ImColor ( 255, 255, 255, ( int ) ( alpha ) ), "6 hp" );
			}
		}
	}

	void visual_projectile::render_path ( ) {

		std::vector<ImVec2> points; points.reserve ( path.size ( ) );

		ImVec2 point;
	
		for ( size_t i = tick; i < path.size ( ); i++ ) {
			auto & p = path.at ( i );

			if ( world_to_screen ( p, point ) ) {
					points.push_back ( point );
				
			}
		}
		/*
		for ( auto & p : path )
			if ( world_to_screen ( p, point ) ) {
				points.push_back ( point );
			}
		*/

		render->AddPolyline ( points.data(), points.size ( ), ImColor ( 255, 0, 0, (int)alpha ), false, 1.f );

	}

}
