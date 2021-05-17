#include <iostream>
#include "../includes.h"

#include "../../../../dependencies/utilities/base64.h"
#include "../../../../dependencies/utilities/crypt_str.h"
#include <shlobj.h>
#include <iostream>
#include <fstream>
#include  <iomanip>
#include "../../../../dependencies/utilities/json.hpp"
#include "../config.h"
#include <sstream>
#include <random>
#include <string>
#include <tchar.h>
#include <thread>
#include <d3dx9.h>
#include <mutex>


	
namespace ui {

	void image_queue::remove ( int index ) {
		images_data.erase ( images_data.begin ( ) + index );
		delete this;

	}
	ui::child_window * items_center = nullptr;

	std::vector<image_queue *> images_data;

	std::vector<int> weapon_ids = { 1,	 2,	 3,	 4,	 7,	 8,	 9,	 10,	 11,	 13,	 14,	16,	 17,	 19,	20,	 23,24,	 25,	26,	27,	28,	 29,	30,	31,	32,	 33,	34,	35,	36,	37,	 38,	 39,	40,	 41,	 42,	 43,	 44,	 45,	46,	 47,	 48,	49,	 57,	59,60,	61,	 63,	64,	 68,	 69,	70,	72,	 74,	75,	76,	78,	 80,	81,	 82,	 83,	84,	 85,	 500,	 503,	 505,	506,	 507,	 508,	 509,	 512,	514,	515,	 516,	517,	 518,	519,	 520,	 521,     522,	 523,	 525,	 4725,	 5027,	 5028,	 5029,     5030,	 5031,	 5032,	 5033,	 5034,	 5035 };

	std::vector<std::string> weapon_names = {
   "weapon_deagle" ,	"weapon_elite" ,	"weapon_fiveseven" ,	"weapon_glock" ,	"weapon_ak47" ,	"weapon_aug" ,	"weapon_awp" ,	"weapon_famas" ,	"weapon_g3sg1" ,	"weapon_galilar" ,	"weapon_m249" ,	"weapon_m4a1" ,	"weapon_mac10" ,	"weapon_p90" ,	"weapon_zone_repulsor" ,	"weapon_mp5sd" ,	"weapon_ump45" ,	"weapon_xm1014" ,	"weapon_bizon" ,	"weapon_mag7" ,	"weapon_negev" ,	"weapon_sawedoff" ,	"weapon_tec9" ,	"weapon_taser" ,	"weapon_hkp2000" ,	"weapon_mp7" ,	"weapon_mp9" ,	"weapon_nova" ,	"weapon_p250" ,	"weapon_shield" ,	"weapon_scar20" ,	"weapon_sg556" ,	"weapon_ssg08" ,	"weapon_knifegg" ,	"weapon_knife" ,	"weapon_flashbang" ,	"weapon_hegrenade" ,	"weapon_smokegrenade" ,	"weapon_molotov" ,	"weapon_decoy" ,	"weapon_incgrenade" ,	"weapon_c4" ,	"weapon_healthshot" ,	"weapon_knife_t" ,	"weapon_m4a1_silencer" ,	"weapon_usp_silencer" ,	"weapon_cz75a" ,	"weapon_revolver" ,	"weapon_tagrenade" ,	"weapon_fists" ,	"weapon_breachcharge" ,	"weapon_tablet" ,	"weapon_melee" ,	"weapon_axe" ,	"weapon_hammer" ,	"weapon_spanner" ,	"weapon_knife_ghost" ,	"weapon_firebomb" ,	"weapon_diversion" ,	"weapon_frag_grenade" ,	"weapon_snowball" ,	"weapon_bumpmine" ,	"weapon_bayonet" ,	"weapon_knife_css" ,	"weapon_knife_flip" ,	"weapon_knife_gut" ,	"weapon_knife_karambit" ,	"weapon_knife_m9_bayonet" ,	"weapon_knife_tactical" ,	"weapon_knife_falchion" ,	"weapon_knife_survival_bowie" ,	"weapon_knife_butterfly" ,	"weapon_knife_push" ,	"weapon_knife_cord" ,	"weapon_knife_canis" ,	"weapon_knife_ursus" ,	"weapon_knife_gypsy_jackknife" ,	"weapon_knife_outdoor" ,	"weapon_knife_stiletto" ,	"weapon_knife_widowmaker" ,	"weapon_knife_skeleton" ,	"studded_brokenfang_gloves" ,	"studded_bloodhound_gloves" ,	"t_gloves" ,	"ct_gloves" ,	"sporty_gloves" ,	"slick_gloves" ,	"leather_handwraps" ,	"motorcycle_gloves" ,	"specialist_gloves" ,	"studded_hydra_gloves" };

