
#ifdef PREVIEW_D3D
#include "D:\repos\mDenis16\getaxe-menu-framework\ImGui Application\includes.h"
#else
#include "../includes.h"
#endif 

#include <tchar.h>
#include <queue>



namespace ui {
	namespace menu {

		static int select_model[3];
		void visuals_player_menu(object* main_window, sub_tab* sub_tab, int player) {
			auto visuals_window = new ui::child_window(GET_STR(""), 100.f, 80.f, ImColor(255, 255, 255, 0), main_window, child_type::main_container, flags::no_background); visuals_window->flex = flex_direction::automatic; visuals_window->padding = 15;

			visuals_window->tab_assign = sub_tab;
			sub_tab->child_assign = visuals_window;

			float child_rounding = 15.f;

			player_visual_base* cfg;
			if (player > 1)
				cfg = &config.local_visual;
			else
				cfg = &config.player_visual[player];

			std::string  s = "";



			auto main_tab = new ui::child_window(GET_STR("Assist"), 45.f, 55.f, ImColor(23, 24, 27, 255), visuals_window, float_side::none, child_rounding, 15.f);

			{


				auto name = new ui::checkbox(GET_STR("Name"), main_tab, cfg->name, cfg->name_color);
;
				auto name_box_extender = new ui::element_extender(name);
				{
					new ui::combobox(GET_STR("Type"), name_box_extender, std::vector<std::string>{"rasterize", "native"}, & cfg->name_render_type, 80.f);
;

					auto panel_container = new ui::panel_container(name_box_extender, cfg->name_render_type);
					{
						auto panel_rasterize = new ui::panel(panel_container);
						{
							new ui::slider(GET_STR("Size"), panel_rasterize, cfg->name_size, 1.f, 25.f, ui::slider_type::floates);

							new ui::checkbox(GET_STR("Dynamic size"), panel_rasterize, cfg->dynamic_name_size);

						}
						auto panel_native = new ui::panel(panel_container);
						{

						}

					}
					new ui::checkbox(GET_STR("Shadow"), name_box_extender, cfg->shadow_name);
;

				}

				auto bounding_box = new ui::checkbox(GET_STR("Bounding Box"), main_tab, cfg->bounding_box, cfg->bound_box_color, cfg->bound_box_keybind);
;
				auto bounding_box_extender = new ui::element_extender(bounding_box);
				{
					new ui::combobox(GET_STR("Type"), bounding_box_extender, std::vector<std::string>{"normal", "corner", "3d"}, & cfg->box_type, 60.f);
;
					new ui::checkbox(GET_STR("Dynamic"), bounding_box_extender, cfg->dynamic_box);
;
					new ui::slider(GET_STR("Rounding"), bounding_box_extender, cfg->bound_box_border_rounding, 0.f, 5.f, ui::slider_type::floates);
;
					new ui::slider(GET_STR("Border"), bounding_box_extender, cfg->bound_box_border_thickness, 0.f, 5.f, ui::slider_type::floates);
;
					new ui::slider(GET_STR("Box"), bounding_box_extender, cfg->bound_box_thickness, 0.f, 5.f, ui::slider_type::floates);
;

				}


				new ui::checkbox(GET_STR("Health"), main_tab, cfg->health, cfg->health_color);
;
				new ui::checkbox(GET_STR("Weapon"), main_tab, cfg->weapon, cfg->weapon_color);
;
				new ui::checkbox(GET_STR("Ammo"), main_tab, cfg->ammo, cfg->ammo_color);
;
				new ui::checkbox(GET_STR("Skeleton"), main_tab, cfg->skeleton, cfg->skeleton_color);
;
				new ui::checkbox(GET_STR("Flags"), main_tab, cfg->flags, cfg->flags_color);
;
				new ui::multibox("Flags input", main_tab, config.flags_list, cfg->flags_input);
			}


			auto second_tab = new ui::child_window(GET_STR("Modulation"), 45.f, 45.0, ImColor(23, 24, 27, 255), visuals_window, float_side::none, child_rounding, 15.f);

			{


				auto glow = new ui::checkbox(GET_STR("Glow"), second_tab, cfg->glow.enable, cfg->glow.color, cfg->glow.keybind);
;


				// here we need to split featurs based on player
				if (player < 2) {
					auto model_selector = new ui::combobox(GET_STR("Model selector"), second_tab, std::vector<std::string>{"visible", "hidden", "history", "hit", "attachments"}, & select_model[player]);
;
					auto& s_cfg = config.player_visual[player];

					auto panel_container = new ui::panel_container(second_tab, select_model[player]);
					{

						for (size_t i = 0; i < CHAMS_MODEL_MAX; i++) {

							auto chams_panel = new ui::panel(panel_container);
							{

								new ui::checkbox(GET_STR("Enable model"), chams_panel, s_cfg.chams[i].enable, s_cfg.chams[i].color);
;
								new ui::combobox(GET_STR("Material"), chams_panel, std::vector<std::string>{"solid", "flat", "reflective", "glow"}, & s_cfg.chams[i].material);
;
								if (i == chams_model::ATTACHMENTS)
									new ui::checkbox(GET_STR("Override Z"), chams_panel, s_cfg.chams[i].force_z);
;
							}
						}

					}
					new ui::checkbox(GET_STR("Ragdoll filter"), second_tab, s_cfg.ragdoll_chams);
;
				}
				else { /*local player chams*/

					auto model_selector = new ui::combobox(GET_STR("Model selector"), second_tab, std::vector<std::string>{"local", "desync", "fakelag", "arms", "weapon"}, & select_model[player]);
;
					auto& l_cfg = config.local_visual;

					auto panel_container = new ui::panel_container(second_tab, select_model[player]);
					{

						for (size_t i = 0; i < local_chams_model::CHAMS_LOCAL_MAX; i++) {

							auto chams_panel = new ui::panel(panel_container);
							{

								new ui::checkbox(GET_STR("Enable model"), chams_panel, l_cfg.chams[i].enable, l_cfg.chams[i].color);
;
								new ui::combobox(GET_STR("Material"), chams_panel, std::vector<std::string>{"solid", "flat", "reflective", "glow"}, & l_cfg.chams[i].material);
;

							}
						}

					}
				}

			}

			auto third_tab = new ui::child_window(GET_STR("Misc"), 45.f, player < 2 ? 70.f : 50.0, ImColor(23, 24, 27, 255), visuals_window, float_side::none, child_rounding, 15.f);

			{
				if (player < 2) {
					auto& s_cfg = config.player_visual[player];
					auto out_of_fov = new ui::checkbox(GET_STR("Out of fov indicator"), third_tab, s_cfg.out_of_pov, s_cfg.out_of_pov_color);
;
					auto out_of_fov_extender = new ui::element_extender(out_of_fov);
					{
						new ui::slider(GET_STR("Distance"), out_of_fov_extender, s_cfg.out_of_pov_radius, 10.f, 140.f, slider_type::floates);
;
						new ui::slider(GET_STR("Size"), out_of_fov_extender, s_cfg.out_of_pov_base_size, 5.f, 35.f, slider_type::floates);
;
					}

					new ui::checkbox(GET_STR("Reveal on radar"), third_tab, s_cfg.force_radar_reveal);
;
					new ui::checkbox(GET_STR("Footsteps"), third_tab, s_cfg.foot_steps, s_cfg.foot_steps_color);
;

					new ui::checkbox(GET_STR("View barrel"), third_tab, s_cfg.view_barrel, s_cfg.view_barrel_color);
;

					new ui::checkbox(GET_STR("Bullet impact"), third_tab, s_cfg.bullet_impacts, s_cfg.bullet_impacts_color, s_cfg.bullet_impacts_keybind);
;
					new ui::checkbox(GET_STR("Bullet tracer"), third_tab, s_cfg.bullet_tracers, s_cfg.bullet_tracers_color, s_cfg.bullet_tracers_keybind);
;
					new ui::checkbox(GET_STR("Knife range"), third_tab, s_cfg.knife_range, s_cfg.knife_range_color, s_cfg.knife_range_keybind);
;
					new ui::checkbox(GET_STR("Taser range"), third_tab, s_cfg.taser_range, s_cfg.taser_range_color, s_cfg.taser_range_keybind);
;
				}
				else {
					auto& l_cfg = config.local_visual;

					new ui::checkbox(GET_STR("Bullet impact"), third_tab, l_cfg.bullet_impacts, l_cfg.bullet_impacts_color, l_cfg.bullet_impacts_keybind);
;
					new ui::checkbox(GET_STR("Bullet tracer"), third_tab, l_cfg.bullet_tracers, l_cfg.bullet_tracers_color, l_cfg.bullet_tracers_keybind);
;
					new ui::checkbox(GET_STR("Knife range"), third_tab, l_cfg.knife_range, l_cfg.knife_range_color, l_cfg.knife_range_keybind);
;
					new ui::checkbox(GET_STR("Taser range"), third_tab, l_cfg.taser_range, l_cfg.taser_range_color, l_cfg.taser_range_keybind);
;
					new ui::checkbox(GET_STR("Weapon spread"), third_tab, l_cfg.weapon_spread);
;
					new ui::checkbox(GET_STR("Weapon recoil"), third_tab, l_cfg.weapon_recoil);
;
					new ui::checkbox(GET_STR("Force crosshair"), third_tab, l_cfg.weapon_force_corsshair);
;
					new ui::checkbox(GET_STR("Vulnerable warning"), third_tab, l_cfg.vulnerable_warning, l_cfg.vulnerable_warning_color, l_cfg.vulnerable_warning_keybind);
;
				}

			}
			if (player >= 2) {
				auto& l_cfg = config.local_visual;
				auto fourth_tab = new ui::child_window(GET_STR("View"), 45.f, 45.0, ImColor(23, 24, 27, 255), visuals_window, float_side::none, child_rounding, 15.f);

				{
					auto thirdperson_checkbox = new ui::checkbox(GET_STR("Third person"), fourth_tab, l_cfg.thirdperson);
;
					{
						auto thirdperson_extender = new ui::element_extender(thirdperson_checkbox);
						{
							new ui::checkbox(GET_STR("Interpolate angles"), thirdperson_extender, l_cfg.interpolate_angles);
;
							new ui::slider(GET_STR("Amount"), thirdperson_extender, l_cfg.interpolation_amount, 1.f, 20.f, ui::slider_type::floates);
;
						}
					}
					new ui::slider(GET_STR("Distance"), fourth_tab, l_cfg.thirdperson_distance, 50.f, 200.f, ui::slider_type::floates);
;
					auto view_model = new ui::slider(GET_STR("Viewmodel"), fourth_tab, l_cfg.view_model_distance, -50.f, 50.f, ui::slider_type::floates);
;
					auto view_model_extender = new ui::element_extender(view_model);
					{

						new ui::slider(GET_STR("X offset"), view_model_extender, l_cfg.view_model_x_offset, -20.f, 20.f, ui::slider_type::floates);
;
						new ui::slider(GET_STR("Y offset"), view_model_extender, l_cfg.view_model_y_offset, -20.f, 20.f, ui::slider_type::floates);
;
						new ui::slider(GET_STR("Z offset"), view_model_extender, l_cfg.view_model_z_offset, -20.f, 20.f, ui::slider_type::floates);
;

					}
					new ui::slider(GET_STR("Aspect ratio"), fourth_tab, l_cfg.aspect_ratio, 0.f, 2.f, ui::slider_type::floates);
;
					new ui::multibox("Removals input", fourth_tab, config.removals_list, l_cfg.removals_input);
				}
			}
		}
	}
}