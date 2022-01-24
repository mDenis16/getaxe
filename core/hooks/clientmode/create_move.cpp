#include "../../features/features.hpp"
#include "../hooks.hpp"
#include "../../features/animations/animations_manager.h"


namespace hooks::callback {
	bool __fastcall create_move(void* ecx, void* edx, int input_sample_frametime, c_usercmd* cmd) {

		if (hooks::unloading)
			return create_move_original(input_sample_frametime, cmd);


		if (!cmd || !cmd->command_number)
			return create_move_original(input_sample_frametime, cmd);


		if (!interfaces::engine->is_in_game())
			return  create_move_original(input_sample_frametime, cmd);

		auto original_result = create_move_original(input_sample_frametime, cmd);

		if (original_result) {
			interfaces::prediction->set_local_view_angles(cmd->viewangles);
			interfaces::engine->set_view_angles(cmd->viewangles);
		}


		uintptr_t* frame_pointer;
		__asm mov frame_pointer, ebp;


		local_player::begin_tick(cmd);

		if (interfaces::inputsystem->is_button_down(KEY_H))
			cmd->buttons |= in_jump;

		///misc.run_createmove(cmd);

		fakelag->run(cmd);



		fakelag->end_run(cmd, frame_pointer);


		
		localdata.lastFlags = local_pointer->flags();

		if (fakelag->b_send_this_tick)
			anim_manager.velocities[local_pointer->index()] = local_pointer->velocity();

		local_player::end_tick(cmd);


		localdata.lastViewangle = cmd->viewangles;

		/*code bellow*/


	}
}