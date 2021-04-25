#include "../../features/features.hpp"
#include "../hooks.hpp"


namespace hooks::callback {
	bool __fastcall sv_cheats ( void * ecx, void * ) {
		if ( !ecx )
			return false;

		if ( !interfaces::engine->is_in_game ( ) )
			return sv_cheats_original ( ecx );

		if ( local_pointer ) {
			static auto cam_think = utilities::pattern_scan ( "client.dll", "85 C0 75 30 38 86" );

			if ( _ReturnAddress ( ) == cam_think )
				return true;
		}

		return sv_cheats_original ( ecx );
	}
}