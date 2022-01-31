#include "../../helpers/helpers.h"
#include "../misc.h"
#include <config.h>

void c_misc::bunnyhop(c_usercmd* cmd)
{

	if (interfaces::inputsystem->is_button_down(KEY_F))
	{
		cmd->buttons |= in_jump;
		std::cout << "force jump " << std::endl;
	}

	if (config.misc.movement.bunny_hop_type == 1) {
		static bool bLastJumped = false;
		static bool bShouldFake = false;

		if (!bLastJumped && bShouldFake) {
			bShouldFake = false;
			cmd->buttons |= in_jump;
		}
		else if (cmd->buttons & in_jump) {
			if (local_player::ptr()->flags() & fl_onground) {
				bShouldFake = bLastJumped = true;
			}
			else {
				cmd->buttons &= ~in_jump;
				bLastJumped = false;
			}
		}
		else {
			bShouldFake = bLastJumped = false;
		}
	} else if (config.misc.movement.bunny_hop_type == 2)
	{
		static int hops_restricted = 0;
		static int hops_hit = 0;

		if (!(cmd->buttons & in_jump)
			|| (local_player::ptr()->move_type() & move_type::movetype_ladder))
			return;

		if (!(local_player::ptr()->flags() & fl_onground))
		{
			cmd->buttons &= ~in_jump;
			hops_restricted = 0;
		}
		else if (rand() % 100 > config.misc.movement.bunny_hop_hitchance && hops_restricted < 12)
		{
			cmd->buttons &= ~in_jump;
			hops_restricted++;
			hops_hit = 0;
		}
		else
			hops_hit++;
	}
		

}

void c_misc::autostrafe(c_usercmd* cmd)
{
	if (config.misc.movement.strafe_mode_type == 1) {
		static vec3_t target_yaw_add = vec3_t();

		static const auto cl_sidespeed = interfaces::console->get_convar("cl_sidespeed");
		if (!(local_player::ptr()->flags() & fl_onground)) {
			bool back = cmd->buttons & in_back;
			bool forward = cmd->buttons & in_forward;
			bool right = cmd->buttons & in_moveleft;
			bool left = cmd->buttons & in_moveright;

			vec3_t yaw_add = vec3_t();
			if (back) {
				yaw_add.y = -180.f;
				if (right)
					yaw_add.y -= 45.f;
				else if (left)
					yaw_add.y += 45.f;
			}
			else if (right) {
				yaw_add.y = 90.f;
				if (back)
					yaw_add.y += 45.f;
				else if (forward)
					yaw_add.y -= 45.f;
			}
			else if (left) {
				yaw_add.y = -90.f;
				if (back)
					yaw_add.y -= 45.f;
				else if (forward)
					yaw_add.y += 45.f;
			}
			else {
				yaw_add.y = 0.f;
			}

			
			math::interpolate_angles(target_yaw_add, yaw_add, target_yaw_add, config.misc.movement.strafe_yaw_speed * 0.01f);
			std::cout << "target_yaw_add " << target_yaw_add.y << std::endl;

			local_player::data().orig_viewangle.y += target_yaw_add.y;
			cmd->forwardmove = 0.f;
			cmd->sidemove = 0.f;

			const auto delta = math::normalize_yaw(local_player::data().orig_viewangle.y - RAD2DEG(atan2(local_player::ptr()->velocity().y, local_player::ptr()->velocity().x)));

			cmd->sidemove = delta > 0.f ? -cl_sidespeed->get_float() : cl_sidespeed->get_float();

			local_player::data().orig_viewangle.y = math::normalize_yaw(local_player::data().orig_viewangle.y - delta);
		}
	}
	else if (config.misc.movement.strafe_mode_type == 2)
	{
		static auto view_angle = vec3_t();
		static auto target_angle = vec3_t();
		static bool was_pressed = false;
		static bool active = false;
		if (!was_pressed && interfaces::inputsystem->is_button_down(KEY_SPACE))
		{
			active = true;
			
		}
		else if (was_pressed && !interfaces::inputsystem->is_button_down(KEY_SPACE))
			active = false;


		static int ticks = 0; static bool side = false;

		if (active) {
			
			if (ticks > 32)
			{
				side = !side;
				std::cout << "angle switch  " << std::endl;
				ticks = 0;
			}

			interfaces::engine->get_view_angles(view_angle);
			target_angle.y = view_angle.y + (side ? 30.f : -30.f);
			target_angle.angle_normalize();

			vec3_t target;
			math::interpolate_angles(cmd->viewangles, target_angle, target, 2.f);

			cmd->viewangles.y = target.y;
			//interfaces::engine->set_view_angles(cmd->viewangles);

			ticks++;
		}

		was_pressed = interfaces::inputsystem->is_button_down(KEY_SPACE);

	}
}