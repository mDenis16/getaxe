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

	if ( csgo::local_player->flags( ) & fl_onground && !( cmd->buttons & in_jump ) )
		return;
	float speed = csgo::local_player->velocity( ).Length2D( );
	auto velocity = csgo::local_player->velocity( );
	float yawVelocity = RAD2DEG( atan2( velocity.y, velocity.x ) );
	float velocityDelta = math::normalize_yaw( cmd->viewangles.y - yawVelocity );
	static float sideSpeed = interfaces::console->get_convar( "cl_sidespeed" )->get_float( );

	if ( fabsf( cmd->mousedx > 2 ) ) {

		cmd->sidemove = ( cmd->mousedx < 0.f )?-sideSpeed:sideSpeed;
		return;
	}

	if ( cmd->buttons & in_back )
		cmd->viewangles.y -= 180.f;
	else if (cmd->buttons & in_moveleft )
		cmd->viewangles.y -= 90.f;
	else if (cmd->buttons & in_moveright )
		cmd->viewangles.y += 90.f;

	if ( !speed > 0.5f || speed == NAN || speed == INFINITE ) {

		cmd->forwardmove = 450.f;
		return;
	}

	cmd->forwardmove = std::clamp( 5850.f / speed, -450.f, 450.f );

	if ( ( cmd->forwardmove < -450.f || cmd->forwardmove > 450.f ) )
		cmd->forwardmove = 0.f;

	cmd->sidemove = ( velocityDelta > 0.0f )?-sideSpeed:sideSpeed;
	cmd->viewangles.y = math::normalize_yaw( cmd->viewangles.y - velocityDelta );
}
void misc::movement::bunny_hop(c_usercmd* cmd) {
	

	static bool last_jumped = false, should_fake = false;

	if (!last_jumped && should_fake) {
		should_fake = false;
		cmd->buttons |= in_jump;
	}
	else if (cmd->buttons & in_jump) {
		if (csgo::local_player->flags() & fl_onground) {
			last_jumped = true;
			should_fake = true;
		}
		else {
			cmd->buttons &= ~in_jump;
			last_jumped = false;
		}
	}
	else {
		last_jumped = false;
		should_fake = false;
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