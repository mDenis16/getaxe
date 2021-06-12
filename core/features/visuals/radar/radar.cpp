#include "../../features.hpp"

#include "../../../render/gui/gui.h"

namespace visuals {

	radar_engine * radar = new radar_engine ( );
	std::string parseString ( const std::string & szBefore, const std::string & szSource ) {
		if ( !szBefore.empty ( ) && !szSource.empty ( ) && ( szSource.find ( szBefore ) != std::string::npos ) ) {
			std::string t = strstr ( szSource.c_str ( ), szBefore.c_str ( ) ); //-V522
			t.erase ( 0, szBefore.length ( ) );
			size_t firstLoc = t.find ( '\"', 0 );
			size_t secondLoc = t.find ( '\"', firstLoc + 1 );
			t = t.substr ( firstLoc + 1, secondLoc - 3 );
			return t;
		}
		else
			return crypt_str ( "" );
	}


	template<class T>
	static T * FindHudElement ( const char * name ) {
		static auto pThis = *reinterpret_cast< DWORD ** >( utilities::pattern_scan ( ( "client.dll" ), "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08" ) + 1 );

		static auto find_hud_element = reinterpret_cast< DWORD ( __thiscall * )( void *, const char * ) >( utilities::pattern_scan ( ( "client.dll" ), "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39" ) );

		return ( T * ) find_hud_element ( pThis, name );
	}

	radar_engine::radar_engine ( ) {
	}

	radar_engine::~radar_engine ( ) {
	}

	void radar_engine::VectorYawRotate ( const ImVec2 & in, float flYaw, ImVec2 & out ) {
		if ( &in == &out ) {
			ImVec2 tmp;
			tmp = in;
			VectorYawRotate ( tmp, flYaw, out );
			return;
		}
		float sy, cy;
		math::sin_cos ( DEG2RAD ( flYaw ), &sy, &cy );
		out.x = in.x * cy - in.y * sy;
		out.y = in.x * sy + in.y * cy;
	}

	ImVec2 radar_engine::WorldToMap ( const vec3_t & worldpos ) {
		ImVec2 offset ( worldpos.x - m_MapOrigin.x, worldpos.y - m_MapOrigin.y );

		offset.x /= m_fMapScale;
		offset.y /= -m_fMapScale;

		return offset;
	}
	void radar_engine::GetSize ( int & x, int & y ) {
		auto window = static_cast< ui::window * >( this->window_target );
		x = window->maxs.x - window->mins.x ;
		y = window->maxs.y - window->mins.y;
	}

	void radar_engine::GetPos ( int & x, int & y ) {
		auto window = static_cast< ui::window * >( this->window_target );

		ImVec2 WindowPos;
		x = window->mins.x;
		y = window->mins.y;
	}

	ImVec2 radar_engine::MapToRadar ( const ImVec2 & mappos ) {
		auto * LocalEntity = local_pointer;

		if ( !LocalEntity )
			return ImVec2 ( 0, 0 );

		vec3_t viewAngle;
		interfaces::engine->get_view_angles ( viewAngle );
		viewAngle.angle_normalize ( );
		viewAngle.angle_clamp ( );

		int pwidth, pheight;
		int px, py;
		ImVec2 panelpos;

		GetSize ( pwidth, pheight );
		GetPos ( px, py );

		ImVec2 OriginOffset = WorldToMap ( LocalEntity->origin ( ) );

		ImVec2 offset;
		offset.x = ( mappos.x - m_MapCenter.x );
		offset.y = ( mappos.y - m_MapCenter.y );

		offset.x += OriginOffset.x;
		offset.y += OriginOffset.y;

		VectorYawRotate ( offset, viewAngle.y + 90.f , offset );

		float fScale = m_fZoom / 1024;

		offset.x *= fScale;
		offset.y *= fScale;

		panelpos.x = px + ( pwidth * 0.5f ) + ( pheight * offset.x );
		panelpos.y = py + ( pheight * 0.5f ) + ( pheight * offset.y );

		return panelpos;
	}
	void radar_engine::VectorYawRotate2 ( ImVec2 center, float angle, ImVec2 * p ) {
		float flSin = sin ( angle );
		float flCos = cos ( angle );

		p->x -= center.x;
		p->y -= center.y;

		float xnew = p->x * flCos - p->y * flSin;
		float ynew = p->x * flSin + p->y * flCos;

		p->x = xnew + center.x;
		p->y = ynew + center.y;
	}

