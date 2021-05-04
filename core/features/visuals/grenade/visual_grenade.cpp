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

/*	std::string get_grenade_icon ( grenade_t * prjectile ) {
		switch ( prjectile->client_class ( )->class_id ) {
		case CInferno:
		{
			return "B";
			break;
		}
		case CSmokeGrenadeProjectile:
			return "m";
			break;
		default:
			return "?";
			break;
		}
	}
	*/


	void visual_grenade::on_render ( ) {

		if ( !cfg )
			return;
		if ( should_animate_alpha ) {

			alpha += ( 1000.0 / ( double ) ImGui::GetIO ( ).Framerate ) / 2.0;
			if ( alpha > 255 ) {
				alpha = 255;
				should_animate_alpha = false;
			}
		}

		//if ( on_screen ) {



			//if ( cfg->grenade_radius_warning ) {
			   render_indicator ( );

				render_inferno ( );
				render_offscreen ( );
			//}

	//	}



	}

	void visual_grenade::render_inferno ( ) {
		
		ImVec2 point;
		std::vector<ImVec2> points;
		points.reserve ( fire_points.size ( ) );
		for ( auto & p : fire_points ) {
			if ( world_to_screen ( p, point ) ) {
				points.push_back ( point );
				render->AddCircleFilled ( point, 2.5f, ImColor ( 0, 255, 0, (int)alpha ) );
			}
		}

		if ( points.size ( ) > 2 ) {
			render->AddShadowConvexPoly ( points.data ( ), points.size ( ), ImColor ( 255, 0, 0, (int )alpha ), pulse_value, ImVec2(0,0));
		}
	}
	void visual_grenade::on_update ( ) {


		grenade = reinterpret_cast < grenade_t * >( entity );


		cfg = &config.projectiles_visual;
		if ( last_tickcount != interfaces::globals->tick_count ) {



			dormant = grenade->dormant ( );

			index = grenade->index ( );
			origin = grenade->origin ( );

			grenade->get_renderable_virtual ( )->get_render_bounds ( mins, maxs );

			float flRadius = ( mins - maxs ).Length2D ( ) * 0.5f;

			warning = ( localdata.eye_position - origin ).length ( ) < flRadius;

			valid = true;


			distance = localdata.eye_position.distance_to ( grenade->origin ( ) );


			int		m_fireCount = *reinterpret_cast< int * >		( ( DWORD ) grenade + 0x13A8		/*0x13A8*/ );
			bool * m_bFireIsBurning = reinterpret_cast< bool * >	( ( DWORD ) grenade + 0xE94 /*0xE94*/ );

			fire_points.clear ( );
			fire_points.reserve ( m_fireCount );


			float step = M_PI_F * 2.0f / static_cast< float >( m_fireCount );

			for ( float a = 0.f; a < ( M_PI_F * 2.0f ); a += step ) {
				vec3_t location ( flRadius * cos ( a ) + origin.x, flRadius * sin ( a ) + origin.y, origin.z );


				float min_distance = 9999.f;
				
				vec3_t best_position = vec3_t ( );

				for ( int i = 0; i < m_fireCount; i++ ) {

					if ( !m_bFireIsBurning [ i ] )
						continue;


					int * m_fireXDelta = reinterpret_cast< int * >		( ( DWORD ) grenade + 0x9E4		/*0x9E4*/ );
					int * m_fireYDelta = reinterpret_cast< int * >		( ( DWORD ) grenade + 0xB74		/*0xB74*/ );
					int * m_fireZDelta = reinterpret_cast< int * >		( ( DWORD ) grenade + 0xD04		/*0xD04*/ );


					vec3_t center = entity->origin ( ) + vec3_t ( m_fireXDelta [ i ], m_fireYDelta [ i ], m_fireZDelta [ i ] );
					float distance = center.distance_to ( location );

					if ( distance < min_distance ) {
						best_position = center;
						min_distance = distance;
					}
				
				}
				fire_points.push_back ( best_position );
			}
			
			last_tickcount = interfaces::globals->tick_count;
		}
	}

	void visual_grenade::render_indicator ( ) {


		ImVec2 base_pos;
		if ( world_to_screen ( origin, base_pos ) ) {

			if ( warning ) {
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
			}
			else {
				pulse = 0.f;
			}

			float progress = 7.f - std::fabs ( interfaces::globals->cur_time - spawn_curtime );

			if ( progress <= 1.f && alpha > 0 ) {
				alpha -= ( ( 1000.0 / ( double ) ImGui::GetIO ( ).Framerate ) / 2.f );
			}

			if ( progress < 0.01f )
				progress = 0.01f;

			render->AddShadowCircle ( base_pos, radius, ImColor ( 0, 0, 0, (int) alpha ), pulse_value, ImVec2 ( 0, 0 ), 0, 30 );
			render->PathClear ( );
			render->PathLineTo ( base_pos );
			render->PathArcTo ( base_pos, radius, DEG2RAD ( -90.f ), DEG2RAD (  360.f / 7.f  * progress - 90.f ), 32 );
			render->AddShadowConvexPoly ( render->_Path.Data, render->_Path.Size, ImColor ( 255, 0, 0, ( int ) alpha ), pulse_value, ImVec2 ( 0, 0 ), 0 );
			render->_Path.Size = 0;

			render->AddCircle ( base_pos, radius, ImColor ( 255, 255, 255, (int)(( alpha / 255.f) * 100.f )), 30, 2.f );

			auto text_size = ImGui::CalcTextSize ( "B", radius, ui::font_icons );

			auto original_pos = base_pos;
			base_pos.x -= text_size.x / 2.f;
			base_pos.y -= text_size.y / 4.f;

			render->AddText ( ui::font_icons, radius, base_pos, ImColor ( 255, 255, 255, ( int)(alpha) ), "B" );

			text_size = ImGui::CalcTextSize ( "6 hp", 0.4f * radius, ui::font_widgets );

			original_pos.x -= text_size.x / 2.f;
			original_pos.y -= text_size.y * 2.f - 0.4f * radius / 2.f;

			render->AddText ( ui::font_title, 0.4f * radius, original_pos, ImColor ( 255, 255, 255, ( int ) ( alpha ) ), "6 hp" );
		}
	}
	
	void visual_grenade::render_offscreen ( ) {
		auto isOnScreen = [ ] ( vec3_t origin, ImVec2 & screen ) -> bool {
			if ( !world_to_screen ( origin, screen ) )
				return false;

			static int iScreenWidth, iScreenHeight;
			interfaces::engine->get_screen_size ( iScreenWidth, iScreenHeight );

			auto xOk = iScreenWidth > screen.x;
			auto yOk = iScreenHeight > screen.y;

			return xOk && yOk;
		};


	//	if ( interfaces::globals->tick_count != last_tick ) {
		//	last_tick = interfaces::globals->tick_count;

			ImVec2 screenPos;

			if ( isOnScreen ( origin, screenPos ) )
				return;

			vec3_t viewAngles;
			interfaces::engine->get_view_angles ( viewAngles );

			static int width, height;
			interfaces::engine->get_screen_size ( width, height );

			auto screenCenter = ImVec2 ( width * 0.5f, height * 0.5f );
			auto angleYawRad = DEG2RAD ( viewAngles.y - math::calc_angle ( localdata.eye_position, origin ).y - 90.0f );

			ImVec2 base_pos;

			cfg->out_of_pov_radius = 102.f;

			base_pos.x = screenCenter.x + ( ( ( ( width - ( cfg->out_of_pov_base_size * 3 ) ) * 0.5f ) * ( cfg->out_of_pov_radius / 100.0f ) ) * cos ( angleYawRad ) ) + ( int ) ( 6.0f * ( ( ( float ) cfg->out_of_pov_base_size - 4.0f ) / 16.0f ) );
			base_pos.y = screenCenter.y + ( ( ( ( height - ( cfg->out_of_pov_base_size * 3 ) ) * 0.5f ) * ( cfg->out_of_pov_radius / 100.0f ) ) * sin ( angleYawRad ) );


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

			auto rot = viewAngles.y - math::calc_angle ( localdata.eye_position, origin ).y;// -90.0f;

			rot = math::normalize_yaw ( rot );


			while ( rot < 0.f )
				rot += 360.f;
			while ( rot > 360.f )
				rot -= 180.f;


			rotate_circle ( offscreen_points, base_pos, rot );
	//	}


		if ( offscreen_points.size ( ) > 0 ) {

			
			cfg->out_of_pov_base_size = 27;
			alpha = 255;

			

			render->AddShadowConvexPoly ( offscreen_points.data ( ), offscreen_points.size ( ), ImColor ( 0, 0, 0, ( int ) alpha ), 18.f, ImVec2 ( 0, 0 ) );

			float progress = 7.f - std::fabs ( interfaces::globals->cur_time - spawn_curtime );
			if ( progress < 0.01f )
				progress = 0.01f;

			render->PathClear ( );
			render->PathLineTo ( base_pos );
			render->PathArcTo ( base_pos, cfg->out_of_pov_base_size, DEG2RAD ( -90.f ), DEG2RAD ( 360.f / 7.f * progress - 90.f ), 32 );
			render->AddShadowConvexPoly ( render->_Path.Data, render->_Path.Size, ImColor ( 255, 0, 0, 255 ), 18.f, ImVec2 ( 0, 0 ), 0 );
			render->_Path.Size = 0;



			auto text_size = ImGui::CalcTextSize ( "B", cfg->out_of_pov_base_size, ui::font_icons );

			auto original_pos = base_pos;
			base_pos.x -= text_size.x / 2.f;
			base_pos.y -= text_size.y / 2.f;

			render->AddText ( ui::font_icons, cfg->out_of_pov_base_size, base_pos, ImColor ( 255, 255, 255, ( int ) ( alpha ) ), "B" );

			base_pos = ( ImVec2 ( base_pos.x, base_pos.y - cfg->out_of_pov_base_size * 2.f + 0.4f ) );
	
			rotate_point ( base_pos, original_pos, rot );

			text_size = ImGui::CalcTextSize ( "2 m", 0.4f * cfg->out_of_pov_base_size, ui::font_widgets );

			base_pos.x -= text_size.x / 2.f;
			base_pos.y -= text_size.y / 2.f;

			render->AddText ( ui::font_title, 0.4f * cfg->out_of_pov_base_size, base_pos, ImColor ( 255, 255, 255, ( int ) ( alpha ) ), "2 m" );

			render->AddPolyline ( offscreen_points.data ( ), offscreen_points.size ( ), ImColor ( 255, 255, 255, 100 ), true, 2.f );
		}
	}
}
