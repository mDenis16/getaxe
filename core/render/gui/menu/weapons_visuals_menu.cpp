
#ifdef PREVIEW_D3D
#include "D:\repos\mDenis16\getaxe-menu-framework\ImGui Application\includes.h"
#else
#include "../includes.h"
#endif 
#include <tchar.h>
#include <queue>



namespace ui {
	namespace menu {


		void visuals_weapons_menu(object* main_window, sub_tab* sub_tab) {
			auto visuals_window = new ui::child_window(GET_STR(""), 100.f, 80.f, ImColor(255, 255, 255, 0), main_window, child_type::main_container, flags::no_background); visuals_window->flex = flex_direction::automatic; visuals_window->padding = 15;

			visuals_window->tab_assign = sub_tab;
			sub_tab->child_assign = visuals_window;

			float child_rounding = 15.f;

			auto& cfg = config.weapons_visual;

			auto main_tab = new ui::child_window(GET_STR("Weapons Assist"), 45.f, 50.f, ImColor(23, 24, 27, 255), visuals_window, float_side::none, child_rounding, 15.f);

			{




				auto name = new ui::checkbox(GET_STR("Name"), main_tab, cfg.name, cfg.name_color);
;
				auto name_box_extender = new ui::element_extender(name);
				{


				}

				auto bounding_box = new ui::checkbox(GET_STR("Bounding Box"), main_tab, cfg.bounding_box, cfg.bound_box_color, cfg.bound_box_keybind);
;
				auto bounding_box_extender = new ui::element_extender(bounding_box);
				{
					new ui::combobox(GET_STR("Type"), bounding_box_extender, std::vector<std::string>{"normal", "corner", "3d"}, & cfg.box_type, 60.f);
;
					new ui::checkbox(GET_STR("Dynamic"), bounding_box_extender, cfg.dynamic_box);
;
					new ui::slider(GET_STR("Rounding"), bounding_box_extender, cfg.bound_box_border_rounding, 0.f, 5.f, ui::slider_type::floates);
;
					new ui::slider(GET_STR("Border"), bounding_box_extender, cfg.bound_box_border_thickness, 0.f, 5.f, ui::slider_type::floates);
;
					new ui::slider(GET_STR("Box"), bounding_box_extender, cfg.bound_box_thickness, 0.f, 5.f, ui::slider_type::floates);
;

				}

				//new ui::checkbox ( "Ammo", main_tab, cfg.ammo, cfg.ammo_color );

			}


			auto second_tab = new ui::child_window(GET_STR("Modulation"), 45.f, 45.0, ImColor(23, 24, 27, 255), visuals_window, float_side::none, child_rounding, 15.f);

			{
				new ui::checkbox(GET_STR("Glow"), second_tab, cfg.glow.enable, cfg.glow.color);

				static int model_select = 0;
				auto model_selector = new ui::combobox(GET_STR("Model selector"), second_tab, std::vector<std::string>{"visible", "hidden", "history", "hit", "attachments"}, & model_select);


				auto panel_container = new ui::panel_container(second_tab, model_select);
				{

					/*for ( size_t i = 0; i < CHAMS_MODEL_MAX; i++ ) {

						auto chams_panel = new ui::panel ( panel_container );
						{

							new ui::checkbox ( "Enable model", chams_panel, cfg.chams [ i ].enable, cfg.chams [ i ].color );
							new ui::combobox ( "Material", chams_panel, std::vector<std::string>{"solid", "flat", "reflective", "glow"}, & cfg.chams [ i ].material );
							if ( i == chams_model::ATTACHMENTS )
								new ui::checkbox ( "Override Z", chams_panel, cfg.chams [ i ].force_z );
						}
					}*/
					/*
					VISIBLE,
	HIDDEN,
	BACKTRACK,
	HIT,
	ATTACHMENTS,
	CHAMS_MODEL_MAX
					*/



				}
				//new ui::checkbox ( "Ragdoll filter", second_tab, cfg.ragdoll_chams );
			}

			auto third_tab = new ui::child_window(GET_STR("Misc"), 45.f, 60.0, ImColor(23, 24, 27, 255), visuals_window, float_side::none, child_rounding, 15.f);

			{

				auto out_of_fov = new ui::checkbox(GET_STR("Vulnerable warning"), third_tab, cfg.out_of_pov, cfg.out_of_pov_color);
;


				/*

								new ui::checkbox ( "Visualize safepoints", third_tab, cfg.foot_steps, cfg.foot_steps_color );
				new ui::checkbox ( "Visualize aimpoints ", third_tab, cfg.foot_steps, cfg.foot_steps_color );
				*/
			}


		}

	}
}