#include "csgo.hpp"

namespace csgo {
	player_t* local_player = nullptr;
	int screen_height = 0;
	view_matrix3x4_t screen_matrix;
	int screen_width = 0;


	matrix3x4_t fake_matrix[ 128 ];
	matrix3x4_t left_player_bones [ 65 ][ 128 ];
	matrix3x4_t right_player_bones [ 65 ][ 128 ];
	matrix3x4_t real_matrix [ 128 ];
	bool override_velocity = false;
	bool choke_next_tick = false;
	vec3_t last_origin;
	float last_send_time = 0.f;
	std::array< float, 24 > pose_params;
	bool in_setup_bones  = false;
	bool in_update_anim = false;
	bool in_create_move = false;
	 bool in_trace = false;
	 bool should_setup_bones = false;
	 vec3_t  m_strafe_angles;
	  bool send_packet = false;
	  bool did_shot_before = false;
	 bool fresh_tick = false;
	 bool lagPeek = false;
	 inline bool inLagPeek = false;
	 int tickbase_shift = 0;
	 int shift = 0;
	 c_usercmd* cmd = nullptr;
	 vec3_t real_angle = vec3_t( );
	 vec3_t real_origin = vec3_t ( );

	 inline vec3_t real_angle_static = vec3_t( );
	 std::vector<vec3_t> points_to_draw;
	 vec3_t fake_angle = vec3_t( );
	  float proper_abs;
	  int m_nTickbaseShift;
	  bool bInSendMove;
	  int m_goal_shift = 13;
	  bool fakeduck = false;
	  bool m_animate = false;
	  bool m_update_fake = false;
	  int32_t nSinceUse;
	  animationlayer m_real_layers [ 13 ];
	  float            m_real_poses [ 24 ];
	  vec3_t            m_angle;
	  vec3_t m_rotation;
	  vec3_t original_viewangle;
	  float choke_yaw;
	  int m_rate;
	  int		  m_shot_command_number;
	  int		  m_shot_tickbase;
	  HWND window;
	  HWND old_window;
	  int fixed_tickbase = 0;
	  bool m_update = false;
	  int m_tick;
	  bool bFirstSendMovePack;
	  animationlayer layers [ 15 ];
	namespace fonts {
		unsigned long watermark_font;
		unsigned long name_font;
	}
}