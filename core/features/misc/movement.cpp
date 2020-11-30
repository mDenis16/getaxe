#include "../features.hpp"

misc::movement::directional_strafer misc::movement::m_strafer;
void misc::movement::directional_movement ( c_usercmd * cmd ) {
	vec3_t velocity;
	float  delta, abs_delta, velocity_delta, correct;
	auto m_buttons = cmd->buttons;
	auto pressing_move = ( m_buttons & ( in_left ) || m_buttons & ( in_forward ) || m_buttons & ( in_back ) ||
		m_buttons & ( in_right ) || m_buttons & ( in_moveleft ) || m_buttons & ( in_moveright ) ||
		m_buttons & ( in_jump ) );
	// save our origin
	m_strafer.m_origin = csgo::local_player->origin ( );

	// disable strafing while pressing shift.
	if ( ( cmd->buttons & in_speed ) || ( csgo::local_player->flags() & fl_onground ) )
		return;

	// for changing direction.
	// we want to change strafe direction every call.
	m_strafer.m_switch_value *= -1.f;

	// for allign strafer.
	++m_strafer.m_strafe_index;

	if ( pressing_move  ) {
		// took this idea from stacker, thank u !!!!
		enum EDirections {
			FORWARDS = 0,
			BACKWARDS = 180,
			LEFT = 90,
			RIGHT = -90,
			BACK_LEFT = 135,
			BACK_RIGHT = -135
		};

		float wish_dir { };

		// get our key presses.
		bool holding_w = cmd->buttons & in_forward;
		bool holding_a = cmd->buttons & in_moveleft;
		bool holding_s = cmd->buttons & in_back;
		bool holding_d = cmd->buttons & in_moveright;

		// move in the appropriate direction.
		if ( holding_w ) {
			//	forward left
			if ( holding_a ) {
				wish_dir += ( EDirections::LEFT / 2 );
			}
			//	forward right
			else if ( holding_d ) {
				wish_dir += ( EDirections::RIGHT / 2 );
			}
			//	forward
			else {
				wish_dir += EDirections::FORWARDS;
			}
		}
		else if ( holding_s ) {
			//	back left
			if ( holding_a ) {
				wish_dir += EDirections::BACK_LEFT;
			}
			//	back right
			else if ( holding_d ) {
				wish_dir += EDirections::BACK_RIGHT;
			}
			//	back
			else {
				wish_dir += EDirections::BACKWARDS;
			}

			cmd->forwardmove = 0;
		}
		else if ( holding_a ) {
			//	left
			wish_dir += EDirections::LEFT;
		}
		else if ( holding_d ) {
			//	right
			wish_dir += EDirections::RIGHT;
		}

		csgo::m_strafe_angles.y += math::normalize_yaw ( wish_dir );
	}

	// cancel out any forwardmove values.
	cmd->forwardmove = 0.f;

	// get our viewangle change.
	delta = math::normalize_yaw ( csgo::m_strafe_angles.y - m_strafer.m_old_yaw );

	// convert to absolute change.
	abs_delta = std::abs ( delta );

	// save old yaw for next call.
	m_strafer.m_circle_yaw = m_strafer.m_old_yaw = csgo::m_strafe_angles.y;

	// set strafe direction based on mouse direction change.
	if ( delta > 0.f )
		cmd->sidemove = -450.f;

	else if ( delta < 0.f )
		cmd->sidemove = 450.f;

	// we can accelerate more, because we strafed less then needed
	// or we got of track and need to be retracked.
	if ( abs_delta <= m_strafer.m_ideal || abs_delta >= 30.f ) {
		// compute angle of the direction we are traveling in.
		vec3_t velocity_angle;
		math::vector_angles ( velocity, velocity_angle );

		// get the delta between our direction and where we are looking at.
		velocity_delta = math::normalize_yaw (  csgo::m_strafe_angles.y - velocity_angle.y );

		// correct our strafe amongst the path of a circle.
		correct = m_strafer.m_ideal;

		if ( velocity_delta <= correct || m_strafer.m_speed <= 15.f ) {
			// not moving mouse, switch strafe every tick.
			if ( -correct <= velocity_delta || m_strafer.m_speed <= 15.f ) {
				csgo::m_strafe_angles.y += ( m_strafer.m_ideal * m_strafer.m_switch_value );
				cmd->sidemove = 450.f * m_strafer.m_switch_value;
			}

			else {
				csgo::m_strafe_angles.y = velocity_angle.y - correct;
				cmd->sidemove = 450.f;
			}
		}

		else {
			csgo::m_strafe_angles.y = velocity_angle.y + correct;
			cmd->sidemove = -450.f;
		}
	}
	
}

