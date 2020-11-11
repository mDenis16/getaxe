
#include "../features.hpp"
std::vector<autostop::draw_point> autostop::points_to_draw;
autostop::autostop_data autostop::m_autostop_data;

void autostop::clamp_movement_speed( c_usercmd* cmd, float speed ) {
	float final_speed = speed;

	cmd->buttons |= in_speed;

	float squirt = std::sqrtf( ( cmd->forwardmove * cmd->forwardmove ) + ( cmd->sidemove * cmd->sidemove ) );

	if ( squirt > speed ) {
		float squirt2 = std::sqrtf( ( cmd->forwardmove * cmd->forwardmove ) + ( cmd->sidemove * cmd->sidemove ) );

		float cock1 = cmd->forwardmove / squirt2;
		float cock2 = cmd->sidemove / squirt2;

		auto Velocity = csgo::local_player->velocity( ).Length2D( );

		if ( final_speed + 1.0 <= Velocity ) {
			cmd->forwardmove = 0;
			cmd->sidemove = 0;
		}
		else {
			cmd->forwardmove = cock1 * final_speed;
			cmd->sidemove = cock2 * final_speed;
		}
	}
}
void autostop::slow_walk( c_usercmd* cmd, bool override = false ) {

	if ( interfaces::inputsystem->is_button_down( button_code_t::KEY_LSHIFT ) )
		override = true;
	if ( override && csgo::local_player && csgo::local_player->active_weapon( ) && csgo::local_player->active_weapon( )->get_weapon_data( ) ) {
		// get the max possible speed whilest we are still accurate.
		float flMaxSpeed = csgo::local_player->is_scoped( ) > 0?csgo::local_player->active_weapon( )->get_weapon_data( )->flMaxSpeedAlt:csgo::local_player->active_weapon( )->get_weapon_data( )->flMaxSpeed;
		float flDesiredSpeed = ( flMaxSpeed * 0.33000001 );

		clamp_movement_speed( cmd, flDesiredSpeed );
	}
}
void autostop::stop_movement( c_usercmd* cmd )
{
	constexpr bool isHexagoneGodlike = true;



	vec3_t hvel = csgo::local_player->velocity( );
	hvel.z = 0;
	float speed = hvel.Length2D( );

	if ( speed < 1.f ) // Will be clipped to zero anyways
	{
		cmd->forwardmove = 0.f;
		cmd->sidemove = 0.f;
		return;
	}

	// Homework: Get these dynamically
	float accel = 5.5f;
	float maxSpeed = 320.f;
	float playerSurfaceFriction = 1.0f; // I'm a slimy boi
	float max_accelspeed = accel * interfaces::globals->interval_per_tick * maxSpeed * playerSurfaceFriction;

	float wishspeed {};

	// Only do custom deceleration if it won't end at zero when applying max_accel
	// Gamemovement truncates speed < 1 to 0
	if ( speed - max_accelspeed <= -1.f )
	{
		// We try to solve for speed being zero after acceleration:
		// speed - accelspeed = 0
		// speed - accel*frametime*wishspeed = 0
		// accel*frametime*wishspeed = speed
		// wishspeed = speed / (accel*frametime)
		// ^ Theoretically, that's the right equation, but it doesn't work as nice as 
		//   doing the reciprocal of that times max_accelspeed, so I'm doing that :shrug:
		wishspeed = max_accelspeed / ( speed / ( accel * interfaces::globals->interval_per_tick ) );
	}
	else // Full deceleration, since it won't overshoot
	{
		// Or use max_accelspeed, doesn't matter
		wishspeed = max_accelspeed;
	}

	// Calculate the negative movement of our velocity, relative to our viewangles
	vec3_t ndir = vec3_t( hvel.x * -1.f, hvel.y * -1.f, hvel.z * -1.f );
	math::vector_angles( ndir, ndir );
	ndir.y = cmd->viewangles.y - ndir.y; // Relative to local view
	ndir = math::angle_vector( ndir );

	cmd->forwardmove = ndir.x * wishspeed;
	cmd->sidemove = ndir.y * wishspeed;
}
void autostop::rotate_movement( c_usercmd* cmd, float yaw )
{
	vec3_t viewangles;
	interfaces::engine->get_view_angles( viewangles );

	float rotation = DEG2RAD( viewangles.y - yaw );

	float cos_rot = cos( rotation );
	float sin_rot = sin( rotation );

	float new_forwardmove = ( cos_rot * cmd->forwardmove ) - ( sin_rot * cmd->sidemove );
	float new_sidemove = ( sin_rot * cmd->forwardmove ) + ( cos_rot * cmd->sidemove );

	cmd->forwardmove = new_forwardmove;
	cmd->sidemove = new_sidemove;
}
void autostop::Strafe( ) {
	// Define rough directions
	auto cmd = csgo::cmd;
	if ( !cmd )
		return;
	enum directions {
		FORWARDS = 0,
		BACKWARDS = 180,
		LEFT = 90,
		RIGHT = -90
	};
	static bool is_bhopping = false; static bool in_transition = false; static bool calculated_direction = false;
	// Reset direction when player is not strafing
	is_bhopping = cmd->buttons & in_jump;
	if ( !is_bhopping && csgo::local_player->flags( ) & fl_onground ) {
		calculated_direction = directions::FORWARDS;
		in_transition = false;
		return;
	}

	// Get true view angles
	vec3_t base { };
    interfaces::engine->get_view_angles( base );

	// Calculate the rough direction closest to the player's true direction
	auto get_rough_direction = [ & ] ( float true_direction ) -> float {
		// Make array with our four rough directions
		std::array< float, 4 > minimum = { directions::FORWARDS, directions::BACKWARDS, directions::LEFT, directions::RIGHT };
		float best_angle, best_delta = 181.f;

		// Loop through our rough directions and find which one is closest to our true direction
		for ( size_t i = 0; i < minimum.size( ); ++i ) {
			float rough_direction = base.y + minimum.at( i );
			float delta = fabsf( math::normalize_yaw( true_direction - rough_direction ) );

			// Only the smallest delta wins out
			if ( delta < best_delta ) {
				best_angle = rough_direction;
				best_delta = delta;
			}
		}

		return best_angle;
	};
	vec3_t a = vec3_t( );
	math::vector_angles( csgo::local_player->velocity( ), a );
	// Get true direction based on player velocity
	float true_direction = a.y;
	float wish_direction = 0.f;
	// Detect wish direction based on movement keypresses
	if ( cmd->buttons & in_forward ) {
		wish_direction = base.y + directions::FORWARDS;
	}
	else if ( cmd->buttons & in_back ) {
		wish_direction = base.y + directions::BACKWARDS;
	}
	else if ( cmd->buttons & in_moveleft ) {
		wish_direction = base.y + directions::LEFT;
	}
	else if ( cmd->buttons & in_moveright ) {
		wish_direction = base.y + directions::RIGHT;
	}
	else {
		// Reset to forward when not pressing a movement key, then fix anti-aim strafing by setting IN_FORWARD
		cmd->buttons |= in_forward;
		wish_direction = base.y + directions::FORWARDS;
	}
	float rough_direction = 0.f;
	
	// Calculate the ideal strafe rotation based on player speed (c) navewindre
	float speed_rotation = min( RAD2DEG( std::asinf( 30.f /  csgo::local_player->velocity( ).Length2D() ) ) * 0.5f, 45.f );
	float step = 0.f;
	if ( in_transition ) {
		// Calculate the step by using our ideal strafe rotation
		float ideal_step = speed_rotation + calculated_direction;
		step = fabsf( math::normalize_yaw( calculated_direction - ideal_step ) );

		// Check when the calculated direction arrives close to the wish direction
		if ( fabsf( math::normalize_yaw( wish_direction - calculated_direction ) ) > step ) {
			float add = math::normalize_yaw( calculated_direction + step );
			float sub = math::normalize_yaw( calculated_direction - step );

			// Step in direction that gets us closer to our wish direction
			if ( fabsf( math::normalize_yaw( wish_direction - add ) ) >= fabsf( math::normalize_yaw( wish_direction - sub ) ) ) {
				calculated_direction -= step;
			}
			else {
				calculated_direction += step;
			}
		}
		else {
			in_transition = false;
		}
	}
	else {
		// Get rough direction and setup calculated direction only when not transitioning
		rough_direction = get_rough_direction( true_direction );
		calculated_direction = rough_direction;

		// When we have a difference between our current (rough) direction and our wish direction, then transition
		if ( rough_direction != wish_direction ) {
			in_transition = true;
		}
	}

	// Set movement up to be rotated
	cmd->forwardmove = 0.f;
	cmd->sidemove = cmd->command_number % 2?450.f:-450.f;

	// Calculate ideal rotation based on our newly calculated direction
	float direction = ( cmd->command_number % 2?speed_rotation:-speed_rotation ) + calculated_direction;

	// Rotate our direction based on our new, defininite direction
	float rotation = DEG2RAD( base.y - direction );

	float cos_rot = cos( rotation );
	float sin_rot = sin( rotation );

	float forwardmove = ( cos_rot * cmd->forwardmove) - ( sin_rot * cmd->forwardmove );
	float sidemove = ( sin_rot * cmd->sidemove ) + ( cos_rot * cmd->sidemove );

	// Apply newly rotated movement
	cmd->forwardmove = forwardmove;
	cmd->sidemove = sidemove;
}
void autostop::JumpRelated( ) {

	if ( !csgo::cmd )
		return;
	if ( !csgo::local_player )
		return;

	if ( !csgo::local_player->is_alive()  )
		return;

	


	// No need to bhop there
	const auto move_type = csgo::local_player->move_type( );
	if ( move_type == movetype_ladder ||
		move_type == movetype_noclip ||
		move_type == movetype_observer )
		return;

	// Is player underwater?
	//if (g_cl.m_local->m_nWaterLevel() >= 2)
	//	return;

	static auto LastJumped = false;
	static auto ShouldFake = false;

	const auto flags = csgo::local_player->flags ( );

	if ( !LastJumped && ShouldFake ) {
		ShouldFake = false;
		csgo::cmd->buttons |= in_jump;
	}
	else if ( csgo::cmd->buttons & in_jump ) {

		//if ( g_cfg [ XOR( "misc_air_duck" ) ].get<bool>( ) ) {
		//	g_cl.m_cmd->m_buttons |= IN_DUCK;
		//}


		if ( flags & fl_onground ) {
			LastJumped = true;
ShouldFake = true;
		}
		else {
csgo::cmd->buttons &= ~in_jump;
LastJumped = false;
		}
	}
	else {
	LastJumped = false;
	ShouldFake = false;
	}
}
int autostop::ticks_to_stop( vec3_t velocity )
{
	static auto sv_maxspeed = interfaces::console->get_convar( ( "sv_maxspeed" ) );

	const float max_speed = 320.f;
	const float acceleration = 5.5f;
	const float max_accelspeed = acceleration * max_speed * interfaces::globals->interval_per_tick;

	return velocity.length( ) / max_accelspeed;
}

