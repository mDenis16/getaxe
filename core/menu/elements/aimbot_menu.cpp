#include "../menu.hpp"
#include "../config.hpp"
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_internal.h"

namespace menu {
	void ragebot ( ) {
		option ( "Aimbot enemy", "Aimbot your enemy", [ = ] ( ) {
			option_single ( "Auto fire", "Makes your aimbot to shot automatically", config.ragebot_auto_fire );
			option_slider ( "Hitchance", "Hitchance of hitting target", 0.f, 100.f, config.ragebot_hitchance );
			option_slider ( "Min damage", "Minimum damage of hitspot", 0.f, 100.f, config.ragebot_min_dmg );
			option_slider ( "Head Scale", "Head Scale of  multipoint", 0.f, 100.f, config.ragebot_head_scale );
			option_slider ( "Body Scale", "Body Scale of  multipoint", 0.f, 100.f, config.ragebot_point_scale );

            option_combobox ( "Safepoint", "Prefer safepoint", std::vector<std::string>{"none", "minimal", "average", "restrict"}, config.ragebot_safe_point );
			option_single ( "Resolver", "Dynamical AI desync correction", config.ragebot_resolver );
			option_combobox ( "Record priority", "Prefered lag compensation", std::vector<std::string>{"last", "safest", "min velocity", "highest damage"}, config.ragebot_record_selection );
			option_combobox ( "Hitbox priority", "Prefered target hitbox", std::vector<std::string>{"head", "body"}, config.ragebot_prioritize_hitbox );
			option_multicombobox ( "Hitscan selection", "Select hitboxes to scan", {{ "head", &config.ragebot_head_scan }, { "body", &config.ragebot_body_scan },{ "feet", &config.ragebot_feet_scan },{ "arms scan", &config.ragebot_arms_scan } } );
			option_multicombobox ( "Auto Stop", "Automatic stops movement", { { "early", &config.ragebot_autostop[0] },  { "minimal", &config.ragebot_autostop [ 1 ] }, { "between shots", &config.ragebot_autostop [ 2 ] } } );
		}, ImGui::GetWindowHeight ( ), config.ragebot_enabled );
		option ( "Anti aim", "Makes getting hit harder", [ = ] ( ) {
		
			option_combobox ( "Pitch", "Where you aimbot shot", std::vector<std::string>{"none", "down", "up"}, config.antiaim_pitch );
			option_combobox ( "Yaw", "Where you aimbot shot", std::vector<std::string>{"none", "freestanding", "backwards"}, config.antiaim_yaw );
			option_single ( "At Targets", "Adjust your yaw against enemy", config.antiaim_at_targets );
			option_single ( "3Way Side", "God bless kibit.", config.antiaim_3way );
			option_single ( "Anti Bruteforce", "Inverts your side when enemy miss.", config.antiaim_antibruteforce );
			option_slider_int ( "Fakelag", "Amount of fakelag", 0, 16, config.antiaim_fakelag );
			option_multicombobox ( "Fakelag Trigger", "When you fakelag", { { "standing", &config.antiaim_fakelag_flags [ 0 ] },  { "moving", &config.antiaim_fakelag_flags [ 1 ] }, { "in air", &config.antiaim_fakelag_flags [ 2 ] },
				{ "on peek", &config.
antiaim_fakelag_flags [ 3 ] }, { "on shot", &config.
antiaim_fakelag_flags [ 4 ] } } );

		}, 600, config.antiaim_enable );
		/*option ( "Exploits", "Gives you a big advatage", [ = ] ( ) {
			option_single ( "Doubletap", "Allows you to shoot twice at the same time", config.ragebot_double_tap );
			option_single ( "Hideshots", "Prevent other players to hit your flip", config.ragebot_hideshots );
			ImGui::Hotkey ( "##dbtap key", &c_config::get ( ).double_tap_key, ImVec2 ( 100, 20 ) );
			ImGui::Hotkey ( "##hideshots key", &c_config::get ( ).hide_shots_key, ImVec2 ( 100, 20 ) );
			

		}, 300, config.antiaim_enable );*/
	
	}
}