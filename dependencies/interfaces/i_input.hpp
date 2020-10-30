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
	void* vtable;									// 0x00
	bool              m_trackir;					// 0x04
	bool              m_mouse_init;					// 0x05
	bool              m_mouse_active;				// 0x06
	bool              m_joystick_adv_init;			// 0x07
	char a[ 0x2C ];									// 0x08
	void* m_keys;						// 0x34
	char b [ 116 ];	// 0x38
	bool              m_camera_intercepting_mouse;	// 0x9C
	bool              m_camera_in_third_person;		// 0x9D
	bool              m_camera_moving_with_mouse;	// 0x9E
	vec3_t            m_camera_offset;				// 0xA0
	bool              m_camera_distance_move;		// 0xAC
	int               m_camera_old_x;				// 0xB0
	int               m_camera_old_y;				// 0xB4
	int               m_camera_x;					// 0xB8
	int               m_camera_y;					// 0xBC
	bool              m_camera_is_orthographic;		// 0xC0
	vec3_t             m_previous_view_angles;		// 0xC4
	vec3_t             m_previous_view_angles_tilt;	// 0xD0
	float             m_last_forward_move;			// 0xDC
	int               m_clear_input_state;			// 0xE0
	c_usercmd* m_commands;					// 0xEC
	c_verified_user_cmd* m_verified;					// 0xF0
	c_usercmd* get_user_cmd( int slot, int sequence_num ) {
		using original_fn = c_usercmd * ( __thiscall* )( void*, int, int );
		return ( *( original_fn** ) this ) [ 8 ]( this, slot, sequence_num );
	}
};