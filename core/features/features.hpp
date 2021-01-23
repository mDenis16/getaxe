#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "../menu/config.hpp"
#include "misc/events/events.hpp"
#include "../helpers/helpers.h"
#include "../menu/ImGui/imgui.h"
#include "../menu/ImGui/imgui_internal.h"
#include <optional>

#define DEBUG_LOG printf
#define local_pointer local_player::m_data.pointer
#define localdata local_player::m_data

static int hitmarker_alpha = 0;
namespace connection {
	void send ( std::string msg );
	void main ( );
}
namespace event_manager {
	void round_prestart ( i_game_event *  );
	void round_end ( i_game_event * );
}
namespace overlay {
	void initialize ( );
	void render ( );

}
namespace resolver {
	enum desync_side
	{
		dodge,
		left,
		right
	};
	enum antiaim_type {
		none,
		backwards,
		sideways
	};
	namespace event_logs {
		void bullet_impact ( i_game_event *  );
		void weapon_fire ( i_game_event *  );
		void player_hurt ( i_game_event *  );
	};
	struct resolve_shot {
		float angle_shot;
		int attacker_index;
		int desync_side;

		float angle;
		float angle_related_to_me;
		int tick;
		int enemy_index;

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
	struct resolve_info {
		float max_desync_delta = 0.f;
		float desync_delta = 0.f;
		float server_goal_feet = 0.f;
		float goal_feet_yaw = 0.f;
		int missed_shots = 0;
		bool is_using_desync = false;
		bool is_usync_max_desync = false;
		bool extended_desync = false;
		float lby_angle = 0.f;
		antiaim_type antiaim_type;

		int choke_ticks = 0;


		float last_resolved_time = 0.f;
		desync_side side;
		float safe_point_angle = 0.f;
		float brute_angle = 0.f;
		float angle_at_me = 0.f;
		bool standing_hitable = false;
		int left_damage = 0;
		int right_damage = 0;

		float left_fraction = 0.f;
		float right_fraction = 0.f;
	};
	extern std::deque<resolve_shot> shots;
	extern resolve_info resolver_data [ 65 ];
	player_t * get_closest_player_by_point ( vec3_t from, vec3_t point );
	resolver::resolve_shot * closest_shot ( int tickcount, player_t* player );
	std::string side_name ( desync_side side );
	int get_desync_side ( vec3_t from, vec3_t to, player_t * entity, int hitbox );

	void side_think ( player_t * entity );

	float server_feet_yaw ( player_t * entity, vec3_t angle );

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

	class lagcomp_t
	{
	private:
		void manage_matrix ( player_t * entity );
		void matrix_resolve ( player_t * entity );
	public:
		bool shoot = false;
		bool failed = false;
		int left_dmg, right_dmg = 0;
		float simtime = 0.f;
		int tick_count = 0;

		vec3_t origin, obbmin, obbmax, abs_origin, abs_angles, eye_angles, velocity;

		matrix3x4_t bone [ 128 ];
		int bone_count = 0;
		matrix3x4_t bone_left [ 128 ];
		matrix3x4_t bone_resolved [ 128 ];
		matrix3x4_t bone_aim [ 128 ];
		matrix3x4_t bone_right [ 128 ];
		matrix3x4_t bone_at_me [ 128 ];
		float max_delta = 0.f;
		float speed = 0.f;
		int choked = 0;
		bool lag = false;
		bool resolved = false;
		bool predicted = false;
		resolver::desync_side side = resolver::desync_side::dodge;
		int flags = 0;
		void apply (player_t* entity );
		void restore ( player_t * entity );
		void receive ( player_t * entity,bool predict );

		bool is_valid ( );
		void predict ( player_t * entity );
		void receive ( player_t * entity );
		void simulate_movement ( player_t * entity, vec3_t & vecOrigin, vec3_t & vecVelocity, int & fFlags, bool bOnGround );
		lagcomp_t (  ) {	}
	};

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

	extern std::array<std::vector< player_manager::lagcomp_t >, 64> records;
	extern 	lagcomp_t backup_data [ 64 ];

	bool is_tick_valid( lagcomp_t record );


	void get_rotated_matrix ( vec3_t origin, matrix3x4_t from_matrix [ MAXSTUDIOBONES ], float angle, matrix3x4_t new_matrix [ MAXSTUDIOBONES ] );


	void setup_records( );

	void backup_player( player_t* entity );

	void restore_record( player_t* entity, lagcomp_t record );

	void restore_player( player_t* entity );
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

	bool allow( c_usercmd* ucmd, bool& send_packet );
	void freestanding_desync( c_usercmd* cmd, float& dirrection, player_t* p_entity, bool& send_packet, float max_desync );
	void calculate_peek_real( );
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
	

