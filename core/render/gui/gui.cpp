#pragma once

enum item_definition_indexes {
	weapon_global = 0,
	weapon_deagle = 1,
	weapon_elite = 2,
	weapon_fiveseven = 3,
	weapon_glock = 4,
	weapon_ak47 = 7,
	weapon_aug = 8,
	weapon_awp = 9,
	weapon_famas = 10,
	weapon_g3sg1 = 11,
	weapon_galilar = 13,
	weapon_m249 = 14,
	weapon_m4a1 = 16,
	weapon_mac10 = 17,
	weapon_p90 = 19,
	weapon_mp5sd = 23,
	weapon_ump45 = 24,
	weapon_xm1014 = 25,
	weapon_bizon = 26,
	weapon_mag7 = 27,
	weapon_negev = 28,
	weapon_sawedoff = 29,
	weapon_tec9 = 30,
	weapon_taser = 31,
	weapon_hkp2000 = 32,
	weapon_mp7 = 33,
	weapon_mp9 = 34,
	weapon_nova = 35,
	weapon_p250 = 36,
	weapon_scar20 = 38,
	weapon_sg556 = 39,
	weapon_ssg08 = 40,
	weapon_knifegg = 41,
	weapon_knife = 42,
	weapon_flashbang = 43,
	weapon_hegrenade = 44,
	weapon_smokegrenade = 45,
	weapon_molotov = 46,
	weapon_decoy = 47,
	weapon_incgrenade = 48,
	weapon_c4 = 49,
	weapon_knife_t = 59,
	weapon_m4a1_silencer = 60,
	weapon_usp_silencer = 61,
	weapon_cz75a = 63,
	weapon_revolver = 64,
	weapon_knife_bayonet = 500,
	weapon_knife_flip = 505,
	weapon_knife_gut = 506,
	weapon_knife_karambit = 507,
	weapon_knife_m9_bayonet = 508,
	weapon_knife_tactical = 509,
	weapon_knife_falchion = 512,
	weapon_knife_bowie = 514,
	weapon_knife_butterfly = 515,
	weapon_knife_shadowdaggers = 516,
	weapon_knife_navaja = 520,
	weapon_knife_stiletto = 522,
	weapon_knife_ursus = 519,
	weapon_knife_widowmaker = 523
};


#include "includes.h"
#include <map>
#include <d3dx9.h>


std::map<int, std::string> weapon_names =
{
{ weapon_global, "Global" },
{ weapon_deagle, "Deagle" },
{ weapon_elite, "Dual Berretas" },
{ weapon_fiveseven, "Five Seven" },
{ weapon_glock, "Glock" },
{ weapon_hkp2000, "P2000" },
{ weapon_p250, "P250" },
{ weapon_usp_silencer, "USP" },
{ weapon_tec9, "Tec9" },
{ weapon_cz75a, "CZ76" },
{ weapon_revolver, "Revolver" },
{ weapon_mac10, "Mac-10" },
{ weapon_ump45, "Ump-45" },
{ weapon_bizon, "Bizon" },
{ weapon_mp7, "MP7" },
{ weapon_mp5sd, "MP5SD" },
{ weapon_mp9, "MP9" },
{ weapon_p90, "P90" },
{ weapon_galilar, "Galilar" },
{ weapon_famas, "Famas" },
{ weapon_m4a1, "M4A4" },
{ weapon_m4a1_silencer, "M4A1" },
{ weapon_aug, "AUG" },
{ weapon_sg556, "SG556" },
{ weapon_ak47, "AK47" },
{ weapon_g3sg1, "G3SG1" },
{ weapon_scar20, "Scar20" },
{ weapon_awp, "AWP" },
{ weapon_ssg08, "SSG08" },
{ weapon_xm1014, "XM1014" },
{ weapon_sawedoff, "Sawed-Off" },
{ weapon_mag7, "MAG7" },
{ weapon_nova, "Nova" },
{ weapon_negev, "Negev" },
{ weapon_m249, "M249" }
};
std::vector<std::string> weapons {  };
namespace ui {
	bool key_state [ 256 ];
	bool prev_key_state [ 256 ];
	void * window_pointer_cheat = nullptr;
	MSG msg;

