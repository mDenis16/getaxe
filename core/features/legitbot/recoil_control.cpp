#include "../features.hpp"

void c_legitbot::control_rcs ( vec3_t & position ) {
	static vec3_t last_punch = vec3_t ( );

	float recoil_value_x = ( 2.0f / 100.f ) * settings->recoil_control_x;
	float recoil_value_y = ( 2.0f / 100.f ) * settings->recoil_control_y;

	vec3_t current_punch = local_player::m_data.pointer->aim_punch_angle ( );
	if ( local_player::m_data.pointer->shots_fired ( ) > 0 ) {
		vec3_t new_punch = { current_punch.x - localdata.last_punch.x, current_punch.y - localdata.last_punch.y, 0 };
		position.x -= current_punch.x * recoil_value_x;
		position.y -= current_punch.y * recoil_value_y;
	}
	else {
		position.x -= current_punch.x * recoil_value_x;
		position.y -= current_punch.y * recoil_value_y;
	}

	
}