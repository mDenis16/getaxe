#include "../features.hpp"
void misc::movement::slow_walk( c_usercmd* cmd )
{
	if (  !interfaces::inputsystem->is_button_down( button_code_t::KEY_LSHIFT ) )
		return;
	constexpr bool isHexagoneGodlike = true;
	if ( !csgo::local_player )
		return;

	const auto weapon = csgo::local_player->active_weapon( );

	if ( !weapon )
			return;
	const auto weapon_data = weapon->get_weapon_data( );
	if ( !weapon_data )
		return;



	vec3_t viewangle = vec3_t( ); interfaces::engine->get_view_angles( viewangle );

	vec3_t velocity = csgo::local_player->velocity( );
	vec3_t direction;

	math::vector_angles( velocity, direction );

	float speed = velocity.Length2D( );

	direction.y = viewangle.y - direction.y;

	vec3_t forward = math::angle_vector( direction );

	vec3_t source = forward * -speed;

	if ( speed >= ( weapon_data->flMaxSpeed * 0.0034f ) )
	{
		cmd->forwardmove = source.x;
		cmd->sidemove = source.y;

	}
}

void misc::movement::auto_strafer( c_usercmd* cmd ) {


	if ( !csgo::local_player )
		return;

	if ( csgo::local_player->flags( ) & fl_onground && !( cmd->buttons & in_jump ) )
		return;

	if ( csgo::local_player->move_type( ) & ( movetype_ladder | movetype_noclip ) )
		return;

	if ( cmd->mousedx < 0 )
		cmd->sidemove - 450.0f;
	else if ( cmd->mousedx > 0 )
		cmd->sidemove = 450.0f;
}
/*
 if (  !interfaces::inputsystem->is_button_down( button_code_t::KEY_LSHIFT ) )
		return;
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
	vec3_t ndir = vec3_t( ); math::vector_angles( ( hvel * -1.f ), ndir );
	ndir.y = cmd->viewangles.y - ndir.y; // Relative to local view
	ndir = math::angle_vector( ndir );

	cmd->forwardmove = ndir.x * wishspeed;
	cmd->sidemove = ndir.y * wishspeed;*/
