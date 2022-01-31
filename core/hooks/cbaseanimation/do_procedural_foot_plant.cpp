
#include "../hooks.hpp"


namespace hooks::callback {
	void __fastcall do_procedural_foot_plant ( void * a1, void *, int a2, int a3, int a4, int a5 ) {

	
		auto orig = *( std::uint32_t * ) ( ( uintptr_t ) a1 + 96 );


		*( std::uint32_t * ) ( ( uintptr_t ) a1 + 96 ) = 0;
		
	
		do_procedural_foot_plant_original ( a1, a2, a3, a4, a5 );
	

		*( std::uint32_t * ) ( ( uintptr_t ) a1 + 96 ) = orig;

	}
}