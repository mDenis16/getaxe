#include "../../features/features.hpp"
#include "../hooks.hpp"


namespace hooks::callback {
	void __fastcall override_view ( void * ecx, void *, view_setup_t * setup ) {
		/*if ( interfaces::engine->is_in_game( ) ) {
	*
			if ( local_player::m_data.pointer && local_player::m_data.pointer->is_alive( ) && variables::visuals::fov > 0 )
				setup->fov = 90 + variables::visuals::fov;
		}*/


		
		//misc::thirdperson::think ( );

		/*if ( config.visuals_world_removals [ 0 ] && local_player::m_data.pointer && local_player::m_data.pointer->is_alive ( ) ) {
			vec3_t viewPunch = local_player::m_data.pointer->punch_angle ( );
			vec3_t aimPunch = local_player::m_data.pointer->aim_punch_angle ( );

			setup->angles [ 0 ] -= ( viewPunch [ 0 ] + ( aimPunch [ 0 ] * 2 * 0.4499999f ) );
			setup->angles [ 1 ] -= ( viewPunch [ 1 ] + ( aimPunch [ 1 ] * 2 * 0.4499999f ) );
			setup->angles [ 2 ] -= ( viewPunch [ 2 ] + ( aimPunch [ 2 ] * 2 * 0.4499999f ) );
		}*/
		static vec3_t last_origin = vec3_t ( );

		if ( local_pointer && local_pointer->is_alive ( ) && interfaces::input->m_camera_in_third_person ) {
			setup->origin.z = local_pointer->abs_origin ( ).z + 64.f;
		
		}
		
		last_origin = setup->origin;
		setup->fovViewmodel = config.local_visual.view_model_distance;


		override_view_original ( interfaces::clientmode, ecx, setup );


	}
}