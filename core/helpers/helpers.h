#pragma once
#include "../../dependencies/utilities/csgo.hpp"

#include "../menu/ImGui/imgui.h"
#include "../menu/ImGui/imgui_internal.h"


namespace local_player {
	struct command {
		bool is_outgoing = false;
		bool is_used = false;
		int previous_command_number = 0;
		int command_number = 0;
	};

	struct correction_data {
		int command_number = 0;
		int tickcount = 0;
		int choked_commands = 0;
	};

	struct data {
		bool in_game = false;

		vec3_t orig_viewangle = vec3_t();
		vec3_t eye_position = vec3_t ( );
		vec3_t velocity = vec3_t ( );
		weapon_t* active_weapon = nullptr;
		weapon_info_t * weapon_data = nullptr;
		player_t* pointer = nullptr;
		bool alive = false;
		bool have_weapon = false;
		float weapon_spread = 0.f;
		float weapon_cone = 0.f;
		std::vector <int> choked_number ;
		std::deque <command> commands ;
		std::deque <correction_data> data;

		bool double_tap_fire = false;
		bool double_tap_aim = false;
		bool double_tap_aim_check = false;


		bool fired_shot = false;
		bool force_send_packet = false;
		bool should_choke_packet = false;
		bool should_send_packet = false;
		bool exploits = false;
		bool in_createmove = false;

		int ticks_allowed = 0;
		int ticks_choke = 0;
		int next_tickbase_shift = 0;
		int tickbase_shift = 0;
		int fixed_tickbase = 0;
		int backup_tickbase = 0;
		int current_weapon = 0;
		int last_aimbot_shot = 0;
		float last_velocity_modifier = 0.0f;
		bool revolver_working = false;
		bool fakeducking = false;
		float original_forwardmove = 0.0f;
		float original_sidemove = 0.0f;
		bool aimbot_working = false;
		float antiaim_yaw = 0.f;
	};
	extern data m_data;
	void detect_game ( );
	bool available ( );
	void begin_tick ( c_usercmd * cmd );
	void end_tick ( c_usercmd * cmd );
}