void misc::movement::fix_mvoe ( c_usercmd * cmd, vec3_t wish_angles ) {
	vec3_t view_fwd, view_right, view_up, cmd_fwd, cmd_right, cmd_up;
	math::angle_vectors ( wish_angles, &view_fwd, &view_right, &view_up );
	math::angle_vectors ( cmd->viewangles, &cmd_fwd, &cmd_right, &cmd_up );

	const auto v8 = sqrtf ( ( view_fwd.x * view_fwd.x ) + ( view_fwd.y * view_fwd.y ) );
	const auto v10 = sqrtf ( ( view_right.x * view_right.x ) + ( view_right.y * view_right.y ) );
	const auto v12 = sqrtf ( view_up.z * view_up.z );

	const vec3_t norm_view_fwd ( ( 1.f / v8 ) * view_fwd.x, ( 1.f / v8 ) * view_fwd.y, 0.f );
	const vec3_t norm_view_right ( ( 1.f / v10 ) * view_right.x, ( 1.f / v10 ) * view_right.y, 0.f );
	const vec3_t norm_view_up ( 0.f, 0.f, ( 1.f / v12 ) * view_up.z );

	const auto v14 = sqrtf ( ( cmd_fwd.x * cmd_fwd.x ) + ( cmd_fwd.y * cmd_fwd.y ) );
	const auto v16 = sqrtf ( ( cmd_right.x * cmd_right.x ) + ( cmd_right.y * cmd_right.y ) );
	const auto v18 = sqrtf ( cmd_up.z * cmd_up.z );

	const vec3_t norm_cmd_fwd ( ( 1.f / v14 ) * cmd_fwd.x, ( 1.f / v14 ) * cmd_fwd.y, 0.f );
	const vec3_t norm_cmd_right ( ( 1.f / v16 ) * cmd_right.x, ( 1.f / v16 ) * cmd_right.y, 0.f );
	const vec3_t norm_cmd_up ( 0.f, 0.f, ( 1.f / v18 ) * cmd_up.z );

	const auto v22 = norm_view_fwd.x * cmd->forwardmove;
	const auto v26 = norm_view_fwd.y * cmd->forwardmove;
	const auto v28 = norm_view_fwd.z * cmd->forwardmove;
	const auto v24 = norm_view_right.x * cmd->sidemove;
	const auto v23 = norm_view_right.y * cmd->sidemove;
	const auto v25 = norm_view_right.z * cmd->sidemove;
	const auto v30 = norm_view_up.x * cmd->upmove;
	const auto v27 = norm_view_up.z * cmd->upmove;
	const auto v29 = norm_view_up.y * cmd->upmove;

	cmd->forwardmove = ( ( ( ( norm_cmd_fwd.x * v24 ) + ( norm_cmd_fwd.y * v23 ) ) + ( norm_cmd_fwd.z * v25 ) )
		+ ( ( ( norm_cmd_fwd.x * v22 ) + ( norm_cmd_fwd.y * v26 ) ) + ( norm_cmd_fwd.z * v28 ) ) )
		+ ( ( ( norm_cmd_fwd.y * v30 ) + ( norm_cmd_fwd.x * v29 ) ) + ( norm_cmd_fwd.z * v27 ) );
	cmd->sidemove = ( ( ( ( norm_cmd_right.x * v24 ) + ( norm_cmd_right.y * v23 ) ) + ( norm_cmd_right.z * v25 ) )
		+ ( ( ( norm_cmd_right.x * v22 ) + ( norm_cmd_right.y * v26 ) ) + ( norm_cmd_right.z * v28 ) ) )
		+ ( ( ( norm_cmd_right.x * v29 ) + ( norm_cmd_right.y * v30 ) ) + ( norm_cmd_right.z * v27 ) );
	cmd->upmove = ( ( ( ( norm_cmd_up.x * v23 ) + ( norm_cmd_up.y * v24 ) ) + ( norm_cmd_up.z * v25 ) )
		+ ( ( ( norm_cmd_up.x * v26 ) + ( norm_cmd_up.y * v22 ) ) + ( norm_cmd_up.z * v28 ) ) )
		+ ( ( ( norm_cmd_up.x * v30 ) + ( norm_cmd_up.y * v29 ) ) + ( norm_cmd_up.z * v27 ) );

	wish_angles = cmd->viewangles;

	if ( csgo::local_player->move_type ( ) != movetype_ladder )
		cmd->buttons &= ~( in_forward | in_back | in_moveright | in_moveleft );
}
/*

void misc::movement::directional_movement ( c_usercmd * cmd ) {


	// Define rough directions
	enum directions {
		FORWARDS = 0,
		BACKWARDS = 180,
		LEFT = 90,
		RIGHT = -90
	};

	// Reset direction when player is not strafing
	m_strafer.is_bhopping = cmd->buttons & in_jump;
	if ( !m_strafer.is_bhopping && csgo::local_player->flags() & fl_onground ) {
		m_strafer.calculated_direction = directions::FORWARDS;
		m_strafer.in_transition = false;
		return;
	}

	// Get true view angles
	vec3_t base { };
	interfaces::engine->get_view_angles ( base );

	// Calculate the rough direction closest to the player's true direction
	auto get_rough_direction = [ & ] ( float true_direction ) -> float {
		// Make array with our four rough directions
		std::array< float, 4 > minimum = { directions::FORWARDS, directions::BACKWARDS, directions::LEFT, directions::RIGHT };
		float best_angle, best_delta = 181.f;

		// Loop through our rough directions and find which one is closest to our true direction
		for ( size_t i = 0; i < minimum.size ( ); ++i ) {
			float rough_direction = base.y + minimum.at ( i );
			float delta = fabsf ( math::normalize_yaw ( true_direction - rough_direction ) );

			// Only the smallest delta wins out
			if ( delta < best_delta ) {
				best_angle = rough_direction;
				best_delta = delta;
			}
		}

		return best_angle;
	};

	// Get true direction based on player velocity
	vec3_t aa = vec3_t ( );
	math::vector_angles ( csgo::local_player->velocity ( ), aa );
	m_strafer.true_direction = aa.y;

	// Detect wish direction based on movement keypresses
	if ( cmd->buttons & in_forward ) {
		m_strafer.wish_direction = base.y + directions::FORWARDS;
	}
	else if ( cmd->buttons & in_back ) {
		m_strafer.wish_direction = base.y + directions::BACKWARDS;
	}
	else if ( cmd->buttons & in_moveleft ) {
		m_strafer.wish_direction = base.y + directions::LEFT;
	}
	else if ( cmd->buttons & in_moveright ) {
		m_strafer.wish_direction = base.y + directions::RIGHT;
	}
	else {
		// Reset to forward when not pressing a movement key, then fix anti-aim strafing by setting IN_FORWARD
		cmd->buttons |= in_forward;
		m_strafer.wish_direction = base.y + directions::FORWARDS;
	}

	// Calculate the ideal strafe rotation based on player speed (c) navewindre

	float speed_rotation = min ( RAD2DEG ( std::asinf ( 30.f / csgo::local_player->velocity ( ).Length2D ( ) ) ) * 0.5f, 45.f );
	if ( m_strafer.in_transition ) {
		// Calculate the step by using our ideal strafe rotation
		float ideal_step = speed_rotation + m_strafer.calculated_direction;
		m_strafer.step = fabsf ( math::normalize_yaw ( m_strafer.calculated_direction - ideal_step ) );

		// Check when the calculated direction arrives close to the wish direction
		if ( fabsf ( math::normalize_yaw ( m_strafer.wish_direction - m_strafer.calculated_direction ) ) > m_strafer.step ) {
			float add = math::normalize_yaw ( m_strafer.calculated_direction + m_strafer.step );
			float sub = math::normalize_yaw ( m_strafer.calculated_direction - m_strafer.step );

			// Step in direction that gets us closer to our wish direction
			if ( fabsf ( math::normalize_yaw ( m_strafer.wish_direction - add ) ) >= fabsf ( math::normalize_yaw ( m_strafer.wish_direction - sub ) ) ) {
				m_strafer.calculated_direction -= m_strafer.step;
			}
			else {
				m_strafer.calculated_direction += m_strafer.step;
			}
		}
		else {
			m_strafer.in_transition = false;
		}
	}
	else {
		// Get rough direction and setup calculated direction only when not transitioning
		m_strafer.rough_direction = get_rough_direction ( m_strafer.true_direction );
		m_strafer.calculated_direction = m_strafer.rough_direction;

		// When we have a difference between our current (rough) direction and our wish direction, then transition
		if ( m_strafer.rough_direction != m_strafer.wish_direction ) {
			m_strafer.in_transition = true;
		}
	}

	// Set movement up to be rotated
	cmd->forwardmove = 0.f;
	cmd->sidemove = cmd->command_number % 2 ? 450.f : -450.f;

	// Calculate ideal rotation based on our newly calculated direction
	float direction = ( cmd->command_number % 2 ? speed_rotation : -speed_rotation ) + m_strafer.calculated_direction;

	// Rotate our direction based on our new, defininite direction
	float rotation = DEG2RAD ( base.y - direction );

	float cos_rot = cos ( rotation );
	float sin_rot = sin ( rotation );

	float forwardmove = ( cos_rot * cmd->forwardmove ) - ( sin_rot * cmd->sidemove );
	float sidemove = ( sin_rot * cmd->forwardmove ) + ( cos_rot * cmd->sidemove );

	// Apply newly rotated movement
	cmd->forwardmove = forwardmove;
	cmd->sidemove = sidemove;
}
*/