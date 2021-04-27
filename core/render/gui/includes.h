#include <string>
#include <vector>
#include <memory>

#include "../../../dependencies/imgui/imgui.h"
#include "../../../dependencies/imgui/imgui_internal.h"

#include "gui.h"

#include "framework/element.h"
#include "framework/window.h"
#include "framework/tab.h"
#include "framework/sub_tab.h"
#include "framework/child_window.h"
#include "framework/checkbox.h"
#include "framework/keybind.h"
#include "framework/key_selector.h"
#include "framework/colorpicker.h"
#include "framework/text_input.h"
#include "framework/small_text_input.h"
#include "framework/combobox.h"
#include "framework/combo_item.h"
#include "framework/multibox.h"
#include "framework/slider.h"
#include "framework/config_item.h"
#include "framework/config_box.h"
#include "framework/button.h"
#include "framework/panel.h"
#include "framework/panel_container.h"

#include "config.h"



namespace ui {
	namespace menu {
		void ragebot_menu ( object* main_window, object * sub_tab );
		void settings_menu ( object * main_window, object * sub_tab );
		void visuals_player_menu ( object * main_window, sub_tab * sub_tab, int player );
		void visuals_local_menu ( object * main_window, sub_tab * sub_tab );

	}
}