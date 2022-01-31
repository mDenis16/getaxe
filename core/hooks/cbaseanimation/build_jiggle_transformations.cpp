//55 8B EC 83 E4 F8 81 EC ? ? ? ? 8B 45 10 56 8B 75 0C F3 0F 11 54 24 ? F3 0F 10 18
#include "../hooks.hpp"

#define JIGGLE_IS_FLEXIBLE				0x01
#define JIGGLE_IS_RIGID					0x02
#define JIGGLE_HAS_YAW_CONSTRAINT		0x04
#define JIGGLE_HAS_PITCH_CONSTRAINT		0x08
#define JIGGLE_HAS_ANGLE_CONSTRAINT		0x10
#define JIGGLE_HAS_LENGTH_CONSTRAINT	0x20
#define JIGGLE_HAS_BASE_SPRING			0x40

namespace hooks::callback {
	void __cdecl build_jiggle_transformations(void* a1, void* a2, void* a3, void* a4, void* a5, void* a6, void* a7, void* a8, void* a9) {

		/*if (*jiggleInfo & JIGGLE_HAS_YAW_CONSTRAINT)
			*jiggleInfo &= ~(JIGGLE_HAS_YAW_CONSTRAINT);

		if (*jiggleInfo & JIGGLE_HAS_PITCH_CONSTRAINT)
			*jiggleInfo &= ~(JIGGLE_HAS_PITCH_CONSTRAINT);

		if (*jiggleInfo & JIGGLE_HAS_ANGLE_CONSTRAINT)
			*jiggleInfo &= ~(JIGGLE_HAS_ANGLE_CONSTRAINT);*/

 		//build_jiggle_transformations_original(ecx, edx, a1,a2,a3,a4,a5,a6);
		reinterpret_cast<void(__cdecl*) (void* a1, void* a2, void* a3, void* a4, void* a5, void* a6, void* a7, void* a8, void* a9)>(hooks::list.at(hooks::hook_index::build_jiggle_transformations)->original);k

	//	std::cout << "build jiggle transformations original " << std::endl;

		//	interfaces::console->console_printf ( "called build_transformations_original for player %s \n", player->get_info ( ).name );
  	}
}