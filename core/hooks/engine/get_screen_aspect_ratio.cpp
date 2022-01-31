#include "../hooks.hpp"
#include <config.h>

namespace hooks::callback {
	
	float __fastcall get_screen_aspect_ratio ( void * ecx, void * edx, int width, int height ) {

		if ( !config.local_visual.aspect_ratio )
			return get_screen_aspect_original ( ecx, width, height );

		return config.local_visual.aspect_ratio;
	}
}