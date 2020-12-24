#include "../features.hpp"
#include "../../menu/render/menu_render.hpp"
#include "../../menu/ImGui/imgui.h"
#include "../../menu/ImGui/imgui_internal.h"
#include "../../../dependencies/interfaces/i_memalloc.h"
class RadarPlayer_t {
public:
	vec3_t pos; //0x0000 
	vec3_t angle; //0x000C 
	vec3_t spotted_map_angle_related; //0x0018 
	DWORD tab_related; //0x0024 
	char pad_0x0028 [ 0xC ]; //0x0028
	float spotted_time; //0x0034 
	float spotted_fraction; //0x0038 
	float time; //0x003C 
	char pad_0x0040 [ 0x4 ]; //0x0040
	__int32 player_index; //0x0044 
	__int32 entity_index; //0x0048 
	char pad_0x004C [ 0x4 ]; //0x004C
	__int32 health; //0x0050 
	char name [ 32 ]; //0x785888 
	char pad_0x0074 [ 0x75 ]; //0x0074
	unsigned char spotted; //0x00E9 
	char pad_0x00EA [ 0x8A ]; //0x00EA
}; //Size=0x0174

class CCSGO_HudRadar {
public:
	char pad_0x0000 [ 0x14C ];
	RadarPlayer_t radar_info [ 65 ];
};
void visuals::player::player_death ( i_game_event * event ) {
	auto userid = event->get_int ( "userid" );
	if ( !userid )
		return;

	if ( !interfaces::engine->is_connected ( ) && !interfaces::engine->is_in_game ( ) )
		return;

	auto engine_userid = interfaces::engine->get_player_for_user_id ( userid );
	if ( !engine_userid )
		return;
	if ( engine_userid == interfaces::engine->get_local_player ( ) )
		return;

	auto entity = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( engine_userid ) );

	if ( !entity )
		return;
	
	m_data [ entity->index ( ) ].alive = false;

	resolver::resolver_data [ entity->index ( ) ].missed_shots = 0;
	
}
std::array<visuals::player::data, 65> visuals::player::m_data;

