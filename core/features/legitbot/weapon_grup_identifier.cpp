#include "../../helpers/helpers.h"
#include <renderlib/imgui/imgui.h>

#include "config.h"
#include "includes.h"
#include "legitbot.h"

bool c_legitbot::is_pistol ( void * weapon ) {
	if ( !weapon )
		return false;

	auto current_weapon = reinterpret_cast< weapon_t * >( weapon );
	int id = current_weapon->item_definition_index ( );
	static const std::vector<int> v = { weapon_glock, weapon_elite, weapon_p250, weapon_tec9, weapon_cz75a, weapon_usp_silencer, weapon_hkp2000, weapon_fiveseven };
	return ( std::find ( v.begin ( ), v.end ( ), id ) != v.end ( ) );
}
bool c_legitbot::is_heavy_pistol ( void * weapon ) {
	if ( !weapon )
		return false;

	auto current_weapon = reinterpret_cast< weapon_t * >( weapon );
	int id = current_weapon->item_definition_index ( );

	return id == weapon_deagle || id == weapon_revolver;
}

bool c_legitbot::is_sniper ( void * weapon ) {
	if ( !weapon )
		return false;

	auto current_weapon = reinterpret_cast< weapon_t * >( weapon );
	int id = current_weapon->item_definition_index ( );
	static const std::vector<int> v = { weapon_awp, weapon_ssg08, weapon_g3sg1, weapon_scar20 };
	return ( std::find ( v.begin ( ), v.end ( ), id ) != v.end ( ) );
}

bool c_legitbot::is_smg ( void * weapon ) {
	if ( !weapon )
		return false;

	auto current_weapon = reinterpret_cast< weapon_t * >( weapon );
	int id = current_weapon->item_definition_index ( );
	static const std::vector<int> v = { weapon_mac10, weapon_mp7, weapon_ump45, weapon_p90, weapon_bizon, weapon_mp9, weapon_mp5sd };
	return ( std::find ( v.begin ( ), v.end ( ), id ) != v.end ( ) );
}

bool c_legitbot::is_heavy ( void * weapon ) {
	if ( !weapon )
		return false;

	auto current_weapon = reinterpret_cast< weapon_t * >( weapon );
	int id = current_weapon->item_definition_index ( );
	static const std::vector<int> v = { weapon_nova, weapon_xm1014, weapon_sawedoff, weapon_m249, weapon_negev, weapon_mag7 };
	return ( std::find ( v.begin ( ), v.end ( ), id ) != v.end ( ) );
}



bool c_legitbot::is_rifle ( void * weapon ) {
	if ( !weapon )
		return false;

	auto current_weapon = reinterpret_cast< weapon_t * >( weapon );
	int id = current_weapon->item_definition_index ( );
	static const std::vector<int> v = { weapon_ak47,weapon_aug,weapon_famas,weapon_galilar,weapon_m249,weapon_m4a1,weapon_m4a1_silencer,weapon_negev,weapon_sg556 };
	return ( std::find ( v.begin ( ), v.end ( ), id ) != v.end ( ) );
}


int c_legitbot::weapon_category ( weapon_t * weapon ) {

	if ( is_pistol ( weapon ) )
		return 0;
	else if ( is_heavy_pistol ( weapon ) )
		return 1;
	else if ( is_smg ( weapon ) )
		return 2;
	else if ( is_rifle ( weapon ) )
		return 3;
	else if ( is_sniper ( weapon ) )
		return 4;

	return 0;
}
void c_legitbot::weapon_change ( ) {
	if ( local_player::data().alive && local_player::data().active_weapon ) {
		//std::cout << " config.active_category " << config.active_category << std::endl;
		if (local_player::data().active_weapon != last_weapon ) {

			config.active_category = weapon_category (local_player::data().active_weapon );

			ui::menu::legitbot::init_values ( config.weapon_mode > 0 ? &config.weapon_type [ config.active_weapon ] : &config.weapon_groups [ config.active_category ] );
			ui::menu::ragebot::init_values(config.ragebot.weapon_mode > 0 ? &config.ragebot.rage_weapon_type[config.active_weapon] : &config.ragebot.rage_weapon_groups[config.active_category]);


			last_weapon = local_player::data().active_weapon;
		}
	}
}