void autostop::auto_peek( c_usercmd* cmd ) {

	if ( !csgo::local_player->get_anim_state( ) )
		return;
	auto animstate = csgo::local_player->get_anim_state( );
	int dir = anti_aim::get_desync_dirrection( );
	std::vector<vec3_t> posible_points;
	vec3_t cur_angles = vec3_t( ); interfaces::engine->get_view_angles( cur_angles );
	cur_angles.y = math::normalize_yaw( cur_angles.y - 90.f );
	auto left = math::angle_vector( cur_angles );

	vec3_t Head = { csgo::local_player->origin( ).x, csgo::local_player->origin( ).y, ( csgo::local_player->get_hitbox_position( 0 ).z + 10.f ) };
	points_to_draw.clear( );
	draw_point dp;
	dp.name = "left";
	dp.pos = left;
	points_to_draw.push_back( dp );
	vec3_t HeadExtr = ( Head + ( csgo::local_player->velocity( ) * math::ticks_to_time( 22 ) ) );
}


void autostop::autostop( c_usercmd* cmd ) {

	if ( variables::ragebot::autostop )
	{

		if ( !csgo::local_player )
			return;
		if ( !csgo::local_player->is_alive( ) )
			return;
		auto active_weapon = csgo::local_player->active_weapon( );
		if ( !active_weapon )
			return;
		if ( active_weapon->is_knife( ) || active_weapon->is_c4( ) || active_weapon->is_defuse_kit( ) || active_weapon->is_grenade( ) )
			return;
		bool found_position = false;
		if ( interfaces::inputsystem->is_button_down( button_code_t::KEY_F ) )
			autostop::m_autostop_data.state = 0;
		autostop::m_autostop_data.cur_origin = csgo::local_player->origin( );

		if ( autostop::m_autostop_data.from.is_zero( ) || autostop::m_autostop_data.to.is_zero( ) || ( autostop::m_autostop_data.state == 0 && !autostop::m_autostop_data.standby ) ) {
			autostop::m_autostop_data.standby = true;
			autostop::m_autostop_data.from = vec3_t( );
			autostop::m_autostop_data.to = vec3_t( );
			autostop::m_autostop_data.ticks = 0;
			autostop::m_autostop_data.state = 0;
			autostop::m_autostop_data.ticks_blocked = 0;

		}

		if ( ( autostop::m_autostop_data.target && !autostop::m_autostop_data.target->is_alive( ) ) || !autostop::m_autostop_data.target || csgo::local_player->health( ) <= 0 ) {
			autostop::m_autostop_data.ticks = 0;
			autostop::m_autostop_data.from = vec3_t( );
			autostop::m_autostop_data.to = vec3_t( );
			autostop::m_autostop_data.state = 0;
			autostop::m_autostop_data.target = nullptr;
			autostop::m_autostop_data.standby = true;
		}

		if ( autostop::m_autostop_data.state <= 0 ) {
			for ( auto ent : ragebot::targets )
			{
				vec3_t enemy_head = { ent->origin( ).x, ent->origin( ).y, ( ent->get_hitbox_position( 0 ).z + 10.f ) };
				vec3_t enemy_pelvis = { ent->origin( ).x, ent->origin( ).y, ( ent->get_hitbox_position( hitbox_pelvis ).z + 10.f ) };
			

				if ( fabs( csgo::local_player->velocity( ).Length2D( ) ) > .1f ) {
					for ( size_t i = 0; i < 16; i++ )
					{
						vec3_t head_extrapolated = ( engine_prediction::unpredicted_eye + ( csgo::local_player->velocity( ) * math::ticks_to_time( i ) ) );
						auto dist = math::calc_distance( csgo::local_player->origin( ), autostop::m_autostop_data.last_from, false );
						if ( dist < 2.f )
							break;
						found_position = autowall::can_hit_float_point( head_extrapolated, enemy_head ) || autowall::can_hit_float_point( head_extrapolated, enemy_pelvis );
						if ( found_position ) {


							autostop::m_autostop_data.target = ent;
							autostop::m_autostop_data.standby = false;
							autostop::m_autostop_data.from = csgo::local_player->origin( ) - ( csgo::local_player->velocity( ) * math::ticks_to_time( 3 ) );

							autostop::m_autostop_data.to = vec3_t( head_extrapolated.x, head_extrapolated.y, csgo::local_player->origin( ).z);
							autostop::m_autostop_data.state = 1;
							autostop::m_autostop_data.eye_pos = head_extrapolated;
							autostop::m_autostop_data.last_from = autostop::m_autostop_data.from;
							interfaces::console->console_printf( "[AUTOSTOP] Found hit position.\n" );

							break;
						}
					}
					if ( found_position )break;
				}

			}



		}
		else if ( autostop::m_autostop_data.state > 0 && !found_position )
		{


			switch ( autostop::m_autostop_data.state )
			{
			case 1:
			{
				auto dist = math::calc_distance( autostop::m_autostop_data.to, autostop::m_autostop_data.cur_origin, false < 0.1f );
				if ( dist < 2.f && !( autowall::can_hit_float_point( autostop::m_autostop_data.eye_pos, autostop::m_autostop_data.target->get_hitbox_position( hitbox_head ) ) ) )
				{
					autostop::m_autostop_data.state = 2;
				}
				if ( csgo::local_player->active_weapon( ) && csgo::local_player->active_weapon( )->is_reloading( ) ) {

					auto keys = { button_code_t::KEY_W, button_code_t::KEY_S, button_code_t::KEY_A, button_code_t::KEY_D };

					for ( auto key : keys )
					{
						if ( interfaces::inputsystem->is_button_down( key ) )
						{
							autostop::m_autostop_data.ticks = 0;
							autostop::m_autostop_data.from = vec3_t( );
							autostop::m_autostop_data.to = vec3_t( );
							autostop::m_autostop_data.state = 0;
							autostop::m_autostop_data.standby = true;
							break;
						}
					}

					return;
				}
				interfaces::console->console_printf( "[AUTOSTOP] Moving to peek dist. %f.\n", dist );
				if ( dist < 1.2f ) {
					autostop::m_autostop_data.state = 2;
					return;
				}
				if (!( csgo::local_player->active_weapon()->next_primary_attack( ) <=  csgo::local_player->get_tick_base( ) * interfaces::globals->interval_per_tick) )
					return;
				float yaw = cmd->viewangles.y;
				vec3_t VecForward = autostop::m_autostop_data.cur_origin - autostop::m_autostop_data.to;

				vec3_t translatedVelocity = vec3_t {
					( float ) ( VecForward.x * cos( yaw / 180 * ( float ) M_PI ) + VecForward.y * sin( yaw / 180 * ( float ) M_PI ) ),
					( float ) ( VecForward.y * cos( yaw / 180 * ( float ) M_PI ) - VecForward.x * sin( yaw / 180 * ( float ) M_PI ) ),
					VecForward.z
				};
				cmd->forwardmove = -translatedVelocity.x * 20.f;
				cmd->sidemove = translatedVelocity.y * 20.f;
				break;
			}
			case 2:
			{
				auto dist = math::calc_distance( autostop::m_autostop_data.from, autostop::m_autostop_data.cur_origin, false < 0.1f );
				if ( dist < 1.2f ) {
					autostop::m_autostop_data.state = 0;
					return;
				}
				interfaces::console->console_printf( "[AUTOSTOP] Going back after peek. distance %f.\n", dist );

				float yaw = cmd->viewangles.y;
				vec3_t VecForward = autostop::m_autostop_data.cur_origin - autostop::m_autostop_data.from;

				vec3_t translatedVelocity = vec3_t {
					( float ) ( VecForward.x * cos( yaw / 180 * ( float ) M_PI ) + VecForward.y * sin( yaw / 180 * ( float ) M_PI ) ),
					( float ) ( VecForward.y * cos( yaw / 180 * ( float ) M_PI ) - VecForward.x * sin( yaw / 180 * ( float ) M_PI ) ),
					VecForward.z
				};
				cmd->forwardmove = -translatedVelocity.x * 20.f;
				cmd->sidemove = translatedVelocity.y * 20.f;
				break;
			}
			default:
				break;
			}
		}

		else {
			autostop::m_autostop_data.ticks = 0;
			autostop::m_autostop_data.from = vec3_t( );
			autostop::m_autostop_data.to = vec3_t( );
			autostop::m_autostop_data.target = nullptr;
			autostop::m_autostop_data.state = 0;
			autostop::m_autostop_data.standby = true;
		}

	}
}


