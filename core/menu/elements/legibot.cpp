#include "../menu.hpp"
#include "../variables.hpp"
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_internal.h"

namespace menu {
	void legitbot ( ) {
		option ( "Aimbot Assist", "Aimbot your enemy", [ = ] ( ) {

			option_slider ( "Fov", "Target fov", 0.f, 180.f, variables::legit_bot::fov );
			option_slider ( "Smooth", "Smooth", 0.f, 30.f, variables::legit_bot::smooth );
			option_slider ( "Curve Factor", "Alterates smooth", 0.f, 100.f, variables::legit_bot::curve_factor );
		}, 300, variables::legit_bot::enable );
		option ( "Recoil Assist", "Stabilize your recoil", [ = ] ( ) {
			option_slider ( "Horizontal Recoil", "Stabilize horizontal recoil", 0.f, 100.f, variables::legit_bot::recoil_x );
			option_slider ( "Vertical Recoil", "Stabilize vertical recoil", 0.f, 100.f, variables::legit_bot::recoil_y );
		}, 300, variables::legit_bot::recoil );
	}
}