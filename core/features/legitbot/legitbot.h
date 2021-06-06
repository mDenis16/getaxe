#pragma once

class c_record_t {
public:
	c_record_t( ){ }
	~c_record_t( ){ }

	void apply_tickcount ( );

	void log_data ( player_t* pl );

	player_t * entity;

	float simulation_time = 0.f;

	vec3_t origin;

	matrix3x4_t bone [ 128 ];

	vec3_t mins, maxs;

};

class c_legitbot {
public:
	vec3_t local_eye;
	bool first_aim = false;
	bool avoid_bezier = false;
	bool changed_target = false;
	float original_angle = 0.f;

	int avoid_count = 0;
	int target_index = -1;
	int last_target_index = -1;
	int best_target ( bool no_fov = false );
	bool is_singleshot = false;
	bool use_linear_smoothing = false;
	void control_rcs ( vec3_t & position );
	c_record_t* find_bestlagcomp ( player_t * entity );
	
	int last_weapon_group = -1;
	float fov_to_finish = 0.f;
	
	weapon_t * last_weapon = nullptr;
	bool in_aiming = false;
	void find_target ( );
	void target_check ( );
	bool low_fov = false;
	float fov = 0.f;
	int aim_start_tick = 0;

	void weapon_change ( );

	void manipulate_lagcomp ( );
	int current_target_shots_fired = 0;
	int last_shots_fired = 0;
	bool force_shoot = false;

	vec3_t start_position = vec3_t ( );

	void delay_shoot ( );

	void store_lagcompensation ( );

	std::vector<int> hitscan;

	int target_hitbox = 0;
	int last_hitscan_size = -1;



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
	 int last_tick_moving_mouse = 0;

	float dt_progress = 0.f;

	weapon_settings * settings;

	int active_target_index = -1;

	player_t * active_target = nullptr;
	c_usercmd * current_cmd = nullptr;

	vec3_t hitbox;

	bool valid_target = false;

	std::array<std::vector<c_record_t *>, 64> records;

	void reset_target ( ) {
		    use_linear_smoothing = false;
			running_curve = false;
			dt_progress = 0.f;
			low_fov = false;
			fov = 0.f;
			aim_start_tick = 0;
			use_linear_smoothing = false;
			in_aiming = false;
			target_index = -1;
			current_target_shots_fired = 0;
			last_shots_fired = 0;
			aim_start_tick = 0;
			last_target_tick = interfaces::globals->tick_count;
			did_attack_before = false;
			start_position = vec3_t ( 0, 0, 0 );
			valid_target = false;
			active_target = nullptr;
			std::cout << "reseted target" << std::endl;
	}
	bool is_pistol ( void * weapon );
	bool is_heavy_pistol ( void * weapon );
	bool is_sniper ( void * weapon );
	bool is_smg ( void * weapon );
	bool is_heavy ( void * weapon );
	bool is_rifle ( void * weapon );
	int weapon_category ( weapon_t * weapon );
};

extern c_legitbot * legitbot;
