#include "../features.hpp"

void c_legitbot::control_rcs ( vec3_t & position ) {
	float recoil_value_x = ( 2.0f / 100.f ) * settings->recoil_control_x;
	float recoil_value_y = ( 2.0f / 100.f ) * settings->recoil_control_y;

	vec3_t current_punch = local_player::m_data.pointer->aim_punch_angle ( );
	position.x -= current_punch.x * recoil_value_x;
	position.y -= current_punch.y * recoil_value_y;
	
}