
#ifdef PREVIEW_D3D
#include "E:\repos\mDenis16\getaxe-menu-framework\ImGui Application\includes.h"
#else
#include "../includes.h"
#endif 

#include <tchar.h>
#include <queue>



namespace ui {
	namespace menu {

	
		namespace legitbot {
			child_window * main_tab = nullptr;
			child_window * filters_tab = nullptr;
			child_window * weapons_tab = nullptr;
			child_window * bezier_tab = nullptr;

			void init_values ( weapon_settings * cfg ) {
				//general tab
				main_tab->empty_children ( );
				{
					new ui::checkbox ( "Enable", main_tab, cfg->enabled, &cfg->enable_keybind );
					new ui::slider ( "Fov Min", main_tab, cfg->minimum_fov, 0.f, 180.f, ui::slider_type::floates, &cfg->minimum_fov_keybind );
					new ui::slider ( "Fov Max", main_tab, cfg->maximum_fov, 0.f, 180.f, ui::slider_type::floates, &cfg->maximum_fov_keybind );
					new ui::slider ( "Target delay", main_tab, cfg->target_delay, 0.f, 180.f, ui::slider_type::floates, &cfg->target_delay_keybind );
					new ui::combobox ( "Hitbox target", main_tab, std::vector<std::string>{"Head", "Body", "Pelvis"}, cfg->hitbox, cfg->hitbox_keybind );
					new ui::multibox ( "Hitscan list", main_tab, std::vector<std::string>{"Head", "Body", "Pelvis", "Feets"}, cfg->hitscan, cfg->hitscan_keybind );

				}

				//filters tab
				{

				}
				//


				//bezier tab

				{
					new ui::bezier_editor ( bezier_tab, cfg->bezier_curve );

				}
			}
			void on_weapon_change ( int weapon, int category ) {
				init_values ( config.weapon_mode > 0 ? &config.weapon_type [ config.active_weapon ] : &config.weapon_groups [ config.active_category ] );
			}
			void init ( object * main_window, sub_tab * sub_tab, std::vector<std::string> weapons ) {
				float child_rounding = 15.f;

				auto legitbot_window = new ui::child_window ( "", 100.f, 80.f, ImColor ( 255, 255, 255, 0 ), main_window, child_type::main_container, flags::no_background ); legitbot_window->flex = flex_direction::automatic; legitbot_window->padding = 15;
				legitbot_window->tab_assign = sub_tab;
				sub_tab->child_assign = legitbot_window;

				 main_tab = new ui::child_window ( "General", 45.f, 50.f, ImColor ( 23, 24, 27, 255 ), legitbot_window, float_side::none, child_rounding, 15.f );
				
				 filters_tab = new ui::child_window ( "Filters", 45.f, 50.f, ImColor ( 23, 24, 27, 255 ), legitbot_window, float_side::none, child_rounding, 15.f );

				 weapons_tab = new ui::child_window ( "Weapon", 45.f, 20.f, ImColor ( 23, 24, 27, 255 ), legitbot_window, float_side::none, child_rounding, 15.f );
				 {
					 new ui::combobox ( "Config mode", weapons_tab, std::vector<std::string>{"Category", "Weapon"}, & config.weapon_mode );

					 auto panel_container = new ui::panel_container ( weapons_tab, config.weapon_mode );
					 {
						 auto category_panel = new ui::panel ( panel_container );
						 {
							 new ui::combobox ( "Category", category_panel, config.weapon_categories, & config.active_category );
						 }
						 auto weapons_panel = new ui::panel ( panel_container );
						 {
							 new ui::combobox ( "Weapon", weapons_panel, weapons, & config.active_weapon );
						 }
					 }

				 }
				 bezier_tab = new ui::child_window ( "Curve settings", 45.f, 50.f, ImColor ( 23, 24, 27, 255 ), legitbot_window, float_side::none, child_rounding, 15.f );
			
				 init_values ( config.weapon_mode > 0 ? &config.weapon_type [ config.active_weapon ] : &config.weapon_groups [ config.active_category ] );

			}
			
		}
		
	}
}