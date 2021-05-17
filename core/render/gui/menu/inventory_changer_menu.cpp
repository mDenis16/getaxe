
#ifdef PREVIEW_D3D
#include "E:\repos\mDenis16\getaxe-menu-framework\ImGui Application\includes.h"
#else
#include "../includes.h"
#endif 
#include <tchar.h>
#include <queue>



namespace ui {
	namespace menu {


		void inventory_changer_menu ( object * main_window, sub_tab * sub_tab ) {
			auto inventory_window = new ui::child_window ( "", 100.f, 100.f, ImColor ( 255, 255, 255, 0 ), main_window, child_type::main_container, flags::no_background ); inventory_window->flex = flex_direction::automatic; inventory_window->padding = 15;
			inventory_window->tab_assign = sub_tab;
			sub_tab->child_assign = inventory_window;

			new inventory_changer ( inventory_window );

		}
	}
}