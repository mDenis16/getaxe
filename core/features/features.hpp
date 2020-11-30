#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "../menu/variables.hpp"
#include "misc/events/events.hpp"
#include "../menu/ImGui/imgui.h"
#include "../menu/ImGui/imgui_internal.h"
#include <optional>

static int hitmarker_alpha = 0;

namespace event_manager {
	void round_prestart ( i_game_event * event );
	void round_end ( i_game_event * event );
}
namespace resolver {
	enum desync_type
	{
		none,
		balance,
		jitter,
		antibrute
	};
	namespace event_logs {
		void bullet_impact ( i_game_event * event );
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
		float server_goal_feet = 0.f;
		int desync_side = false;
		bool resolved = false;
		bool will_peek = false;
		float real_head = 0.f;
		int missed_shots = 0;
		bool is_using_desync = false;
		bool is_usync_max_desync = false;
		bool extended_desync = false;
		float last_desync_detection;
		float last_stopped_moving_goal_feet;
		float final_goal_feet;
		int choke_ticks = 0;
		std::vector<float> last_hit_angle_related_to_me;
		std::vector<float> last_feet_miss;
		float last_reset_time;
		float predicted_delta = 0.f;
		float goal_feet_yaw;
		std::string resolve_type;
		int side = 0;
		desync_type type;
		int safe_points = 0;
		int brute_side = 0;
		float last_side_change = 0.f;
		int safety = 0;
		float safe_point_angle = 0.f;
		float brute_angle = 0.f;
		float first_shoot_correction = 0.f;
		float last_networked_time;

	};
	extern std::deque<resolve_shot> shots;
	extern resolve_info resolver_data [ 65 ];
	player_t * get_closest_player_by_point ( vec3_t from, vec3_t point );
	resolver::resolve_shot * closest_shot ( int tickcount, player_t* player );
	std::string side_name ( int side );
	int get_desync_side ( vec3_t from, vec3_t to, player_t * entity, int hitbox );
	bool get_desync_side( player_t* entity );
	float server_feet_yaw ( player_t * entity, vec3_t angle );

	float max_desync_delta( player_t* entity );
	bool is_player_peeking( player_t* player );
	void create_move( c_usercmd* cmd );
	float get_safe_point_angle ( int i );

	float get_real_head( player_t* entity );
	void manage_shots ( );
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
	void initialize( player_t* player, c_usercmd* cmd );
	void unload( player_t* player );
}
namespace player_manager {

	struct lagcomp_t
	{
		bool shoot = false, moving = false, slow_walking = false, lby_update = false;
		int flags;

		float simtime = 0.f, duckamount, curtime, lby;
		std::array< float, 24 > pose_params;
		vec3_t absorigin, origin, eyeangles, absangles, obbmin, obbmax, velocity;
		player_t* entity;
		matrix_t bone [ 128 ];
		int bone_count = 0;
		matrix_t bone_left [ 128 ];
		matrix_t bone_right [ 128 ];
		anim_state state;
		resolver::resolve_info resolver;
		animationlayer anim_layer [ 15 ];
		float speed = 0.f;
	};

	player_t* util_player_by_index( int index );


	void update_animations( player_t* entity );

	void manage_bones( );

	void setup_players( void );

	void recieve_record( player_t* entity, lagcomp_t& record );

	namespace event_logs {
		void bullet_impact( i_game_event* event );
	};

	extern lagcomp_t best_tick_global [ 65 ];
	float interpolation_time( );
	float get_lerp_time( );
	extern std::deque< lagcomp_t > records [ 64 ];
	extern 	lagcomp_t backup_data [ 64 ];

	bool is_tick_valid( lagcomp_t record );


	void get_rotated_matrix( lagcomp_t record, float angle, matrix_t new_matrix [ MAXSTUDIOBONES ] );

	void setup_records( );

	void backup_player( player_t* entity );

	void restore_record( player_t* entity, lagcomp_t record );

	void restore_player( player_t* entity );

}
namespace anti_aim {
	struct angle_data {
		float angle;
		float thickness;
		float curtime;
		float number;
		angle_data( const float angle, const float thickness, const float curtime, const int number ) : angle( angle ), thickness( thickness ), curtime( curtime ), number( number ) {}
	};

	namespace event_logs {
		void bullet_impact( i_game_event* event );
	};

	struct freestand_point
	{
		vec3_t end;
		vec3_t start;
		float angle;
		float tickness;
		bool selected = false;
		bool original = false;
		
	};

