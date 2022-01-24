#include "../features.hpp"

void c_fakelag::run(c_usercmd* cmd)
{
	b_send_this_tick = interfaces::clientstate->m_choked_commands >= config.antiaim.fakelag[0].choke_limit;

}

void c_fakelag::end_run(c_usercmd* cmd, uintptr_t* framepointer)
{
	*reinterpret_cast<bool*>(*framepointer - 0x1C) = b_send_this_tick;// interfaces::clientstate->m_choked_commands >= config.antiaim.fakelag[0].choke_limit;

	last_tick_status = b_send_this_tick;
}