	void radar_engine::on_update ( ) {

	}
	void rotate_circlecc ( std::vector<ImVec2> & points, ImVec2 center, float rotation ) {


		for ( auto & point : points ) {
			point.x -= center.x;	point.y -= center.y;

			const auto tempX = point.x;
			const auto tempY = point.y;

			const auto theta = DEG2RAD ( rotation );
			const auto c = cos ( theta );
			const auto s = sin ( theta );

			point.x = tempX * c - tempY * s;
			point.y = tempX * s + tempY * c;

			point.x += center.x; point.y += center.y;
		}
	}

	void radar_engine::on_map_load ( const char * pszMapName ) {

		isValidMap = false;
		char szPath [ MAX_PATH ];
		sprintf ( szPath, crypt_str ( "csgo\\resource\\overviews\\%s_radar.dds" ), pszMapName );

		std::ifstream file;
		file.open ( szPath );

		if ( !file.is_open ( ) )
			return;

		file.close ( );

		if ( FAILED ( D3DXCreateTextureFromFileA ( reinterpret_cast< IDirect3DDevice9 * >( ui::window_device ), szPath, &mapTexture ) ) )
			return;


		sprintf ( szPath, crypt_str ( ".\\csgo\\resource\\overviews\\%s.txt" ), pszMapName );
		std::ifstream ifs ( szPath );

		if ( ifs.bad ( ) )
			return;

		std::string szInfo ( ( std::istreambuf_iterator<char> ( ifs ) ), ( std::istreambuf_iterator<char> ( ) ) );

		if ( szInfo.empty ( ) )
			return;

		m_flPosX = std::stoi ( parseString ( crypt_str ( "\"pos_x\"" ), szInfo ) );
		m_flPosY = std::stoi ( parseString ( crypt_str ( "\"pos_y\"" ), szInfo ) );
		m_flScale = std::stod ( parseString ( crypt_str ( "\"scale\"" ), szInfo ) );
		m_MapOrigin = vec3_t ( m_flPosX, m_flPosY, 0 );

		m_fMapScale = m_flScale * 2;
		isValidMap = true;
	}


