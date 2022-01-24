#include "../../features/features.hpp"
#include "../hooks.hpp"

#include "../../features/animations/animations_manager.h"

namespace hooks::callback {
	bool __fastcall setup_bones ( void * ecx, void *, matrix3x4_t * bone_to_world_out, int max_bones, int bone_mask, float curtime ) {
	
		auto result = true;

		auto e = reinterpret_cast < player_t * >( reinterpret_cast < uintptr_t > ( ecx ) - 0x4 );

		if ( !e )
			return setup_bones_original ( ecx, bone_to_world_out, max_bones, bone_mask, curtime );

		if ( e->dormant ( ) )
			return setup_bones_original ( ecx, bone_to_world_out, max_bones, bone_mask, curtime );


		if ( !e->is_alive ( ) )
			return  setup_bones_original ( ecx, bone_to_world_out, max_bones, bone_mask, curtime );


		auto animstate = e->get_anim_state ( );
		auto previous_weapon = animstate ? animstate->m_last_weapon : nullptr;

		if ( previous_weapon )
			animstate->m_last_weapon = animstate->m_weapon;

		const auto& anim = anim_manager.allow_bones[e->index()];

		if ( anim || !e->m_CachedBoneData ( ).count ( ) )
			result = setup_bones_original ( ecx, bone_to_world_out, max_bones, bone_mask, curtime );
		else if ( bone_to_world_out && max_bones != -1 )
			memcpy ( bone_to_world_out, e->m_CachedBoneData ( ).base ( ), e->m_CachedBoneData ( ).count ( ) * sizeof ( matrix3x4_t ) );



		if ( previous_weapon )
			animstate->m_last_weapon = previous_weapon;

		

		return result;
	}
}