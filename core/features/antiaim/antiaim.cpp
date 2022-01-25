#include "../features.hpp"

void c_antiaim::run(c_usercmd* cmd)
{
	auto weapon = local_player::m_data.pointer->active_weapon();

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!local_player::m_data.pointer->is_alive())
		return;

	if (!local_player::m_data.pointer->active_weapon())
		return;


	if (!weapon)
		return;


	if (!allow(cmd))
		return;

	handle_pitch(cmd);


	if (cmd->buttons & in_use)
		cmd->viewangles.x = 0;


	yaw_base(cmd);

	yaw_desync(cmd);



	cmd->viewangles.angle_normalize();
	cmd->viewangles.angle_clamp();
}

bool c_antiaim::allow(c_usercmd* ucmd) {


	auto local = local_player::m_data.pointer;
	if (!local && !local->is_alive())
		return false;

	const auto move_type = local->move_type();
	if (move_type == (int)movetype_ladder || move_type == (int)movetype_noclip)
		return false;

	auto weapon = local->active_weapon();
	if (!weapon)
		return false;

	if (weapon->is_knife()) {
		float next_secondary_attack = weapon->next_secondary_attack() - interfaces::globals->cur_time;
		float next_primary_attack = weapon->next_primary_attack() - interfaces::globals->cur_time;

		if (ucmd->buttons & (int)in_attack && next_primary_attack <= 0.f || ucmd->buttons & (int)in_attack2 && next_secondary_attack <= 0.f)
			return false;
	}

	if (weapon->is_grenade()) {
		if (!weapon->pin_pulled() || (ucmd->buttons & in_attack) || (ucmd->buttons & in_attack2))
		{
			float throwTime = weapon->throw_time();

			if ((throwTime > 0) && (throwTime < interfaces::globals->cur_time))
				return false;
		}
	}
	else {

		if (ucmd->buttons & (int)in_attack && local->can_shoot(false)) {
			fakelag->choke_next_tick = true;
			fakelag->b_send_this_tick = true;
			return false;
		}

	}


	return true;
}
void c_antiaim::yaw_desync(c_usercmd* cmd)
{

	if (!fakelag->b_send_this_tick)
		switch (config.antiaim.desync_side)
		{
	    case 0:
		cmd->viewangles.y = localdata.antiaim_yaw - config.antiaim.desync_ammount[0];
		break;
		case 1:
			cmd->viewangles.y = localdata.antiaim_yaw + config.antiaim.desync_ammount[0];
			break;
		default:break;
		}
	

	if (fabsf(cmd->sidemove) < 2.0f) {
		cmd->sidemove = cmd->tick_count % 2 ? 1.10f : -1.10f;
	}
	
}

void c_antiaim::yaw_base(c_usercmd* cmd)
{
	switch (config.antiaim.base_yaw)
	{
	case 1:
		cmd->viewangles.y += 180.f;
		break;
	}

	cmd->viewangles.angle_normalize();
	cmd->viewangles.angle_clamp();
}

void c_antiaim::handle_pitch(c_usercmd* cmd)
{
	switch (config.antiaim.base_pitch)
	{
	case 1: cmd->viewangles.x = 89.9f; break;
	case 3: cmd->viewangles.x = -89.9f; break;
	}
}