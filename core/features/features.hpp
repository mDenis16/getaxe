#pragma once
#include "../../dependencies/utilities/csgo.hpp"

#include "misc/events/events.hpp"
#include "../helpers/helpers.h"

#include <optional>

#define DEBUG_LOG printf
#define local_pointer local_player::m_data.pointer
#define localdata local_player::m_data

static int hitmarker_alpha = 0;
namespace connection {
	void send ( std::string msg );
	void main ( );
	class telemetry {
	public:
		telemetry ( ) { 
			side = 0;
			hit_side = 0;
		}
	
		int id = 0;
		std::string map;
		vec3_t position = vec3_t();
		float max_desync_delta = 0.f;
		float lby = 0.f;
		int autowall_side = 0;
		int side = 0;
		int hit_side = 0;
		bool missed = false;
		float left_yaw = 0.f;
		float right_yaw = 0.f;
		float yaw = 0.f;
		float goal_feet = 0.f;
		void send ( );
	
	};
}
namespace event_manager {
	void round_prestart ( i_game_event *  );
	void round_end ( i_game_event * );
}
namespace overlay {
	void initialize ( IDirect3DDevice9 * device );
	void __stdcall end_present ( IDirect3DDevice9 * /*device*/ );
	void __stdcall pre_render ( IDirect3DDevice9 * device );
	void present ( IDirect3DDevice9 * device );
	inline IDirect3DDevice9 * g_pDevice;

}
namespace resolver {

	namespace event_logs {
		void bullet_impact ( i_game_event * event  );
		void weapon_fire ( i_game_event * event );
		void player_hurt ( i_game_event * event );
	};
	struct resolve_shot {
		float angle_shot;
		int attacker_index;
		int desync_side;

		float angle;
		float angle_related_to_me;
		int tick;
		int enemy_index;
		bool bullat_impact_approved = false;
		bool failed = false;
		struct {
			int victim = -1;
			int damage = -1;
			int hitgroup = -1;
		}hit_info;

		vec3_t shotpos;
		vec3_t hitpos;
		float curtime;
		bool hurt = false;
		bool hit = false;
		bool approved = false;
	};
	
	extern std::deque<resolve_shot> shots;
	
	player_t * get_closest_player_by_point ( vec3_t from, vec3_t point );
	resolver::resolve_shot * closest_shot ( int tickcount, player_t* player );

	int get_desync_side ( vec3_t from, vec3_t to, player_t * entity, int hitbox );

	float server_feet_yaw ( player_t * entity );

	void side_think ( player_t * entity );

	int get_desync_side ( player_t * entity, vec3_t from, vec3_t to );



	float max_desync_delta( player_t* entity );
	bool is_player_peeking( player_t* player );
	void create_move( c_usercmd* cmd );
	float get_safe_point_angle ( int i );

	float get_brute_angle ( player_t * player );

	float get_real_head( player_t* entity );
	void manage_shots ( );
	void guess_desync_side_from_lby ( player_t * player );
	std::string antiaim_name (int index );
	void resolve_player ( player_t * player );
	void draw_shots ( );
	void frame_stage( );
	void frame_stage( c_usercmd* cmd );
}
namespace engine_prediction {
	struct Variables_t {
		float m_flFrametime;
		float m_flCurtime;
		float m_flVelocityModifier;

		vec3_t m_vecVelocity;
		vec3_t m_vecOrigin;
		int m_fFlags;
	};
	extern Variables_t m_stored_variables;
	struct Misc_t {
		int m_iRunCommandTickbase;
		bool m_bOverrideModifier;
	}; extern Misc_t Misc;
	extern vec3_t unpredicted_eye;
	extern vec3_t unpredicted_velocity;
	void run_think ( );
	void initialize( player_t* player, c_usercmd* cmd );
	void unload( player_t* player );
	void patch_attack_packet ( c_usercmd * m_nCmd, bool m_bPredicted );
}
namespace player_manager {
	inline int old_simulation [ 65 ];
	enum desync_type {
		NONE,
		MICROMOVEMENTS,
		LBY_OPOSITE,
		LBY_SIDEWAYS
	};
	enum predicted_side {
		UNKNOWN, 
		LEFT,
		RIGHT
	};
	struct resolve_info {
		desync_type type;
		predicted_side side = UNKNOWN;
		predicted_side wall_side = UNKNOWN;
		float delta = 0.f;

	};
	std::string side_name ( predicted_side side );
	std::string method_name ( desync_type type );

