
#ifdef PREVIEW_D3D
#include "E:\repos\mDenis16\getaxe-menu-framework\ImGui Application\includes.h"
#else
#include "../includes.h"
#endif 

#include <tchar.h>
#include <queue>



namespace ui {
	namespace menu {


		void visuals_player_menu ( object * main_window, sub_tab * sub_tab, int player ) {
			auto visuals_window = new ui::child_window ( "", 100.f, 80.f, ImColor ( 255, 255, 255, 0 ), main_window, child_type::main_container, flags::no_background ); visuals_window->flex = flex_direction::automatic; visuals_window->padding = 15;
			visuals_window->tab_assign = sub_tab;
			sub_tab->child_assign = visuals_window;

			float child_rounding = 15.f;

			auto & cfg = c_config::get ( ).player_visual [ player ];

			auto main_tab = new ui::child_window ( player > 0 ? "Team Assist" : "Enemy Assist", 45.f, 43.f, ImColor ( 23, 24, 27, 255 ), visuals_window, float_side::none, child_rounding, 15.f );
			{
				
				new ui::checkbox ( "Bounding Box", main_tab, cfg.bounding_box, cfg.bound_box_color, cfg.bound_box_keybind );
				new ui::checkbox ( "Name", main_tab, cfg.name, cfg.name_color );
				new ui::checkbox ( "Health", main_tab, cfg.health );
				new ui::checkbox ( "Weapon", main_tab, cfg.weapon );
				new ui::checkbox ( "Ammo", main_tab, cfg.ammo, cfg.ammo_color );
				new ui::checkbox ( "Skeleton", main_tab, cfg.skeleton, cfg.skeleton_color );
				new ui::multibox ( "Flags", main_tab, std::vector<std::string>{"solid", "flat"}, cfg.flags, cfg.flags_keybind);
					
			}


			auto second_tab = new ui::child_window ( "Modulation", 45.f, 50.0, ImColor ( 23, 24, 27, 255 ), visuals_window, float_side::none, child_rounding, 15.f );
			{
				new ui::checkbox ( "Glow", second_tab, cfg.glow, cfg.glow_color );
				new ui::combobox ( "Material type", second_tab, std::vector<std::string>{"solid", "flat", "reflective", "glow"}, cfg.chams_material, cfg.chams_material_keybind);
				new ui::checkbox ( "Visible chams", second_tab, cfg.visible_chams, cfg.visible_chams_color );
				new ui::checkbox ( "Hidden chams", second_tab, cfg.hidden_chams, cfg.hidden_chams_color );

				 if ( player == player_visual_type::ENEMY )
				new ui::checkbox ( "History chams", second_tab, cfg.history_chams, cfg.history_chams_color );

			}

			auto third_tab = new ui::child_window ( "Misc", 45.f, 30.0, ImColor ( 23, 24, 27, 255 ), visuals_window, float_side::none, child_rounding, 15.f );
			{
				if ( player == player_visual_type::ENEMY )
				new ui::checkbox ( "Out of fov indicator", third_tab, cfg.out_of_pov, cfg.out_of_pov_color );
				new ui::checkbox ( "Reveal on radar", third_tab, cfg.force_radar_reveal );
				new ui::checkbox ( "Footsteps", third_tab, cfg.foot_steps, cfg.foot_steps_color );
			}

		}
	}
}