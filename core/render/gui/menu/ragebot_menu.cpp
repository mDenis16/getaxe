

#ifdef PREVIEW_D3D
#include "E:\repos\mDenis16\getaxe-menu-framework\ImGui Application\includes.h"
#else
#include "../includes.h"
#endif 


namespace ui {
	namespace menu {
		void ragebot_menu(object* main_window, object* sub_tab) {
			auto rage_window = new ui::child_window(GET_STR(""), 100.f, 100.f, ImColor(255, 255, 255, 0), main_window, child_type::main_container); rage_window->flex = flex_direction::automatic; rage_window->padding = 15;

			rage_window->tab_assign = sub_tab;

			float child_rounding = 15.f;

			auto main_tab = new ui::child_window(GET_STR("Enemy Assist"), 45.f, -1.f, ImColor(23, 24, 27, 255), rage_window, float_side::none, child_rounding, 15.f);

			{
				static bool chesad = false;
				static ImColor ceaudar = ImColor(255, 0, 0, 255);

				static int key_bind_Type = 0;
				static int  key_bind = 0;

			}


			new ui::child_window(GET_STR("Accuracy2"), 45.f, 50.0, ImColor(23, 24, 27, 255), rage_window, float_side::none, child_rounding, 15.f);

			new ui::child_window(GET_STR("Exploits"), 45.f, 20.0, ImColor(23, 24, 27, 255), rage_window, float_side::none, child_rounding, 55.f);


			new ui::child_window(GET_STR("Exploits"), 45.f, 20.0, ImColor(255, 255, 255, 255), rage_window, float_side::none, child_rounding, 15.f);


			new ui::child_window(GET_STR("Exploits"), 45.f, 30.0, ImColor(23, 24, 27, 255), rage_window, float_side::none, child_rounding, 15.f);

		}
	}
}