void misc::movement::strafe( c_usercmd* cmd ) {

	float speed = csgo::local_player->velocity( ).Length2D( );
	auto velocity = csgo::local_player->velocity( );
	float yawVelocity = RAD2DEG( atan2( velocity.y, velocity.x ) );
	float velocityDelta = math::normalize_yaw( cmd->viewangles.y - yawVelocity );
	static float sideSpeed = interfaces::console->get_convar( "cl_sidespeed" )->get_float( );

	if ( fabsf( cmd->mousedx > 2 ) ) {

		cmd->sidemove = ( cmd->mousedx < 0.f )?-sideSpeed:sideSpeed;
		return;
	}
	auto m_buttons = cmd->buttons;
	auto pressing_move = ( m_buttons & ( in_left ) || m_buttons & ( in_forward ) || m_buttons & ( in_back ) ||
		m_buttons & ( in_right ) || m_buttons & ( in_moveleft ) || m_buttons & ( in_moveright ) ||
		m_buttons & ( in_jump ) );

	// disable strafing while pressing shift.
	if ( ( cmd->buttons & in_speed ) || ( csgo::local_player->flags ( ) & fl_onground ) )
		return;

	float wish_dir { };
	if ( pressing_move ) {
		// took this idea from stacker, thank u !!!!
		enum EDirections {
			FORWARDS = 0,
			BACKWARDS = 180,
			LEFT = 90,
			RIGHT = -90,
			BACK_LEFT = 135,
			BACK_RIGHT = -135
		};



		// get our key presses.
		bool holding_w = interfaces::inputsystem->is_button_down(button_code_t::KEY_W);
		bool holding_a = interfaces::inputsystem->is_button_down ( button_code_t::KEY_A );
		bool holding_s = interfaces::inputsystem->is_button_down ( button_code_t::KEY_S );
		bool holding_d = interfaces::inputsystem->is_button_down ( button_code_t::KEY_D );

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
	}
	printf ( "wish_dir %f\n", wish_dir );
		csgo::m_strafe_angles.y += math::normalize_yaw ( wish_dir );
	if ( !speed > 0.5f || speed == NAN || speed == INFINITE ) {

		cmd->forwardmove = 450.f;
		return;
	}

	cmd->forwardmove = std::clamp( 5850.f / speed, -450.f, 450.f );

	if ( ( cmd->forwardmove < -450.f || cmd->forwardmove > 450.f ) )
		cmd->forwardmove = 0.f;

	cmd->sidemove = ( velocityDelta > 0.0f )?-sideSpeed:sideSpeed;
	csgo::m_strafe_angles.y = math::normalize_yaw( csgo::m_strafe_angles.y );
}
void misc::movement::bunny_hop(c_usercmd* cmd) {
	

	static bool bLastJumped = false;
	static bool bShouldFake = false;

	if ( !bLastJumped && bShouldFake ) {
		bShouldFake = false;
		cmd->buttons |= in_jump;
	}
	else if ( cmd->buttons & in_jump ) {
		if ( csgo::local_player->flags ( ) & fl_onground ) {
			bShouldFake = bLastJumped = true;
		}
		else {
			cmd->buttons &= ~in_jump;
			bLastJumped = false;
		}
	}
	else {
		bShouldFake = bLastJumped = false;
	}
	if ( csgo::local_player->move_type() == movetype_noclip || csgo::local_player->move_type ( )  == movetype_ladder )
		return;

	if ( cmd->buttons & in_speed && csgo::local_player->velocity ( ).Length2D ( ) < 1.f )
		return; // doesn't allow strafe when you hold shift and you're not moving

	static float yaw_add = 0.f;
	static const auto cl_sidespeed = interfaces::console->get_convar ( "cl_sidespeed" );
	if ( !( csgo::local_player->flags ( ) & fl_onground ) ) {
		bool back = cmd->buttons & in_back;
		bool forward = cmd->buttons & in_forward;
		bool right = cmd->buttons & in_moveleft;
		bool left = cmd->buttons & in_moveright;

		if ( back ) {
			yaw_add = -180.f;
			if ( right )
				yaw_add -= 45.f;
			else if ( left )
				yaw_add += 45.f;
		}
		else if ( right ) {
			yaw_add = 90.f;
			if ( back )
				yaw_add += 45.f;
			else if ( forward )
				yaw_add -= 45.f;
		}
		else if ( left ) {
			yaw_add = -90.f;
			if ( back )
				yaw_add -= 45.f;
			else if ( forward )
				yaw_add += 45.f;
		}
		else {
			yaw_add = 0.f;
		}

		csgo::m_strafe_angles.y += yaw_add;
		cmd->forwardmove = 0.f;
		cmd->sidemove = 0.f;

		const auto delta = math::normalize_yaw ( csgo::m_strafe_angles.y - RAD2DEG ( atan2 ( csgo::local_player->velocity().y, csgo::local_player->velocity ( ).x ) ) );

		cmd->sidemove = delta > 0.f ? -cl_sidespeed->get_float ( ) : cl_sidespeed->get_float ( );

		csgo::m_strafe_angles.y = math::normalize_yaw ( csgo::m_strafe_angles.y - delta );
	}
};
void misc::thirdperson::pre_framestagenotify(  ) {


	if ( !interfaces::engine->is_in_game( ) )
		return;
	if ( !interfaces::engine->is_connected( ) )
		return;
	if ( !csgo::local_player || !csgo::local_player->is_alive( ) )
		return;

	if ( !csgo::cmd )
		return;


	//math::clamp( csgo::real_angle );

	//if ( csgo::local_player &&  csgo::local_player->is_alive() && csgo::local_player->health() > 0 && interfaces::input->m_fCameraInThirdPerson )
//		csgo::local_player->set_local_viewangles( csgo::real_angle );
}
void misc::thirdperson::think( ) {
	if ( !csgo::local_player )
		return;
	if ( !interfaces::engine->is_in_game( ) )
		return;
	if ( !interfaces::engine->is_connected( ) )
		return;

	static bool enabled = false;
	static bool was_in_thirdperson = false;
	static auto thirdperson = false;

	thirdperson = true;

	static auto pressed = false;

	static auto key = false;

	if ( interfaces::inputsystem->is_button_down( button_code_t::MOUSE_MIDDLE ) && !pressed )
		pressed = true;
	else if ( !interfaces::inputsystem->is_button_down( button_code_t::MOUSE_MIDDLE ) && pressed ) {
		pressed = false;
		key = !key;
	}

	if ( key )
		thirdperson = false;


	static vec3_t vecAngles;
	interfaces::engine->get_view_angles( vecAngles );

	if ( thirdperson && csgo::local_player->is_alive( ) ) {
		if ( !interfaces::input->m_camera_in_third_person ) {
			auto get_correct_position = [ & ] ( float ideal_distance ) -> vec3_t {
				vec3_t viewAngles;
				interfaces::engine->get_view_angles( viewAngles );
				trace_t tr;
				ray_t traceRay;
				vec3_t eyePos = csgo::local_player->get_eye_pos( );

				vec3_t camOff = vec3_t( cos( DEG2RAD( viewAngles.y ) ) * ideal_distance,
					sin( DEG2RAD( viewAngles.y ) ) * ideal_distance,
					sin( DEG2RAD( -viewAngles.x ) ) * ideal_distance );

				traceRay.initialize( eyePos, ( eyePos - camOff ) );
				trace_filter_skip_one_entity traceFilter( csgo::local_player );

				interfaces::trace_ray->trace_ray( traceRay, MASK_SOLID, &traceFilter, &tr );


				return vec3_t( viewAngles.x, viewAngles.y, ideal_distance * ( ( tr.flFraction < 1.0f )?tr.flFraction:1.0f ) );
			};

			interfaces::input->m_camera_in_third_person = true;
			interfaces::input->m_camera_offset = get_correct_position( 150.f );
		}
	}
	else {
		interfaces::input->m_camera_in_third_person = false;
		interfaces::input->m_camera_offset = vec3_t( vecAngles.x, vecAngles.y, 0 );
	}

}

