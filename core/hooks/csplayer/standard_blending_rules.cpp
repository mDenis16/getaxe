#include "../hooks.hpp"

static constexpr auto BONE_USED_BY_SERVER = BONE_USED_BY_HITBOX | BONE_USED_BY_VERTEX_LOD0 | BONE_USED_BY_VERTEX_LOD1 | BONE_USED_BY_VERTEX_LOD2;

namespace hooks::callback {
	void _fastcall standard_blending_rules ( player_t * player, uint32_t, c_studio_hdr * hdr, vec3_t * pos, void * q, const float time, int mask ) {


		if ( !player )
			return standard_blending_rules_original ( player, hdr, pos, q, time, mask );

		
		if ( player ) {
			

			if ( !( player->Effects( ) & 0x008 ) )
				player->Effects( ) |= 0x008;
			standard_blending_rules_original ( player, hdr, pos, q, time, BONE_USED_BY_SERVER );
			player->Effects( ) &= ~0x008;
		}
	}
}