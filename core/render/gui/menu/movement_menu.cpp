
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

			auto& cfg = c_config::get().misc.movement;

			auto main_tab = new ui::child_window(GET_STR("Bhop assist"), 45.f, 50.f, ImColor(23, 24, 27, 255), visuals_window, float_side::none, child_rounding, 15.f);

			{




				auto bhop = new ui::checkbox(GET_STR("Auto bhop"), main_tab, cfg.bhop, &cfg.bhop_keybind);

				new ui::checkbox(GET_STR("Duck in air"), main_tab, cfg.air_duck);

				new ui::checkbox(GET_STR("Slow walk"), main_tab, cfg.slow_walk, &cfg.slow_walk_keybind);

				//new ui::checkbox ( "Fakeduck", main_tab, cfg.fake_duck, &cfg.slow_walk_keybind );

				new ui::checkbox(GET_STR("Auto strafe"), main_tab, cfg.auto_strafe, &cfg.auto_strafe_keybind);

				new ui::checkbox(GET_STR("Key strafer"), main_tab, cfg.wasd_strafe, &cfg.wasd_strafe_keybind);

				new ui::checkbox(GET_STR("Circle strafer"), main_tab, cfg.circle_strafe, &cfg.circle_keybind);

				new ui::checkbox(GET_STR("Align strafer"), main_tab, cfg.a_strafe, &cfg.auto_strafe_keybind);

				auto z_strafer = new ui::checkbox(GET_STR("Z strafer"), main_tab, cfg.z_strafe, &cfg.z_keybind);

				auto z_strafer_extender = new ui::element_extender(z_strafer);
				{

					new ui::slider(GET_STR("Frequency"), z_strafer_extender, cfg.z_strafe_frequency, 1.f, 100.f, ui::slider_type::floates);

					new ui::slider(GET_STR("Distance"), z_strafer_extender, cfg.z_strafer_distance, 1.f, 100.f, ui::slider_type::floates);



				}


			}


			/*auto second_tab = new ui::child_window ( "Misc", 45.f, 45.0, ImColor ( 23, 24, 27, 255 ), visuals_window, float_side::none, child_rounding, 15.f );
			{
				new ui::checkbox ( "Glow", second_tab, cfg.glow.enable, cfg.glow.color );
				static int model_select = 0;
				auto model_selector = new ui::combobox ( "Model selector", second_tab, std::vector<std::string>{"visible", "hidden", "history", "hit", "attachments"}, & model_select );

				auto panel_container = new ui::panel_container ( second_tab, model_select );
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




					//	}
						//new ui::checkbox ( "Ragdoll filter", second_tab, cfg.ragdoll_chams );
				//	}

					/*auto third_tab = new ui::child_window ( "Misc", 45.f, 60.0, ImColor ( 23, 24, 27, 255 ), visuals_window, float_side::none, child_rounding, 15.f );
					{

						new ui::checkbox ( "Grenade warning", third_tab, cfg.grenade_warning, cfg.out_of_pov_color );
						new ui::checkbox ( "Grenade path", third_tab, cfg.grenade_path, cfg.out_of_pov_color );


						/*

										new ui::checkbox ( "Visualize safepoints", third_tab, cfg.foot_steps, cfg.foot_steps_color );
						new ui::checkbox ( "Visualize aimpoints ", third_tab, cfg.foot_steps, cfg.foot_steps_color );
						*/
						/*}*/
		}
	}
}