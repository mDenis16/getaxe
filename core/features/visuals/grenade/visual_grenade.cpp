#pragma once

#include "../../helpers/helpers.h"
#include <renderlib/imgui/imgui.h>

#include <renderlib/imgui/imgui_internal.h>
#include <UIFramework/framework_internal.h>

#include <config.h>

#include "../visual_entities.h"
#include "../player/visual_player.h"
#include "../projectile/visual_projectile.h"
#include "../weapon/visual_weapon.h"
#include "../grenade/visual_grenade.h"
#include "../visuals.h"


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
		if ( class_id != 157 )
			   render_indicator ( );

			   if ( class_id == 157 )
				   render_smoke ( );
			   else
				   render_inferno ( );

				render_offscreen ( );
			//}

	//	}



	}
	float InvLerp ( float a, float b, float v ) {
		return ( v - a ) / ( b - a );
	}
	float valve2deg ( float yaw ) {
		float l = InvLerp ( -180.f, 180.f, yaw );
		return std::lerp ( 0.f, 360.f, l );
	}
	bool intersect (  vec3_t & n,  vec3_t & p0,  vec3_t & l0,  vec3_t & l, float & t ) {
		float denom = math::dot_product ( n, l );
		if ( denom > 1e-6 ) {
			vec3_t p0l0 = p0 - l0;
			t = math::dot_product ( p0l0, n ) / denom;
			return ( t >= 0 );
		}

		return false;
	}

	int inline GetIntersection ( float fDst1, float fDst2, vec3_t P1, vec3_t P2, vec3_t & Hit ) {
		if ( ( fDst1 * fDst2 ) >= 0.0f ) return 0;
		if ( fDst1 == fDst2 ) return 0;
		Hit = P1 + ( P2 - P1 ) * ( -fDst1 / ( fDst2 - fDst1 ) );
		return 1;
	}

	int inline InBox ( vec3_t Hit, vec3_t B1, vec3_t B2, const int Axis ) {
		if ( Axis == 1 && Hit.z > B1.z && Hit.z < B2.z && Hit.y > B1.y && Hit.y < B2.y ) return 1;
		if ( Axis == 2 && Hit.z > B1.z && Hit.z < B2.z && Hit.x > B1.x && Hit.x < B2.x ) return 1;
		if ( Axis == 3 && Hit.x > B1.x && Hit.x < B2.x && Hit.y > B1.y && Hit.y < B2.y ) return 1;
		return 0;
	}

	// returns true if line (L1, L2) intersects with the box (B1, B2)
	// returns intersection point in Hit
	int CheckLineBox ( vec3_t B1, vec3_t B2, vec3_t L1, vec3_t L2, vec3_t & Hit ) {
		if ( L2.x < B1.x && L1.x < B1.x ) return false;
		if ( L2.x > B2.x && L1.x > B2.x ) return false;
		if ( L2.y < B1.y && L1.y < B1.y ) return false;
		if ( L2.y > B2.y && L1.y > B2.y ) return false;
		if ( L2.z < B1.z && L1.z < B1.z ) return false;
		if ( L2.z > B2.z && L1.z > B2.z ) return false;
		if ( L1.x > B1.x && L1.x < B2.x &&
			L1.y > B1.y && L1.y < B2.y &&
			L1.z > B1.z && L1.z < B2.z ) {
			Hit = L1;
			return true;
		}
		if ( ( GetIntersection ( L1.x - B1.x, L2.x - B1.x, L1, L2, Hit ) && InBox ( Hit, B1, B2, 1 ) )
			|| ( GetIntersection ( L1.y - B1.y, L2.y - B1.y, L1, L2, Hit ) && InBox ( Hit, B1, B2, 2 ) )
			|| ( GetIntersection ( L1.z - B1.z, L2.z - B1.z, L1, L2, Hit ) && InBox ( Hit, B1, B2, 3 ) )
			|| ( GetIntersection ( L1.x - B2.x, L2.x - B2.x, L1, L2, Hit ) && InBox ( Hit, B1, B2, 1 ) )
			|| ( GetIntersection ( L1.y - B2.y, L2.y - B2.y, L1, L2, Hit ) && InBox ( Hit, B1, B2, 2 ) )
			|| ( GetIntersection ( L1.z - B2.z, L2.z - B2.z, L1, L2, Hit ) && InBox ( Hit, B1, B2, 3 ) ) )
			return true;

		return false;
	}

	bool visual_grenade::is_intersected ( ) {
		std::vector<vec3_t> points; points.reserve ( 4 );

		float angle = math::calc_angle ( local_player::data().eye_position, origin ).y;
		vec3_t view;
		interfaces::engine->get_view_angles ( view );
		view.x = 0;
		vec3_t angle_left = view; angle_left.y = math::normalize_yaw ( view.y + 90.f );
		vec3_t angle_right = view; angle_right.y = math::normalize_yaw ( view.y - 90.f );
		
		vec3_t left = math::angle_vector ( angle_left );
		vec3_t right = math::angle_vector ( angle_right );
		vec3_t location_1 = origin + left * 160.f;
		vec3_t location_2 = origin + right * 160.f;
		location_1.z += 160.f;

		vec3_t viewangle;
		interfaces::engine->get_view_angles ( viewangle );
		vec3_t forward = math::angle_vector ( viewangle );
		vec3_t end = local_player::data().eye_position + forward * 4092.f;

		vec3_t ht;
		vec3_t m, a;
		m = location_1;
		a = location_2;

	
		vec3_t center;
		center = ( m + a ) * 0.5f;

	
		points.push_back ( center );
		//points.push_back ( location_2 );

		int i = 0;

		for ( auto & p : points ) {
			ImVec2 pos;
			i++;

			visuals::world_to_screen ( p, pos );
			render->AddCircle ( pos, 13.f, ImColor(255,0,255,255) );
			render->AddText ( pos,  ImColor ( 0, 255, 255, 255 ), std::to_string ( i ).c_str ( ) );
		}
		return false;
	}

	void visual_grenade::render_smoke ( ) {
		//is_intersected ( );


		return;
		ImVec2 point;
		std::vector<ImVec2> points;
		points.reserve ( circle_points.size ( ) );\

			for ( auto & p : circle_points ) {
				if ( world_to_screen ( p, point ) ) {
					points.push_back ( point );
					//render->AddCircleFilled ( point, 2.5f, ImColor ( 0, 255, 0, ( int ) alpha ) );
				}
			}

		if ( points.size ( ) > 2 ) {

			
			if (  std::fabs ( interfaces::globals->cur_time - spawn_curtime ) > 14.f )
				alpha = 18.f - std::fabs ( interfaces::globals->cur_time - spawn_curtime ) / 18.f * 255.f;

		  	render->AddShadowConvexPoly ( points.data ( ), points.size ( ), ImColor ( 54, 69, 79, (int)alpha ), 30.f, ImVec2 ( 0, 0 ) );
		}

		mins = vec3_t ( 160, -40, -160 );
		maxs = vec3_t (0, 40, 160 );

		on_screen = calculate_box ( );

		if ( on_screen ) {

			std::vector<vec3_t> points; points.reserve ( 4 );
		

			static auto yaws  = { 90.f, -90.f };

			for ( auto & yaw : yaws ) {
				float y = math::normalize_yaw ( math::calc_angle ( local_player::data().eye_position, origin ).y - yaw );
				float a = DEG2RAD ( y );

				vec3_t location ( 160.f * cos ( a ) + origin.x, 160.f * sin ( a ) + origin.y, origin.z );

				points.push_back ( location );
			}
			

			points.push_back ( vec3_t( points.at(0).x, points.at ( 0 ).y, points.at ( 0 ).z + 160.f) );
			points.push_back ( vec3_t ( points.at ( 1 ).x, points.at ( 1 ).y, points.at ( 1 ).z + 160.f ) );

			ImColor clr = ImColor ( 255, 0, 0, 255 );

			vec3_t viewangle;
			interfaces::engine->get_view_angles ( viewangle );
			vec3_t forward = math::angle_vector ( viewangle );
			vec3_t end = local_player::data().eye_position + forward * 8192.f;

			vec3_t ht;
			vec3_t m, a;
			m = points.at ( 1 );
			a = points.at ( 2 );

			bool intersect = CheckLineBox ( m, a, local_player::data().eye_position, end, ht);
			
			if ( intersect )
				clr = ImColor ( 255, 255, 255, 255 );

			std::cout << " intersect " << intersect << std::endl;

			points.clear ( );
			points.push_back ( a );
			points.push_back ( m );
			points.push_back ( ht );

			int i = 0;

			for ( auto & p : points) {
				ImVec2 pos;
				i++;
			
				visuals::world_to_screen ( p, pos );
				render->AddCircle ( pos, 13.f, clr );
				render->AddText ( pos, ImColor ( 0, 255, 255, 255 ), std::to_string ( i ).c_str ( ) );
			}

		}
		

		/*on_screen = calculate_box ( );

		if ( on_screen ) {
			ImVec2 pos;
			visuals::world_to_screen ( origin, pos );
			render->AddCircle ( pos, 13.f, ImColor ( 255, 0, 0, 255 ) );

		}*/
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
			is_fire = true;
			index = grenade->index ( );
			origin = grenade->origin ( );

			grenade->get_renderable_virtual ( )->get_render_bounds ( mins, maxs );

			float flRadius = ( mins - maxs ).Length2D ( ) * 0.5f;

		

			warning = ( local_player::data().eye_position - origin ).length ( ) < flRadius;

			valid = true;

			if ( class_id == 157 )
				flRadius = 156;

			distance = local_player::data().eye_position.distance_to ( grenade->origin ( ) );

			if ( this->class_id == 157 && flRadius != lastRadius) {
				float step = M_PI_F * 2.0f / static_cast< float >( 30 );
				circle_points.clear ( );
				circle_points.reserve ( 30 );
				for ( float a = 0.f; a < ( M_PI_F * 2.0f ); a += step ) {
					vec3_t location ( flRadius * cos ( a ) + origin.x, flRadius * sin ( a ) + origin.y, origin.z );
					circle_points.push_back ( location );
				}
				lastRadius = flRadius;
			}
			else {

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

					vec3_t angle = math::calc_angle ( best_position, location );
					vec3_t forward = math::angle_vector ( angle );
					best_position = best_position + forward * ( 60.f );

					fire_points.push_back ( best_position );

				}
			}
			
			last_tickcount = interfaces::globals->tick_count;
		}
	}

	void visual_grenade::render_indicator ( ) {


		ImVec2 base_pos;

		float screen_x = 1920.f / distance;
		float screen_y = 1080.f / distance;

		float rd = ( screen_x + screen_y ) * 500.f;

		std::cout << "rd " << rd << std::endl;

		radius = 30.f;
	



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

			float progress = class_id == 157 ? 18.f : 7.f - std::fabs ( interfaces::globals->cur_time - spawn_curtime );

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

			auto text_size = ImGui::CalcTextSize ( "C", radius, ui::font_icons );

			auto original_pos = base_pos;
			base_pos.x -= text_size.x / 2.f;
			base_pos.y -= text_size.y / 4.f;

			render->AddText ( ui::font_icons, radius, base_pos, ImColor ( 255, 255, 255, ( int)(alpha) ), "C" );

			text_size = ImGui::CalcTextSize ( "2 m", 0.4f * radius, ui::font_widgets );

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
			auto angleYawRad = DEG2RAD ( viewAngles.y - math::calc_angle ( local_player::data().eye_position, origin ).y - 90.0f );

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

			auto rot = viewAngles.y - math::calc_angle ( local_player::data().eye_position, origin ).y;// -90.0f;

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



			auto text_size = ImGui::CalcTextSize ( "c", cfg->out_of_pov_base_size, ui::font_icons );

			auto original_pos = base_pos;
			base_pos.x -= text_size.x / 2.f;
			base_pos.y -= text_size.y / 2.f;

			render->AddText ( ui::font_icons, cfg->out_of_pov_base_size, base_pos, ImColor ( 255, 255, 255, ( int ) ( alpha ) ), "C" );

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
