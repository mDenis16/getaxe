#pragma once
#include <windows.h>
#include <iostream>
#include <cstdint>
#include <memory>
#include <vector>
#include <thread>
#include <chrono>
#include <array>
#include <fstream>
#include <istream>
#include <unordered_map>
#include <intrin.h>
#include <filesystem>

#include "../utilities/singleton.hpp"
#include "../utilities/fnv.hpp"
#include "../utilities/utilities.hpp"
#include "../../dependencies/minhook/minhook.h"
#include "../interfaces/interfaces.hpp"
#include "../../source-sdk/sdk.hpp"
#include "../../core/hooks/hooks.hpp"
#include "../../dependencies/math/math.hpp"
#include "../../dependencies/utilities/console/console.hpp"
#include "../utilities/csgo.hpp"

//interfaces
#define sig_client_state "A1 ? ? ? ? 8B 80 ? ? ? ? C3"
#define sig_directx "A1 ? ? ? ? 50 8B 08 FF 51 0C"
#define sig_input "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10"
#define sig_glow_manager "0F 11 05 ? ? ? ? 83 C8 01 C7 05 ? ? ? ? 00 00 00 00"
#define sig_player_move_helper "8B 0D ? ? ? ? 8B 46 08 68"
#define sig_weapon_data "8B 35 ? ? ? ? FF 10 0F B7 C0"

//misc
#define sig_set_angles "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1"
#define sig_prediction_random_seed "8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04"

namespace csgo {
	extern player_t* local_player;
	extern matrix_t player_bones[ 65 ][ 128 ];
	extern matrix_t fake_matrix[ 128 ];
	extern matrix_t real_matrix [ 128 ];
	extern vec3_t m_strafe_angles;
	extern matrix_t right_player_bones [ 65 ][ 128 ];
	extern matrix_t left_player_bones [ 65 ][ 128 ];
	extern vec3_t last_origin;
	extern bool lagPeek;
	extern float last_time;
	extern int shift;
	extern vec3_t real_angle_static;
	extern inline bool inLagPeek;
	extern int m_nTickbaseShift;
	extern bool bInSendMove;
	extern int32_t nSinceUse;
	extern bool choke_next_tick;
	extern bool m_update_fake;
	extern int m_goal_shift;
	extern bool fakeduck;
	extern   animationlayer m_real_layers [ 13 ];
	extern float            m_real_poses [ 24 ];

	extern 	int screen_height;
	extern int screen_width;
	extern view_matrix_t screen_matrix;
	extern bool m_animate;
	extern int m_rate;

	extern bool bFirstSendMovePack;
	extern  HWND old_window;
	extern float proper_abs;
	extern   vec3_t            m_angle;
	
	extern int m_tick;
	extern  animationlayer layers [ 15 ];
	extern bool  in_setup_bones[65];
	extern bool m_update;
	extern int		  m_shot_command_number;
	extern int		  m_shot_tickbase;
	extern vec3_t m_rotation;
	extern bool send_packet;
	extern float choke_yaw;
	extern 	 bool should_setup_bones;
	extern HWND window;
	extern bool in_trace;
	extern  vec3_t original_viewangle;
	extern  bool fresh_tick;
	extern vec3_t real_angle;

	extern vec3_t fake_angle ;

	extern  c_usercmd* cmd;
}