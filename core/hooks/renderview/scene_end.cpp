
#include "../hooks.hpp"
#include <config.h>
#include "../../features/visuals/modulation/modulation.h"

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
		if ( local_player::ptr() ) {

			vec3_t abs_origin = local_player::ptr()->abs_origin ( );
			interfaces::model_render->override_material ( visuals::chams::materials.at ( config.visuals_modulation_enemy_material ) );
			interfaces::render_view->modulate_color ( config.visuals_modulation_enemy_xyz_color );
			local_player::ptr()->set_abs_origin ( csgo::real_origin );
			local_player::ptr()->draw_model ( 0x0, 255 );
			interfaces::model_render->override_material ( nullptr );
			local_player::ptr()->set_abs_origin ( abs_origin );
		}*/

		if ( !interfaces::engine->is_connected ( ) || !interfaces::engine->is_in_game ( ) ) {
			scene_end_original ( interfaces::render_view );
			return;
		}
		if ( local_player::ptr() && local_player::ptr()->is_alive ( ) ) {
			auto hViewModel = local_player::ptr()->view_model ( );
			if ( hViewModel != 0xFFFFFFFF && modulation::materials.at ( 0 ).mat_address ) {
				auto pViewModel = reinterpret_cast< base_view_model_t * >( interfaces::entity_list->get_client_entity_handle ( hViewModel ) );
				if ( pViewModel ) {
					//interfaces::model_render->override_material ( modulation::materials.at ( 0 ).mat_address );

					//interfaces::render_view->modulate_color ( ( float * ) &config.local_visual.chams [ WEAPON ].color.Value );

					//pViewModel->draw_model ( 0x0, 255 );
					//interfaces::model_render->override_material ( nullptr );
				}
			}
		}
		scene_end_original ( interfaces::render_view );
	}
	
}