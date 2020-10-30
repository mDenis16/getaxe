#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "../menu/variables.hpp"
#include "misc/events/events.hpp"
#include <optional>

static int hitmarker_alpha = 0;

namespace legit_bot {
	int find_target( c_usercmd* user_cmd ) noexcept;
	void think( c_usercmd* cmd );
	player_t* get_target( );

	vec3_t get_closest_bone( player_t* target, vec3_t view );

}
namespace resolver {
	enum desync_type
	{
		none,
		balance,
		jitter
	};
	
	struct resolve_info {
		float max_desync_delta = 0.f;
		float server_goal_feet = 0.f;
		bool desync_side = false;
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
		float last_reset_time;
		std::string resolve_type;
		int side = 0;
		desync_type type;
	};
	extern resolve_info resolver_data [ 65 ];
	bool get_desync_side( player_t* entity );
	float server_feet_yaw( player_t* entity );
	float max_desync_delta( player_t* entity );
	bool is_player_peeking( player_t* player );
	void create_move( c_usercmd* cmd );
	float get_real_head( player_t* entity );
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
	void initialize( player_t* player, c_usercmd* cmd );
	void unload( player_t* player );
}
namespace player_manager {

	struct backtrack_records_t {
		bool shoot, moving, slow_walking, lby_update;
		int flags;

		float simtime, duckamount, curtime, lby;
		std::array< float, 24 > pose_params;
		vec3_t absorigin, origin, eyeangles, absangles, obbmin, obbmax, velocity;
		player_t* entity;
		matrix_t bone [ 128 ];
		matrix_t bone_left [ 128 ];
		matrix_t bone_right [ 128 ];
		anim_state state;
		resolver::resolve_info resolver;
		animationlayer anim_layer [ 15 ];
	};

	namespace extrapolate {

		void extrapolate_player( player_t* entity, backtrack_records_t& record );
		extern  backtrack_records_t extrapolate_records [ 64 ];
	}

	player_t* util_player_by_index( int index );

	
	backtrack_records_t find_best_tick( player_t* entity );
	void update_animations( player_t* entity );

	void manage_bones( );

	void setup_players( void );


	void recieve_record( player_t* entity, backtrack_records_t& record );

	namespace event_logs {
		void bullet_impact( i_game_event* event );
	};

	extern backtrack_records_t best_tick_global [ 65 ];
	float interpolation_time( );
	float get_lerp_time( );
	extern std::deque< backtrack_records_t > records [ 64 ];
	extern 	backtrack_records_t backup_data [ 64 ];

	bool is_tick_valid( backtrack_records_t record );


	void get_rotated_matrix( backtrack_records_t record, float angle, matrix_t new_matrix [ MAXSTUDIOBONES ] );

	void setup_records( );

	void backup_player( player_t* entity );

	void restore_record( player_t* entity, backtrack_records_t record );

	void restore_player( player_t* entity );
	std::vector<vec3_t> on_shot_safe( player_t* entity, backtrack_records_t& record );
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
namespace autostop {
	vec3_t calculate_next_stop( );
	vec3_t calculate_next_stop( player_t* entity );
	
	
	void clamp_movement_speed( c_usercmd* cmd, float speed );

	void slow_walk( c_usercmd* cmd, bool override );

	
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
namespace ragebot {
	

	
	
	struct target_data
	{
		player_t* entity;
		vec3_t aimpos;
		int hitbox;
		player_manager::backtrack_records_t record;
		bool backtrack;
		bool extrapolation;
		float simtime;
		float dmg;
	};
	struct point {
		vec3_t point;
		bool is_safe;
	};
	struct safe_point {
		vec3_t point;
		vec3_t point2d;
	};

	
	extern bool m_shoot_next_tick;
	extern int last_target_index;

	vec3_t get_point( player_t* entity, const int& hit_box, matrix_t bone_matrix [ 128 ] );

	std::vector<point> multi_point( player_t* entity, const int& hit_box );
	std::optional<vec3_t> get_intersect_point( vec3_t start, vec3_t end, vec3_t mins, vec3_t maxs, float radius );
	bool is_point_is_safe( player_t* entity, vec3_t from, vec3_t point, int hitbox, matrix_t new_matrix [ MAXSTUDIOBONES ] );

	void calculate_safe_points( player_t* entity );

	int prioritize_hitbox( );

	std::vector< int > hitscan_list( player_t* entity, bool extrapolation );
	void modify_eye_pos( anim_state* animstate, vec3_t* pos );
	vec3_t weapon_shot_pos( vec3_t target );
	bool hit_chance( vec3_t point, int hit_box, vec3_t angle, player_t* ent, float hitchance, int hitbox, matrix_t matrix [ MAXSTUDIOBONES ] );
	void get_rotated_matrix( player_t* entity, float angle, matrix_t new_matrix [ MAXSTUDIOBONES ] );
	extern vec3_t weapon_shoot_pos;
	std::pair<vec3_t, float> get_best_point_backtrack( player_t* entity, float min_dmg );

	std::pair<vec3_t, std::pair<bool, float>> get_best_player( player_t* entity, float min_dmg );

	std::pair<vec3_t, float> get_best_point( matrix_t* matrix, player_t* entity, float min_dmg );

	std::pair<player_manager::backtrack_records_t, bool> find_best_tick( player_t* entity );

	std::pair< vec3_t, float > get_best_point_backtrack( player_t* entity, float min_dmg );
	extern player_manager::backtrack_records_t best_tick_global [ 65 ];
	extern target_data get_last_target;
	extern std::vector<point> hitscan_points;
    target_data find_best_point( player_t* entity );

	std::vector<vec3_t> get_safe_points( player_t* entity, int hitbox );
	target_data find_best_point_backtrack( player_t* entity );
	target_data find_best_point_extrapolation( player_t* entity );

	ragebot::target_data get_target_data( player_t* entity );
	target_data get_target_data( player_t* entity );
	void sort_targets( );
	
	void createmove( c_usercmd* cmd, bool& send_packet );
	extern std::deque<player_t*> targets;

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
		int tick;
		int enemy_index;
		bool is_backtrack = false;
		player_manager::backtrack_records_t backtrack;

	};
	extern std::deque<shot_processor::shot_data> shots;
	void hurt_event( i_game_event* event );
	void weapon_fire( i_game_event* event );
	void bullet_impact( i_game_event* event );
	void add_shot( const vec3_t shotpos, const int& entity_index, bool backtrack, player_manager::backtrack_records_t record );
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
	namespace glow {
		void think( );
	};

	namespace chams {
		enum class shader_type_t {
			VertexLitGeneric = 0,
			UnlitGeneric,
			Modulate
		};
		i_material* create_material( shader_type_t shade, bool ignorez, bool wireframe );
		void run( i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* bone_to_world );
	};
	struct loginfo
	{
		loginfo( std::string text, color color, float time ) {
			this->text = text;
			this->color = color;
			this->time = time;
		}

		std::string text;
		color color;
		float time;
	};

	extern std::deque<loginfo>event_;
	void think( );
	void player( player_t* entity );
	bool calculate_box( entity_t* ent, bbox_t& box );
	void capsule_overlay( player_t* entity, float duration, matrix_t pBoneToWorldOut [ 128 ] );
}

namespace misc {
	namespace movement {
		void slow_walk( c_usercmd* cmd );
		void auto_strafer( c_usercmd* cmd );
		void strafe( c_usercmd* cmd );
		void bunny_hop(c_usercmd* cmd);
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
}