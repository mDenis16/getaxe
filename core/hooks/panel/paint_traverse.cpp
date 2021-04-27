#include "../../features/features.hpp"
#include "../hooks.hpp"


namespace hooks::callback {
	void __stdcall paint_traverse ( unsigned int panel, bool force_repaint, bool allow_force ) {
		auto panel_to_draw = fnv::hash ( interfaces::panel->get_panel_name ( panel ) );



		switch ( panel_to_draw ) {
		case fnv::hash ( "MatSystemTopPanel" ):
		{

			/*interfaces::engine->get_screen_size ( csgo::screen_width, csgo::screen_height );
			if ( interfaces::engine->is_in_game ( ) && interfaces::engine->is_connected ( ) && local_pointer && local_pointer->is_alive ( ) ) {


				for ( int i = 1; i <= interfaces::globals->max_clients; i++ ) {
					player_t * player = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( i ) );

					if ( !player )
						continue;


					if ( !player->dormant ( ) && player->is_alive ( ) ) {
						draw_angles ( player, vec3_t ( 0, player->lower_body_yaw ( ), 0 ), color ( 0, 255, 0, 255 ), "LBY" );
					}
				}

			}*/
		}
		break;

		case fnv::hash ( "FocusOverlayPanel" ):
			interfaces::panel->set_keyboard_input_enabled ( panel, menu_opened );
			interfaces::panel->set_mouse_input_enabled ( panel, menu_opened );
			break;
		}


		static unsigned int panelID = 0, panelHudID = 0;

		/*if ( !panelHudID )
			if ( config.visuals_world_removals [ 4 ] && fnv::hash ( "HudZoom" ) == panel_to_draw ) {
				panelHudID = panel;
			}

		if ( panelHudID == panel && local_player::m_data.pointer && local_player::m_data.pointer->is_alive ( ) ) {
			if ( local_player::m_data.pointer->is_scoped ( ) )
				return;
		}*/

		paint_traverse_original ( interfaces::panel, panel, force_repaint, allow_force );
	}
}