	void real_animation_fix( );
	void update_local_animations( );

	float get_freestanding_angle( );

	bool allow( c_usercmd* ucmd, bool& send_packet );
	void freestanding_desync( c_usercmd* cmd, float& dirrection, player_t* p_entity, bool& send_packet, float max_desync );
	int get_desync_dirrection( );
	void on_create_move( c_usercmd* cmd, bool& send_packet );

	void fix_call_animation( );
	extern std::vector<freestand_point> points;
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
namespace autostop {
	vec3_t calculate_next_stop( );
	vec3_t calculate_next_stop( player_t* entity );
	
	
	void clamp_movement_speed( c_usercmd* cmd, float speed );

	void slow_walk( c_usercmd* cmd, bool override );
	struct slow_data {
		bool called_this_tick;

	};
	extern slow_data m_slow_data;

	void stop_movement( c_usercmd* cmd );

	void rotate_movement( c_usercmd* cmd, float yaw );

	void Strafe( );

	void JumpRelated( );
	struct autostop_data {
		vec3_t from;
		vec3_t to;
		vec3_t cur_origin;
		vec3_t eye_pos;
		vec3_t last_from;
		int ticks = 0;
		int ticks_blocked = 0;
		int state = 0;
		player_t* target;
		bool standby = true;
		int damage_scan = 0;
	};
	extern autostop_data m_autostop_data;
	int ticks_to_stop( vec3_t velocity );
	void autostop( c_usercmd* cmd );
	void auto_slow_down( c_usercmd* cmd );
	void auto_peek( c_usercmd* cmd );
	struct draw_point {
		std::string name;
		vec3_t pos;
	};
	extern std::vector<draw_point> points_to_draw;
}
namespace fake_lag {
	extern vec3_t peek_position;
	void on_peek( c_usercmd* cmd, bool& send_packet );
	void create_move( c_usercmd* cmd, bool& send_packet );
}
namespace accuracy {
	struct auto_stop {
		bool searching_spot;
		int last_time;
		vec3_t shot_spot;
		vec3_t shot_origin;
		
	};
	extern auto_stop m_data;
	void create_move ( c_usercmd* cmd );
}
namespace animations {
	struct anim_data {
		// animation variables.
		vec3_t m_rotation;
		float            m_anim_time;
		float            m_anim_frame;
		float            m_real_poses [ 24 ];
		vec3_t fake_rotation;
		animationlayer real_layers [ 15 ];
		animationlayer m_real_layers [ 13 ];
		float min_delta = 0.f, max_delta = 0.f,
			stop_to_full_running_fraction = 0.f,
			feet_speed_stand = 0.f, feet_speed_ducked = 0.f;
		bool is_standing = false;
		animationlayer m_layers [ 13 ];
		bool m_ground = false;
		float m_speed = 0.f;
		bool should_update = false;
		bool m_animate = false;

		bool should_setup_bones = false;
		bool should_update_client_side_animations = false;
		int tick_count = 0;
		float m_poses [ 24 ];
	};
	extern anim_data m_data;
	void update_animations_local ( );
	void update_animations ( );
	void update_anim_angle ( anim_state * state, vec3_t ang );
}
namespace aimbot {
	
	struct collision {
		vec3_t mins, maxs;
		float radius;
	};
	struct bestpoint {
		vec3_t position = vec3_t ( );
		vec3_t center = vec3_t ( );
		float dmg = 0.f;
		int hitbox = 0;
		collision col;
	};
	struct point {
		vec3_t pos;
		bool safe;
	};
	
	struct multipoint {
		std::vector<point> points;
		vec3_t center;
	};

