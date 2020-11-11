#include "../features.hpp"
#include "../../menu/render/menu_render.hpp"
#include "../../menu/ImGui/imgui.h"
#include "../../menu/ImGui/imgui_internal.h"

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
	resolver::resolver_data [ entity->index ( ) ].brute_side = 0;
	resolver::resolver_data [ entity->index ( ) ].missed_shots = 0;
	
}

 visuals::player::data visuals::player::m_data[65];
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
	in.y = ( int ) top;
	in.w = int( right - left );
	in.h = int( bottom - top );

	return true;
}
float dsdsDrawText( ImFont* pFont, const std::string& text, const ImVec2& pos, float size, float const * color, float shadow, bool center, bool bold )
{
	ImGuiWindow* window = ImGui::GetCurrentWindow( );


	std::stringstream stream( text );
	std::string line;

	float y = 0.0f;
	int i = 0;

	while ( std::getline( stream, line ) )
	{
		ImVec2 textSize = pFont->CalcTextSizeA( size, FLT_MAX, 0.0f, line.c_str( ) );

		if ( center )
		{
			if ( bold ) {
				window->DrawList->AddText( pFont, size, ImVec2( ( pos.x - textSize.x / 2.0f ) + shadow, ( pos.y + textSize.y * i ) + shadow ), ImColor( 0, 0, 0, (int)color[3] ), line.c_str( ) );
				window->DrawList->AddText( pFont, size, ImVec2( ( pos.x - textSize.x / 2.0f ) - shadow, ( pos.y + textSize.y * i ) - shadow ), ImColor( 0, 0, 0, ( int ) color [ 3 ] ), line.c_str( ) );
				window->DrawList->AddText( pFont, size, ImVec2( ( pos.x - textSize.x / 2.0f ) + shadow, ( pos.y + textSize.y * i ) - shadow ), ImColor( 0, 0, 0, ( int ) color [ 3 ] ), line.c_str( ) );
				window->DrawList->AddText( pFont, size, ImVec2( ( pos.x - textSize.x / 2.0f ) - shadow, ( pos.y + textSize.y * i ) + shadow ), ImColor( 0, 0, 0, ( int ) color [ 3 ] ), line.c_str( ) );

			}
			window->DrawList->AddText( pFont, size, ImVec2( pos.x - textSize.x / 2.0f, pos.y + textSize.y * i ), ImColor( color[0], color [ 1 ], color [ 2 ], color [ 3 ] ), line.c_str( ) );
		}
		else
		{
			if ( bold ) {
				window->DrawList->AddText ( pFont, size, ImVec2 ( ( pos.x ) + 1, ( pos.y + textSize.y * i ) + shadow ), ImColor ( 0, 0, 0, ( int ) color [ 3 ] ), line.c_str ( ) );
				window->DrawList->AddText ( pFont, size, ImVec2 ( ( pos.x ) - 1, ( pos.y + textSize.y * i ) - shadow ), ImColor ( 0, 0, 0, ( int ) color [ 3 ] ), line.c_str ( ) );
				window->DrawList->AddText ( pFont, size, ImVec2 ( ( pos.x ) + 1, ( pos.y + textSize.y * i ) - shadow ), ImColor ( 0, 0, 0, ( int ) color [ 3 ] ), line.c_str ( ) );
				window->DrawList->AddText ( pFont, size, ImVec2 ( ( pos.x ) - 1, ( pos.y + textSize.y * i ) + shadow ), ImColor ( 0, 0, 0, ( int ) color [ 3 ] ), line.c_str ( ) );
			}
			window->DrawList->AddText( pFont, size, ImVec2( pos.x, pos.y + textSize.y * i ), ImColor( color[0], color [ 1 ], color [ 2 ], color[3] ), line.c_str( ) );
		}

		y = pos.y + textSize.y * ( i + 1 );
		i++;
	}

	return y;
}
void visuals::player::name( visuals::player::data _data ) {

	std::string print( _data.player_info.fakeplayer?std::string( "bot " ).append( _data.player_info.name ).c_str( ):_data.player_info.name );
	std::transform( print.begin( ), print.end( ), print.begin( ), ::tolower );

	auto right = ( _data.box_data.x + _data.box_data.w );
	auto x = ( _data.box_data.x + ( ( _data.box_data.x + _data.box_data.w ) - _data.box_data.x ) / 2.0f );

	//c_menu::get( ).draw->AddText( c_menu::get( ).smallf, 15, ImVec2( x, _data.box_data.y - 15 ), ImColor( 167, 24, 71, 255 ), print.data( ) );
	dsdsDrawText( c_menu::get( ).smallf, print, ImVec2( ( _data.box_data.x + ( ( _data.box_data.x + _data.box_data.w ) - _data.box_data.x ) / 2.0f ), _data.box_data.y  ), 15, _data.enemy ? variables::visuals::enemy::name_color : variables::visuals::team::name_color, 0.1f ,true , false);

	dsdsDrawText ( c_menu::get ( ).smallf, resolver::side_name(resolver::resolver_data[_data.index].brute_side), ImVec2 ( right + 2.f, _data.box_data.y + 5 ), 11, _data.enemy ? variables::visuals::enemy::name_color : variables::visuals::team::name_color, 0.1f, false, false );
	dsdsDrawText ( c_menu::get ( ).smallf, std::string( resolver::resolver_data [ _data.index ].extended_desync ? "full" : "low"), ImVec2 ( right + 2.f, _data.box_data.y + 10 ), 11, _data.enemy ? variables::visuals::enemy::name_color : variables::visuals::team::name_color, 0.1f, false, false );
	std::stringstream ss;
	ss << "safety " << resolver::resolver_data [ _data.index ].safety << "%";

	dsdsDrawText ( c_menu::get ( ).smallf, ss.str(), ImVec2 ( right + 2.f, _data.box_data.y + 20 ), 11, _data.enemy ? variables::visuals::enemy::name_color : variables::visuals::team::name_color, 0.1f, false, false );


	std::stringstream hit_feet;
	hit_feet << "correction " << resolver::resolver_data [ _data.index ].first_shoot_correction << "";
	if ( resolver::resolver_data [ _data.index ].first_shoot_correction > 0.f )
	dsdsDrawText ( c_menu::get ( ).smallf, hit_feet.str ( ), ImVec2 ( right + 2.f, _data.box_data.y + 30 ), 11, _data.enemy ? variables::visuals::enemy::name_color : variables::visuals::team::name_color, 0.1f, false, false );
	
}
void visuals::player::box( visuals::player::data _data ) {

	auto clr = _data.enemy ? variables::visuals::enemy::box_color : variables::visuals::team::box_color;

	c_menu::get( ).draw->AddRect( ImVec2( _data.box_data.x - 0.5 - 1, _data.box_data.y - 0.5 ), ImVec2( _data.box_data.x + _data.box_data.w + 0.5, _data.box_data.y + _data.box_data.h  - 0.5 ), ImColor( 0, 0, 0, 155 ), 0.0f );
	c_menu::get( ).draw->AddRect( ImVec2( _data.box_data.x - 1, _data.box_data.y ), ImVec2( _data.box_data.x + _data.box_data.w, _data.box_data.y + _data.box_data.h ), ImColor( clr[0], clr[1], clr[2], clr[3] ), 0.0f );

}
void visuals::player::health( visuals::player::data _data ) {

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

	auto clr_h = _data.enemy ? variables::visuals::enemy::health_color : variables::visuals::team::health_color;

	c_menu::get( ).draw->AddRectFilled( ImVec2( _data.box_data.x  - 6, _data.box_data.y - 0.5f  ), ImVec2( _data.box_data.x - 2.5f, ( _data.box_data.y + ( _data.box_data.h  * ( ( float ) _data.health / 100.0f )  ) ) + 0.5f ), get_health_clr_1(_data.health) );
	c_menu::get( ).draw->AddRect( ImVec2( _data.box_data.x - 6 - 0.5, _data.box_data.y - 0.5f ), ImVec2( _data.box_data.x - 2.5f, ( _data.box_data.y + _data.box_data.h ) ), ImColor( clr_h[0], clr_h[1], clr_h[2], clr_h[3] ) );


}
void visuals::player::weapon( visuals::player::data _data ) {
	
	auto clr_h = _data.enemy ? variables::visuals::enemy::weapon_color : variables::visuals::team::weapon_color;

	dsdsDrawText( c_menu::get( ).weapon_icons, _data.weapon_icon, ImVec2( ( _data.box_data.x + ( ( _data.box_data.x + _data.box_data.w ) - _data.box_data.x ) / 2.0f ), _data.box_data.y + _data.box_data.h + 10 ), 8, clr_h, 0.1f, true, true );

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
	bool is_atleast_one = variables::visuals::enemy::enabled || variables::visuals::team::enabled;
	if ( !( is_atleast_one ) )
		return;
	for ( size_t i = 1; i <= 64; i++ ) {
		auto data = m_data [ i ];
		if ( data.alive && data.valid ) {
			if ( data.on_screen ) {
				visuals::player::name ( data );
				visuals::player::box ( data );
				visuals::player::health ( data );
				visuals::player::weapon ( data );
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
void visuals::player::sound ( ) {
	std::vector<sound_info> sound_list;

	interfaces::engine_sound->get_active_sounds ( sound_list );
	auto sound_size = sound_list.size ( );
	if ( sound_size > 128 )
		sound_size = 128;
	for ( int i = 0; i < sound_size; i++ ) {
		if ( !sound_list.at(i).origin )
			continue;

		auto player = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( sound_list.at ( i ).sound_source ) );
	if ( visuals::player::m_data [ player->index ( ) ].dormant )
		visuals::player::m_data [ player->index ( ) ].origin = *sound_list.at ( i ).origin;

	}
}
void visuals::player::paint_traverse ( ) {
	bool is_atleast_one = variables::visuals::enemy::enabled || variables::visuals::team::enabled;
	if ( !( is_atleast_one ) )
		return;

	visuals::player::sound ( );

	//m_data.clear( );
	for ( int i = 1; i <= interfaces::globals->max_clients; i++ ) {
		auto player = reinterpret_cast< player_t* >( interfaces::entity_list->get_client_entity( i ) );

		data & current_data = m_data [ i ];
		current_data.valid = true;
		if ( !player ) {
			current_data.valid = false;
			continue;
		}
		if ( player == csgo::local_player )
			continue;

		if ( player->health ( ) <= 0 ) {
			current_data.alive = false;
			continue;
		}
		if ( player->team( ) == csgo::local_player->team() && !variables::visuals::team::enabled )
			continue;

		if ( player->team ( ) != csgo::local_player->team ( ) && !variables::visuals::enemy::enabled )
			continue;

		
	
		current_data.enemy = player->is_enemy ( );
		current_data.dormant = player->dormant ( );
		current_data.alive = player->health ( ) > 0;
		current_data.index = player->index();
		if ( !current_data.dormant ) {
			current_data.health = player->health ( );
			current_data.alive = player->health ( ) > 0;
			interfaces::engine->get_player_info ( player->index ( ), &current_data.player_info );
			current_data.origin = player->get_absolute_origin ( );
			current_data.mins = player->collideable ( )->mins ( ) + current_data.origin;
			current_data.maxs = player->collideable ( )->maxs ( ) + current_data.origin;
		

			if ( player->active_weapon ( ) ) {
				current_data.weapon_icon = weapon_to_icon ( player->active_weapon ( )->item_definition_index ( ) );
				current_data.weapon_name = player->active_weapon ( )->get_weapon_data ( )->szWeaponName;
			}

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
				current_data.alive = current_data.health > 0;
			}
		}

		if ( current_data.alive )
	    	current_data.on_screen = get_playerbox ( player, current_data.box_data, current_data );
		

	}

}