void autostop::auto_slow_down( c_usercmd* cmd ) {
	if ( !variables::ragebot::auto_slow_down )
		return;
	for ( auto ent : ragebot::targets )
	{


		vec3_t EnemyHead = { ent->origin( ).x, ent->origin( ).y, ( ent->get_hitbox_position( 0 ).z + 10.f ) };

		vec3_t Head = { csgo::local_player->origin( ).x, csgo::local_player->origin( ).y, ( csgo::local_player->get_hitbox_position( 0 ).z + 10.f ) };
		vec3_t HeadExtr = ( Head + ( csgo::local_player->velocity( ) * math::ticks_to_time(16 ) ) );
		vec3_t OriginExtr = ( ( csgo::local_player->origin( ) + ( csgo::local_player->velocity( ) * math::ticks_to_time( 16 ) ) ) + vec3_t( 0, 0, 8 ) );

		if ( fabs( csgo::local_player->velocity( ).Length2D( ) ) > .1f && ( autowall::can_hit_float_point( HeadExtr, EnemyHead ) || autowall::can_hit_float_point( OriginExtr, EnemyHead ) ) )
		{
			slow_walk( cmd, true );
			interfaces::console->console_printf( "[AUTO SLOW WALK] TRIGGERED. \n" );
		}
	}

}
