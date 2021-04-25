#include "../../features/features.hpp"
#include "../hooks.hpp"


namespace hooks::callback {
	bool __fastcall create_move ( void * ecx, void * edx, int input_sample_frametime, c_usercmd * cmd ) {

		if ( !cmd || !cmd->command_number )
			return create_move_original ( input_sample_frametime, cmd );


		if ( !interfaces::engine->is_in_game ( ) )
			return  create_move_original ( input_sample_frametime, cmd );

		auto original_result = create_move_original ( input_sample_frametime, cmd );

		if ( original_result ) {
			interfaces::prediction->set_local_view_angles ( cmd->viewangles );
			interfaces::engine->set_view_angles ( cmd->viewangles );
		}


		/*code bellow*/


	}
}