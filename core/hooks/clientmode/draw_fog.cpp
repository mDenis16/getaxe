#include "../hooks.hpp"
#include <config.h>

namespace hooks::callback {
	bool __fastcall draw_fog ( void * ecx, void * edx ) {
		return !config.local_visual.removals_input [ REMOVALS_FOGS ];
	}
}