	std::vector<std::string> display_names = {
	   "Deagle" ,	"Elite" ,	"Fiveseven" ,	"Glock" ,	"Ak47" ,	"Aug" ,	"Awp" ,	"Famas" ,	"G3sg1" ,	"Galilar" ,	"M249" ,	"M4a1" ,	"Mac10" ,	"P90" ,	"Zone Repulsor" ,	"Mp5sd" ,	"Ump45" ,	"Xm1014" ,	"Bizon" ,	"Mag7" ,	"Negev" ,	"Sawedoff" ,	"Tec9" ,	"Taser" ,	"Hkp2000" ,	"Mp7" ,	"Mp9" ,	"Nova" ,	"P250" ,	"Shield" ,	"Scar20" ,	"Sg556" ,	"Ssg08" ,	"Knifegg" ,	"Knife" ,	"Flashbang" ,	"Hegrenade" ,	"Smokegrenade" ,	"Molotov" ,	"Decoy" ,	"Incgrenade" ,	"C4" ,	"Healthshot" ,	"Knife T" ,	"M4a1 Silencer" ,	"Usp Silencer" ,	"Cz75a" ,	"Revolver" ,	"Tagrenade" ,	"Fists" ,	"Breachcharge" ,	"Tablet" ,	"Melee" ,	"Axe" ,	"Hammer" ,	"Spanner" ,	"Knife Ghost" ,	"Firebomb" ,	"Diversion" ,	"Frag Grenade" ,	"Snowball" ,	"Bumpmine" ,	"Bayonet" ,	"Knife Css" ,	"Knife Flip" ,	"Knife Gut" ,	"Knife Karambit" ,	"Knife M9 Bayonet" ,	"Knife Tactical" ,	"Knife Falchion" ,	"Knife Survival Bowie" ,	"Knife Butterfly" ,	"Knife Push" ,	"Knife Cord" ,	"Knife Canis" ,	"Knife Ursus" ,	"Knife Gypsy Jackknife" ,	"Knife Outdoor" ,	"Knife Stiletto" ,	"Knife Widowmaker" ,	"Knife Skeleton" ,	"Studded Brokenfang Gloves" ,	"Studded Bloodhound Gloves" ,	"T Gloves" ,	"Ct Gloves" ,	"Sporty Gloves" ,	"Slick Gloves" ,	"Leather Handwraps" ,	"Motorcycle Gloves" ,	"Specialist Gloves" ,	"Studded Hydra Gloves" };

	std::mutex mutex_image_data;
	void ui::handle_image_data ( ) {
		//images_data.reserve ( 100 );



		static auto start_time = 0.f;


		int i = 0;

		while ( true ) {
			if ( PeekMessage ( &ui::msg, NULL, 0U, 0U, PM_REMOVE ) ) {
				TranslateMessage ( &ui::msg );
				DispatchMessage ( &ui::msg );
				continue;
			}

			static float last_gpu_scan = 0.f;


			if ( ui::window_device && mutex_image_data.try_lock ( ) ) {
				//if ( ImGui::GetTime ( ) != -1.0 && ( ImGui::GetTime ( ) - start_time ) > 8.f ) {
					if ( images_data.size ( ) > 0 ) {
						if ( ImGui::GetTime ( ) != -1.0 && ( ImGui::GetTime ( ) - last_gpu_scan ) > 0.1f ) {
							last_gpu_scan = ImGui::GetTime ( );

							if ( i >= images_data.size ( ) )
								i = 0;

							//for ( size_t i = 0; i < images_data.size ( ); i++ ) {

							auto & img = images_data.at ( i );

							if ( !img->done ) {

								std::optional<c_vpk_entry> entry = static_cast< c_vpk_archive * >( img->archive )->get_file ( img->location );

								if ( entry ) {
									std::optional<std::vector<uint8_t>> png_data = entry.value ( ).get_data ( );
									if ( png_data.has_value ( ) )
										D3DXCreateTextureFromFileInMemory ( reinterpret_cast< IDirect3DDevice9 * >( ui::window_device ), ( char * ) png_data.value ( ).data ( ), png_data.value ( ).size ( ), img->image_buffer );


								}

								img->done = true;
							}
							//}
							i++;
						}
					}


			//	}
				mutex_image_data.unlock ( );
			}
		}
	}

