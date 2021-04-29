#include "../../features/features.hpp"
#include "../hooks.hpp"

namespace hooks::callback {
	void __fastcall update_client_side_animations ( player_t * player, uint32_t ) {

		if ( !player )
			return update_client_side_animation_original ( player );

		if ( !player->is_alive ( ) )
			return update_client_side_animation_original ( player );

		if ( player->dormant ( ) )
			return update_client_side_animation_original ( player );

		if ( player != local_pointer ) {
				update_client_side_animation_original ( player );
		}
		

	}
}