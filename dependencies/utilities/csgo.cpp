#include "csgo.hpp"

namespace csgo {
	player_t* local_player = nullptr;
	int screen_height = 0;
	view_matrix_t screen_matrix;
	int screen_width = 0;
	matrix_t player_bones[ 65 ][ 128 ];
	matrix_t real_matrix [ 128 ];
	matrix_t right_player_bones [ 65 ][ 128 ];
	matrix_t left_player_bones [ 65 ][ 128 ];
	matrix_t fake_matrix[ 128 ];

	bool choke_next_tick = false;
	vec3_t last_origin;
	float last_send_time = 0.f;
	std::array< float, 24 > pose_params;
	bool in_setup_bones [ 65 ];
	 bool in_trace = false;
	 bool should_setup_bones = false;
	 vec3_t  m_strafe_angles;
	  bool send_packet = false;
	 bool fresh_tick = false;
	 bool lagPeek = false;
	 inline bool inLagPeek = false;
	 int shift = 0;
	 c_usercmd* cmd = nullptr;
	 vec3_t real_angle = vec3_t( );
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
	  bool m_update = false;
	  int m_tick;
	  bool bFirstSendMovePack;
	  animationlayer layers [ 15 ];
	namespace fonts {
		unsigned long watermark_font;
		unsigned long name_font;
	}
}