	void inventory_changer::on_start ( ) {
		std::ifstream in ( "E:\\repos\\mDenis16\\getaxe-menu-framework\\Release\\skins.json" );

		archive = new c_vpk_archive ( );
		static_cast<c_vpk_archive*>(archive)->load ( "E:/SteamLibrary/steamapps/common/Counter-Strike Global Offensive/csgo/pak01_dir.vpk" );


		nlohmann::json json;
		in >> json;

		nlohmann::json paintkit_display_names = json [ "paintkit_names" ];
		nlohmann::json paintkit_ids = json [ "paintkit_ids" ];

		for ( size_t i = 0; i < weapon_ids.size(); i++ ) {
			auto  weap = new weapon ( weapon_ids.at(i) );


			auto str = std::to_string ( weapon_ids.at ( i ) );

			nlohmann::json paintkit_names = json [ "weapon_skins" ][ str ][ "paintkit_names" ];
			nlohmann::json paintkit_rarities = json [ "weapon_skins" ][ str ][ "paintkit_rarities" ];
			weap->weapon_name = weapon_names.at ( i );
			weap->display_name = display_names.at ( i );

			for ( size_t i = 0; i < paintkit_names.size ( ); i++ ) {
				auto  paint = new paintkit ( );
				paint->game_name = paintkit_names [ i ].get<std::string> ( );
				paint->rarity = paintkit_rarities [ i ].get<int> ( );
				paint->weapon_name = weap->weapon_name;
				paint->display_name = paintkit_display_names [ paint->game_name ].get<std::string> ( );
				paint->id = paintkit_ids [ paint->game_name ].get<int> ( );
				paint->generate_image_path ( );

				weap->paintkits.emplace_back ( paint );
			}

			this->weapons.emplace_back ( weap );

		}
		
		


	}
	void inventory_changer::asdad ( ) {

		for ( auto & weap : weapons ) {
			new inventory_item ( items_center, weap, this->archive );

		}

	}
	inventory_changer::inventory_changer ( object * parent ) {

		on_start ( );


		this->parrent = parent;
		this->renderer = this->parrent->renderer;


		this->parrent->add_children ( this );

		update ( );

		//	child_window ( std::string title_font, float percent_width, float percent_height, ImColor color, object * parent, float_side __float, float _rounding = 0.f, float _padding = 0.f ) {

		auto panel_container = new ui::panel_container ( this, stage ); panel_container->flags = flags::fullscreen;
		{
			auto weapons_panel = new ui::panel ( panel_container ); weapons_panel->flags = flags::fullscreen;
			weapons_panel->flex = flex_direction::automatic;
			{
				auto window = new ui::child_window ( "", 100.f, 100.f, ImColor ( 255, 255, 255, 0 ), weapons_panel, child_type::normal, flags::no_background ); window->flex = flex_direction::block; window->padding = 0;

				auto top_header = new ui::child_window ( "Header", 100.f, 10.f, ImColor ( 23, 24, 27, 255 ), window, float_side::none, 0.f, 0.f ); 
				top_header->flex = flex_direction::automatic;
				top_header->padding = 0.f;


				auto left_bottom_side = new ui::child_window ( "", 45.f, 100.0f, ImColor ( 255, 24, 255, 255 ), top_header, child_type::normal, flags::align_left | flags::vertical_align_center | flags::position_absolute );
				{
					new ui::text_input ( left_bottom_side, "Search weapon", this->search_text, text_type::string, 70.f, 50.f );
				}
			
				auto right_bottom_side = new ui::child_window ( "", 45.f, 100.0f, ImColor ( 255, 255, 255, 255 ), top_header, child_type::normal, flags::align_right | flags::vertical_align_center | flags::position_absolute );
				{
					
					new ui::button ( "New skin", right_bottom_side, 40.f, 50.f, std::bind ( &inventory_changer::asdad, this ) );
				}

				items_center = new ui::child_window ( "Items", 100.f, 90.f, ImColor ( 23, 24, 27, 255 ), window, float_side::none, 0.f, 0.f ); items_center->flags |= flags::hide_overflow;
				items_center->flags |= flags::scrollbar;

			 
				
			}
		}
	}
			/*	auto left_bottom_side = new ui::child_window ( "", 50.f, 100.0f, ImColor ( 255, 24, 255, 255 ), top_side, child_type::normal, flags::align_left | flags::vertical_align_center | flags::position_absolute );
				{

					new ui::text_input ( left_bottom_side, "Search weapon", this->search_text, text_type::string, 70.f, 50.f );



				}

				auto right_bottom_side = new ui::child_window ( "", 50.f, 100.0f, ImColor ( 255, 255, 255, 255 ), top_side, child_type::normal, flags::align_right | flags::vertical_align_center | flags::position_absolute );
				{
					new ui::button ( "New skin", right_bottom_side, 40.f, 50.f, refresh_ext );
				}
	*/
	//	auto center_side = new child_window ( "sadds", 100.f, 50.f, ImColor ( 255, 255, 255, 0 ), weapons_panel, float_side::none, 0.f, 0.f );
		//center_side->flags |= flags::hide_overflow;

