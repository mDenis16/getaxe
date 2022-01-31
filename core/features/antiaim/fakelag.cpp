#include "../../helpers/helpers.h"
#include "fakelag.h"
#include "config.h"


void c_fakelag::run(c_usercmd* cmd)
{
	b_send_this_tick = interfaces::clientstate->m_choked_commands >= config.antiaim.fakelag[0].choke_limit;

	if (choke_next_tick)
	{
		force_choke_this_tick = true;
		choke_next_tick = false;
	}
}

void c_fakelag::end_run(c_usercmd* cmd, uintptr_t* framepointer)
{
	if (force_choke_this_tick){
		b_send_this_tick = true;
		force_choke_this_tick = false;
	}

	*reinterpret_cast<bool*>(*framepointer - 0x1C) = b_send_this_tick;// interfaces::clientstate->m_choked_commands >= config.antiaim.fakelag[0].choke_limit;

	last_tick_status = b_send_this_tick;
}