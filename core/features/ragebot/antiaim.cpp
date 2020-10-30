#include "../features.hpp"

bool need_to_flip = false;
bool choke_next_tick;
std::vector<anti_aim::freestand_point> anti_aim::points;
float get_curtime( c_usercmd* ucmd ) {
	auto local_player = csgo::local_player;

	if ( !local_player )
		return 0;

	int g_tick = 0;
	c_usercmd* g_pLastCmd = nullptr;
	if ( !g_pLastCmd || g_pLastCmd->hasbeenpredicted ) {
		g_tick = ( float ) local_player->get_tick_base( );
	}
	else {
		++g_tick;
	}
	g_pLastCmd = ucmd;
	float curtime = g_tick * interfaces::globals->interval_per_tick;
	return curtime;
}
inline void UpdateAnimationState( anim_state* state, vec3_t ang ) {
	using fn = void( __vectorcall* )( void*, void*, float, float, float, void* );
	static auto ret = reinterpret_cast< fn >( utilities::pattern_scan( "client.dll", ( "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24" ) ) );

	if ( !ret || !state )
		return;

	ret( state, nullptr, 0.f, ang.y, ang.x, nullptr );
}
void anti_aim::real_animation_fix( ) {
	const auto local = reinterpret_cast < player_t* > ( interfaces::entity_list->get_client_entity( interfaces::engine->get_local_player( ) ) );

	if ( !local || !local->is_alive( ) )
		return;

	auto state = local->get_anim_state( );
	if ( !state ) {
		return;
	}

	if ( !interfaces::clientstate ) {
		return;
	}

	// allow re-animating in the same frame.
	if ( state->m_iLastClientSideAnimationUpdateFramecount == interfaces::globals->frame_count ) {
		state->m_iLastClientSideAnimationUpdateFramecount -= 1;
	}

	// update anim update delta as server build.
	state->m_flUpdateTimeDelta = max( 0.0f, interfaces::globals->cur_time - state->m_flLastClientSideAnimationUpdateTime ); // negative values possible when clocks on client and server go out of sync..

	if ( csgo::m_animate ) {
		// get layers.
		std::memcpy( csgo::m_real_layers, local->get_anim_overlays( ), sizeof( animationlayer ) * 13 );

		// allow the game to update animations this tick.
		csgo::m_update = true;

		//interfaces::console->console_printf( "update animstate yaw %f \n", csgo::real_angle.y );
		// update animations.

		UpdateAnimationState( state, csgo::m_angle );
		local->update_client_side_animations( );

		// disallow the game from updating animations this tick.
		csgo::m_update = false;

		// save data when our choke cycle resets.
		if ( !interfaces::clientstate->m_choked_commands ) {
			csgo::m_rotation.y = state->m_flGoalFeetYaw;

			std::memcpy( csgo::m_real_poses, local->m_flPoseParameter( ), sizeof( float ) * 24 );
		}

		// remove model sway.
		csgo::m_real_layers [ 12 ].weight = 0.f;

		// make sure to only animate once per tick.
		csgo::m_animate = false;
	}

	std::memcpy( local->get_anim_overlays( ), csgo::m_real_layers, sizeof( animationlayer ) * 13 );
	std::memcpy( local->m_flPoseParameter( ), csgo::m_real_poses, sizeof( float ) * 24 );


	if (!csgo::m_rotation.is_zero() )
	local->set_abs_angles( csgo::m_rotation );
}
void anti_aim::event_logs::bullet_impact( i_game_event* event ) {
	auto userid = event->get_int( "userid" );
	if ( !userid )
		return;

	if ( !interfaces::engine->is_connected( ) && !interfaces::engine->is_in_game( ) )
		return;

	auto engine_userid = interfaces::engine->get_player_for_user_id( userid );
	if ( !engine_userid )
		return;
	if ( engine_userid == interfaces::engine->get_local_player( ) )
		return;

	auto entity = reinterpret_cast< player_t* >( interfaces::entity_list->get_client_entity( engine_userid ) );

	if ( !entity )
		return;
//	if ( !entity->is_alive( ) )
///		return;
///	if ( entity->dormant( ) )
//		return;
	
//	if (!entity->is_enemy( ) )
	//	return;


	vec3_t bullet_impact = vec3_t( event->get_float( "x" ), event->get_float( "y" ), event->get_float( "z" ) );

	auto angle_bullet = math::calc_angle( entity->get_eye_pos( ), bullet_impact );
	auto angle_head = math::calc_angle( entity->get_eye_pos( ), csgo::local_player->get_hitbox_position( hitbox_head ) );

	vec3_t angDelta = angle_head - angle_bullet;
	angDelta.y += 180;
	if ( angDelta.y > 360 ) angDelta.y = fmodf( angDelta.y, 360 );
	angDelta.y -= 180;
	float fovDifference = angDelta.Length2D( );
	interfaces::console->console_printf( "[ANTI BRUTE] Bullet fov to you %f \n ", fovDifference );

}
bool should_predict( )
{

	float server_time = get_curtime( csgo::cmd );
	static float predirect_time = 0.f;
	static bool initialized;
	bool will_update = false;

	if ( !initialized && !csgo::local_player->is_moving( ) )
	{
		initialized = true;
		predirect_time = server_time + 0.22f;
	}
	else if ( csgo::local_player->is_moving( ) )
	{
		initialized = false;
	}

	if ( server_time >= ( predirect_time ) && csgo::local_player->flags( ) & fl_onground )
	{
		predirect_time = server_time + 1.1f;
		will_update = true;
	}
	return will_update;
}