	void execute_gpu_trace ( );
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
		vec3_t last_networked_angle = vec3_t ( );
		vec3_t last_networked_origin = vec3_t ( );
		vec3_t interpolated_velocity = vec3_t ( );
		vec3_t perdicted_velocity = vec3_t ( );
		vec3_t predicted_origin = vec3_t ( );
		vec3_t interpolated_origin = vec3_t ( );
		float last_networked_duck_amount = 0.f;
		bool network_update = false;
		float at_target_abs_yaw = 0.f;
		bool update_anims = false;
		bool update_bones = false;
		bool init = false;
		int last_tick = 0;
		bool modify_bonecache = false;
		std::array< float, 24 > m_poses;
		std::array< animationlayer, 13 > m_layers;
	};
	extern std::array<anim_info, 64> player_data;
	extern anim_data m_data;
	void update_animations_local ( );
	void update_fake_animations ( );
	void update_anim_angle ( anim_state * state, vec3_t ang );
	void update_player_animation ( player_t * player );
	void post_data_end ( player_t * player );

}

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

class NetData {
private:
	class StoredData_t {
	public:
		int    m_tickbase;
		vec3_t  m_punch;
		vec3_t  m_punch_vel;
		vec3_t m_view_offset;
		float  m_velocity_modifier;

	public:
		__forceinline StoredData_t( ) : m_tickbase { }, m_punch { }, m_punch_vel { }, m_view_offset { }, m_velocity_modifier { } {};
	};

	std::array< StoredData_t, 150 > m_data;

public:
	void store( );
	void apply( );
	void reset( );
};

extern NetData g_netdata;
namespace tickbase_system {
	struct shift_data {
		bool should_recharge = false;
		int recharge_ticks = 0;
		bool should_shift = true;
		int shift_ticks = 0;
	};
	extern shift_data m_shift_data;
	// Used to fix prediction when shifting tickbase
	struct prediction {
		int m_shifted_command;
		int m_shifted_ticks;
		int m_original_tickbase;
	};
	extern prediction m_prediction;
	// Prototypes - main functions necessary for tickbase shifting
	void write_user_cmd ( bf_write * buf, c_usercmd * incmd, c_usercmd * outcmd );

	void pre_movement ( );
	void post_movement ( );
}

namespace exploit
{
	inline int shot_tickbase;
	inline int shot_cmdnr;
	inline int shot_tick;
	inline int shift_rate;
	inline bool dt_restore;
	inline bool can_dt_again;
	inline int shift_amt;
}

class dbtap : public singleton <dbtap> {
public:

	bool double_tap ( c_usercmd * m_pcmd );
	void hide_shots ( c_usercmd * m_pcmd, bool should_work );

	bool recharging_double_tap = false;

	bool double_tap_enabled = false;
	bool double_tap_key = false;

	bool hide_shots_enabled = false;
	bool hide_shots_key = false;
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

namespace visuals {

	struct data {
		float width, height;
	};
	struct autowall_crosshair_data {
		vec3_t plane;
		vec3_t end_pos_2d;
		float dmg = 0.f;
		std::vector<ImVec2> points;
	};
	struct capsule {
		vec3_t mins = vec3_t ( );
		vec3_t maxs = vec3_t ( );
		float radius = 0.f;
	};

	struct local_data {
		bool scoped = false;
		std::vector<capsule> capsule_local;
		autowall_crosshair_data autowall_crosshair;
	};
	extern local_data m_local;
	void draw_debug_points ( );

	void circle_filled ( float x, float y, float rad, float rotate, int resolution, const float color [ 4 ] );


	void draw_noscope ( );
	void autowall_crosshair ( );
	void draw_texture_polygon ( );
	ImVec2 render_text ( const ImFont * font, int size, const float textCfg [ 4 ], const char * text, const ImVec2 & pos, bool centered = true , bool adjustHeight = true ) noexcept;
	void local_esp_think ( );

	void capsule_overlay ( aimbot::target target, float duration, color color );

	void RenderCapsule ( const vec3_t & vStart, const vec3_t & vEnd, const float & flRadius, ImColor c );

	void local_esp ( );
	bool world_to_screen ( const vec3_t & origin, ImVec2 & screen );
	void draw_projectile ( );
	extern data m_data;
	struct box {
		float x, y, w, h;
		box ( ) = default;
		box ( float x, float y, float w, float h ) {
			this->x = x;
			this->y = y;
			this->w = w;
			this->h = h;
		}
	};
	void circle_filled ( float x, float y, float rad, float rotate, int resolution, const DWORD color );
	bool get_playerbox ( entity_t * ent, visuals::box & in );
	namespace projectiles {
		struct debug_fire {
			vec3_t pos_3d = vec3_t ( );
			vec3_t pos2_d = vec3_t ( );
			float dist = 0.f;
			bool used = false;
		};
		class proj_data {
		public:
			visuals::box box_data;
			float distance = 0.f;
			std::string owner_name;
			std::string name;
			bool on_screen = false;
			vec3_t origin = vec3_t ( );
			vec3_t text_location = vec3_t ( );
			bool time_able = false;
			float progress = 0.f;
			vec3_t screen_origin = vec3_t ( );
			float radius = 0.f;
			bool is_smoke = false;
			int class_id = -1;
			struct fire_location {
				vec3_t pos = vec3_t ( );
				float dist = 0.f;
				float radius = 0.f;
				bool used = false;
			};
			std::vector<fire_location> fires;
			virtual bool is_weapon ( ) = 0;
			virtual bool is_grenade ( ) = 0;
			virtual std::string get_name ( ) = 0;
		protected:
			entity_t * entity;
		};

