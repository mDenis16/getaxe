#include "../../features/features.hpp"
#include "../hooks.hpp"


namespace hooks::callback {
	bool __fastcall post_screen_space_fx ( uintptr_t ecx, uintptr_t, const view_setup_t * setup ) {
		if ( !local_player::m_data.pointer )
			return post_screen_space_fx_original ( ecx, setup );

		//visuals::player::think ( );

		return post_screen_space_fx_original ( ecx, setup );
	}
}