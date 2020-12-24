#include "../menu.hpp"

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_internal.h"
#include "../config.hpp"

namespace menu {
	void legitbot ( ) {
		option ( "Aimbot Assist", "Aimbot your enemy", [ = ] ( ) {

			option_slider ( "Fov", "Target fov", 0.f, 180.f, config.legitbot_fov );
			option_slider ( "Smooth", "Smooth", 0.f, 30.f, config.legitbot_smooth );
			option_slider ( "Curve Factor", "Alterates smooth", 0.f, 100.f, config.legitbot_curve_factor );
		}, 300, config.legitbot_enable );

		option ( "Recoil Assist", "Stabilize your recoil", [ = ] ( ) {
			option_slider ( "Horizontal Recoil", "Stabilize horizontal recoil", 0.f, 100.f, config.legitbot_recoil_x );
			option_slider ( "Vertical Recoil", "Stabilize vertical recoil", 0.f, 100.f, config.legitbot_recoil_y );
		}, 300, config.legitbot_recoil );

	}
}