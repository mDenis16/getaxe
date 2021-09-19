
#ifdef PREVIEW_D3D
#include "E:\repos\mDenis16\getaxe-menu-framework\ImGui Application\includes.h"
#else
#include "../includes.h"
#endif 

#include <tchar.h>
#include <queue>


namespace ui {
	namespace menu {

		static std::string config_name = "";
		static int config_list_id = -1;
		static ui::child_window* main_tab;

		ui::object* find_element(ui::object* base, int id) {/*trebuie  sa ne apucam de alogirtmica ca e grav https://www.geeksforgeeks.org/number-children-given-node-n-ary-tree/*/
			if (id == -1)
				return nullptr;

			ui::object* numChildren = nullptr;

			if (base == NULL)
				return 0;

			// Creating a queue and pushing the root
			std::queue<ui::object*> q;
			q.push(base);

			while (!q.empty()) {
				int n = q.size();

				// If this node has children
				while (n > 0) {

					// Dequeue an item from queue and
					// check if it is equal to x
					// If YES, then return number of children
					ui::object* p = q.front();
					q.pop();
					if (p->_id == id) {
						numChildren = p;
						return p;
					}

					// Enqueue all children of the dequeued item
					for (int i = 0; i < p->children.size(); i++)
						q.push(p->children[i]);
					n--;
				}
			}
			return numChildren;
		}
		void open_folder() { }
		void create_config() {
			auto window_ptr = static_cast<object*>(ui::window_pointer_cheat);
			auto ptr_list = static_cast<ui::config_box*>(find_element(window_ptr, config_list_id));

			if (!ptr_list)
				return;

			ptr_list->save_config(config_name, false);


		}
		void refresh_ext() {

			auto window_ptr = static_cast<object*>(ui::window_pointer_cheat);
			auto ptr_list = static_cast<ui::config_box*>(find_element(window_ptr, config_list_id));

			if (!ptr_list)
				return;

			ptr_list->refresh_list();




		}

		void settings_menu(object* main_window, object* sub_tab) {

			auto rage_window = new ui::child_window(GET_STR(""), 100.f, 100.f, ImColor(255, 255, 255, 0), main_window, child_type::main_container); rage_window->flex = flex_direction::block; rage_window->padding = 15;

			rage_window->tab_assign = sub_tab;

			float child_rounding = 15.f;

			static bool cc = false;


			main_tab = new ui::child_window(GET_STR(""), 100.f, 78.0, ImColor(255, 24, 27, 0), rage_window, float_side::none, child_rounding, 15.f);


			auto config_box = new ui::config_box(main_tab);
			config_list_id = config_box->_id;












			auto bottom_side = new ui::child_window(GET_STR(""), 100.f, 15.0, ImColor(255, 214, 27, 0), rage_window, float_side::none, child_rounding, 15.f);

			bottom_side->flex = flex_direction::automatic;
			bottom_side->padding = 0.f;

			bottom_side->flags |= flags::no_bg_shadow; bottom_side->flags |= flags::no_background;
			/*auto left_bottom_side = new ui::child_window ( "", 45.f, 100.0f, ImColor ( 255, 24, 255, 0 ), bottom_side, child_type::normal, flags::align_left | flags::vertical_align_center | flags::position_absolute );
			{

				new ui::text_input ( left_bottom_side, "Config name", config_name, text_type::string, 80.f, 40.f);
				new ui::button ( "Create config", left_bottom_side, create_config );


			}
			auto right_bottom_side = new ui::child_window ( "", 45.f, 100.0f, ImColor ( 255, 24, 255, 0 ), bottom_side, child_type::normal, flags::align_right  );
			{
				new ui::button ( "Refresh list", right_bottom_side, refresh_ext );
				new ui::button ( "Open folder", right_bottom_side, open_folder );
			}*/


			//
			auto left_bottom_side = new ui::child_window(GET_STR(""), 45.f, 100.0f, ImColor(255, 24, 255, 255), bottom_side, child_type::normal, flags::align_left | flags::vertical_align_center | flags::position_absolute); left_bottom_side->flags |= flags::no_bg_shadow; left_bottom_side->flags |= flags::no_background;

			{
				new ui::text_input(left_bottom_side, GET_STR("Config name"), config_name, text_type::string, 80.f, 40.f);
				new ui::button(GET_STR("Create config"), left_bottom_side, 80.f, 40.f, create_config);
			}

			auto right_bottom_side = new ui::child_window(GET_STR(""), 45.f, 100.0f, ImColor(255, 255, 255, 255), bottom_side, child_type::normal, flags::align_right | flags::vertical_align_center | flags::position_absolute); right_bottom_side->flags |= flags::no_bg_shadow; right_bottom_side->flags |= flags::no_background;

			{

				new ui::button(GET_STR("Refresh list"), right_bottom_side, 80.f, 40.f, refresh_ext);
				new ui::button(GET_STR("Open folder"), right_bottom_side, 80.f, 40.f, open_folder);
			}
		}
	}
}