	class lagcomp_t
	{
	private:
		void manage_matrix ( player_t * entity );
		void manage_animations ( player_t * entity );
		void update_animations ( );
		lagcomp_t & get_latest_networked ( ); 
		void matrix_resolve ( player_t * entity );
		void body_rotation ( );
		void delta ( );
	public:
		
		resolve_info resolve_info;
		bool shoot = false;
		bool failed = false;
		int left_dmg, right_dmg = 0;
		float simtime = 0.f;
		int tick_count = 0;
		vec3_t old_vec_origin;
		player_t * entity = nullptr;
		float m_flSpeed = 0.f;
		bool changed_this_tick = false;
		int third_delta = 0;
		vec3_t origin, obbmin, obbmax, abs_origin, abs_angles, eye_angles, velocity;
		vec3_t old_origin;

		matrix3x4_t bone [ 128 ];
		matrix3x4_t bone_left [ 128 ];
		matrix3x4_t bone_right [ 128 ];

	    matrix3x4_t* bones ( predicted_side i ) {
			switch ( i ) {
			case predicted_side::UNKNOWN:
				return this->bone;
				break;
			case predicted_side::LEFT:
				return this->bone_left;
				break;

			case predicted_side::RIGHT:
				return this->bone_right;
				break;
			default:
				break;
			}
		}


		int bone_count = 0;
		int delta_rate = 0;
		float max_delta = 0.f;
		float time_delta = 0.f;
		float speed = 0.f;
		int choked_ticks = 0;
		int simulation_ticks = 0;
		bool in_air  = false;
		bool lag = false;
		bool resolved = false;
		float duck_amount = 0.f;
		bool predicted = false;
		float lby = 0.f;
		bool setup = false;
		bool network_update = false;
		float goal_feet = 0.f;
		bool invalid = false;
		animationlayer layer [ 15 ];
		animationlayer resolve_layers [ 15 ];
		std::array<float, 24> pos_params;
		anim_state animstate;

		anim_state fake_left_state;
		anim_state fake_right_state;

		animationlayer fake_left_layers [ 15 ];
		animationlayer fake_right_layers [ 15 ];

		animationlayer predicted_layers [ 15 ];

		int flags = 0;
		void apply_anims ( player_t * entity );
		void receive_anims ( player_t * entity );
		void apply (player_t* entity, bool collision = false, bool restore = false );
		void restore ( player_t * entity );
		void receive ( player_t * entity,bool predict );

		bool is_valid ( );
		void predict ( player_t * entity );
		void receive ( player_t * entity );
		void wall_detection ( );
		void resolve ( );
		lagcomp_t (  ) {	}
		
	};
	inline std::array<std::deque< lagcomp_t >, 64> records;

	player_t* util_player_by_index( int index );


	void update_animations( player_t* entity );

	void manage_bones( );

	void setup_players( void );

	void recieve_record( player_t* entity, lagcomp_t& record );

	void create_fake_matrix ( player_t * pl, matrix3x4_t bones [ 128 ], float delta );

	void create_fake_matrix_based_on_angle ( player_t * pl, matrix3x4_t bones [ 128 ], vec3_t angle );
	void simulate_movement ( player_t * entity, vec3_t & vecOrigin, vec3_t & vecVelocity, int & fFlags, bool bOnGround );
	float get_fake_abs_yaw ( player_t * pl, float yaw );

	namespace event_logs {
		void bullet_impact( i_game_event* event );
	};

	extern lagcomp_t best_tick_global [ 65 ];
	float interpolation_time( );
	float get_lerp_time( );

	extern 	lagcomp_t backup_data [ 64 ];

	bool is_tick_valid( lagcomp_t record );



	void get_rotated_matrix ( vec3_t origin, matrix3x4_t * from_matrix, float angle, matrix3x4_t * new_matrix );

	void filter_records ( );

	void setup_records( client_frame_stage_t stage);

	void backup_player( player_t* entity );

