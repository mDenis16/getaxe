#pragma once
#include <string>
#include <vector>
#include <memory>

#include <renderlib/imgui/imgui.h>
#include <renderlib/imgui/imgui_internal.h>

#include "../../../dependencies/utilities/vpk_parser.h"
#include "../../../dependencies/utilities/stack_strings.hpp"


#include "config.h"

#include "gui.h"

#include <UIFramework/framework_public.h>

//#include "framework/element.h"
//#include "framework/window.h"
//#include "framework/tab.h"
//#include "framework/sub_tab.h"
//#include "framework/child_window.h"
//#include "framework/checkbox.h"
//#include "framework/keybind.h"
//#include "framework/key_selector.h"
//#include "framework/colorpicker.h"
//#include "framework/text_input.h"
//#include "framework/small_text_input.h"
//#include "framework/combobox.h"
//#include "framework/combo_item.h"
//#include "framework/multibox.h"
//#include "framework/slider.h"
//#include "framework/config_item.h"
//#include "framework/config_box.h"
//#include "framework/button.h"
//#include "framework/panel.h"
//#include "framework/panel_container.h"
//#include "framework/element_extender.h"
//#include "framework/bezier_editor.h"
//#include "framework/inventory_changer.h"
//#include "framework/inventory_item.h"





namespace ui {
	namespace menu {
		void ragebot_menu ( object* main_window, object * sub_tab );
		void settings_menu ( object * main_window, object * sub_tab );
		void visuals_player_menu ( object * main_window, sub_tab * sub_tab, int player );
		void visuals_weapons_menu ( object * main_window, sub_tab * sub_tab );
		void projectiles_weapons_menu ( object * main_window, sub_tab * sub_tab );
		void visuals_world_menu ( object * main_window, sub_tab * sub_tab );
		void movement_menu ( object * main_window, sub_tab * sub_tab );

		namespace legitbot {
			extern 	child_window * main_tab;
			void on_weapon_change ( int weapon, int category );
			void init ( object * main_window, sub_tab * sub_tab, std::vector<std::string> weapons );
			void init_values ( weapon_settings * cfg );
		}
		namespace ragebot {
			extern 	child_window* main_tab;
			void on_weapon_change(int weapon, int category);
			void init(object* main_window, sub_tab* sub_tab, std::vector<std::string> weapons);
			void init_values(rage_weapon_settings* cfg);
		}
		namespace antiaim {
			extern 	child_window* main_tab;
			
			void init(object* main_window, sub_tab* sub_tab );

		}
		void inventory_changer_menu ( object * main_window, sub_tab * sub_tab );
	}
}