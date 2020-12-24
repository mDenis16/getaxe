#include <vector>
#include "../../features.hpp"
#include "../../../menu/render/menu_render.hpp"
#undef max

namespace visuals::projectiles::grenades {


	std::vector<grenade_data> grenades;
	
	void think ( entity_t * grenade ) {

		if ( !config.visuals_world_draw_grenades )
			return;



		auto client_class = grenade->client_class ( );
		if ( !client_class )
			return;

		auto name_data = grenade_name ( grenade, client_class->class_id );


		grenade_data _data;
		_data.name = std::get<0> ( name_data );
		_data.origin = grenade->origin ( );
		_data.on_screen = visuals::get_playerbox ( grenade, _data.box_data );
		_data.distance = math::calc_distance ( local_player::m_data.pointer->origin ( ), _data.origin, false );

		_data.weapon_icon = std::get<1> ( name_data );
		_data.is_smoke = client_class->class_id == 157 && grenade->m_bDidSmokeEffect ( );
		if ( client_class->class_id == 100 || _data.is_smoke )
			_data.time_able = true;

		if ( _data.time_able ) {

			vec3_t mins, maxs;
			grenade->get_renderable_virtual ( )->get_render_bounds ( mins, maxs );

			auto origin = grenade->origin ( );

			if ( !interfaces::debug_overlay->world_to_screen ( origin, _data.screen_origin ) )
				return;





			_data.class_id = client_class->class_id;
			if ( _data.is_smoke ) {
				_data.progress = 20.f - std::fabs ( interfaces::globals->cur_time - math::time_to_ticks ( grenade->m_nSmokeEffectTickBegin ( ) ) );

				float step = M_PI * 2.0 / 30;


				for ( float a = 0; a < ( M_PI * 2.0 ); a += step ) {
					vec3_t location ( 144 * cos ( a ) + origin.x, 144 * sin ( a ) + origin.y, origin.z );
					vec3_t w2s_pos = vec3_t ( );
					if ( interfaces::debug_overlay->world_to_screen ( location, w2s_pos ) ) {
						_data.polygon.push_back ( ImVec2 ( w2s_pos.x, w2s_pos.y ) );

					}
				}
			}
			else {
				float flRadius = ( mins - maxs ).Length2D ( ) * 0.5f;
				_data.radius = flRadius;
				const auto spawn_time = grenade->get_spawn_time ( );
				_data.progress = 7.f - std::fabs ( interfaces::globals->cur_time - spawn_time );


				int		m_fireCount = *reinterpret_cast< int * >		( ( DWORD ) grenade + 0x13A8		/*0x13A8*/ );
				bool * m_bFireIsBurning = reinterpret_cast< bool * >	( ( DWORD ) grenade + 0xE94 /*0xE94*/ );

				struct fire_location {
					vec3_t pos = vec3_t ( );
					float dist = 0.f;
					float radius = 0.f;
					bool used = false;
				};
				std::vector<fire_location> fires;
				for ( int i = 0; i <= m_fireCount; i++ ) {
					if ( !m_bFireIsBurning [ i ] )
						continue;

					int * m_fireXDelta = reinterpret_cast< int * >		( ( DWORD ) grenade + 0x9E4		/*0x9E4*/ );
					int * m_fireYDelta = reinterpret_cast< int * >		( ( DWORD ) grenade + 0xB74		/*0xB74*/ );
					int * m_fireZDelta = reinterpret_cast< int * >		( ( DWORD ) grenade + 0xD04		/*0xD04*/ );

					auto position = origin + vec3_t ( m_fireXDelta [ i ], m_fireYDelta [ i ], m_fireZDelta [ i ] );

					fires.push_back ( { position } );

				}


				std::vector<fire_location> limits_fire_filtered;

				float step = M_PI * 2.0 / fires.size ( );

				vec3_t position = grenade->origin ( );
				for ( float a = 0; a < ( M_PI * 2.0 ); a += step ) {
					vec3_t location ( flRadius * cos ( a ) + position.x, flRadius * sin ( a ) + position.y, position.z );
					vec3_t pos_2d = vec3_t ( );
					if ( interfaces::debug_overlay->world_to_screen ( location, pos_2d ) )
						_data.raw_polygon.push_back ( ImVec2 ( pos_2d.x, pos_2d.y ) );
					float min_distance = 9999.f;
					fire_location found_fire;

					for ( auto & fire_d : fires ) {
						fire_d.dist = math::calc_distance ( fire_d.pos, location, false );
						if ( !fire_d.used && fire_d.dist <= min_distance ) {
							found_fire = fire_d;
							min_distance = fire_d.dist;
						}
					}
					if ( !found_fire.pos.is_zero ( ) ) {
						limits_fire_filtered.push_back ( { found_fire.pos, found_fire.dist } );
						found_fire.used = true;
					}
				}
				/*for ( size_t i = 0; i < limits_fire_filtered.size(); i++ ) {
					auto current_fire = limits_fire_filtered.at ( i );
					/*if ( limits_fire_filtered.size ( ) > 2 && i == limits_fire_filtered.size ( ) - 1 ) {
						auto next_fire = limits_fire_filtered.at ( 0 );
						auto previous_fire = limits_fire_filtered.at ( limits_fire_filtered.size ( ) - 2 );
						auto average = ( next_fire.dist + previous_fire.dist ) / 2.f;
						if ( current_fire.dist < average )
							limits_fire_filtered.erase ( limits_fire_filtered.begin ( ) + limits_fire_filtered.size ( ) - 1 );
					}*/
					/*	if ( i > 0 && i < limits_fire_filtered.size ( )) {

							auto next_fire = i == limits_fire_filtered.size ( ) - 1 ? limits_fire_filtered.at ( 0 ) : limits_fire_filtered.at ( i + 1 ) ;
							auto previous_fire = limits_fire_filtered.at ( i - 1 );
							bool should_anyway = next_fire.dist > current_fire.dist || previous_fire.dist > current_fire.dist;
							auto average = ( next_fire.dist + previous_fire.dist ) / 2.f;
							if ( current_fire.dist < average )
								limits_fire_filtered.erase ( limits_fire_filtered.begin ( ) + i );
						}

					}*/
					/*	for ( size_t i = 0; i < limits_fire_filtered.size ( ); i++ ) {
							auto current_fire = limits_fire_filtered.at ( i );
							if ( limits_fire_filtered.size ( ) > 2 && i == limits_fire_filtered.size ( ) - 1 ) {
								auto next_fire = limits_fire_filtered.at ( 0 );
								auto previous_fire = limits_fire_filtered.at ( limits_fire_filtered.size ( ) - 2 );
								auto average = ( next_fire.dist + previous_fire.dist ) / 2.f;
								bool should_anyway = next_fire.dist > current_fire.dist || previous_fire.dist > current_fire.dist;
								if (math::calc_distance( current_fire.pos, render_origin, false) < average  || should_anyway )
									limits_fire_filtered.erase ( limits_fire_filtered.begin ( ) + limits_fire_filtered.size ( ) - 1 );
							}
						}*/
						/*struct {
							bool operator()( struct fire_location a, struct fire_location b ) const {

								return a.dist > b.dist;

							}
						} target_sort;
						*/


				for ( auto location : limits_fire_filtered ) {

					interfaces::console->console_printf ( "DISTANCE %f \n", location.dist );
					vec3_t w2s_pos = vec3_t ( );
					if ( interfaces::debug_overlay->world_to_screen ( location.pos, w2s_pos ) ) {
						_data.polygon.push_back ( ImVec2 ( w2s_pos.x, w2s_pos.y ) );

					}
				}

			}
		}

		grenades.push_back ( _data );
	}
	float dsdsDrawText ( ImFont * pFont, const std::string text, const ImVec2 & pos, float size, float const * color, float shadow, bool center, bool bold ) {
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
	bool world_to_screen ( const vec3_t & origin, ImVec2 & screen ) {
		vec3_t screen_game = vec3_t ( );
		const auto screen_transform = [ &origin, &screen ] ( ) -> bool {
			static std::uint8_t * matrix;
			if ( !matrix ) {
				matrix = static_cast< std::uint8_t * >( utilities::pattern_scan ( "client.dll", "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9" ) );
				matrix += 3;
				matrix = *reinterpret_cast< std::uint8_t ** >( matrix );
				matrix += 176;
			}

			const matrix_t & w2s_matrix = *reinterpret_cast< matrix_t * >( matrix );
			screen.x = w2s_matrix.mat_val [ 0 ][ 0 ] * origin.x + w2s_matrix.mat_val [ 0 ][ 1 ] * origin.y + w2s_matrix.mat_val [ 0 ][ 2 ] * origin.z + w2s_matrix.mat_val [ 0 ][ 3 ];
			screen.y = w2s_matrix.mat_val [ 1 ][ 0 ] * origin.x + w2s_matrix.mat_val [ 1 ][ 1 ] * origin.y + w2s_matrix.mat_val [ 1 ][ 2 ] * origin.z + w2s_matrix.mat_val [ 1 ][ 3 ];
			screen.z = 0.0f;

			float w = w2s_matrix.mat_val [ 3 ][ 0 ] * origin.x + w2s_matrix.mat_val [ 3 ][ 1 ] * origin.y + w2s_matrix.mat_val [ 3 ][ 2 ] * origin.z + w2s_matrix.mat_val [ 3 ][ 3 ];

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
	void draw ( ) {
		for ( auto _data : grenades ) {
			if ( _data.on_screen ) {
				if ( _data.time_able ) {
			
					static auto size = vec2_t ( 60, 10 );
					auto screen_origin = ImVec2( _data.screen_origin.x, _data.screen_origin.y);

					constexpr float pi = M_PI;
				

					if ( _data.polygon.size ( ) )
					    c_menu::get ( ).draw->AddConvexPolyFilled ( _data.polygon.data ( ), _data.polygon.size ( ), ImColor ( 255, 0, 0, 100 ) );
				
		
					auto arc = [ ] ( float x, float y, float radius, float min_angle, float max_angle, ImColor col, float thickness ) {
						auto draw = ImGui::GetWindowDrawList ( );
						draw->PathArcTo ( ImVec2 ( x, y ), radius, DEG2RAD ( min_angle ), DEG2RAD ( max_angle ), 32 );
						draw->PathStroke ( col, false, thickness );

					};
					
					auto radius = std::max ( 15.0f - _data.distance / 700.0f, 1.0f );
					c_menu::get ( ).draw->AddCircleFilled ( screen_origin, radius, ImColor ( 37, 17, 1, 255 ), 40 );
					auto prog = ( 270 / 7 ) * _data.progress;
					arc ( screen_origin.x, screen_origin.y, radius, -90, prog, ImColor ( 255, 255, 255, 255 ), 1.5f );
				
					const float warning_color [ 4 ] = { 255,0,0,255 };
					auto color = _data.distance <= _data.radius ? warning_color : c_config::get ( ).visuals_world_draw_weapons_color;
					dsdsDrawText ( c_menu::get ( ).normalf, "!", ImVec2(screen_origin.x, screen_origin.y  - 15), 30, color, 0.1f, true, false );

				
				}else
				dsdsDrawText ( c_menu::get ( ).weapon_icons, _data.weapon_icon.c_str ( ), ImVec2(_data.box_data.x, _data.box_data.y)/**/, 20, c_config::get ( ).visuals_world_draw_weapons_color, 0.1f, true, false );
			}
		}

	}

}