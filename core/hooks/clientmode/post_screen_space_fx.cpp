#include "../hooks.hpp"


namespace hooks::callback {
	bool __fastcall post_screen_space_fx ( uintptr_t ecx, uintptr_t, const view_setup_t * setup ) {
		if ( !local_player::ptr() )
			return post_screen_space_fx_original ( ecx, setup );

		
	   
		//modulation::run_glow ( );


		return post_screen_space_fx_original ( ecx, setup );
	}
}