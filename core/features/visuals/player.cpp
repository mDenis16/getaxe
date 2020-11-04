#include "../features.hpp"
#include "../../menu/render/menu_render.hpp"
#include "../../menu/ImGui/imgui.h"
#include "../../menu/ImGui/imgui_internal.h"

std::vector< visuals::player::data> visuals::player::m_data;
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
}; bool get_playerbox( entity_t* ent, visuals::box& in ) {
	vec3_t origin, min, max, flb, brt, blb, frt, frb, brb, blt, flt;
	float left, top, right, bottom;

	origin = ent->get_absolute_origin( );
	min = ent->collideable( )->mins( ) + origin;
	max = ent->collideable( )->maxs( ) + origin;

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
float dsdsDrawText( ImFont* pFont, const std::string& text, const ImVec2& pos, float size, color color, float shadow, bool center, bool bold )
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
				window->DrawList->AddText( pFont, size, ImVec2( ( pos.x - textSize.x / 2.0f ) + shadow, ( pos.y + textSize.y * i ) + shadow ), ImColor( 0, 0, 0, color.a ), line.c_str( ) );
				window->DrawList->AddText( pFont, size, ImVec2( ( pos.x - textSize.x / 2.0f ) - shadow, ( pos.y + textSize.y * i ) - shadow ), ImColor( 0, 0, 0, color.a ), line.c_str( ) );
				window->DrawList->AddText( pFont, size, ImVec2( ( pos.x - textSize.x / 2.0f ) + shadow, ( pos.y + textSize.y * i ) - shadow ), ImColor( 0, 0, 0, color.a ), line.c_str( ) );
				window->DrawList->AddText( pFont, size, ImVec2( ( pos.x - textSize.x / 2.0f ) - shadow, ( pos.y + textSize.y * i ) + shadow ), ImColor( 0, 0, 0, color.a ), line.c_str( ) );

			}
			window->DrawList->AddText( pFont, size, ImVec2( pos.x - textSize.x / 2.0f, pos.y + textSize.y * i ), ImColor( color.r, color.g, color.b, color.a), line.c_str( ) );
		}
		else
		{
			window->DrawList->AddText( pFont, size, ImVec2( ( pos.x ) + 1, ( pos.y + textSize.y * i ) + shadow ), ImColor( 0, 0, 0, color.a ), line.c_str( ) );
			window->DrawList->AddText( pFont, size, ImVec2( ( pos.x ) - 1, ( pos.y + textSize.y * i ) - shadow ), ImColor( 0, 0, 0, color.a ), line.c_str( ) );
			window->DrawList->AddText( pFont, size, ImVec2( ( pos.x ) + 1, ( pos.y + textSize.y * i ) - shadow ), ImColor( 0, 0, 0, color.a ), line.c_str( ) );
			window->DrawList->AddText( pFont, size, ImVec2( ( pos.x ) - 1, ( pos.y + textSize.y * i ) + shadow ), ImColor( 0, 0, 0, color.a ), line.c_str( ) );

			window->DrawList->AddText( pFont, size, ImVec2( pos.x, pos.y + textSize.y * i ), ImColor( color.r, color.g, color.b, color.a ), line.c_str( ) );
		}

		y = pos.y + textSize.y * ( i + 1 );
		i++;
	}

	return y;
}
void visuals::player::name( visuals::player::data _data ) {
	if ( !variables::visuals::enemy::name )
		return;


	std::string print( _data.player_info.fakeplayer?std::string( "bot " ).append( _data.player_info.name ).c_str( ):_data.player_info.name );
	std::transform( print.begin( ), print.end( ), print.begin( ), ::tolower );

	auto right = ( _data.box_data.x + _data.box_data.w );
	auto x = ( _data.box_data.x + ( ( _data.box_data.x + _data.box_data.w ) - _data.box_data.x ) / 2.0f );

	//c_menu::get( ).draw->AddText( c_menu::get( ).smallf, 15, ImVec2( x, _data.box_data.y - 15 ), ImColor( 167, 24, 71, 255 ), print.data( ) );
	dsdsDrawText( c_menu::get( ).smallf, print, ImVec2( ( _data.box_data.x + ( ( _data.box_data.x + _data.box_data.w ) - _data.box_data.x ) / 2.0f ), _data.box_data.y - 15 ), 15, color(255, 0,0, 170), 0.1f ,true , false);
}
void visuals::player::box( visuals::player::data _data ) {
	if ( !variables::visuals::enemy::box )
		return;

	c_menu::get( ).draw->AddRect( ImVec2( _data.box_data.x - 0.5 - 1, _data.box_data.y - 0.5 ), ImVec2( _data.box_data.x + _data.box_data.w + 0.5, _data.box_data.y + _data.box_data.h  - 0.5 ), ImColor( 0, 0, 0, 155 ), 0.0f );
	c_menu::get( ).draw->AddRect( ImVec2( _data.box_data.x - 1, _data.box_data.y ), ImVec2( _data.box_data.x + _data.box_data.w, _data.box_data.y + _data.box_data.h ), ImColor( 167, 24, 71, 255 ), 0.0f );

}
void visuals::player::health( visuals::player::data _data ) {
	if ( !variables::visuals::enemy::health )
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


	c_menu::get( ).draw->AddRectFilled( ImVec2( _data.box_data.x  - 6, _data.box_data.y - 0.5f  ), ImVec2( _data.box_data.x - 2.5f, ( _data.box_data.y + ( _data.box_data.h  * ( ( float ) _data.health / 100.0f )  ) ) + 0.5f ), get_health_clr_1(_data.health) );
	c_menu::get( ).draw->AddRect( ImVec2( _data.box_data.x - 6 - 0.5, _data.box_data.y - 0.5f ), ImVec2( _data.box_data.x - 2.5f, ( _data.box_data.y + _data.box_data.h ) ), ImColor( 0, 0, 0, 155 ) );


}
void visuals::player::weapon( visuals::player::data _data ) {
	if ( !variables::visuals::enemy::weapon )
		return;
	dsdsDrawText( c_menu::get( ).weapon_icons, _data.weapon_icon, ImVec2( ( _data.box_data.x + ( ( _data.box_data.x + _data.box_data.w ) - _data.box_data.x ) / 2.0f ), _data.box_data.y + _data.box_data.h + 10 ), 8, color( 255, 255, 255, 170 ), 0.1f, true, true );

}
void visuals::player::present( ) {
	if ( !variables::visuals::enemy::enabled )
		return;
	for ( auto data : m_data ) {
		visuals::player::name(data );
		visuals::player::box( data );
		visuals::player::health( data );
		visuals::player::weapon( data );
	}


}

std::string visuals::weapon_to_icon( const int id )
{
	auto search = weapon_icons.find( id );
	if ( search != weapon_icons.end( ) )
		return std::string( &search->second, 1 );

	return "";
}

void visuals::player::paint_traverse( ) {
	if ( !variables::visuals::enemy::enabled )
		return;
	m_data.clear( );
	for ( int i = 1; i <= interfaces::globals->max_clients; i++ ) {
		auto player = reinterpret_cast< player_t* >( interfaces::entity_list->get_client_entity( i ) );

		if ( !player )
			continue;

		if ( player == csgo::local_player )
			continue;

		if ( player->health( ) <= 0 )
			continue;

		//if ( player->team( ) == interfaces:: )
		//	continue;
		
		data current_data;
	
		current_data.health = player->health( );
		interfaces::engine->get_player_info( player->index( ), &current_data.player_info );
		get_playerbox( player, current_data.box_data );

		if ( player->active_weapon( ) )
		{
			current_data.weapon_icon = weapon_to_icon( player->active_weapon( )->item_definition_index( ) );
			current_data.weapon_name = player->active_weapon( )->get_weapon_data( )->szWeaponName;
		}

		m_data.push_back( current_data );

	}
}