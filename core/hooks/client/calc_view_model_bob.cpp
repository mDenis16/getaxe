#include "../../features/features.hpp"
#include "../hooks.hpp"


namespace hooks::callback {
	void __fastcall calc_view_model_bob (player_t * player, void* edx, vec3_t & position ) {

		return calc_view_model_bob_original ( player, edx, position );

	}
}