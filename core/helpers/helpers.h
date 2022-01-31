#pragma once
#include "../../dependencies/utilities/csgo.hpp"

class c_local_data
{
public:
	bool in_game = false;
	bool break_lby = false;
	bool pressing_move = false;
	vec3_t last_lby_angle = vec3_t();
	vec3_t orig_viewangle = vec3_t();
	vec3_t lastViewangle = vec3_t();
	vec3_t eye_position = vec3_t();
	vec3_t head_position = vec3_t();
	vec3_t velocity = vec3_t();
	matrix3x4a_t left_matrix[128];
	matrix3x4a_t right_matrix[128];

	int lastFlags = 0;
	vec3_t strafe_angles = vec3_t();

	bool init_local_anim = false;
	anim_state real_state;
	anim_state fake_state;
	vec3_t old_real_origin = vec3_t();
	vec3_t real_origin = vec3_t();

	vec3_t fake_velocity = vec3_t();

	vec3_t predicted_origin = vec3_t();
	weapon_t* active_weapon = nullptr;
	weapon_info_t* weapon_data = nullptr;
	player_t* pointer = nullptr;
	player_t* old_pointer = nullptr;
	bool alive = false;
	bool have_weapon = false;
	float last_mouse_x = 0.f;
	float last_mouse_y = 0.f;
	float weapon_spread = 0.f;
	float weapon_cone = 0.f;

	int force_choke = 0;
	bool fired_shot = false;
	bool force_send_packet = false;
	bool should_choke_packet = false;
	bool should_send_packet = false;
	bool exploits = false;
	bool in_createmove = false;

	vec3_t abs_origin = vec3_t();

	int ticks_allowed = 0;

	bool should_animate = false;

	int ticks_choke = 0;
	int next_tickbase_shift = 0;
	int tickbase_shift = 0;
	int fixed_tickbase = 0;
	vec3_t last_punch = vec3_t();
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
	bool recharging = false;
};
inline c_local_data i_local_data;



class local_player {
	
public:

	static local_player* instance;

	player_t* player = nullptr;

	static inline c_local_data& data()
	{
		return i_local_data;
	}

	static inline player_t* ptr()
	{
		return instance->player;
	}

	static void detect_game();
	static bool available();
	static void begin_tick(c_usercmd* cmd);
	static void end_tick(c_usercmd* cmd);
	static void post_predict(c_usercmd* cmd);
};
