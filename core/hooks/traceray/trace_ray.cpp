
#include "../hooks.hpp"


namespace hooks::callback {
	void __fastcall trace_ray ( void * thisptr, void *, const ray_t & ray, unsigned int fMask, trace_filter * pTraceFilter, trace_t * pTrace ) { /*need to complete implementation*/
		/*if ( !csgo::in_trace )
			return trace_ray_original ( thisptr, ray, fMask, pTraceFilter, pTrace );*/
		trace_ray_original ( thisptr, ray, fMask, pTraceFilter, pTrace );
		pTrace->surface.flags |= SURF_SKY;
	}
}