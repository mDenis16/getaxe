
#ifdef PREVIEW_D3D
#include "E:\repos\mDenis16\getaxe-menu-framework\ImGui Application\includes.h"
#else
#include "../includes.h"
#endif 
#include <tchar.h>
#include <queue>



namespace ui {
	namespace menu {

		
		void visuals_local_menu ( object * main_window, sub_tab * sub_tab) {
			auto visuals_window = new ui::child_window ( "", 100.f, 80.f, ImColor ( 255, 255, 255, 0 ), main_window, child_type::main_container ); visuals_window->flex = flex_direction::automatic; visuals_window->padding = 15;
			visuals_window->tab_assign = sub_tab;
			sub_tab->child_assign = visuals_window;

			float child_rounding = 15.f;

			auto main_tab = new ui::child_window ( "View assist", 45.f, 43.f, ImColor ( 23, 24, 27, 255 ), visuals_window, float_side::none, child_rounding, 15.f );
			{

				static int keybind = 0;
				static int keybind_type = 0;

				new ui::checkbox ( "Thirdperson", main_tab, config.local.thirdperson, &config.local.key_bind_thirdperson );
				new ui::slider ( "Distance", main_tab, config.local.thirdperson_distance, 0.f, 200.f, slider_type::floates, &config.local.key_bind_thirdperson_distance);

			}


			auto second_tab = new ui::child_window ( "Modulation", 45.f, 50.0, ImColor ( 23, 24, 27, 255 ), visuals_window, float_side::none, child_rounding, 15.f );
			{
				
			

			}

			auto third_tab = new ui::child_window ( "Misc", 45.f, 30.0, ImColor ( 23, 24, 27, 255 ), visuals_window, float_side::none, child_rounding, 15.f );
			{
		
			}

		}
	
	}
}