	void restore_record( player_t* entity, lagcomp_t record );

	void restore_player( player_t* entity );

	inline bool is_dormant [ 65 ];
	inline int update_tick = 0;
	inline float previous_goal_feet_yaw [ 65 ];
	extern float players_health [ 64 ];
}
namespace anti_aim {
	
	extern bool choke_next_tick;
	namespace event_logs {
		void bullet_impact( i_game_event* event );
	};


	extern bool desync_side;
	void real_animation_fix( );
	void update_local_animations( );

	float get_freestanding_angle( );

	void lowerbody_desync ( c_usercmd * cmd, bool & send_packet );

	bool allow( c_usercmd* ucmd, bool& send_packet );
	int freestanding_angle ( );
	void calculate_peek_real ( );
	void desync_freestanding ( c_usercmd * cmd );
	void on_create_move( c_usercmd* cmd, bool& send_packet );
	int best_freestanding_angle ( );
	struct freestand_point {
		int dmg = 0;
		vec3_t point = vec3_t ( );
	};
	extern std::vector<freestand_point> points;
	void fix_call_animation( );

}
namespace gpu_task {
	enum task_type {
	   distance

	};



	class task {
	public:
		int id = 0;
		task_type type;
		bool finished = false;
		long time = 0;
		int attemps = 0;
		void wait_for ( );
		virtual void handle ( ) = 0;
	};
	class calc_distance : public task {
	public:
		vec3_t from, to;
		bool calc_2d =  false;
		float result = 0.f;
		void handle ( ) override;

		calc_distance ( vec3_t _from, vec3_t _to, bool _calc_2d );
	};

	void execute_tasks ( );
	long get_mils ( );
	extern int last_task_id;
	extern std::vector<task *> tasklist;
	extern size_t thread_lock;
}
namespace autopeek {
	
	enum class peek_state {
		standby = 0,
		going_forward,
		going_backward
	};
	struct peek_data {
		peek_state state = peek_state::standby;
		vec3_t backward_position = vec3_t ( );
		vec3_t forward_position = vec3_t ( );
		vec3_t moving_position = vec3_t ( );
		vec3_t origin = vec3_t ( );
		bool pressed_last_tick = false;
		bool should_disable = false;
		bool run = false;
		bool finished = false;
	};
	extern bool shot_fired;
	void run_cm ( c_usercmd * cmd );
	void search_position ( );
	void move ( c_usercmd * cmd );
	void moving_check ( );
	void reset ( );
	void key_check ( );
	
	extern peek_data m_data;
}

namespace fake_lag {
	void create_move ( bool & send_packet );
}



namespace animations {
	struct anim_data {
		bool should_animate  = false ;

		std::array< float, 24 > m_poses;
		std::array< animationlayer, 13 > m_layers;

		anim_state * fake_anim_state = nullptr;
		std::array< float, 24 > m_fake_poses;
		std::array< animationlayer, 13 > m_fake_layers;
		float proper_abs_yaw = 0.f;
		float fake_abs_yaw = 0.f;
		bool init = false;
	};
	struct anim_info {
		

		vec3_t perdicted_velocity = vec3_t ( );
		vec3_t predicted_origin = vec3_t ( );
		vec3_t simulated_origin = vec3_t ( );
		vec3_t interpolated_origin = vec3_t ( );
		float time_delta = 0.f;
		struct backup_data {
			anim_state state;
			std::array<animationlayer, 13> layers;
			std::array<float, 24U> poses;
		};  backup_data backup;
	   

		struct network_data {
			vec3_t angle = vec3_t ( );
			vec3_t origin = vec3_t ( );
			vec3_t velocity = vec3_t ( );
		};  network_data network;

	
		bool network_update = false;
		bool outdated_anims = false;
	
		bool update_anims = false;
		bool update_bones = false;

	
	};
	
	inline anim_data m_data;
	void update_animations_local ( );

	void update_anim_angle ( anim_state * state, vec3_t ang );
	void predict_velocity ( player_t * player );
	void post_data_end ( player_t * player );
	void update_player_animation ( player_t * player );
	void update_globals ( player_t * player, float time );
	void restore ( );
	void backup ( );

	inline std::array<anim_info, 64> player_data;


