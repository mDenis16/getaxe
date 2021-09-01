#include "../../features/features.hpp"
#include "../hooks.hpp"


namespace hooks::callback {
	bool __fastcall create_move ( void * ecx, void * edx, int input_sample_frametime, c_usercmd * cmd ) {

		if ( !cmd)
			return create_move_original ( input_sample_frametime, cmd );

		local_player::m_data.last_mouse_x = cmd->mousedx;
		local_player::m_data.last_mouse_y = cmd->mousedy;


		if (cmd->command_number != 0)
			return  create_move_original(input_sample_frametime, cmd);

		if ( !interfaces::engine->is_in_game ( ) )
			return  create_move_original ( input_sample_frametime, cmd );

		



		/*code bellow*/

		uintptr_t * frame_pointer;
		__asm mov frame_pointer, ebp;


		csgo::in_create_move = false;
		csgo::send_packet = true;


		 local_player::begin_tick ( cmd );


		//	std::cout << "createmove original called " << std::endl;
			legitbot->run ( cmd );

		
	


		local_player::end_tick ( cmd );


		auto original_result = create_move_original ( input_sample_frametime, cmd );
	
		if (cmd->buttons & in_attack) {
			printf("In attack \n");

		}

	}
}