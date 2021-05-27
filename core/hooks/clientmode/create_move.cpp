#include "../../features/features.hpp"
#include "../hooks.hpp"


namespace hooks::callback {
	bool __fastcall create_move ( void * ecx, void * edx, int input_sample_frametime, c_usercmd * cmd ) {

		if ( !cmd )
			return create_move_original ( input_sample_frametime, cmd );


		if ( !interfaces::engine->is_in_game ( ) )
			return  create_move_original ( input_sample_frametime, cmd );

		



		/*code bellow*/

		uintptr_t * frame_pointer;
		__asm mov frame_pointer, ebp;


		csgo::in_create_move = false;
		csgo::send_packet = true;


		 local_player::begin_tick ( cmd );


			std::cout << "createmove original called " << std::endl;
			legitbot->run ( cmd );

			interfaces::prediction->set_local_view_angles ( cmd->viewangles );
			interfaces::engine->set_view_angles ( cmd->viewangles );
		
		




		local_player::end_tick ( cmd );

		auto original_result = create_move_original ( input_sample_frametime, cmd );
	
	}
}