	struct global_data {
		float cur_time = 0.f;
		float real_time = 0.f;
		float frame_time = 0.f;
		float abs_frametime = 0.f;
		float absolute_frame_start_time = 0.f;
		float interpolation_amount = 0.f;

		int tickcount = 0;
		int framecount = 0;

	};

	inline global_data globals;
}
namespace shifting {
	void create_move( c_usercmd* cmd  );
	struct shift_data {
		bool can_shift = false;
		bool did_last_tick_shoot = false;

		bool shift_this_tick = false;
		int shift_ticks = 0;
		int  next_tickbase_shift = 0;
		int shift_target = 0;
		int recharge_ticks = 16;
		bool did_shift_before = false;
		bool allow = false;
		bool was_zero_before = false;
		int original_tickbase = 0;
		int predicted_tickbase = 0;
		int last_shift_tick = 0;
		int backup_tickbase = 0;
		float backup_curtime = 0.f;
		bool recharging = false;
		int shifted_command = 0;
		int recharge_command = 0;
		int wait_for_recharge = 0;
		int current_shift = 0;
		bool can_recharge = true;
		bool finished_shift_last_tick = false;
		c_usercmd * shift_cmd = nullptr;
	};
	inline shift_data _shift;
}

enum Prediction_stage {
	SETUP,
	PREDICT,
	FINISH
};

class engineprediction : public singleton <engineprediction> {
	struct Netvars_data {
		int tickbase = INT_MIN;

		vec3_t m_aimPunchAngle = vec3_t(0,0,0);
		vec3_t m_aimPunchAngleVel = vec3_t ( 0, 0, 0 );
		vec3_t m_viewPunchAngle = vec3_t ( 0, 0, 0 );
		vec3_t m_vecViewOffset = vec3_t ( 0, 0, 0 );
	};

	struct Backup_data {
		int flags = 0;
		vec3_t velocity = vec3_t ( 0, 0, 0 );
	};

	struct Prediction_data {
		void reset ( ) {
			prediction_stage = SETUP;
			old_curtime = 0.0f;
			old_frametime = 0.0f;
		}

		Prediction_stage prediction_stage = SETUP;
		float old_curtime = 0.0f;
		float old_frametime = 0.0f;
		int * prediction_random_seed = nullptr;
		int * prediction_player = nullptr;
	};

	struct Viewmodel_data {
		weapon_t * weapon = nullptr;

		int viewmodel_index = 0;
		int sequence = 0;
		int animation_parity = 0;

		float cycle = 0.0f;
		float animation_time = 0.0f;
	};
public:
	Netvars_data netvars_data [ 150 ];
	Backup_data backup_data;
	Prediction_data prediction_data;
	Viewmodel_data viewmodel_data;

	void store_netvars ( );
	void restore_netvars ( );
	void setup ( );
	void predict ( c_usercmd * m_pcmd );
	void finish ( );
};
namespace autowall {
	struct FireBulletData_t
	{
		vec3_t			vecPosition;
		vec3_t			vecDir;
		trace_t			enterTrace;
		trace_filter	filter;

		float			flCurrentDamage;
		int				iPenetrateCount;

		bool failed_penetrate = false;
		int iPenetrateCountLimit = 4;
		int penetrationTry = 0;
		player_t * target = nullptr;
	};
	int get_damage( player_t* from, const vec3_t& start, const vec3_t & end, FireBulletData_t & data );
	
	void ScaleDamage( int iHitGroup, player_t* pEntity, float flWeaponArmorRatio, float& flDamage );
	void ClipTraceToPlayers( const vec3_t& vecAbsStart, const vec3_t& vecAbsEnd, unsigned int fMask, trace_filter* pFilter, trace_t* pTrace );
	bool IsBreakableEntity( player_t* pEntity );
	bool trace_to_exit( vec3_t start, vec3_t dir, vec3_t& end, trace_t& trEnter, trace_t& trExit, float flStepSize, float flMaxDistance );
	bool TraceToExit( trace_t* pEnterTrace, trace_t* pExitTrace, vec3_t vecPosition, vec3_t vecDirection );
	bool HandleBulletPenetration( player_t* pLocal, surfacedata_t* pEnterSurfaceData, weapon_info_t* pWeaponData, FireBulletData_t& data );
	bool SimulateFireBullet( player_t* pLocal, weapon_t* pWeapon, FireBulletData_t& data );
	void TraceLine( const vec3_t& vecAbsStart, const vec3_t& vecAbsEnd, unsigned int fMask, player_t* pSkip, trace_t* pTrace );

