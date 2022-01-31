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


		//csgo::cmd = cmd;

		uintptr_t* frame_pointer;
		__asm mov frame_pointer, ebp;


		//local_player::begin_tick(cmd);

		if (interfaces::inputsystem->is_button_down(KEY_H))
			cmd->buttons |= in_jump;

		///misc.run_createmove(cmd);

		//fakelag->run(cmd);



		//antiaim->run(cmd);
		//fakelag->end_run(cmd, frame_pointer);


		
		local_player::data().lastFlags = local_player::ptr()->flags();

		cmd->viewangles.angle_normalize();
		cmd->viewangles.angle_clamp();

		/*if (fakelag->b_send_this_tick) {
			anim_manager.velocities[local_player::ptr()->index()] = local_player::ptr()->velocity();
			local_player::data().antiaim_yaw = cmd->viewangles.y;
			local_player::data().lastViewangle = cmd->viewangles;
		}else
		{
			
		}*/
		
		//local_player::end_tick(cmd);


		

		local_player::data().should_animate = true;

		/*code bellow*/

		
		return false;
	}
	
}