std::map<int, char> weapon_icons =
{
{ weapon_deagle, 'A' },
{ weapon_elite, 'B' },
{ weapon_fiveseven, 'C' },
{ weapon_glock, 'D' },
{ weapon_hkp2000, 'E' },
{ weapon_p250, 'F' },
{ weapon_usp_silencer, 'G' },
{ weapon_tec9, 'H' },
{ weapon_cz75a, 'I' },
{ weapon_revolver, 'J' },
{ weapon_mac10, 'K' },
{ weapon_ump45, 'L' },
{ weapon_bizon, 'M' },
{ weapon_mp7, 'N' },
{ weapon_mp5sd, 'L' },
{ weapon_mp9, 'O' },
{ weapon_p90, 'P' },
{ weapon_galilar, 'Q' },
{ weapon_famas, 'R' },
{ weapon_m4a1, 'S' },
{ weapon_m4a1_silencer, 'T' },
{ weapon_aug, 'U' },
{ weapon_sg556, 'V' },
{ weapon_ak47, 'W' },
{ weapon_g3sg1, 'X' },
{ weapon_scar20, 'Y' },
{ weapon_awp, 'Z' },
{ weapon_ssg08, 'a' },
{ weapon_xm1014, 'b' },
{ weapon_sawedoff, 'c' },
{ weapon_mag7, 'd' },
{ weapon_nova, 'e' },
{ weapon_negev, 'f' },
{ weapon_m249, 'g' },
{ weapon_taser, 'h' },
{ weapon_knife_t, 'i' },
{ weapon_knife, 'j' },
{ weapon_knife_falchion, '0' },
{ weapon_knife_bayonet, '1' },
{ weapon_knife_flip, '2' },
{ weapon_knife_gut, '3' },
{ weapon_knife_karambit, '4' },
{ weapon_knife_m9_bayonet, '5' },
{ weapon_knife_tactical, '6' },
{ weapon_knife_bowie, '7' },
{ weapon_knife_butterfly, '8' },
{ weapon_knife_ursus, 'j' },
{ weapon_knife, 'j' },
{ weapon_knife_stiletto, 'j' },
{ weapon_knife_widowmaker, 'j' },
{ weapon_flashbang, 'k' },
{ weapon_hegrenade, 'l' },
{ weapon_smokegrenade, 'm' },
{ weapon_molotov, 'n' },
{ weapon_decoy, 'o' },
{ weapon_incgrenade, 'p' },
{ weapon_c4, 'q' },
}; bool get_playerbox( entity_t* ent, visuals::box& in, visuals::player::data data ) {
	vec3_t origin, min, max, flb, brt, blb, frt, frb, brb, blt, flt;
	float left, top, right, bottom;

	/*origin = ent->get_absolute_origin( );
	min = ent->collideable( )->mins( ) + origin;
	max = ent->collideable( )->maxs( ) + origin;
	*/
	origin = data.origin;
	min = data.mins;
	max = data.maxs;
	vec3_t points [ ] = {
		vec3_t( min.x, min.y, min.z ),
		vec3_t( min.x, max.y, min.z ),
		vec3_t( max.x, max.y, min.z ),
		vec3_t( max.x, min.y, min.z ),
		vec3_t( max.x, max.y, max.z ),
		vec3_t( min.x, max.y, max.z ),
		vec3_t( min.x, min.y, max.z ),
		vec3_t( max.x, min.y, max.z )
	};

	if ( !interfaces::debug_overlay->world_to_screen( points [ 3 ], flb ) || !interfaces::debug_overlay->world_to_screen( points [ 5 ], brt )
		|| !interfaces::debug_overlay->world_to_screen( points [ 0 ], blb ) || !interfaces::debug_overlay->world_to_screen( points [ 4 ], frt )
		|| !interfaces::debug_overlay->world_to_screen( points [ 2 ], frb ) || !interfaces::debug_overlay->world_to_screen( points [ 1 ], brb )
		|| !interfaces::debug_overlay->world_to_screen( points [ 6 ], blt ) || !interfaces::debug_overlay->world_to_screen( points [ 7 ], flt ) )
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
	in.y = ( int ) top + 3;
	in.w = int( right - left ) ;
	in.h = int( bottom - top ) + 3;

	return true;
}
float visuals::draw_name_text ( ImFont * pFont, const std::string text, const ImVec2 & pos, float size, float const * color, float shadow, bool center, bool bold ) {
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
void visuals::player::name( visuals::player::data _data ) {
	bool should_show = _data.enemy ? config.visuals_enemy_name : config.visuals_team_name;
	if ( !should_show )   
		return;

	std::string print( _data.player_info.fakeplayer?std::string( "bot " ).append( _data.player_info.name ).c_str( ):_data.player_info.name );
	std::transform( print.begin( ), print.end( ), print.begin( ), ::tolower );


	dsdsDrawText ( c_menu::get ( ).smallf, print, ImVec2 ( ( _data.box_data.x + ( ( _data.box_data.x + _data.box_data.w ) - _data.box_data.x ) / 2.0f ), _data.box_data.y - 15 ), 12, _data.enemy ? config.visuals_enemy_name_color : config.visuals_team_name_color, 0.1f, true, false );
}
void visuals::player::box( visuals::player::data _data ) {
	bool should_show = _data.enemy ? config.visuals_enemy_box : config.visuals_team_box;
	if ( !should_show )
		return;

	auto clr = _data.enemy ? config.visuals_enemy_box_color : config.visuals_team_box_color;

	c_menu::get( ).draw->AddRect( ImVec2( _data.box_data.x - 0.5f, _data.box_data.y - 0.5f ), ImVec2( _data.box_data.x + _data.box_data.w + 0.5, _data.box_data.y + _data.box_data.h  + 0.5 ), ImColor( 0, 0, 0, 155 ), 0.0f );

	c_menu::get( ).draw->AddRect( ImVec2( _data.box_data.x, _data.box_data.y ), ImVec2( _data.box_data.x + _data.box_data.w, _data.box_data.y + _data.box_data.h ), ImColor( clr[0], clr[1], clr[2], clr[3] ), 0.0f );


}
void visuals::player::health( visuals::player::data _data ) {
	bool should_show = _data.enemy ? config.visuals_enemy_health : config.visuals_team_health;
	if ( !should_show )
		return;
	static auto get_health_clr = [ & ] ( int health ) {
		return color( static_cast< int >( 255 - ( health * 2.55f ) ), static_cast< int >( health * 2.55f ), 0, 100 );
	};

	static auto get_health_clr_1 = [ ] ( int health ) {
		auto clr = get_health_clr( health );

		clr.r -= 75;
		clr.g -= 75;
		clr.b -= 75;

		if ( clr.r < 0 )
			clr.r = 0;

		if ( clr.g < 0 )
			clr.g = 0;

		if ( clr.b < 0 )
			clr.b = 0;

		if ( clr.a < 0 )
			clr.a = 0;

		return ImColor(clr.r,clr.g,clr.b);
	};


	static auto filled_box_outlined = [ ] ( const int x, const int y, const int w, const int h, const ImColor color, const ImColor outline, const int thickness ) {
		c_menu::get ( ).draw->AddRectFilled ( ImVec2 ( x, y ), ImVec2 ( x + w, y + h), color );
		c_menu::get ( ).draw->AddRect ( ImVec2 ( x, y ), ImVec2 ( x + w, y + h ), outline, 0, thickness );
	};
	static auto filled_box = [ ] ( const int x, const int y, const int w, const int h, const ImColor color) {
		c_menu::get ( ).draw->AddRectFilled ( ImVec2 ( x, y ), ImVec2 ( x + w, y + h ), color );
	};
	auto clr_h = _data.enemy ? config.visuals_enemy_health_color : config.visuals_team_health_color;

	

	int elements = 0;

	auto health = _data.health;
	health = std::clamp ( health, 0, 100 );
	auto multiplier = health / 100.f;
	multiplier = std::clamp ( multiplier, 0.f, 1.f );
	const auto height = ( _data.box_data.h - 2 ) * multiplier;

	const int red = 255 - health * 2.55;
	const int green = health * 2.55;

	filled_box_outlined ( _data.box_data.x - 7 - elements * 6, _data.box_data.y - 0.5f, 4, _data.box_data.h + 0.5f, ImColor ( 0, 0, 0, static_cast< int >( 200 * 0.3f ) ), ImColor ( 0, 0, 0, static_cast< int >( 255 ) ), 1 );
	filled_box ( _data.box_data.x  - 6 - elements * 6, _data.box_data.y + _data.box_data.h - height - 1 - 0.5f, 2, height + 0.5f, ImColor ( red, green, 0, static_cast< int >( 255 ) ) );

	//c_menu::get ( ).draw->AddRectFilled ( ImVec2 ( _data.box_data.x - _data.box_data.w - 4 - elements * 6, _data.box_data.y - 0.5f ), ImVec2 ( _data.box_data.x - 2.5f, ( _data.box_data.y + ( _data.box_data.h * ( ( float ) _data.health / 100.0f ) ) ) + 0.5f ), get_health_clr_1 ( _data.health ) );
	//_ ( d, "%d" );

//	if ( health != 100 )
	//	render::get ( ).text ( ImVec2 ( player.top.x - player.width - 3, player.top.y + player.height - height - 5 ), tfm::format ( d, player.health ), Color ( 255, 255, 255, static_cast< int >( player.alpha ) ), fonts::esp_small, c_font::centered_x | c_font::centered_y | c_font::drop_shadow );
		auto x_p = _data.box_data.x - 6 - elements * 6;
		auto w_p = 4;
		dsdsDrawText ( c_menu::get ( ).smallf, std::to_string(_data.health), ImVec2 ( ( x_p + ( ( x_p + w_p ) - x_p ) / 2.0f ),  (_data.box_data.y + _data.box_data.h)  - height ), 10, _data.enemy ? config.visuals_enemy_name_color : config.visuals_team_name_color, 0.1f, true, false );

}
void visuals::player::weapon( visuals::player::data _data ) {
	
	auto clr_h = _data.enemy ? config.visuals_enemy_weapon_color : config.visuals_team_weapon_color;
	auto x = _data.box_data.x - ( _data.box_data.x + _data.box_data.w / 2 );
	dsdsDrawText ( c_menu::get ( ).weapon_icons, _data.weapon_icon, ImVec2 ( ( _data.box_data.x + ( ( _data.box_data.x + _data.box_data.w ) - _data.box_data.x ) / 2.0f ), _data.box_data.y + _data.box_data.h + 5 ), 8, clr_h, 0.1f, true, true );
}

void visuals::player::arrow ( visuals::player::data _data ) {

	auto arc = [ ] ( float x, float y, float radius, float min_angle, float max_angle, ImColor col, float thickness ) {
		
			c_menu::get ( ).draw->PathArcTo ( ImVec2 ( x, y ), radius, DEG2RAD ( min_angle ), DEG2RAD ( max_angle ), 32 );
			c_menu::get ( ).draw->PathStroke ( col, false, thickness );
	
	};
	auto angle = csgo::original_viewangle.y - math::calc_angle ( engine_prediction::unpredicted_eye, _data.origin).y	 - 90;
	float width = 5.f;
	arc ( ImGui::GetWindowWidth() / 2, ImGui::GetWindowHeight ( ) / 2, 256, angle - width, angle + width, ImColor ( 1.f, 1.f, 1.f, 1.f ), 4.f );
	arc ( ImGui::GetWindowWidth ( ) / 2, ImGui::GetWindowHeight ( ) / 2, 250, angle - width, angle + width, ImColor ( 1.f, 1.f, 1.f, 0.5f ), 1.5f );

}
void visuals::player::present( ) {
	bool is_atleast_one = config.visuals_enemy_enabled || config.visuals_team_enabled;
	if ( !( is_atleast_one ) )
		return;
	

	for ( size_t i = 1; i <= 64; i++ ) {
		auto data = m_data.at ( i );
		if ( data.alive && data.valid && !data.out_of_pov ) {
			if ( data.on_screen ) {
				visuals::player::name ( data );
				visuals::player::box ( data );
				visuals::player::health ( data );
				visuals::player::weapon ( data );
				visuals::player::aimbot ( data );
			}
			else {
				visuals::player::arrow ( data );

			}
		}
	}


}

std::string visuals::weapon_to_icon( const int id )
{
	auto search = weapon_icons.find( id );
	if ( search != weapon_icons.end( ) )
		return std::string( &search->second, 1 );

	return "";
}

std::tuple<std::string, std::string> visuals::grenade_name (entity_t* ent, class_ids id ) {
	std::string name = "";
	std::string icon = "";
	switch ( id ) {
	case class_ids::molotov_projectile:
		name = "incendiary";
		icon = "p";
		break;
	case class_ids::smoke_grenade_projectile:
		name = "smoke";
		icon = "m";
		break;
	case class_ids::base_cs_grenade_projectile:
	{
		const model_t * model = ent->model ( );

		if ( !model )
			return std::tuple< std::string, std::string> ( name, icon );

		const auto hdr = interfaces::model_info->get_studio_model ( model );
		if ( hdr->name_char_array [ 16 ] == 's' ) {
			name = "flashbang";
			icon = "k";
		}
		else {
			name = "he grenade";
			icon = "l";
		}
	}

		break;
	case class_ids::decoy_projectile:
		name = "decoy";
		icon = "o";
		break;
	default:
		break;
	}
	return std::tuple< std::string, std::string> ( name, icon );
}
/*
{ weapon_flashbang, 'k' },
{ weapon_hegrenade, 'l' },
{ weapon_smokegrenade, 'm' },
{ weapon_molotov, 'n' },
{ weapon_decoy, 'o' },
{ weapon_incgrenade, 'p' },
{ weapon_c4, 'q' },*/


void visuals::player::sound ( ) {
	static CUtlVector<sound_info> sound_list;

	interfaces::engine_sound->get_active_sounds ( sound_list );
	auto sound_size = sound_list.Count();

	for ( int i = 0; i < sound_size; i++ ) {
		if ( !sound_list[i].origin )
			continue;

		auto player = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( sound_list [ i ].sound_source ) );
		if ( sound_list [ i ].m_bUpdatePositions ) {
			if ( visuals::player::m_data [ player->index ( ) ].dormant ) {
				visuals::player::m_data [ player->index ( ) ].origin = *sound_list [ i ].origin;
				visuals::player::m_data [ player->index ( ) ].last_seen_time = interfaces::globals->cur_time;
			}
		}

	}
}
void visuals::player::paint_traverse ( ) {
	bool is_atleast_one = config.visuals_enemy_enabled || config.visuals_team_enabled;
	if ( !( is_atleast_one ) )
		return;

	//visuals::player::sound ( );

	//m_data.clear( );
	for ( int i = 1; i <= interfaces::globals->max_clients; i++ ) {
		auto player = reinterpret_cast< player_t* >( interfaces::entity_list->get_client_entity( i ) );
		if ( player == local_player::m_data.pointer )
			continue;

		data & current_data = m_data [ i ];
	
		if ( !player ) {
			current_data.valid = false;
			continue;
		}
		current_data.valid = true;
		current_data.enemy = player->is_enemy ( );
		current_data.dormant = player->dormant ( );
		current_data.alive = player->health ( ) > 0;
		current_data.index = player->index ( );

	

		/*if ( current_data.valid && std::fabs ( current_data.last_seen_time - interfaces::globals->cur_time ) > 10.f ) {
			current_data.alive = false;
			current_data.valid = false;
		}*/

		if ( player->health ( ) <= 0 ) {
			current_data.alive = false;
			continue;
		}

		if ( !current_data.out_of_pov && std::fabs ( interfaces::globals->realtime - current_data.last_seen_time ) > 5.f ) {
			current_data.alive = false;
			current_data.dormant = true;
			current_data.out_of_pov = true;
			current_data.last_seen_time = 0;
			continue;
		}

		if ( player->team( ) == local_player::m_data.pointer->team() && !config.visuals_team_enabled )
			continue;
	
		if ( player->team ( ) != local_player::m_data.pointer->team ( ) && !config.visuals_enemy_enabled )
			continue;

		
	
	
		if ( current_data.alive ) {
		
			if ( !current_data.dormant ) {
				current_data.health = player->health ( );
				current_data.alive = player->health ( ) > 0;
				interfaces::engine->get_player_info ( player->index ( ), &current_data.player_info );
				current_data.origin = player->abs_origin ( );
				current_data.mins = player->collideable ( )->mins ( ) + current_data.origin;
				current_data.maxs = player->collideable ( )->maxs ( ) + current_data.origin;
				
				current_data.distance = player->abs_origin ( ).distance_to ( local_player::m_data.eye_position );
				if ( player->active_weapon ( ) ) {
					current_data.weapon_icon = weapon_to_icon ( player->active_weapon ( )->item_definition_index ( ) );
					current_data.weapon_name = player->active_weapon ( )->get_weapon_data ( )->szWeaponName;
				}
				current_data.out_of_pov = false;
				current_data.last_seen_time = interfaces::globals->realtime;
			}
			else {
				
				static auto FindHudElement = ( DWORD ( __thiscall * )( void *, const char * ) )utilities::pattern_scan ( "client.dll", "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28" );
				static auto hud_ptr = *( DWORD ** ) ( utilities::pattern_scan ( "client.dll", "81 25 ? ? ? ? ? ? ? ? 8B 01" ) + 2 );

				auto radar_base = FindHudElement ( hud_ptr, "CCSGO_HudRadar" );
				CCSGO_HudRadar * hud_radar = ( CCSGO_HudRadar * ) ( radar_base - 20 );

				if ( radar_base && hud_radar ) {
					const RadarPlayer_t & radar = hud_radar->radar_info [ i ];
					current_data.origin = radar.pos;
					current_data.health = radar.health;
					//current_data.last_seen_time = interfaces::globals->cur_time;
  					//current_data.alive = current_data.health > 0;
					//current_data.out_of_pov = false;
				}
			}
			if (!current_data.out_of_pov )
			   current_data.on_screen = get_playerbox ( player, current_data.box_data, current_data );
		}
		
	

	}

}
void visuals::player::aimbot ( visuals::player::data _data ) {
/*	for ( auto point : ragebot::hitscan_points [ _data.index ] ) {
		auto w2s = vec3_t ( );
		if ( interfaces::debug_overlay->world_to_screen ( point.point, w2s ) ) {
			c_menu::get ( ).draw->AddCircleFilled ( ImVec2 ( w2s.x, w2s.y ), 3.f, !point.is_safe ? ImColor ( 255, 0, 0, 255 ) : ImColor ( 255, 255, 0, 255 ) );
		}
	}*/
}