	float get_thickness( vec3_t& start, vec3_t& end );
	int get_peneration_count ( player_t * player, vec3_t from, vec3_t to );
	bool trace_to_exit_short( vec3_t& point, vec3_t& dir, const float step_size, float max_distance );
	int hbp ( weapon_t* weapon,  vec3_t from,  vec3_t to );
	bool can_hit_float_point( const vec3_t& point, const vec3_t& source );
	int get_dmg ( player_t * from, const vec3_t & to );
}
namespace prediction {
	void post_think( player_t* local );
};




namespace misc {
	bool can_fire ( weapon_t * weapon, bool check_revolver );
	namespace movement {
		void slow_walk( c_usercmd* cmd );
		void auto_strafer( c_usercmd* cmd );
		void strafe( c_usercmd* cmd );
		void fake_duck ( c_usercmd * cmd );
		void bunny_hop(c_usercmd* cmd);
		void slide ( c_usercmd * cmd );
		struct directional_strafer {
			float  m_speed;
			float  m_ideal;
			float  m_ideal2;
			vec3_t m_mins;
			vec3_t m_maxs;
			vec3_t m_origin;
			float  m_switch_value = 1.f;
			int    m_strafe_index;
			float  m_old_yaw;
			float  m_circle_yaw;
			bool   m_invert;
			bool m_slow_motion;

		};
		extern directional_strafer m_strafer;

		void fix_move ( c_usercmd * cmd );
		namespace recorder {
			enum recording_state {
			   state_idle,
			   state_playing,
			   state_recording
			};
			struct custom_cmd {
				vec3_t			viewangles;
				vec3_t			pos;
				float			forwardmove;
				float			sidemove;
				float			upmove;
				int				buttons;
				int step;
			};

			struct movement_data {

				std::string map;
				vec3_t start_position;
				vec3_t end_position;
				vec3_t name;
				std::vector< custom_cmd > cmds;


			};

			struct global_data {
				recording_state state;

				int step = 0;
				movement_data current_playing;
				movement_data current_recording;
			};
			extern global_data m_global_data;

			void recording ( c_usercmd * cmd );
			void playing ( c_usercmd * cmd );
			void run ( c_usercmd * cmd );
			void apply_cmd ( c_usercmd * cmd, custom_cmd & cmd_copy );
			void read_cmd ( c_usercmd * cmd, custom_cmd & cmd_copy );
			void handle_input ( );
		}
		
	};
	namespace removals {
		void remove_smoke( );
		void remove_flash( );
	}
	namespace thirdperson {
		void pre_framestagenotify( );
		void think();
	}
	namespace hitmarker {
		void think( );
		void event( i_game_event* event );
	};

	namespace event_logs {
		void player_hurt( i_game_event* event );
		void item_purchase( i_game_event* event );
	};
	namespace render {
		void setup_render( );

	}
}



namespace legit_bot {
	struct data_t {
		player_t * current_target = nullptr;
		vec3_t aimbot_angle;
		int hitbox;
		vec3_t hitbox_position;
		vec3_t hitbox_angle;
		vec3_t target_angle;
		
		vec3_t current_angle;
		vec3_t random_hitbox_position;
		vec3_t random_angle;
		bool shoot_before = false;
		bool hit_random_angle = false;
		float random_x = 0.f;
		int last_shot_tickcount = 0;
		float random_y = 0.f;
		vec3_t last_punch = vec3_t ( );
		bool attack_last_tick = false;

	};
	extern data_t m_data;
	player_t * get_closest_target ( );
	void run ( c_usercmd * cmd );
	void validate_target ( );
	void linear_smooth ( );
	void calculate_angle ( );
	void apply_rcs ( );
	void calculate_hitbox ( );
	void apply_angle ( c_usercmd * cmd );
	namespace  trigger_bot{
		std::pair < player_manager::lagcomp_t, bool > should_attack ( player_t * player );
		void run ( c_usercmd * cmd );
	}
}
namespace aimbot {
	namespace autostop {
		struct auto_data {
			player_t * target = nullptr;
			int ticks_to_stop = 0;
			bool slow_walk_this_tick = false;
			int failed_hitchance = 0;
			int target_hitchance = 999;
			vec3_t last_scanned_point = vec3_t ( );
			vec3_t futute_shot_position = vec3_t ( );
		};

