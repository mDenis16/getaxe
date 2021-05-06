#include "../../features/features.hpp"
#include "../hooks.hpp"


namespace hooks::callback {
	void __fastcall override_view ( void * ecx, void *, view_setup_t * setup ) {
		/*if ( interfaces::engine->is_in_game( ) ) {
	*
			if ( local_player::m_data.pointer && local_player::m_data.pointer->is_alive( ) && variables::visuals::fov > 0 )
				setup->fov = 90 + variables::visuals::fov;
		}*/


		
		visuals::thirdperson ( );


		if ( config.local_visual.removals_input [ REMOVALS_RECOIL ] && local_player::m_data.pointer && local_player::m_data.pointer->is_alive ( ) ) {
			vec3_t viewPunch = local_player::m_data.pointer->punch_angle ( );
			vec3_t aimPunch = local_player::m_data.pointer->aim_punch_angle ( );

			setup->angles [ 0 ] -= ( viewPunch [ 0 ] + ( aimPunch [ 0 ] * 2 * 0.4499999f ) );
			setup->angles [ 1 ] -= ( viewPunch [ 1 ] + ( aimPunch [ 1 ] * 2 * 0.4499999f ) );
			setup->angles [ 2 ] -= ( viewPunch [ 2 ] + ( aimPunch [ 2 ] * 2 * 0.4499999f ) );
		}
		static vec3_t last_origin = vec3_t ( );
		static int tick = 0;

		if ( local_pointer && local_pointer->is_alive ( ) && interfaces::input->m_camera_in_third_person ) {
		//	setup->origin.z = local_pointer->abs_origin ( ).z + 64.f;
		
		}
		if ( local_pointer && local_pointer->is_alive ( ) ) {
			auto weapon = local_pointer->active_weapon ( );

			if ( weapon ) {
				if ( !local_pointer->is_scoped ( ))
					setup->fov += config.local_visual.view_model_distance;
				else if (config.local_visual.removals_input[ REMOVALS_ZOOM ]) {
					if ( weapon->zoom_level ( ) == 1 )
						setup->fov = 90.0f + ( float ) config.local_visual.view_model_distance;
					else
						setup->fov += ( float ) config.local_visual.view_model_distance;
				}

				if ( interfaces::globals->tick_count != tick ) {

					if ( config.local_visual.view_model_x_offset != 0.f ) {
					
						static auto viewXcvar = interfaces::console->get_convar ( crypt_str ( "viewmodel_offset_x" ) ); 

						if ( viewXcvar->get_float ( ) != config.local_visual.view_model_x_offset ) //-V550
						{
							*( float * ) ( ( DWORD ) &viewXcvar->change_callback_fn + 0xC ) = 0.0f;
							viewXcvar->set_value ( config.local_visual.view_model_x_offset );
						}
					}

					
					if ( config.local_visual.view_model_y_offset != 0.f ) {

						static auto viewXcvar = interfaces::console->get_convar ( crypt_str ( "viewmodel_offset_y" ) );

						if ( viewXcvar->get_float ( ) != config.local_visual.view_model_y_offset ) //-V550
						{
							*( float * ) ( ( DWORD ) &viewXcvar->change_callback_fn + 0xC ) = 0.0f;
							viewXcvar->set_value ( config.local_visual.view_model_y_offset );
						}
					}

					if ( config.local_visual.view_model_z_offset != 0.f ) {

						static auto viewXcvar = interfaces::console->get_convar ( crypt_str ( "viewmodel_offset_z" ) ); 

						if ( viewXcvar->get_float ( ) != config.local_visual.view_model_z_offset ) //-V550
						{
							*( float * ) ( ( DWORD ) &viewXcvar->change_callback_fn + 0xC ) = 0.0f;
							viewXcvar->set_value ( config.local_visual.view_model_z_offset );
						}
					}

					

					tick = interfaces::globals->tick_count;
				}
			}
		}

		override_view_original ( interfaces::clientmode, ecx, setup );


	}
}