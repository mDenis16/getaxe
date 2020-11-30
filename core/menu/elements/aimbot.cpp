#include "../menu.hpp"
#include "../variables.hpp"
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_internal.h"

namespace menu {
	void ragebot ( ) {
		option ( "Aimbot enemy", "Aimbot your enemy", [ = ] ( ) {
			option_single ( "Auto fire", "Makes your aimbot to shot automatically", variables::ragebot::auto_fire );
			option_single ( "Silent aim", "Makes your aimbot silent local", variables::ragebot::silent_aim );
			option_slider ( "Hitchance", "Hitchance of hitting target", 0.f, 100.f, variables::ragebot::hitchance );
			option_slider ( "Min damage", "Minimum damage of hitspot", 0.f, 100.f, variables::ragebot::min_dmg );
			option_slider ( "Head Scale", "Head Scale of  multipoint", 0.f, 100.f, variables::ragebot::head_scale );
			option_slider ( "Body Scale", "Body Scale of  multipoint", 0.f, 100.f, variables::ragebot::point_scale );
			option_single ( "No Spread", "Assit recoil", variables::ragebot::nospread );
			//option_single ( "Resolver", "Shoot only at safe points", variables::ragebot::resolver );
            option_combobox ( "Safepoint", "Prefer safepoint", std::vector<std::string>{"none", "minimal", "average", "restrict"}, variables::ragebot::safe_point );

			option_combobox ( "Record priority", "Where you backtrack shot", std::vector<std::string>{"last", "safest", "min velocity", "highest damage"}, variables::ragebot::prioritize_record );
		
			option_combobox ( "Hitbox priority", "Where you aimbot shot", std::vector<std::string>{"head", "body"}, variables::ragebot::prioritize_hitbox );
			
			option_multicombobox ( "Hitscan selection", "Where your aimbot scan", {{ "head", &variables::ragebot::head_scan }, { "body", &variables::ragebot::body_scan },{ "feet", &variables::ragebot::feet_scan },{ "arms scan", &variables::ragebot::arms_scan } } );


		}, ImGui::GetWindowHeight ( ), variables::ragebot::enabled );
		option ( "Anti aim", "Makes getting hit harder", [ = ] ( ) {
		
			option_combobox ( "Pitch", "Where you aimbot shot", std::vector<std::string>{"none", "down", "up"}, variables::antiaim::pitch );
			option_combobox ( "Yaw", "Where you aimbot shot", std::vector<std::string>{"none", "freestanding", "backwards"}, variables::antiaim::yaw );
		
			option_slider_int ( "Fakelag", "Amount of fakelag", 0, 16, variables::antiaim::fakelag );
			option_single ( "Fakelag on peek", "Teleports you in front of enemy", variables::antiaim::on_peek );
			option_single ( "Yaw Shoot", "Changes yaw after shoot", variables::antiaim::antiaimAfterShoot );
		}, 500, variables::antiaim::enable );
		option ( "Exploits", "Gives you a big advatage", [ = ] ( ) {
			option_single ( "Doubletap", "Allows you to shoot twice at the same time", variables::ragebot::double_tap );
			option_slider_int ( "ticks ", "db tap ticks", 2, 18, variables::antiaim::db_tap_ticks );
		}, 300, variables::antiaim::enable );
	
	}
}