		void minimal_stop ( c_usercmd * cmd );
		void early_stop ( c_usercmd * cmd );
		void run ( c_usercmd * cmd );
		void between_shots ( c_usercmd * cmd );
		void slow_walk ( c_usercmd * cmd, bool override = false );
		extern auto_data m_data;
	}
	struct seed {
		float a, b, c, d;
	};

	struct collision {
		vec3_t mins = vec3_t(), maxs = vec3_t();
		float radius = 0.f;
	};
	struct bestpoint {
		vec3_t position = vec3_t ( );
		vec3_t center = vec3_t ( );
		int dmg = 0;
		int hitbox = 0;
		collision col = {};
		int bone = 0;
		bool should_trace = false;
		bool finished_trace = false;
		bool passed_hitchance = false;
	};
	struct point {
		vec3_t pos = vec3_t();
		bool safe = false;
	};

	struct multipoint {
		std::vector<point> points = {};
		vec3_t center = vec3_t();
	};

	struct aim_data {
		bestpoint best_point = {};
		player_manager::lagcomp_t record;
		vec3_t angle = vec3_t();
	};
	struct target {
		player_t * player = nullptr;
		int index = -1;
		int health = 0;
		studio_hitbox_set_t * hitbox_set = nullptr;
		aim_data aimbot;
		bool is_current_record_hidden = false;
		bool should_shot = false;
		bool should_stop = false;
	};

	struct visual_debug {
		vec3_t from, end;
		vec3_t aimbot_hit, aimbot_hit_2d;
		vec3_t from_2d, end_2d;
		std::vector<point> points;
	};

	extern visual_debug m_visual_debug;
//	extern std::vector<seed> seed_list;
	bestpoint best_point ( target & entity, player_manager::lagcomp_t & record );
	bool hitchance ( target & entity );
	float get_innacuracy_level ( );
	void select_targets ( );
	void sort_list ( );
	void update_targets ( );
	void populate_list ( );
	void  multi_point ( target & entity, const int & hit_box, player_manager::lagcomp_t & record, multipoint & points, bool should_disable = false );
	void scan ( target & entity );
	extern std::vector<target> targets;
	extern std::vector<int> hitscan_list;
	std::optional<vec3_t> get_intersect_point ( vec3_t start, vec3_t end, vec3_t mins, vec3_t maxs, float radius );

	void populate_hitscan ( );
	bool does_point_intersect ( target & entity, vec3_t point, int hitbox, matrix3x4_t bones [ 128 ] );
	void render ( );
	extern target best_target;
	extern target last_target;
	void create_move ( c_usercmd * cmd, bool & send_packet );
	void cm ( c_usercmd * cmd, bool & send_packet );

}
namespace autothrow {
	void create_move ( c_usercmd * cmd );

}
namespace visuals {

	



}

namespace shot_processor {
	struct shot_data {
		vec3_t shotpos;
		vec3_t hitpos;
		float curtime;
		bool hurt = false;
		bool hit = false;
		int tick = 0;
		int enemy_index = 0;
		bool approved = false;
		bool approved_bullet = false;
		bool approved_hit = false;
		struct {
			int victim = -1;
			int damage = -1;
			int hitgroup = -1;
		}hit_info;
		aimbot::target target;

	};
	extern std::deque<shot_processor::shot_data> shots;
	void hurt_event ( i_game_event * event );
	void weapon_fire ( i_game_event * event );
	void bullet_impact ( i_game_event * event );


	void add_shot ( const vec3_t shotpos, aimbot::target target );
	shot_data * closest_shot ( int tickcount );
	const char * hitbox_text ( hitboxes hitbox );
	void manage_shots ( );
	void draw_shots ( );
	void update_missed_shots ( const client_frame_stage_t & stage );
};