void misc::removals::remove_smoke( ) {
	if ( !variables::misc::removals::smoke )
		return;

	if ( !interfaces::engine->is_connected( ) && !interfaces::engine->is_in_game( ) )
		return;

	static auto smoke_count = *reinterpret_cast< int** >( utilities::pattern_scan( "client.dll", "A3 ? ? ? ? 57 8B CB" ) + 1 );

	static std::vector<const char*> smoke_materials = {
		"particle/vistasmokev1/vistasmokev1_fire",
		"particle/vistasmokev1/vistasmokev1_smokegrenade",
		"particle/vistasmokev1/vistasmokev1_emods",
		"particle/vistasmokev1/vistasmokev1_emods_impactdust"
	};

	for ( auto material_name : smoke_materials ) {
		i_material* smoke = interfaces::material_system->find_material( material_name, TEXTURE_GROUP_OTHER );
		smoke->increment_reference_count( );
		smoke->set_material_var_flag( material_var_wireframe, true );

		*( int* ) smoke_count = 0;
	}
}
void misc::removals::remove_flash( ) {
	auto local_player = reinterpret_cast< player_t* >( interfaces::entity_list->get_client_entity( interfaces::engine->get_local_player( ) ) );
	if ( !local_player )
		return;

	if ( local_player->flash_duration( ) > 0.0f )
		local_player->flash_duration( ) = 0.0f;
}