	std::vector< key_bind_component *> key_bind_list;
	

	static bool init = false;

	long last_time_clicked_element = 0;
	static int weapon_id = 0;


	void initialize ( ImDrawList * render ) {

		for ( auto const & weap : weapon_names )
			weapons.push_back ( weap.second );

	

		main_window = new ui::window ( "neverwin", 600, 700, render, menu_opened, 12.f );
		main_window->flex = flex_direction::automatic;
		main_window->is_main_window = true;
		main_window->flags |= flags::window_resizable;
		window_pointer_cheat = static_cast< void * >( main_window );

		auto top_side = new ui::child_window (GET_STR(""), 100, 100, ImColor(25, 25, 25, 255), main_window, child_type::tab_container);
		top_side->rounding = 12.f;

		auto aimbot_tab = new ui::tab ( GET_STR ("Aimbot"), top_side, 13 );
		{
			auto ragebot_sub_tab = new ui::sub_tab ( GET_STR("Rage"), ICON_RAGEBOT, font_icons, 0.f, 20.f, aimbot_tab );
			menu::ragebot::init(main_window, ragebot_sub_tab, weapons);

		



			auto antiaim_sub_tab = new ui::sub_tab ( GET_STR("Anti-aim"), ICON_ANTIAIM, font_icons, 0.f, 20.f, aimbot_tab );
			menu::antiaim::init(main_window, antiaim_sub_tab);

			auto legitbot_sub_tab = new ui::sub_tab ( GET_STR("Legitbot"), ICON_LEGITBOT, font_icons, 0.f, 20.f, aimbot_tab );

			menu::legitbot::init ( main_window, legitbot_sub_tab, weapons );

			//auto combobox_weapon = new ui::combobox ( "", aimbot_tab, weapons, weapon_id );
		}


		auto visuals_tab = new ui::tab ( GET_STR("Visuals"), top_side, 13 );
		{
			/*
				{ "Team", "f" },
	{ "Enemy", "e" },
	{ "Local", "D" }
			*/
			auto team_sub_tab = new ui::sub_tab(GET_STR("Team"), config.player_types.at(0).second, font_icons, -0.5875f, 20.f, visuals_tab);
			menu::visuals_player_menu(main_window, team_sub_tab, 0);

			auto enemy_sub_tab = new ui::sub_tab(GET_STR("Enemy"), config.player_types.at(1).second, font_icons, -0.5875f, 20.f, visuals_tab);
			menu::visuals_player_menu(main_window, enemy_sub_tab, 1);

			auto local_sub_tab = new ui::sub_tab(GET_STR("Local"), config.player_types.at(2).second, font_icons, -0.5875f, 20.f, visuals_tab);
			menu::visuals_player_menu(main_window, local_sub_tab,2);

			auto weapons_sub_tab = new ui::sub_tab ( GET_STR("Weapons"), "W", ui::weapon_icons, -0.5875f, 20.f, visuals_tab );
			menu::visuals_weapons_menu ( main_window, weapons_sub_tab );

			auto projectiles_sub_tab = new ui::sub_tab ( GET_STR("Projectiles"), "o", ui::weapon_icons, -0.5875f, 20.f, visuals_tab );
			menu::projectiles_weapons_menu ( main_window, projectiles_sub_tab );

			auto world_sub_tab = new ui::sub_tab ( GET_STR("World"), ICON_WORLD, font_icons, -0.5875f, 20.f, visuals_tab );

			menu::visuals_world_menu ( main_window, world_sub_tab );

		}



		auto misc_tab = new ui::tab ( GET_STR("Miscellaneus"), top_side, 13 );
		{
			auto movement_sub_tab = new ui::sub_tab ( GET_STR("Movement"), ICON_MOVEMENT, font_icons, 0.f, 20.f, misc_tab );
		 	menu::movement_menu ( main_window, movement_sub_tab );


			auto skins_sub_tab = new ui::sub_tab ( GET_STR("Skins"), ICON_SETTINGS, font_icons, 0.f, 20.f, misc_tab );
			menu::inventory_changer_menu ( main_window, skins_sub_tab );

		}
		auto settings_tab = new ui::tab (GET_STR("Settings"), top_side, 13 );
		{
			auto configs_sub_tab = new ui::sub_tab (GET_STR("Configs"), ICON_SETTINGS, font_icons, 0.f, 20.f, settings_tab );
		   //	menu::settings_menu ( main_window, configs_sub_tab );
		}

		static bool open_radar = true;

		/* radar_window = new ui::window ( "Radar", 80, 80, render, open_radar, 12.f );
		 radar_window->flags |= flags::window_resizable;
		 radar_window_extern = radar_window;*/
	}

