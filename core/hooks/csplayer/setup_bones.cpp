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
		auto previous_weapon = animstate ? animstate->m_pLastActiveWeapon : nullptr;

		if ( previous_weapon )
			animstate->m_pLastActiveWeapon = animstate->m_pActiveWeapon;

		const auto& anim = anim_manager.allow_bones[e->index()];

		if (anim || !e->m_CachedBoneData().count()) {

			static auto invalidate_bone_bache_fn = utilities::pattern_scan("client.dll", "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81");

			static auto force_multithread_bones = utilities::pattern_scan("client.dll", "C6 05 ? ? ? ? ? 83 FE 01 0F 8E ? ? ? ? A1 ? ? ? ? 8B 90 ? ? ? ? 89 55 F8 85 D2 74 24 6A 04 6A 00 68 ? ? ? ? 6A 00 68 ? ? ? ? 8B C8");


			unsigned long g_iModelBoneCounter = **(unsigned long**)(invalidate_bone_bache_fn + 10);

			//*(bool*)(force_multithread_bones) = true;

			///std::cout << "force_multithread_bones addr " << std::hex << (std::uint32_t)(force_multithread_bones) << std::endl;
			
			//m_iMostRecentModelBoneCounter 
			*(unsigned long*)((uintptr_t)ecx + 0x268C) = g_iModelBoneCounter - 1;

			//m_flLastBoneSetupTime 
			*(float*)((uintptr_t)ecx + 0x2924) = -FLT_MAX;

		
			result = setup_bones_original(ecx, bone_to_world_out, max_bones, bone_mask, curtime);
		}
		else if ( bone_to_world_out && max_bones != -1 )
			memcpy ( bone_to_world_out, e->m_CachedBoneData ( ).base ( ), e->m_CachedBoneData ( ).count ( ) * sizeof ( matrix3x4_t ) );



		if ( previous_weapon )
			animstate->m_pLastActiveWeapon = previous_weapon;

		

		return result;
	}
}