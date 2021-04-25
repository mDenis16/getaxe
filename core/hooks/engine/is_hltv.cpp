#include "../../features/features.hpp"
#include "../hooks.hpp"


namespace hooks::callback {
	bool __fastcall is_hltv ( void * ecx, void * ) {
		return is_hltv_original ( ecx );

		static const auto accumulate_layers_call = reinterpret_cast< void * >( utilities::pattern_scan ( "client.dll", "84 C0 75 0D F6 87" ) );
		static const auto setupvelocity_call = reinterpret_cast< void * >( utilities::pattern_scan ( "client.dll", "84 C0 75 38 8B 0D ? ? ? ? 8B 01 8B 80 ? ? ? ? FF D0" ) );

		if ( _ReturnAddress ( ) == accumulate_layers_call || _ReturnAddress ( ) == setupvelocity_call )
			return true;

		return is_hltv_original ( ecx );
	}
}