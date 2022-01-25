#pragma once


class c_fakelag
{
public:
	void run(c_usercmd* cmd);

	void end_run(c_usercmd* cmd, uintptr_t* framepointer);

	int m_last_choked_ticks = 0;

	bool b_send_this_tick = false;
	bool last_tick_status = false;
	bool choke_next_tick = false;
	bool force_choke_this_tick = false;
};

inline c_fakelag* fakelag = new c_fakelag();
