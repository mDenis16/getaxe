
#include "../hooks.hpp"


namespace hooks::callback {
	int __fastcall file_system ( void *, void * ) {
		static auto return_to_client_panorama = utilities::pattern_scan ( "client.dll", "83 F8 02 0F 85 ? ? ? ? 8B 3D ? ? ? ? FF D7" );
		static auto return_to_engine = utilities::pattern_scan ( "engine.dll", "83 F8 02 75 6C 68 ? ? ? ? FF 15 ? ? ? ? 8B 4C 24 28 83 C4 04" );

		if ( _ReturnAddress ( ) == return_to_client_panorama || _ReturnAddress ( ) == return_to_engine )
			return 0; //return 0 in some cases to not being kicked from game

		return 1; //return 1 to allow loading of chams materials
	}

}