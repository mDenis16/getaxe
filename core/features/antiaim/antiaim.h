#pragma once
#include "..\..\..\source-sdk\classes\c_usercmd.hpp"


class c_antiaim
{
public:
	void run(c_usercmd* cmd);

	void yaw_desync(c_usercmd* cmd);
	void yaw_base(c_usercmd* cmd);

	void handle_pitch(c_usercmd* cmd);
	bool allow(c_usercmd* ucmd);
};

inline c_antiaim* antiaim = new c_antiaim();