	struct aim_data {
		bestpoint best_point;
		player_manager::lagcomp_t record;
		vec3_t angle;
	};
	struct target {
		player_t * player = nullptr;
		int index = -1;
		float health = 0.f;
		studio_hitbox_set_t * hitbox_set;
		aim_data aimbot;
	};
	struct visual_debug {
		vec3_t from, end;
		vec3_t aimbot_hit, aimbot_hit_2d;
		vec3_t from_2d, end_2d;
	};
	extern visual_debug m_visual_debug;
	extern target  best_target;
	bestpoint best_point ( target & entity, player_manager::lagcomp_t & record );
	bool hitchance ( target & entity );
	void select_targets ( );
	void sort_list ( );
	void update_targets ( );
	void populate_list ( );
	void  multi_point ( target & entity, const int & hit_box, player_manager::lagcomp_t & record, multipoint & points, bool should_disable = false );
	void scan ( target & entity );
	extern std::vector<target> targets;
	extern std::vector<int> hitscan_list;
	std::optional<vec3_t> get_intersect_point ( vec3_t start, vec3_t end, vec3_t mins, vec3_t maxs, float radius );
	void create_move ( c_usercmd * cmd );
	void populate_hitscan ( );
	bool does_point_intersect ( target & entity, vec3_t point, int hitbox, matrix_t bones[128] );
	void render ( );
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
	};
	float GetDamage( player_t* pLocal, const vec3_t& vecPoint, FireBulletData_t& dataOut );
	float GetDamage( player_t* pLocal, const vec3_t& vecStart, const vec3_t& vecPoint, FireBulletData_t& dataOut );
	void ScaleDamage( int iHitGroup, player_t* pEntity, float flWeaponArmorRatio, float& flDamage );
	void ClipTraceToPlayers( const vec3_t& vecAbsStart, const vec3_t& vecAbsEnd, unsigned int fMask, trace_filter* pFilter, trace_t* pTrace );
	bool IsBreakableEntity( player_t* pEntity );
	bool trace_to_exit( vec3_t start, vec3_t dir, vec3_t& end, trace_t& trEnter, trace_t& trExit, float flStepSize, float flMaxDistance );
	bool TraceToExit( trace_t* pEnterTrace, trace_t* pExitTrace, vec3_t vecPosition, vec3_t vecDirection );
	bool HandleBulletPenetration( player_t* pLocal, surfacedata_t* pEnterSurfaceData, weapon_info_t* pWeaponData, FireBulletData_t& data );
	bool SimulateFireBullet( player_t* pLocal, weapon_t* pWeapon, FireBulletData_t& data );
	void TraceLine( const vec3_t& vecAbsStart, const vec3_t& vecAbsEnd, unsigned int fMask, player_t* pSkip, trace_t* pTrace );

	float get_thickness( vec3_t& start, vec3_t& end );

	bool trace_to_exit_short( vec3_t& point, vec3_t& dir, const float step_size, float max_distance );

	bool can_hit_float_point( const vec3_t& point, const vec3_t& source );

}
namespace prediction {
	void post_think( player_t* local );
};
namespace shot_processor {
	struct shot_data {
		vec3_t shotpos;
		vec3_t hitpos;
		float curtime;
		bool hurt = false;
		bool hit = false;
		bool approved = false;
		struct
		{
			int victim = -1;
			int damage = -1;
			int hitgroup = -1;
		}hit_info;
		hitboxes hitbox;
		int tick;
		int enemy_index;
		vec3_t mins;
		vec3_t maxs;
		float radius;
		matrix_t matrix [ 128 ];

	};
	extern std::deque<shot_processor::shot_data> shots;
	void hurt_event( i_game_event* event );
	void weapon_fire( i_game_event* event );
	void bullet_impact( i_game_event* event );

	
	void add_shot ( const vec3_t shotpos, const int & entity_index, hitboxes hitbox, matrix_t matrix [ 128 ], vec3_t mins, vec3_t maxs, float radius );
	shot_data* closest_shot( int tickcount );
	void manage_shots( );
	void update_missed_shots( const client_frame_stage_t& stage );
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
		int m_next_shift_amount;
		int m_ticks_to_shift;
		int m_current_shift;
		int m_stored_tickbase;
		int m_ticks_shifted_last;
		int interval_per_tick;
		bool m_did_shift_before;
		bool m_should_attempt_shift;
		int m_needs_recharge;
		bool m_prepare_recharge;
		bool m_should_be_ready;
		bool m_can_shift_tickbase;
		bool m_should_disable;
		bool m_should_recharge_peek;
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
	void write_user_cmd( bf_write* buf, c_usercmd* incmd, c_usercmd* outcmd );

	void pre_movement( );
	void post_movement( );
}

namespace visuals {

	struct data {
		float width, height;
	};
	struct local_data {
		bool scoped = false;
	};
	extern local_data m_local;
	void draw_debug_points ( );

	float draw_text ( ImFont * pFont, const std::string text, const ImVec2 & pos, float size, float const * color, float shadow, bool center, bool bold );
	
	void draw_noscope ( );
	void local_esp_think ( );
	