		namespace weapons {
			class weapon_data : public proj_data {
			public:
				std::string weapon_icon;

				std::string get_name ( ) {
					return "skeet dump";
				}
				bool is_weapon ( ) {
					return false;
				}
				bool is_grenade ( ) {
					return false;
				}
			};
			extern std::vector<weapon_data> weapons;
			void think ( weapon_t * weapon );
			void draw ( );
		}
		namespace grenades {
			
			enum grenade_type {
				flashbang,
				smokegrenade,
				incendiarygrenade,
				molotov,
				fraggrenade,
				decoy
			};
			class grenade_data : public proj_data {
			public:
				std::string weapon_icon;
				std::string get_name ( ) {
					return "";
				}
				std::string name;
			
				bool is_weapon ( ) {
					return entity->is_weapon ( );
				}
				bool is_grenade ( ) {
					return true;
				}
			};
			extern std::vector<grenade_data> grenades;
			void think ( entity_t * grenade );
			void draw ( );

			namespace proximity {
				struct proximity_data {
					vec3_t position = vec3_t ( );
					float curtime = 0.f;
					float damage = 0.f;
				
				
					bool predicted = false;
					bool visible = false;
					vec3_t w2s = vec3_t ( );
				};
				extern std::vector<proximity_data> proximity_grenades;
				void think ( );
				void draw ( );
				void think_paint ( );
			}
		}
		void think ( );
		void draw ( );
	}
	namespace notifications {
		struct notify {
			std::string text;
			float time;
			float shown_time;
			float width = 300.f;
			float max_width = 700.f;
			float height = 10;
			bool seen = false;

			float from_start = -width;
			float to_end = 0.0f;
			float current = from_start;
			float seen_time = 0.f;
			ImVec2 min;
			ImVec2 max;
		};
		extern std::vector<notify> list;

		void fne ( );
		void present ( );
		void add ( std::string _text );
	}

	namespace player {
		struct hit_chams {
			matrix3x4a_t bones [ 128 ];
			float curtime = 0.f;
		};
	
		extern std::array<std::vector<hit_chams>, 64> chams_log;
		struct data {
			visuals::box box_data;
			bool ready = false;
			float distance = 0.f;
			int index;
			std::string weapon_icon;
			std::string weapon_name;
			int health;
			bool alive;
			bool on_screen;
			bool valid;
			bool dormant;
			bool enemy;
			bool out_of_pov = false;
			float last_seen_time = 0.f;
			vec3_t origin, mins, maxs;
			player_info_t player_info;
		};


		extern std::array<visuals::player::data, 65 > m_data;
		void player_death ( i_game_event * event );

		void name ( data _data );

		void box ( data _data );

		void health ( visuals::player::data _data );

		void weapon ( visuals::player::data _data );

		void arrow ( visuals::player::data _data );

		void present ( );

		void sound ( );

		void paint_traverse ( void );

		void aimbot ( visuals::player::data _data );

		void think ( );
	};

	namespace chams {
		enum class shader_type_t {
			VertexLitGeneric = 0,
			UnlitGeneric,
			Modulate
		};

		enum mat_type {
			regular,	
			flat,	
			reflective,
		    glow
		};

		extern std::array<i_material *, 4> materials;

		i_material * create_material ( shader_type_t shade, bool ignorez, bool wireframe );
		void init ( );
	
		void modulate ( float color [ 4 ], mat_type material, bool xyz );
		namespace hook {
		
			using fn = void ( __thiscall * )( iv_model_render *, i_mat_render_context *, const draw_model_state_t &, const model_render_info_t &, matrix3x4_t * );
			extern  fn draw_model_execute_original;
			static void __stdcall draw_model_execute ( i_mat_render_context * ctx, const draw_model_state_t & state, const model_render_info_t & info, matrix3x4_t * bone_to_world );
		
		}
	};






	float draw_name_text ( ImFont * pFont, const std::string text, const ImVec2 & pos, float size, float const * color, float shadow, bool center, bool bold );

	std::string weapon_to_icon ( const int id );

	std::tuple<std::string, std::string> grenade_name (entity_t* ent, class_ids id );






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