		//for(auto& weap : weapons )
			//new inventory_item ( center_side, weap );

	

	void inventory_changer::parse_data ( ) {
		std::ifstream in ( "skins.json" );

		nlohmann::json json_data; in >> json_data;


	}

	void inventory_changer::draw ( ) {
		handle ( );

		this->renderer->AddRectFilled ( this->mins, this->maxs, ImColor ( 255, 0, 0, 255 ) );

	

		this->handle ( );

		
	
		for ( auto & child : children ) {
			child->draw ( );

		}




	}
	void inventory_changer::handle_mouse_input ( ) {
		if ( ui::focused_item != -1 )
			return;

		auto mouse_pos = ui::get_cursor ( );

		this->hovering = ( mouse_pos.x > this->mins.x && mouse_pos.y > this->mins.y && mouse_pos.x < this->maxs.x && mouse_pos.y < this->maxs.y );
	}

	void inventory_changer::handle ( ) {

		handle_mouse_input ( );
		auto mouse_pos = ui::get_cursor ( );




	}


	void inventory_changer::update ( ) {




		this->mins.x = this->parrent->mins.x + 30;
		this->maxs.x = this->parrent->maxs.x - 30;

		this->mins.y = this->parrent->mins.y + 10;
		this->maxs.y = this->parrent->maxs.y - 50;

		this->width = this->maxs.x - this->mins.x;
		this->height = this->maxs.y - this->mins.y;


		for ( auto & child : children ) {
			child->update ( );
		}

	}
	
	weapon::weapon ( int item_definition_index ) {
		

	}

	void paintkit::generate_image_path ( ) {

		auto skin_name = game_name;
		

		if ( strcmp ( weapon_name.c_str(), crypt_str ( "unknown" ) ) && strcmp ( weapon_name.c_str(), crypt_str ( "knife" ) ) && strcmp ( weapon_name.c_str(), crypt_str ( "gloves" ) ) ) //-V526
		{
			if ( skin_name.empty ( ) || skin_name == crypt_str ( "default" ) )
				image_path = crypt_str ( "resource/flash/econ/weapons/base_weapons/" ) + std::string ( weapon_name ) + crypt_str ( ".png" );
			else
				image_path = crypt_str ( "resource/flash/econ/default_generated/" ) + std::string ( weapon_name ) + crypt_str ( "_" ) + std::string ( skin_name ) + crypt_str ( "_light_large.png" );
		}
		else {
			if ( !strcmp ( weapon_name.c_str ( ), crypt_str ( "knife" ) ) )
				image_path = crypt_str ( "resource/flash/econ/weapons/base_weapons/weapon_knife.png" );
			else if ( !strcmp ( weapon_name.c_str ( ), crypt_str ( "gloves" ) ) )
				image_path = crypt_str ( "resource/flash/econ/weapons/base_weapons/ct_gloves.png" );
			else if ( !strcmp ( weapon_name.c_str ( ), crypt_str ( "unknown" ) ) )
				image_path = crypt_str ( "resource/flash/econ/weapons/base_weapons/weapon_snowball.png" );

		}
	}
}