	void local_esp ( );
	void draw_projectile ( );
	extern data m_data;
	struct box {
		int x, y, w, h;
		box( ) = default;
		box( int x, int y, int w, int h ) {
			this->x = x;
			this->y = y;
			this->w = w;
			this->h = h;
		}
	};
	bool get_playerbox ( entity_t * ent, visuals::box & in );
	namespace projectiles {
		class proj_data {
		public:
			visuals::box box_data;
			float distance = 0.f;
			std::string owner_name;
			std::string name;
			bool on_screen = false;
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
					if ( !entity )
						return "weapon";
					if ( strstr ( entity->client_class ( )->network_name, ( "CWeapon" ) ) ) {
						std::string data = strstr ( entity->client_class ( )->network_name, ( "CWeapon" ) );
						std::transform ( data.begin ( ), data.end ( ), data.begin ( ), ::tolower ); //convert dropped weapons names to lowercase, looks cleaner - designer
						return data;
					}
					if ( !entity->client_class ( ) )
						return "weapon";
					auto class_id = entity->client_class ( )->class_id;
					switch ( class_id ) {
					case cc4:
						return "c4";
						break;
					case cdeagle:
						return "deagle";
						break;
					case cak47:
						return "ak47";
						break;
					default:
						break;
					}
					auto model_name = interfaces::model_info->get_model_name ( entity->model ( ) );
					if ( strstr ( model_name, "w_defuser" ) )
						return "defuse kit";

				}
				bool is_weapon ( ) {
					return entity->is_weapon ( );
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
					if ( !entity )
						return "weapon";
					if ( strstr ( entity->client_class ( )->network_name, ( "CWeapon" ) ) ) {
						std::string data = strstr ( entity->client_class ( )->network_name, ( "CWeapon" ) );
						std::transform ( data.begin ( ), data.end ( ), data.begin ( ), ::tolower ); //convert dropped weapons names to lowercase, looks cleaner - designer
						return data;
					}
					if ( !entity->client_class ( ) )
						return "weapon";
					auto class_id = entity->client_class ( )->class_id;
					switch ( class_id ) {
					case csmokegrenadeprojectile:
						return "c4";
						break;
					case cdeagle:
						return "deagle";
						break;
					case cak47:
						return "ak47";
						break;
					default:
						break;
					}
					auto model_name = interfaces::model_info->get_model_name ( entity->model ( ) );
					if ( strstr ( model_name, "w_defuser" ) )
						return "defuse kit";

				}
				std::string name;
				grenade_type get_type ( ) {
					if ( name.find ( "flashbang" ) != std::string::npos ) {
						return grenade_type::flashbang;
					}

					else if ( name.find ( "smokegrenade" ) != std::string::npos ) {
						return grenade_type::smokegrenade;
					}

					else if ( name.find ( "incendiarygrenade" ) != std::string::npos ) {
						return grenade_type::incendiarygrenade;
					}

					else if ( name.find ( "molotov" ) != std::string::npos ) {
						return grenade_type::molotov;
					}

					else if ( name.find ( "fraggrenade" ) != std::string::npos ) {
						return grenade_type::fraggrenade;
					}

					else if ( name.find ( "decoy" ) != std::string::npos ) {
						return grenade_type::decoy;
					}
				}
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
		}
		void think ( );
		void draw ( );
	}
	namespace notifications {
		struct notify {
			std::string text;
			float time;
			float shown_time;
			float width =  300.f;
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

		extern  std::array<visuals::player::data, 65> m_data;

		void player_death ( i_game_event * event );

		void name( data _data );

		void box( data _data);

		void health( visuals::player::data _data );

		void weapon( visuals::player::data _data );

		void arrow ( visuals::player::data _data );

		void present( );

		void sound ( );

		void paint_traverse( void );

		void aimbot ( visuals::player::data _data );
	
		void think ( );
	};
	
	namespace chams {
		enum class shader_type_t {
			VertexLitGeneric = 0,
			UnlitGeneric,
			Modulate
		};
		i_material* create_material( shader_type_t shade, bool ignorez, bool wireframe );
		void think ( );
		void run( i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* bone_to_world );
	};
	

	

	
	std::string weapon_to_icon( const int id );

}

namespace misc {
	namespace movement {
		void slow_walk( c_usercmd* cmd );
		void auto_strafer( c_usercmd* cmd );
		void strafe( c_usercmd* cmd );
		void bunny_hop(c_usercmd* cmd);
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

		void directional_movement ( c_usercmd * cmd );
		void fix_mvoe ( c_usercmd * cmd, vec3_t wish_angles );
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
		void think( );
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
}