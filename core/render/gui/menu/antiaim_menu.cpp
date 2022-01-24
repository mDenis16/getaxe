
#ifdef PREVIEW_D3D
#include "D:\repos\mDenis16\getaxe-menu-framework\ImGui Application\includes.h"
#else
#include "../includes.h"
#endif 

#include <tchar.h>
#include <queue>



namespace ui {
	namespace menu {

		//
		namespace antiaim {
			child_window* main_tab = nullptr;
			child_window* accuracy_tab = nullptr;
			child_window* misc_tab = nullptr;
			child_window* weapons_tab = nullptr;
			child_window* hit_tab = nullptr;
			child_window* exploits_tab = nullptr;

			static int fakelag_trigger_select = 0;

			void init(object* main_window, sub_tab* sub_tab) {
				float child_rounding = 15.f;

				auto legitbot_window = new ui::child_window(GET_STR(""), 100.f, 80.f, ImColor(255, 255, 255, 0), main_window, child_type::main_container, flags::no_background); legitbot_window->flex = flex_direction::automatic; legitbot_window->padding = 15;

				legitbot_window->tab_assign = sub_tab;
				sub_tab->child_assign = legitbot_window;

				main_tab = new ui::child_window(GET_STR("General"), 45.f, 35.f, ImColor(23, 24, 27, 255), legitbot_window, float_side::none, child_rounding, 15.f);
				{
					new ui::checkbox(GET_STR("Enable"), main_tab, config.antiaim.enable_fakelag, &config.antiaim.enable_fakelag_keybind);

					auto model_selector = new ui::combobox(GET_STR("Fakelag trigger"), main_tab, std::vector<std::string>{"standing", "moving", "in air", "on peek", "on shoot", "on land"}, & fakelag_trigger_select);

					auto panel_container = new ui::panel_container(main_tab, fakelag_trigger_select);
					{

						for (size_t i = 0; i < FAKELAG_MAXS; i++) {

							auto fakelagtr_panel = new ui::panel(panel_container);
							{

								new ui::slider(GET_STR("Fakelag "), fakelagtr_panel, config.antiaim.fakelag[i].choke_limit, 0.f, 16.f, ui::slider_type::integer, &config.antiaim.fakelag[i].choke_limit_keybind);
//								new ui::checkbox(GET_STR("Enable model"), chams_panel, s_cfg.chams[i].enable, s_cfg.chams[i].color);
								
								/*new ui::combobox(GET_STR("Material"), chams_panel, std::vector<std::string>{"solid", "flat", "reflective", "glow"}, & s_cfg.chams[i].material);
								;
								if (i == chams_model::ATTACHMENTS)
									new ui::checkbox(GET_STR("Override Z"), chams_panel, s_cfg.chams[i].force_z);
								;*/
							}
						}

					}
				}

				auto dummy = new ui::child_window(GET_STR(""), 45.f, 50.f, ImColor(23, 24, 27, 255), legitbot_window, float_side::none, child_rounding, 15.f);

				auto antiaim = new ui::child_window(GET_STR("Anti aim"), 45.f, 50.f, ImColor(23, 24, 27, 255), legitbot_window, float_side::none, child_rounding, 15.f);
				{
				

					new ui::combobox(GET_STR("Base pitch"), antiaim, config.base_pitch_list, config.antiaim.base_pitch, config.antiaim.base_pitch_keybind);
					new ui::combobox(GET_STR("Base yaw"), antiaim, config.base_yaw_list, config.antiaim.base_yaw, config.antiaim.base_yaw_keybind);
					new ui::combobox(GET_STR("Desync mode"), antiaim, config.desync_mode_list, config.antiaim.desync_mode, config.antiaim.desync_mode_keybind);
					new ui::combobox(GET_STR("Desync side"), antiaim, config.desync_side_list, config.antiaim.desync_side, config.antiaim.desync_side_keybind);
		

					auto panel_container = new ui::panel_container(antiaim, config.antiaim.desync_side);
					{

						for (size_t i = 0; i < 3; i++) {

							auto desync_panel = new ui::panel(panel_container);
							{

								new ui::slider(GET_STR("Desync ammount"), desync_panel, config.antiaim.desync_ammount[i], 0.f, 100.f, ui::slider_type::floates, &config.antiaim.desync_ammount_keybind[3]);
							}
						}
					}

					new ui::checkbox(GET_STR("Prevent angle storing"), antiaim, config.antiaim.prevent_angle_storing, &config.antiaim.prevent_angle_storing_keybind);
				}

			}

		}

	}
}