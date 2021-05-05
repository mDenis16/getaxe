#include "../../features/features.hpp"
#include "../hooks.hpp"


namespace hooks::callback {
	void __fastcall run_command ( void * ecx, void * edx, player_t * player, c_usercmd * cmd, player_move_helper * move_helper ) {

		if ( !player || player != local_pointer )
			return run_command_original ( ecx, 0, player, cmd, move_helper );

		if ( !cmd )
			return run_command_original ( ecx, 0, player, cmd, move_helper );

		run_command_original ( ecx, 0, player, cmd, move_helper );
	}
}