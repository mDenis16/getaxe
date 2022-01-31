
#include "../hooks.hpp"


namespace hooks::callback {
	void __fastcall build_transformations ( void * ecx, void * edx, int a2, int a3, int a4, int a5, int a6, int a7 ) {
	
		bool* m_isJiggleBonesEnabled = (bool*)(((std::uint32_t)ecx) + 0x2930);
		
		auto player = ( player_t * ) ( ecx );

		if ( !player )
			return build_transformations_original ( ecx, edx, a2, a3, a4, a5, a6, a7 );

		if ( player->dormant ( ) )
			return build_transformations_original ( ecx, edx, a2, a3, a4, a5, a6, a7 );

		if ( !player->is_alive ( ) )
			return build_transformations_original ( ecx, edx, a2, a3, a4, a5, a6, a7 );

		//if ( player == local_player::ptr() )
			//return build_transformations_original ( ecx, edx, a2, a3, a4, a5, a6, a7 );

		bool backup = *m_isJiggleBonesEnabled;

		*m_isJiggleBonesEnabled = false;

	
		build_transformations_original ( ecx, edx, a2, a3, a4, a5, a6, a7 );

		*m_isJiggleBonesEnabled = backup;
	}
}