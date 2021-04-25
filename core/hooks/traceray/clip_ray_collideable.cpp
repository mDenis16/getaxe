#include "../../features/features.hpp"
#include "../hooks.hpp"


namespace hooks::callback {
	void __fastcall clip_ray_collideable ( void * ecx, void * edx, const ray_t & ray, uint32_t fMask, collideable_t * pCollide, void * pTrace ) {

		clip_ray_collideable_original ( ecx, ray, fMask, pCollide, pTrace ); /*need to get working again => misses head in air :(*/
		return;
		if ( pCollide ) {
			// extend the tracking
			const auto old_max = pCollide->maxs ( ).z;

			auto v9 = ( player_t * ) ( uintptr_t ( pCollide ) - 4 );

			if ( v9 == local_pointer ) {
				printf ( "modify local traceray \n" );
				pCollide->maxs ( ).z += 5;
				pCollide->maxs ( ).z -= 5;

				pCollide->maxs ( ).x += 10;
				pCollide->maxs ( ).x -= 10;

				pCollide->maxs ( ).y += 10;
				pCollide->maxs ( ).y -= 10;
			}
			else {

				pCollide->maxs ( ).z += 5; // if the player is holding a knife and ducking in air we can still trace to this faggot and hit him
			}
			clip_ray_collideable_original ( ecx, ray, fMask, pCollide, pTrace );

			// restore Z
			pCollide->maxs ( ).z = old_max;
			return;
		}

		clip_ray_collideable_original ( ecx, ray, fMask, pCollide, pTrace );
	}
}