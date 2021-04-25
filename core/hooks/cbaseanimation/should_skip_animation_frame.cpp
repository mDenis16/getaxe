#include "../../features/features.hpp"
#include "../hooks.hpp"


namespace hooks::callback {
	bool __fastcall should_skip_animation_frame ( void *, void * ) {
		return false;
	}
}