	void render_debug ( ) {

		static float old_time = 0.f;

		float time = std::fabs ( ImGui::GetIO ( ).DeltaTime - old_time );

		old_time = ImGui::GetIO ( ).DeltaTime;

	//	ImGui::Text ( std::to_string ( old_time ).c_str ( ) );

		for ( auto & item : cfg_manager->items ) {
			if ( item->key_bind ) {
				if ( item->key_bind->key ) {
					//ImGui::Text ( std::to_string ( ( float ) *( float * ) item->key_bind->new_value ).c_str ( ) );

					/*if ( key_down ( item->key_bind->key ) ) {
						*( bool * ) item->key_bind->value = true;
					}*/
					item->key_bind->update ( );

				}
			}
		}

	}

	void render ( ImDrawList * render ) {

		if ( !ui::font_widgets )
			return;

		static int last_type = 0;
		if ( config.active_weapon != last_type ) {
			menu::legitbot::init_values ( &config.weapon_type [ config.active_weapon ] );
			menu::ragebot::init_values(&config.ragebot.rage_weapon_type[config.active_weapon]);
			last_type = config.active_weapon;
		}


		if ( !main_window )
			initialize ( render );

#ifdef PREVIEW_D3D
		static IDirect3DTexture9 * image_buffer;
		if (!image_buffer )
		    D3DXCreateTextureFromFile ( reinterpret_cast< IDirect3DDevice9 * >( ui::window_device ), "C:\\Users\\topor\\Desktop\\Portofoliu fizica\\test.png", &image_buffer );

		render->AddImage ( image_buffer, ImVec2 ( 0, 0 ), ImVec2 ( 1920, 1080 ) );
#endif

		main_window->handle ( );
		main_window->draw ( );

	//	radar_window->handle ( );
		//radar_window->draw ( );

	//	std::string focused = ui::se
	//	render->AddText ( ui::font_widgets, 13.f, ImVec2 ( 1920 - 130, 50 ), ImColor ( 123, 255, 125, 255 ), log->text.c_str ( ) );


#ifdef PREVIEW_D3D
		int i = 0;
		static const float log_expire = 5;
		for ( auto & log : debug_logs ) {
			render->AddText ( ui::font_widgets, 13.f, ImVec2 ( 1920 - 130, 30 + ++i * 18 ), ImColor ( 123, 255, 125, 255 ), log->text.c_str() );

			if ( debug_logs.size ( ) > 20 )
				debug_logs.pop_front ( );
		}


#endif

		//printf ( "ssss \n" );
	}

	
	// ========================================================================

	void to_rgb ( ImColor & original ) {
		auto & Value = original.Value;

		Value.x = ( float ) Value.x * 255;
		Value.y = ( float ) Value.y * 255;
		Value.z = ( float ) Value.z * 255;

		Value.w = ( float ) Value.w * 255;

	}

	

}

