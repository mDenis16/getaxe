
#ifdef PREVIEW_D3D
#include "D:\repos\mDenis16\getaxe-menu-framework\ImGui Application\includes.h"
#else
#include "../includes.h"
#endif 
#include <tchar.h>
#include <queue>



namespace ui {
	namespace menu {


		void movement_menu(object* main_window, sub_tab* sub_tab) {
			auto visuals_window = new ui::child_window(GET_STR(""), 100.f, 80.f, ImColor(255, 255, 255, 0), main_window, child_type::main_container, flags::no_background); visuals_window->flex = flex_direction::automatic; visuals_window->padding = 15;

			visuals_window->tab_assign = sub_tab;
			sub_tab->child_assign = visuals_window;

			float child_rounding = 15.f;

			auto& movement_cfg = config.misc.movement;

			auto main_tab = new ui::child_window(GET_STR("Jump assist"), 45.f, 50.f, ImColor(23, 24, 27, 255), visuals_window, float_side::none, child_rounding, 15.f);

			{

				new ui::combobox(GET_STR("Bunny Hop"), main_tab, std::vector<std::string>{"none", "perfect", "legit"}, &movement_cfg.bunny_hop_type, movement_cfg.bunny_hop_type);
				auto bunnyhop_container = new ui::panel_container(main_tab, movement_cfg.bunny_hop_type);
				{

					new ui::panel(bunnyhop_container);
					new ui::panel(bunnyhop_container);
					auto legit_bunnyhop = new ui::panel(bunnyhop_container);
					new ui::slider(GET_STR("Jump Hitchance"), legit_bunnyhop,movement_cfg.bunny_hop_hitchance, 0.f, 100.f, ui::slider_type::floates);
				}
				new ui::checkbox(GET_STR("Edge Jump"), main_tab, movement_cfg.edge_jump, &movement_cfg.edge_jump_keybind);

				new ui::checkbox(GET_STR("Duck Jump"), main_tab, movement_cfg.duck_jump, &movement_cfg.duck_jump_keybind);
				
				new ui::checkbox(GET_STR("Jump Bug"), main_tab, movement_cfg.jump_bug, &movement_cfg.jump_bug_keybind);

			}

			auto third_tab = new ui::child_window(GET_STR("Misc"), 45.f, 50.f, ImColor(23, 24, 27, 255), visuals_window, float_side::none, child_rounding, 15.f);

			{

				
				new ui::checkbox(GET_STR("Quickstop"), third_tab, movement_cfg.edge_jump, &movement_cfg.edge_jump_keybind);
				new ui::checkbox(GET_STR("Infinite duck"), third_tab, movement_cfg.edge_jump, &movement_cfg.edge_jump_keybind);
				new ui::checkbox(GET_STR("Slide walk"), third_tab, movement_cfg.edge_jump, &movement_cfg.edge_jump_keybind);
			}

			auto second_tab = new ui::child_window(GET_STR("Strafe assist"), 45.f, 50.f, ImColor(23, 24, 27, 255), visuals_window, float_side::none, child_rounding, 15.f);

			{

				new ui::combobox(GET_STR("Strafe"), second_tab, std::vector<std::string>{"none", "perfect", "legit"}, & movement_cfg.strafe_mode_type);
				auto strafe_container = new ui::panel_container(second_tab, movement_cfg.strafe_mode_type);
				{

					new ui::panel(strafe_container);
					auto perfect = new ui::panel(strafe_container);
					{
						new ui::combobox(GET_STR("Strafe movement"), perfect, std::vector<std::string>{"View", "Keys"},  movement_cfg.strafe_movement_type, movement_cfg.strafe_movement_type_keybind);
						new ui::slider(GET_STR("Change speed"), perfect, movement_cfg.strafe_yaw_speed, 0.f, 100.f, ui::slider_type::floates, &movement_cfg.strafe_yaw_speed_keybind);
					}
					auto legit_strafe = new ui::panel(strafe_container);
					new ui::slider(GET_STR("Strafe Hitchance"), legit_strafe, movement_cfg.strafe_hitchance, 0.f, 100.f, ui::slider_type::floates);
				}
				new ui::checkbox(GET_STR("Circle Strafe"), second_tab, movement_cfg.circle_strafe, &movement_cfg.circle_strafe_keybind);
				new ui::checkbox(GET_STR("Z Strafe"), second_tab, movement_cfg.z_strafe, &movement_cfg.z_strafe_keybind);

			}

		}
	}
}