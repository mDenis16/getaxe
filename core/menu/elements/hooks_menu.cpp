#include "../menu.hpp"
#include "../config.hpp"
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_internal.h"

namespace menu {
	void hooks ( ) {
		option ( "Manage hooks", "Allows you to modify hooks", [ = ] ( ) {
			option_single ( "should_skip_animation_frame_target", "Disable hook related to animation frame", config.ragebot_auto_fire );
			option_single ( "is_hltv", "Enable hook related to animation frame", config.is_hltv_target );
			option_single ( "setup_bones", "Enable hook related to stopping game setupbones", config.setup_bones );
			option_single ( "do_procedural_foot_plant_target", "Enable hook feet fix", config.do_procedural_foot_plant_target );
			option_single ( "standard_blending_rules_target", "Enable hook related to anim interpolation", config.standard_blending_rules_target );
			option_single ( "update_client_side_animation", "Enable hook related to stopping animations", config.update_client_side_animation );

		}, ImGui::GetWindowHeight ( ), config.antiaim_at_targets );
	

	}
}