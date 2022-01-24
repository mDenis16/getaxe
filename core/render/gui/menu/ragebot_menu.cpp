
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
		namespace ragebot {
			child_window* main_tab = nullptr;
			child_window* accuracy_tab = nullptr;
			child_window* misc_tab = nullptr;
			child_window* weapons_tab = nullptr;
			child_window* hit_tab = nullptr;
			child_window* exploits_tab = nullptr;

			void init_values(rage_weapon_settings* cfg) {
				if (!main_tab)return;
				//general tab
				main_tab->empty_children();
				{
					new ui::checkbox(GET_STR("Enable"), main_tab, cfg->enabled, &cfg->enabled_keybind);

					new ui::checkbox(GET_STR("Silent"), main_tab, cfg->silent, &cfg->silent_keybind);

					new ui::checkbox(GET_STR("Automatic fire"), main_tab, cfg->automatic_fire, &cfg->automatic_fire_keybind);

					new ui::checkbox(GET_STR("Automatic scope"), main_tab, cfg->automatic_scope, &cfg->automatic_scope_keybind);

		
				}

				accuracy_tab->empty_children();
				//filters tab
				{

					new ui::slider(GET_STR("Hitchance"), accuracy_tab, cfg->hitchance, 0.f, 100.f, ui::slider_type::floates, &cfg->hitchance_keybind);

					new ui::slider(GET_STR("Visible min dmg"), accuracy_tab, cfg->hitchance, 0.f, 100.f, ui::slider_type::floates, &cfg->hitchance_keybind);

					new ui::slider(GET_STR("Autowall min dmg"), accuracy_tab, cfg->hitchance, 0.f, 100.f, ui::slider_type::floates, &cfg->hitchance_keybind);

					new ui::checkbox(GET_STR("Safe point"), accuracy_tab, cfg->safepoint_enable, &cfg->safepoint_enable_keybind);

					new ui::slider(GET_STR("Safe point limit"), accuracy_tab, cfg->safepoint_val, 0.f, 100.f, ui::slider_type::floates, &cfg->safepoint_val_keybind);


					new ui::checkbox(GET_STR("Lag compensation"), accuracy_tab, cfg->lag_compensation, &cfg->lag_compensation_keybind);

					new ui::checkbox(GET_STR("Use shared resolver"), accuracy_tab, cfg->shared_resolver, &cfg->shared_resolver_keybind);
					
				
				}
				//misc tab
				misc_tab->empty_children();
				{
					new ui::checkbox(GET_STR("Automatic stop"), misc_tab, cfg->automatic_scope, &cfg->automatic_scope_keybind);
					new ui::combobox(GET_STR("Stop Trigger"), misc_tab, std::vector<std::string>{"Head", "Body", "Pelvis"}, cfg->prority_hitbox, cfg->prority_hitbox_keybind);
				}



				hit_tab->empty_children();
				//hit_tab 

				{
					//new ui::combobox(GET_STR("Priority hitbox"), hit_tab, config.hitbox, cfg->prority_hitbox, cfg->prority_hitbox_keybind);
				
					
					
					new ui::combobox(GET_STR("Hitbox priority"), hit_tab, std::vector<std::string>{"Head", "Body", "Pelvis"}, cfg->prority_hitbox, cfg->prority_hitbox_keybind);
					new ui::multibox("Hitscan list", hit_tab, config.hitscan_list, cfg->hitscan, cfg->hitscan_keybind);

					new ui::slider(GET_STR("Head scale"), hit_tab, cfg->safepoint_val, 0.f, 100.f, ui::slider_type::floates, &cfg->safepoint_val_keybind);
					new ui::slider(GET_STR("Point scale"), hit_tab, cfg->safepoint_val, 0.f, 100.f, ui::slider_type::floates, &cfg->safepoint_val_keybind);
					new ui::checkbox(GET_STR("Dynamic scale"), hit_tab, cfg->ignore_limbs_on_moving, &cfg->ignore_limbs_on_moving_keybind);
					new ui::checkbox(GET_STR("Ignore limbs on moving"), hit_tab, cfg->ignore_limbs_on_moving, &cfg->ignore_limbs_on_moving_keybind);

				}


				exploits_tab->empty_children();
				//exploits_tab
				{
					new ui::checkbox(GET_STR("Double Tap"), exploits_tab, cfg->ignore_limbs_on_moving, &cfg->ignore_limbs_on_moving_keybind);
					new ui::combobox(GET_STR("Mode"), exploits_tab, std::vector<std::string>{"Head", "Body", "Pelvis"}, cfg->prority_hitbox, cfg->prority_hitbox_keybind);

					new ui::checkbox(GET_STR("Hide shots"), exploits_tab, cfg->ignore_limbs_on_moving, &cfg->ignore_limbs_on_moving_keybind);
					new ui::checkbox(GET_STR("Precharge peek"), exploits_tab, cfg->ignore_limbs_on_moving, &cfg->ignore_limbs_on_moving_keybind);
				}
			}
			void on_weapon_change(int weapon, int category) {
				init_values(config.weapon_mode > 0 ? &config.ragebot.rage_weapon_type[config.active_weapon] : &config.ragebot.rage_weapon_groups[config.active_category]);
			}
			void init(object* main_window, sub_tab* sub_tab, std::vector<std::string> weapons) {
				float child_rounding = 15.f;

				auto legitbot_window = new ui::child_window(GET_STR(""), 100.f, 80.f, ImColor(255, 255, 255, 0), main_window, child_type::main_container, flags::no_background); legitbot_window->flex = flex_direction::automatic; legitbot_window->padding = 15;

				legitbot_window->tab_assign = sub_tab;
				sub_tab->child_assign = legitbot_window;

				main_tab = new ui::child_window(GET_STR("General"), 45.f, 35.f, ImColor(23, 24, 27, 255), legitbot_window, float_side::none, child_rounding, 15.f);


				accuracy_tab = new ui::child_window(GET_STR("Accuracy"), 45.f, 50.f, ImColor(23, 24, 27, 255), legitbot_window, float_side::none, child_rounding, 15.f);

				misc_tab = new ui::child_window(GET_STR("Misc"), 45.f, 25.f, ImColor(23, 24, 27, 255), legitbot_window, float_side::none, child_rounding, 15.f);

				weapons_tab = new ui::child_window(GET_STR("Weapon"), 45.f, 25.f, ImColor(23, 24, 27, 255), legitbot_window, float_side::none, child_rounding, 15.f);
				{
					new ui::combobox(GET_STR("Config mode"), weapons_tab, std::vector<std::string>{"Category", "Weapon"}, & config.weapon_mode);
					;
					auto panel_container = new ui::panel_container(weapons_tab, config.weapon_mode);
					{
						auto category_panel = new ui::panel(panel_container);
						{
							new ui::combobox(GET_STR("Category"), category_panel, config.weapon_categories, &config.active_category);
							;
						}
						auto weapons_panel = new ui::panel(panel_container);
						{
							new ui::combobox(GET_STR("Weapon"), weapons_panel, weapons, &config.active_weapon);
							;
						}
					}

				}


				hit_tab = new ui::child_window(GET_STR("Hitscan"), 45.f, 50.f, ImColor(23, 24, 27, 255), legitbot_window, float_side::none, child_rounding, 15.f);

				exploits_tab = new ui::child_window(GET_STR("Exploits"), 45.f, 35.f, ImColor(23, 24, 27, 255), legitbot_window, float_side::none, child_rounding, 15.f);


				init_values(config.weapon_mode > 0 ? &config.ragebot.rage_weapon_type[config.active_weapon] : &config.ragebot.rage_weapon_groups[config.active_category]);


			}

		}

	}
}