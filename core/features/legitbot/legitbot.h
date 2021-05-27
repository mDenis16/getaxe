#pragma once

class c_legitbot {
public:
	vec3_t local_eye;

	bool can_see_player_pos ( player_t * player );
	void control_rcs ( vec3_t & position );

	player_t * get_closest_target ( );

	void target_check ( );

	void find_target ( );
	int best_hitbox ( );

	int current_target_shots_fired = 0;
	int last_shots_fired = 0;

	vec3_t start_position = vec3_t ( );
	void calc_target_hitbox ( );
	void compute_hitscan ( );

	std::vector<int> hitscan;

	int target_hitbox = 0;
	int last_hitscan_size = -1;

	int last_target_index = -1;

	bool running_curve = false;
	bool did_attack_before = false;
	void run ( c_usercmd * cmd );
	void draw_debug ( ImDrawList * render );
	void aim_at ( vec3_t& position );
	vec3_t calculate_trail ( );

	vec3_t aim_angles;
	vec3_t old_angles;

	std::vector<vec3_t> aim_trail;

	int last_target_tick = 0;

	float t = 0.f;

	bool moving_mouse = false;

	float dt_progress = 0.f;

	weapon_settings * settings;

	int active_target_index = -1;

	player_t * active_target = nullptr;
	c_usercmd * current_cmd = nullptr;

	vec3_t hitbox;

	bool valid_target = false;

};

extern c_legitbot * legitbot;
