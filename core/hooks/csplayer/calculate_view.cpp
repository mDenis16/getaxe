#include "../../features/features.hpp"
#include "../hooks.hpp"


namespace hooks::callback {
	void __fastcall calculate_view ( void * this_pointer, void * edx, vec3_t & eye_origin, vec3_t & eye_angles, float & z_near, float & z_far, float & fov ) {

		
		const auto player = reinterpret_cast< player_t * >( this_pointer );

		if ( player != local_pointer )
			return calculate_view_original ( this_pointer, edx, eye_origin, eye_angles, z_near, z_far, fov );

		const auto old_use_new_animation_state = player->use_new_animation_state ( );

		// prevent calls to ModifyEyePosition
		player->use_new_animation_state ( ) = false;

		calculate_view_original ( this_pointer, edx, eye_origin, eye_angles, z_near, z_far, fov );

		player->use_new_animation_state ( ) = old_use_new_animation_state;

	}
}