void visuals::player::think ( ) {
	if ( !config.visuals_modulation_enemy_enabled )
		return;
	for ( int i = 0; i < interfaces::glow_manager->size; i++ ) {
		if ( interfaces::glow_manager->objects [ i ].unused ( ) || !interfaces::glow_manager->objects [ i ].entity )
			continue;

		auto & glow_object = interfaces::glow_manager->objects [ i ];

		auto entity = reinterpret_cast< player_t * >( glow_object.entity );
		if ( !entity || entity->dormant ( ) )
			continue;

		if ( entity == local_player::m_data.pointer )
			continue;

		const auto client_class = entity->client_class ( );
		if ( !client_class )
			continue;

		bool is_teammate = entity->team ( ) == local_player::m_data.pointer->team ( );
		bool is_enemy = entity->team ( ) != local_player::m_data.pointer->team ( );

		switch ( client_class->class_id ) {
		case cs_player:
		{


			if ( is_enemy && config.visuals_modulation_enemy_glow )
				glow_object.set ( config.visuals_modulation_enemy_glow_color [ 0 ], config.visuals_modulation_enemy_glow_color [ 1 ], config.visuals_modulation_enemy_glow_color [ 2 ], 0.8f );
			else if ( is_teammate && config.visuals_modulation_team_glow_color )
				glow_object.set ( config.visuals_modulation_team_glow_color [ 0 ], config.visuals_modulation_team_glow_color [ 1 ], config.visuals_modulation_team_glow_color [ 2 ], 0.8f );

			break;
		}
		}
	}
}