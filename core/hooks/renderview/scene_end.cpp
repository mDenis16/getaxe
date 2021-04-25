#include "../../features/features.hpp"
#include "../hooks.hpp"


namespace hooks::callback {
	void __stdcall scene_end ( ) {
		/*visuals::player::paint_traverse ( );

		visuals::local_esp_think ( );
		nade_pred.paint_traverse ( );
		visuals::projectiles::think ( );
		if ( !interfaces::engine->is_connected ( ) || !interfaces::engine->is_in_game ( ) ) {
			scene_end_original ( interfaces::render_view );
			return;
		}

		static const auto mat = interfaces::material_system->find_material ( "dev/glow_armsrace.vmt", nullptr );
		if ( local_pointer ) {

			vec3_t abs_origin = local_pointer->abs_origin ( );
			interfaces::model_render->override_material ( visuals::chams::materials.at ( config.visuals_modulation_enemy_material ) );
			interfaces::render_view->modulate_color ( config.visuals_modulation_enemy_xyz_color );
			local_pointer->set_abs_origin ( csgo::real_origin );
			local_pointer->draw_model ( 0x0, 255 );
			interfaces::model_render->override_material ( nullptr );
			local_pointer->set_abs_origin ( abs_origin );
		}*/

		scene_end_original ( interfaces::render_view );
	}
	
}