	void radar_engine::on_render ( ) {

		if ( !window_target ) {
			window_target = ui::radar_window_extern;
			return;
		}
		if ( window_target ) {

			auto tg = static_cast< ui::window * >( window_target );

			if ( !interfaces::engine->is_in_game ( ) )
				return;
			if ( !local_pointer )
				return;
			if ( !local_pointer->is_alive ( ) )
				return;

			if ( !isValidMap )
				on_map_load ( interfaces::engine->get_level_name_short ( ) );

			if ( !mapTexture ) return;

			ImVec2 points [ 4 ] =
			{
				MapToRadar ( ImVec2 ( 0, 0 ) ),
				MapToRadar ( ImVec2 ( 512, 0 ) ),
				MapToRadar ( ImVec2 ( 512, 512 ) ),
				MapToRadar ( ImVec2 ( 0, 512 ) )
			};

			/*Rotate texture in 180 degrees cuz valve dolbayobi*/
			VectorYawRotate2 ( MapToRadar ( ImVec2 ( 256, 256 ) ), DEG2RAD ( 180 ), &points [ 0 ] );
			VectorYawRotate2 ( MapToRadar ( ImVec2 ( 256, 256 ) ), DEG2RAD ( 180 ), &points [ 1 ] );
			VectorYawRotate2 ( MapToRadar ( ImVec2 ( 256, 256 ) ), DEG2RAD ( 180 ), &points [ 2 ] );
			VectorYawRotate2 ( MapToRadar ( ImVec2 ( 256, 256 ) ), DEG2RAD ( 180 ), &points [ 3 ] );

			// Draw texture
			
			render->PushClipRect ( ImVec2 ( tg->mins.x + 10, tg->mins.y + 10 ), ImVec2 ( tg->maxs.x - 10, tg->maxs.y - 10 ) );
			render->AddImageQuad ( ( void * ) mapTexture, points [ 0 ], points [ 1 ], points [ 2 ], points [ 3 ], ImVec2 ( 0, 0 ), ImVec2 ( 1, 0 ), ImVec2 ( 1, 1 ), ImVec2 ( 0, 1 ), ImColor ( 255, 255, 255, 255 ) );
			

			ImVec2 center;
			static float animate = 0.f;
			animate += interfaces::globals->frame_time; if ( animate >= 1.f ) animate = 0.f;
			center.x = ( tg->mins.x + tg->maxs.x ) * 0.5f;
			center.y = ( tg->mins.y + tg->maxs.y ) * 0.5f;

			std::vector<ImVec2> points_circle;
			if ( points_circle.size ( ) < 24 )
				points_circle.resize ( 24 );

			if ( visuals::handler ) {
				for ( auto entity : visuals::handler->entity_list ) {

					if ( entity->class_id == CCSPlayer ) {

						auto player = reinterpret_cast< visual_player * >( entity );

						if ( player->valid && !player->dormant && player->health > 0 && player->player != local_pointer ) {
							const auto  origin = player->origin;
							ImVec2 map = WorldToMap ( origin );
							ImVec2 radar = MapToRadar ( map );


							//float progress = ImGui::GetTime ( ) - entity->spot_time;  progress = std::clamp ( progress * 3.5f, 0.f, 1.f );

							//VectorYawRotate2 ( center, DEG2RAD (-90 ), &radar );
							VectorYawRotate2 ( MapToRadar ( ImVec2 ( 256, 256 ) ), DEG2RAD ( 180 ), &radar );
							
							


							//
							
							int rounding_error = 16;
							float step = ( M_PI / rounding_error );
							double a = -step * 3.0;

						

							int count = 0;
							float out_of_pov_base_size = 5.f;
							while ( a <= M_PI + ( step * 4.0 ) ) {
								points_circle.at ( count ) = ( ImVec2 ( radar.x + out_of_pov_base_size * cos ( a ), radar.y + out_of_pov_base_size * sin ( a ) ) );
								a += step;
								count++;
							}

							points_circle.at ( count ) = ( ImVec2 ( radar.x, radar.y - out_of_pov_base_size * 2.f ) );
							
							rotate_circlecc ( points_circle, radar, player->player->eye_angles ( ).y );
						
							render->AddShadowConvexPoly ( points_circle.data ( ), points_circle.size ( ), (player->type == 0 ? ImColor ( 25, 125, 123, 255 ) : ImColor ( 255, 1, 24, 255 )), 22.f,  ImVec2(0,0) );
							render->AddShadowConvexPoly ( points_circle.data ( ), points_circle.size ( ), ( player->type == 0 ? ImColor ( 25, 125, 123, 255 ) : ImColor ( 255, 1, 24, 255 ) ), 22.f, ImVec2 ( 0, 0 ) );
							if ( player->spot_time  != 0.f && (ImGui::GetTime() - player->spot_time ) < 16)
						    	render->AddPolyline ( points_circle.data ( ), points_circle.size ( ), ( player->type == 0 ? ImColor ( 25, 125, 123, 255 ) : ImColor ( 255, 1, 24, 255 ) ), true, 1.f );
						
						}
					}
					else if ( entity->is_fire ) {
						auto fire = reinterpret_cast< visual_grenade * >( entity );
						if ( fire->fire_points.size ( ) > 4 ) {
							std::vector<ImVec2> points_fire;
							points_fire.reserve ( fire->fire_points.size ( ) );

							for ( auto &  as : fire->fire_points ) {

								ImVec2 dsad = WorldToMap ( as );
								ImVec2 asddsd = MapToRadar ( dsad );
								VectorYawRotate2 ( MapToRadar ( ImVec2 ( 256, 256 ) ), DEG2RAD ( 180 ), &asddsd );
								points_fire.push_back ( asddsd );
							}
							if ( points_fire.size() > 4 )
						    	render->AddShadowConvexPoly ( points_fire.data ( ), points_fire.size ( ), ImColor ( 255, 25, 23, 255 ), 7.f, ImVec2(0,0) );
						}
					}
				}
			}
			/*
			
			
			*/

			if ( local_pointer && local_pointer->is_alive ( ) ) {
				ImVec2 map = WorldToMap ( local_pointer->origin ( ) );

				

				tg->renderer->AddShadowCircle ( center, 5.f, ImColor ( 255, 255, 255 ), 10.f, ImVec2(0,0) );
			}

			render->PopClipRect ( );

		}
	}

}


