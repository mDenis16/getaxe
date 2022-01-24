#pragma once
#include "../../source-sdk/classes/c_usercmd.hpp"
class c_verified_user_cmd {
public:
	c_usercmd   m_cmd;
	unsigned long    m_crc;
};
class i_input {
public:
	enum indices : size_t {
		CREATEMOVE = 3,
		WRITEUSERCMDDELTATOBUFFER = 5,
		ENCODEUSERCMDTOBUFFER = 6,
		GETUSERCMD = 8,
		CAMISTHIRDPERSON = 32,
		CAMTOTHIRDPERSON = 35,
		CAMTOFIRSTPERSON = 36,
	};

public:
	//void* vtable;									// 0x00
	//bool              m_trackir;					// 0x04
	//bool              m_mouse_init;					// 0x05
	//bool              m_mouse_active;				// 0x06
	//bool              m_joystick_adv_init;			// 0x07
	//char a[ 0x2C ];									// 0x08
	//void* m_keys;						// 0x34
	//char b [ 116 ];	// 0x38
	//bool              m_camera_intercepting_mouse;	// 0x9C
	//bool              m_camera_in_third_person;		// 0x9D
	//bool              m_camera_moving_with_mouse;	// 0x9E
	//vec3_t            m_camera_offset;				// 0xA0
	//bool              m_camera_distance_move;		// 0xAC
	//int               m_camera_old_x;				// 0xB0
	//int               m_camera_old_y;				// 0xB4
	//int               m_camera_x;					// 0xB8
	//int               m_camera_y;					// 0xBC
	//bool              m_camera_is_orthographic;		// 0xC0
	//vec3_t             m_previous_view_angles;		// 0xC4
	//vec3_t             m_previous_view_angles_tilt;	// 0xD0
	//float             m_last_forward_move;			// 0xDC
	//int               m_clear_input_state;			// 0xE0
	//c_usercmd* m_commands;					// 0xEC
	//c_verified_user_cmd* m_verified;					// 0xF0
	//c_usercmd* get_user_cmd( int sequence_number ) {
	//	return &m_commands [ sequence_number % 150 ];
	//}
	//c_verified_user_cmd * get_verified_user_cmd ( int sequence_number ) {
	//	return &m_verified [ sequence_number % 150 ];
	//}
	char pad0[0xC]; // 0x0
	bool m_trackir_available; // 0xC
	bool m_mouse_initialized; // 0xD
	bool m_mouse_active; // 0xE
	char pad1[0x9E]; // 0xF
	bool m_camera_in_thirdperson; // 0xAD
	char pad2[0x2]; // 0xAE
	vec3_t m_camera_offset; // 0xB0
	char pad3[0x38]; // 0xBC
	c_usercmd* m_commands; // 0xF4
	c_verified_user_cmd* m_verified_commands; // 0xF8

	c_usercmd* get_user_cmd(int sequence_number)
	{
		return &m_commands[sequence_number % 150];
	}

	c_verified_user_cmd* get_verified_user_cmd(int sequence_number)
	{
		return &m_verified_commands[sequence_number % 150];
	}
};