
#ifdef PREVIEW_D3D
#include "E:\repos\mDenis16\getaxe-menu-framework\ImGui Application\includes.h"
#else
#include "../includes.h"
#endif 

#include <tchar.h>
#include <queue>



namespace ui {
	namespace menu {


		void visuals_player_menu ( object * main_window, sub_tab * sub_tab, int player ) {
			auto visuals_window = new ui::child_window ( "", 100.f, 80.f, ImColor ( 255, 255, 255, 0 ), main_window, child_type::main_container, flags::no_background ); visuals_window->flex = flex_direction::automatic; visuals_window->padding = 15;
			visuals_window->tab_assign = sub_tab;
			sub_tab->child_assign = visuals_window;

			float child_rounding = 15.f;

			auto & cfg = c_config::get ( ).player_visual [ player ];

			auto main_tab = new ui::child_window ( player > 0 ? "Team Assist" : "Enemy Assist", 45.f, 50.f, ImColor ( 23, 24, 27, 255 ), visuals_window, float_side::none, child_rounding, 15.f );
			{
				
				
				

				auto name = new ui::checkbox ( "Name", main_tab, cfg.name, cfg.name_color );
				auto name_box_extender = new ui::element_extender ( name );
				{
					new ui::combobox ( "Type", name_box_extender, std::vector<std::string>{"rasterize", "native"}, & cfg.name_render_type, 80.f );

					auto panel_container = new ui::panel_container ( name_box_extender, cfg.name_render_type );
					{
						auto panel_rasterize = new ui::panel ( panel_container );
						{
							new ui::slider ( "Size", panel_rasterize, cfg.name_size, 1.f, 25.f, ui::slider_type::floates );
							new ui::checkbox ( "Dynamic size", panel_rasterize, cfg.dynamic_name_size );
						}
						auto panel_native = new ui::panel ( panel_container );
						{
					
						}

					}
					new ui::checkbox ( "Shadow", name_box_extender, cfg.shadow_name );

				}

				auto bounding_box = new ui::checkbox ( "Bounding Box", main_tab, cfg.bounding_box, cfg.bound_box_color, cfg.bound_box_keybind );
				auto bounding_box_extender = new ui::element_extender ( bounding_box );
				{
					new ui::combobox ( "Type", bounding_box_extender, std::vector<std::string>{"normal", "corner", "3d"}, & cfg.box_type, 60.f );
					new ui::checkbox ( "Dynamic", bounding_box_extender, cfg.dynamic_box );
					new ui::slider ( "Rounding", bounding_box_extender, cfg.bound_box_border_rounding, 0.f, 5.f, ui::slider_type::floates );
					new ui::slider ( "Border", bounding_box_extender, cfg.bound_box_border_thickness, 0.f, 5.f, ui::slider_type::floates );
					new ui::slider ( "Box", bounding_box_extender, cfg.bound_box_thickness, 0.f, 5.f, ui::slider_type::floates );

				}
				

				new ui::checkbox ( "Health", main_tab, cfg.health, cfg.health_color );
				new ui::checkbox ( "Weapon", main_tab, cfg.weapon, cfg.weapon_color );
				new ui::checkbox ( "Ammo", main_tab, cfg.ammo, cfg.ammo_color );
				new ui::checkbox ( "Skeleton", main_tab, cfg.skeleton, cfg.skeleton_color );
				new ui::checkbox ( "Flags", main_tab, cfg.flags, cfg.flags_color );
				new ui::multibox ( "Flags input", main_tab, config.flags_list, cfg.flags_input );
			}


			auto second_tab = new ui::child_window ( "Modulation", 45.f, 45.0, ImColor ( 23, 24, 27, 255 ), visuals_window, float_side::none, child_rounding, 15.f );
			{
				new ui::checkbox ( "Glow", second_tab, cfg.glow, cfg.glow_color );
				static int model_select = 0;
				auto model_selector = new ui::combobox ( "Model selector", second_tab, std::vector<std::string>{"visible", "hidden", "history", "hit"}, &model_select);
				
				auto panel_container = new ui::panel_container ( second_tab, model_select );
				{

					for ( size_t i = 0; i < CHAMS_MODEL_MAX; i++ ) {
						auto chams_panel = new ui::panel ( panel_container );
						{
							new ui::checkbox ( "Enable model", chams_panel, cfg.chams[i].enable, cfg.health_color );
							new ui::combobox ( "Material", chams_panel, std::vector<std::string>{"solid", "flat", "reflective", "glow"}, &cfg.chams[i].material );
						}
					}
				     
					
				
				}
				
			}

			auto third_tab = new ui::child_window ( "Misc", 45.f, 60.0, ImColor ( 23, 24, 27, 255 ), visuals_window, float_side::none, child_rounding, 15.f );
			{
			
				auto out_of_fov = new ui::checkbox ( "Out of fov indicator", third_tab, cfg.out_of_pov, cfg.out_of_pov_color ); 
				auto out_of_fov_extender = new ui::element_extender ( out_of_fov );
				{
					new ui::slider ( "Distance", out_of_fov_extender, cfg.out_of_pov_radius, 10.f, 140.f, slider_type::floates );
					new ui::slider ( "Size", out_of_fov_extender, cfg.out_of_pov_base_size, 5.f, 35.f, slider_type::floates );
				}

				new ui::checkbox ( "Reveal on radar", third_tab, cfg.force_radar_reveal );
				new ui::checkbox ( "Footsteps", third_tab, cfg.foot_steps, cfg.foot_steps_color );
				new ui::checkbox ( "View barrel", third_tab, cfg.foot_steps, cfg.foot_steps_color );

				/*
				
								new ui::checkbox ( "Visualize safepoints", third_tab, cfg.foot_steps, cfg.foot_steps_color );
				new ui::checkbox ( "Visualize aimpoints ", third_tab, cfg.foot_steps, cfg.foot_steps_color );
				*/
			}

		}
	}
}