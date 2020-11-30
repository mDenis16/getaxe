#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "../menu/variables.hpp"
#include "../menu/ImGui/imgui.h"
#include "../menu/ImGui/imgui_internal.h"


namespace local_player {
	struct data {
		vec3_t orig_viewangle;
		vec3_t eye_position;
		vec3_t velocity;
		weapon_t* active_weapon;
		weapon_info_t * weapon_data;
		player_t* pointer;
		bool alive = false;
		bool have_weapon = false;
	};
	extern data m_data;
	void begin_tick ( c_usercmd * cmd );
}