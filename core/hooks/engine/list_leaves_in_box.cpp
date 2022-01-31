
#include "../hooks.hpp"


namespace hooks::callback {
	int __fastcall list_leaves_in_box ( void * bsp, void * edx, vec3_t & mins, vec3_t & maxs, unsigned short * pList, int listMax ) {
	     return list_leaves_in_box_original ( bsp, edx, mins, maxs, pList, listMax );	
	}
}