void anti_aim::update_local_animations( ) {
	if ( !csgo::cmd )
		return;

	auto state = csgo::local_player->get_anim_state( );
	if ( !state ) {
		return;
	}

	if ( !interfaces::clientstate ) {
		return;
	}
	if ( !interfaces::engine )
	{
		return;
	}
	auto nci = interfaces::engine->get_net_channel( );
	if ( !nci )
		return;


	// allow re-animating in the same frame.
	if ( state->m_iLastClientSideAnimationUpdateFramecount == interfaces::globals->frame_count ) {
		state->m_iLastClientSideAnimationUpdateFramecount -= 1;
	}

	// update anim update delta as server build.
	state->m_flUpdateTimeDelta = max( 0.0f, interfaces::globals->cur_time - state->m_flLastClientSideAnimationUpdateTime ); // negative values possible when clocks on client and server go out of sync..
	static int last_tick = 0;
	if (!csgo::local_player->get_anim_overlays( ))
		return;
	static vec3_t angle = vec3_t( );
	

		csgo::local_player->update_client_side_animations( );

	
		float lby_delta = csgo::local_player->lower_body_yaw( ) - csgo::real_angle.y;
		lby_delta = std::remainderf( lby_delta, 360.f );
		lby_delta = std::clamp( lby_delta, -60.f, 60.f );

		float feet_yaw = std::remainderf( csgo::real_angle_static.y + lby_delta, 360.f );

		if ( feet_yaw < 0.f ) {
			feet_yaw += 360.f;
		}
		//csgo::local_player->get_anim_state( )->m_flGoalFeetYaw
		//UpdateAnimationState( state, csgo::real_angle_static );
	///	if (!csgo::send_packet )
	///		angle = csgo::cmd->viewangles;
		//interfaces::console->console_printf( "angleY %f \n", csgo::real_angle_static.y );

	//csgo::local_player->set_abs_angles(vec3_t(0, angle.y, 0)  );

}
float anti_aim::get_freestanding_angle( ) {
	static float last_angle = 0.f; static float last_angle_change_time = 0.f;

	if ( ( interfaces::globals->cur_time - last_angle_change_time ) < 0.4f )
		return last_angle;


	points.clear( );

	vec3_t position = csgo::local_player->origin( );
	position.z = csgo::local_player->get_bone_position( hitbox_body ).z;


	float radius = 50.f;
	float step = M_PI * 2.0 / 8;

	for ( float a = 0; a < ( M_PI * 2.0 ); a += step )
	{
		vec3_t location( radius * cos( a ) + position.x, radius * sin( a ) + position.y, position.z );

		ray_t ray;
		trace_t tr;
		ray.initialize( position, location );
		trace_filter_skip_one_entity traceFilter( csgo::local_player );

		interfaces::trace_ray->trace_ray( ray, 0x4600400B, &traceFilter, &tr );
		float tickness = autowall::get_thickness( position, location );
		freestand_point point;
		point.start = tr.start;
		point.end = tr.end;
		point.tickness = tickness;
		point.angle = RAD2DEG( a );
		points.push_back( point );

	}


	float lowest_tickness = 0.f; int lowest_number = 0;
	for ( size_t i = 0; i < points.size( ); i++ )
	{
		if ( points [ i ].tickness > lowest_tickness )
		{
			lowest_tickness = points [ i ].tickness;

			last_angle = points [ i ].angle;
			lowest_number = i;
			

		}
	}points [ lowest_number ].original = true;
	
	if ( last_angle < 180.f && last_angle > 45.f )
	{
		last_angle = points [ lowest_number - 1 ].angle;
		points [ lowest_number - 1 ].selected = true;
	}
	else if ( ( int ) ( last_angle ) == 314 )
	{
		
		last_angle = points [ lowest_number - 1 ].angle;
		points [ lowest_number - 1 ].selected = true;
	}
	else {
		last_angle = points [ lowest_number + 1 ].angle;
		points [ lowest_number + 1 ].selected = true;
	}
	/*if ( lowest_tickness < 100.f )
	{
		vec3_t viewangle = vec3_t( );
		static vec3_t temp_angle = vec3_t( );
		interfaces::engine->get_view_angles( viewangle );
		viewangle.y += 180.f;
		viewangle.angle_normalize( ); viewangle.angle_clamp( );
		last_angle = viewangle.y;
	}*/
	last_angle_change_time = interfaces::globals->cur_time;

	return last_angle;
}
bool anti_aim::allow( c_usercmd* ucmd, bool& send_packet ) {


	auto local = csgo::local_player;
	if ( !local && !local->is_alive( ) )
		return false;

	const auto move_type = local->move_type( );
	if ( move_type == ( int ) movetype_ladder || move_type == ( int ) movetype_noclip )
		return false;

	if ( ucmd->buttons & ( int ) in_use )
		return false;

	auto weapon = local->active_weapon( );
	if ( !weapon )
		return false;

	if ( weapon->is_knife( ) ) {
		float next_secondary_attack = weapon->next_secondary_attack( ) - interfaces::globals->cur_time;
		float next_primary_attack = weapon->next_primary_attack( ) - interfaces::globals->cur_time;

		if ( ucmd->buttons & ( int ) in_attack && next_primary_attack <= 0.f || ucmd->buttons & ( int ) in_attack2 && next_secondary_attack <= 0.f )
			return false;
	}

	if ( ucmd->buttons & ( int ) in_attack && local->can_shoot( false ) ) {
		choke_next_tick = true;
		send_packet = true;
		return false;
	}
	
	if ( weapon->is_grenade( ) ) {
		if ( !weapon->pin_pulled( ) || ( ucmd->buttons & in_attack ) || ( ucmd->buttons & in_attack2 ) )
		{
			float throwTime = weapon->throw_time( );

			if ( ( throwTime > 0 ) && ( throwTime < interfaces::globals->cur_time ) )
				return false;
		}
	}


	return true;
}
void anti_aim::freestanding_desync( c_usercmd* cmd, float& dirrection, player_t* p_entity, bool& send_packet, float max_desync ) {
	std::vector<angle_data> points;
	int side = 0;

	if ( !cmd )
		return;

	const auto local = csgo::local_player;

	if ( !local )
		return;

	if ( !local->is_alive( ) )
		return;

	const auto local_position = local->get_eye_pos( );



	if ( !p_entity ) return;

	const auto view = math::calc_angle( local_position, p_entity->get_eye_pos( ) );



	auto enemy_eyes = p_entity->get_eye_pos( );

	const auto head = csgo::local_player->get_hitbox_position( hitbox_head );

	float radius = 100.f;

	for ( float a = 0; a < ( M_PI * 2.0 ); a += M_PI * 2.0 / 16 )
	{
		vec3_t location( radius * cos( a ) + head.x, radius * sin( a ) + head.y, head.z );

		ray_t ray;
		trace_t trace;
		ray.initialize( head, location );
		trace_filter_skip_one_entity traceFilter( csgo::local_player );


		interfaces::trace_ray->trace_ray( ray, 0x4600400B, &traceFilter, &trace );
		//debug_overlay->add_line_overlay( head, trace.end_pos, 255, 0, 0, 0, 0.1f );
		float tickness = autowall::get_thickness( enemy_eyes, trace.end );
		//	debug_overlay->add_text_overlay( trace.end, 0.1f, std::to_string( tickness ).c_str( ) );
		points.push_back( angle_data( math::clamp_yaw( a * ( 180.0 / 3.141 ) ), tickness, interfaces::globals->cur_time, a ) );

	}


	float l1 = 0.0f; float l2 = 999.f; float lowest_angle = 0.0f; float highest_angle = 0.0f;
	for ( auto& i : points )
	{
		if ( i.thickness != 0.f )
		{
			if ( i.thickness > l1 )
			{
				l1 = i.thickness;
				lowest_angle = i.angle;
			}
			if ( i.thickness < l2 )
			{
				l2 = i.thickness;
				highest_angle = i.angle;
			}
		}

	}

	float yaw = highest_angle - lowest_angle;

	if ( yaw > 0 )
		side = 0;
	else if ( yaw < 0 )
		side = 1;
	yaw = fabs( yaw ); std::clamp( yaw, 0.f, max_desync );

	if ( send_packet )
	{
		if ( side == 1 )
			dirrection = lowest_angle;
		else if ( side == 2 )
			dirrection = highest_angle;
	}
	else
	{
		if ( side == 1 )
		{
			if ( should_predict( ) )
				dirrection = lowest_angle - max_desync - 120.f;
			else
				dirrection = lowest_angle - max_desync;
		}
		else if ( side == 2 )
		{
			if ( should_predict( ) )
				dirrection = highest_angle + max_desync + 120.f;
			else
				dirrection = highest_angle + max_desync;
		}
	}

}
player_t* get_target( ) {
	int index = -1;
	float min_fov = 180.f;
	player_t* target = nullptr;



	vec3_t local_position = csgo::local_player->get_eye_pos( );
	vec3_t angles; interfaces::engine->get_view_angles( angles );

	for ( int i = 1; i <= interfaces::globals->max_clients; i++ ) {
		auto entity = reinterpret_cast< player_t* >( interfaces::entity_list->get_client_entity( i ) );

		if ( !entity )
			continue;

		if ( entity->index( ) == csgo::local_player->index( ) )
			continue;

		if ( entity->dormant( ) )
			continue;

		if ( !csgo::player_bones [ entity->index( ) ] )
			continue;

		if ( entity->health( ) <= 0 )
			continue;

		if ( entity->team( ) == csgo::local_player->team( ) )
			continue;

		vec3_t viewangle = vec3_t( ); interfaces::engine->get_view_angles( viewangle );

		auto angle = math::calc_angle( csgo::local_player->get_eye_pos( ), entity->get_eye_pos( ) );
		auto fov = math::get_fov( viewangle, angle );
		if ( fov < min_fov ) {
			min_fov = fov;
			index = i;
			target = entity;
		}

	}

	return target;
}
int anti_aim::get_desync_dirrection( ) {
	float Back, Right, Left;

	vec3_t src3D, dst3D, forward, right, up, src, dst;
	trace_t tr;
	ray_t ray, ray2, ray3, ray4, ray5;
	trace_filter_skip_one_entity filter(csgo::local_player);

	vec3_t engineViewAngles;
	interfaces::engine->get_view_angles( engineViewAngles );

	engineViewAngles.x = 0;

	math::angle_vectors ( engineViewAngles, &forward, &right, &up );

	src3D = csgo::local_player->get_eye_pos( );
	dst3D = src3D + ( forward * 384 );

	ray.initialize( src3D, dst3D );

	 interfaces::trace_ray->trace_ray( ray, MASK_SHOT, &filter, &tr );

	Back = ( tr.end - tr.start ).length( );

	ray2.initialize( src3D + right * 35, dst3D + right * 35 );

	interfaces::trace_ray->trace_ray( ray2, MASK_SHOT, &filter, &tr );

	Right = ( tr.end - tr.start ).length( );

	ray3.initialize( src3D - right * 35, dst3D - right * 35 );

	interfaces::trace_ray->trace_ray( ray3, MASK_SHOT, &filter, &tr );

	Left = ( tr.end - tr.start ).length( );

	if ( Left > Right )
	{
		return 0;
	}
	else if ( Right > Left )
	{
		return 1;
	}
	else if ( Back > Right || Back > Left )
	{
		return 2;
	}
	return 0;
}
void anti_aim::on_create_move( c_usercmd* cmd, bool& send_packet )
{
	auto weapon = csgo::local_player->active_weapon( );

	if ( !interfaces::engine->is_connected( ) || !interfaces::engine->is_in_game( ) )
		return;

	if ( !csgo::local_player->is_alive( ) )
		return;

	if ( !csgo::local_player->active_weapon( ) )
		return;

	if ( cmd->buttons & in_use )
		return;




	if ( !weapon )
		return;

	if ( !allow( cmd, send_packet ) )
		return;


	switch ( variables::antiaim::pitch ) {
	case 1: cmd->viewangles.x = 89.9f; break;
	case 2: cmd->viewangles.x = 0; break;
	case 3: cmd->viewangles.x = -89.9f; break;
	}

	static bool dir = false;
	static int tick = 0;
	if ( interfaces::inputsystem->is_button_down( button_code_t::KEY_V ) && tick != interfaces::globals->tick_count )
	{
		dir = !dir;
		tick = interfaces::globals->tick_count;
		printf( "changed dirrection \n" );
	}
/*	if ( autostop::m_autostop_data.state == 0 ) {
		dir = !dir;
	}*/
	static bool jitter = false;
	jitter = !jitter;
	switch ( variables::antiaim::yaw ) {
	case 1:
	{
		vec3_t viewangle = vec3_t( );
		static vec3_t temp_angle = vec3_t( );
		interfaces::engine->get_view_angles( viewangle );
		//viewangle.y += 180.f;
		viewangle.y = get_freestanding_angle( );
		viewangle.angle_normalize( ); viewangle.angle_clamp( ); cmd->viewangles.y = viewangle.y;
	

		if ( jitter )
			cmd->viewangles.y += 120.f;
		else
			cmd->viewangles.y -= 120.f;

		if ( should_predict( ) ) {
			if ( dir )
				cmd->viewangles.y -= 120;
			else
				cmd->viewangles.y += 120;
			send_packet = false;
		
		}
		else if ( !send_packet ) {
			if ( dir )
				cmd->viewangles.y += 120.f;
			else
				cmd->viewangles.y -= 120.f;
		}

		if ( fabsf( cmd->sidemove ) < 2.0f ) {
			cmd->sidemove = cmd->tick_count % 2?1.10:-1.10;
		}
	}

	break;
	case 2:
	{
		vec3_t viewangle = vec3_t( );
		static vec3_t temp_angle = vec3_t( );
		interfaces::engine->get_view_angles( viewangle );
		

			viewangle.y += 180.f;
	

		viewangle.angle_normalize( ); viewangle.angle_clamp( ); 
		cmd->viewangles.y = viewangle.y;
	

		if ( should_predict( ) ) {
			if ( !dir )
				cmd->viewangles.y -= 120;
			else
				cmd->viewangles.y += 120;
			send_packet = false;
		}
		else if ( !send_packet ) {
			if ( !dir )
				cmd->viewangles.y -= cmd->command_number % 2?30:60;
			else
				cmd->viewangles.y += cmd->command_number % 2?30:60;
		}

		if ( fabsf( cmd->sidemove ) < 2.0f ) {
			cmd->sidemove = cmd->tick_count % 2?1.10:-1.10;
		}

	}
	break;
	case 3:
	{
		vec3_t viewangle = vec3_t( );
		static vec3_t temp_angle = vec3_t( );
		interfaces::engine->get_view_angles( viewangle );
		viewangle.y += 180.f;
		viewangle.angle_normalize( ); viewangle.angle_clamp( ); cmd->viewangles.y = viewangle.y;
		cmd->viewangles.y += variables::antiaim::inverted_body_lean;
		if ( variables::antiaim::antiaimAfterShoot && csgo::local_player->active_weapon( ) ) {
			int ticks_after_shot = interfaces::globals->tick_count - math::time_to_ticks( csgo::local_player->active_weapon( )->m_fLastShotTime( ) );
			if ( ticks_after_shot > 0 && ticks_after_shot <= 22 )
			{
				cmd->viewangles.y -= cmd->tick_count % 2?120:60;
			}
		}

		if ( send_packet ) {

			cmd->viewangles.y += 120.f;

		}

		if ( fabsf( cmd->sidemove ) < 2.0f ) {
			cmd->sidemove = cmd->tick_count % 2?1.10:-1.10;
		}

	}
	break;
	}
	

	
	cmd->viewangles.angle_normalize( );
	cmd->viewangles.angle_clamp( );

	//if ( c_config::get( ).leg )
	cmd->buttons &= ~( in_forward | in_back | in_moveright | in_moveleft );

}
void anti_aim::fix_call_animation( ) {
	const auto local = csgo::local_player;
	if ( !local )
		return;
	if ( !local->is_alive( ) )
		return;
	if ( local->health( ) <= 0 )
		return;
	auto state = csgo::local_player->get_anim_state( );
	if ( !state )
		return;
	if ( !csgo::cmd )
		return;

	static float proper_abs = local->get_anim_state( )->AbsYaw( );
	static animationlayer backup_layers [ 13 ];
	static int last_tick = 0;
	static std::array<float, 24> sent_pose_params;

	if ( interfaces::globals->tick_count != last_tick ) // Only update animations each tick, though we are doing this each frame.
	{
		std::memcpy( backup_layers, local->get_anim_overlays( ), ( sizeof( animationlayer ) * 13 ) );
		local->get_clientside_animation( ) = true;
		local->update_state( local->get_anim_state( ), csgo::cmd->viewangles );
		if ( csgo::send_packet )
		{
			proper_abs = local->get_anim_state( )->AbsYaw( );
			sent_pose_params = local->get_poseparameter( );
		}
		last_tick = interfaces::globals->tick_count;
	}
	local->get_clientside_animation( ) = false;
	csgo::local_player->set_abs_angles( vec3_t( 0, proper_abs, 0 ) );
	local->get_anim_state( )->m_flMovingFraction = 0.f; // Lol.
	std::memcpy( local->get_anim_overlays( ), backup_layers, ( sizeof( animationlayer ) * 13 ) );
	local->get_poseparameter( ) = sent_pose_params;
}

