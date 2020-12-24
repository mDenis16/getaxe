#include "../features.hpp"

bool need_to_flip = false;
bool choke_next_tick;
 static float last_hp = 100;
 static bool desync_side = false;
std::vector<anti_aim::freestand_point> anti_aim::points;
float get_curtime( c_usercmd* ucmd ) {
	auto local_player = local_player::m_data.pointer;

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

static int last_user_id = -1;
static int shots_on_me = 0;
static float last_angle_bullet_impact = 0.0f;

void anti_aim::event_logs::bullet_impact( i_game_event* event ) {
	static int last_time_shoot[64];
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
	if ( !entity->is_alive( ) )
		return;
	if ( entity->dormant( ) )
		return;

	if (!entity->is_enemy( ) )
		return;

   
	vec3_t bullet_impact = vec3_t( event->get_float( "x" ), event->get_float( "y" ), event->get_float( "z" ) );

	auto angle_bullet = math::calc_angle ( entity->get_eye_pos ( ), bullet_impact ); math::clamp ( angle_bullet );
	auto angle_head = math::calc_angle( entity->get_eye_pos( ), local_player::m_data.pointer->get_eye_pos() ); 

	math::clamp ( angle_head );
	

	auto angDelta = (angle_head - angle_bullet).Length2D();
	
	if ( angDelta < 3.0f && last_time_shoot [ entity->index ( ) ] != interfaces::globals->tick_count) {
		visuals::notifications::add ( "Enemy missed shot." );
		desync_side = !desync_side;
		last_time_shoot [ entity->index ( ) ] = interfaces::globals->tick_count;
	}

}

bool should_predict( )
{

	float server_time = get_curtime( csgo::cmd );
	static float predirect_time = 0.f;
	static bool initialized;
	bool will_update = false;

	if ( !initialized && !local_player::m_data.pointer->is_moving( ) )
	{
		initialized = true;
		predirect_time = server_time + 0.22f;
	}
	else if ( local_player::m_data.pointer->is_moving( ) )
	{
		initialized = false;
	}

	if ( server_time >= ( predirect_time ) && local_player::m_data.pointer->flags( ) & fl_onground )
	{
		predirect_time = server_time + 1.1f;
		will_update = true;
	}
	return will_update;
}

float anti_aim::get_freestanding_angle( ) {
	static float last_angle = 0.f; static float last_angle_change_time = 0.f;

	if ( ( interfaces::globals->cur_time - last_angle_change_time ) < 0.4f )
		return last_angle;


	points.clear( );

	vec3_t position = local_player::m_data.pointer->origin( );
	position.z = local_player::m_data.pointer->get_bone_position( hitbox_body ).z;


	float radius = 50.f;
	float step = M_PI * 2.0 / 8;

	for ( float a = 0; a < ( M_PI * 2.0 ); a += step )
	{
		vec3_t location( radius * cos( a ) + position.x, radius * sin( a ) + position.y, position.z );

		ray_t ray;
		trace_t tr;
		ray.initialize( position, location );
		trace_filter_skip_one_entity traceFilter( local_player::m_data.pointer );

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


	auto local = local_player::m_data.pointer;
	if ( !local && !local->is_alive( ) )
		return false;

	const auto move_type = local->move_type( );
	if ( move_type == ( int ) movetype_ladder || move_type == ( int ) movetype_noclip )
		return false;

	//if ( ucmd->buttons & ( int ) in_use )
		//return false;

	auto weapon = local->active_weapon( );
	if ( !weapon )
		return false;

	if ( weapon->is_knife( ) ) {
		float next_secondary_attack = weapon->next_secondary_attack( ) - interfaces::globals->cur_time;
		float next_primary_attack = weapon->next_primary_attack( ) - interfaces::globals->cur_time;

		if ( ucmd->buttons & ( int ) in_attack && next_primary_attack <= 0.f || ucmd->buttons & ( int ) in_attack2 && next_secondary_attack <= 0.f )
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
	else {

		if ( ucmd->buttons & ( int ) in_attack && local->can_shoot ( false ) ) {
			choke_next_tick = true;
			send_packet = true;
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

	const auto local = local_player::m_data.pointer;

	if ( !local )
		return;

	if ( !local->is_alive( ) )
		return;

	const auto local_position = local->get_eye_pos( );



	if ( !p_entity ) return;

	const auto view = math::calc_angle( local_position, p_entity->get_eye_pos( ) );



	auto enemy_eyes = p_entity->get_eye_pos( );

	const auto head = local_player::m_data.pointer->get_hitbox_position( hitbox_head );

	float radius = 100.f;

	for ( float a = 0; a < ( M_PI * 2.0 ); a += M_PI * 2.0 / 16 )
	{
		vec3_t location( radius * cos( a ) + head.x, radius * sin( a ) + head.y, head.z );

		ray_t ray;
		trace_t trace;
		ray.initialize( head, location );
		trace_filter_skip_one_entity traceFilter( local_player::m_data.pointer );


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



	vec3_t local_position = local_player::m_data.pointer->get_eye_pos( );
	vec3_t angles; interfaces::engine->get_view_angles( angles );

	for ( int i = 1; i <= interfaces::globals->max_clients; i++ ) {
		auto entity = reinterpret_cast< player_t* >( interfaces::entity_list->get_client_entity( i ) );

		if ( !entity )
			continue;

		if ( entity->index( ) == local_player::m_data.pointer->index( ) )
			continue;

		if ( entity->dormant( ) )
			continue;

		if ( !csgo::player_bones [ entity->index( ) ] )
			continue;

		if ( entity->health( ) <= 0 )
			continue;

		if ( entity->team( ) == local_player::m_data.pointer->team( ) )
			continue;

		vec3_t viewangle = vec3_t( ); interfaces::engine->get_view_angles( viewangle );

		auto angle = math::calc_angle( local_player::m_data.pointer->get_eye_pos( ), entity->get_eye_pos( ) );
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
	trace_filter_skip_one_entity filter(local_player::m_data.pointer);

	vec3_t engineViewAngles;
	interfaces::engine->get_view_angles( engineViewAngles );

	engineViewAngles.x = 0;

	math::angle_vectors ( engineViewAngles, &forward, &right, &up );

	src3D = local_player::m_data.pointer->get_eye_pos( );
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
	if ( !c_config::get ( ).antiaim_enable )
		return;

	auto weapon = local_player::m_data.pointer->active_weapon( );

	if ( !interfaces::engine->is_connected( ) || !interfaces::engine->is_in_game( ) )
		return;

	if ( !local_player::m_data.pointer->is_alive( ) )
		return;

	if ( !local_player::m_data.pointer->active_weapon( ) )
		return;

	//if ( cmd->buttons & in_use && cmd->buttons & in_speed )
		//return;




	if ( !weapon )
		return;

	if ( !allow( cmd, send_packet ) )
		return;

	static int last_tick_check = 0;
	if ( std::abs ( last_tick_check - interfaces::globals->tick_count ) > 64 ) {
		last_hp = local_player::m_data.pointer->health ( );
		last_tick_check = interfaces::globals->tick_count;
	}
	switch ( c_config::get().antiaim_pitch) {
	case 1: cmd->viewangles.x = 89.9f; break;
	case 2: cmd->viewangles.x = 0; break;
	case 3: cmd->viewangles.x = -89.9f; break;
	}
	if ( cmd->buttons & in_use )
		cmd->viewangles.x = 0;

	
	static int tick = 0;
	if ( interfaces::inputsystem->is_button_down( button_code_t::KEY_V ) && tick != interfaces::globals->tick_count )
	{
		desync_side = !desync_side;
		tick = interfaces::globals->tick_count;
		//printf( "changed dirrection \n" );
	}
/*	if ( autostop::m_autostop_data.state == 0 ) {
		dir = !dir;
	}*/

	switch ( config.antiaim_yaw ) {
	case 1:
	{
		vec3_t viewangle = vec3_t ( );
		static vec3_t temp_angle = vec3_t ( );
		interfaces::engine->get_view_angles ( viewangle );
		if ( !( cmd->buttons & in_use ) ) {

			viewangle.y -= 180.f;
			if ( aimbot::targets.size ( ) && aimbot::targets.front ( ).player ) {
				viewangle.y = math::calc_angle ( local_player::m_data.pointer->origin ( ), aimbot::targets.front ( ).player->origin ( ) ).y - 180.f;
			}
			viewangle.angle_normalize ( ); viewangle.angle_clamp ( );
		}

		if ( !desync_side )
			cmd->viewangles.y = viewangle.y;
		else
			cmd->viewangles.y = viewangle.y;

		

		if ( should_predict ( ) ) {
			if ( !desync_side )
				cmd->viewangles.y += 120;
			else
				cmd->viewangles.y -= 120;
			send_packet = false;
		}
		else if ( !send_packet ) {
			if ( !desync_side )
				cmd->viewangles.y = csgo::real_angle.y + ( local_player::m_data.pointer->max_desync ( false )  );
			else
				cmd->viewangles.y = csgo::real_angle.y - ( local_player::m_data.pointer->max_desync ( false )  );
		}
	}

	break;
	case 2:
	{
		vec3_t viewangle = vec3_t ( );
		static vec3_t temp_angle = vec3_t ( );
		interfaces::engine->get_view_angles ( viewangle );
		if ( !( cmd->buttons & in_use ) ) {

			viewangle.y -= 180.f;
			if ( aimbot::targets.size ( ) && aimbot::targets.front ( ).player ) {
				viewangle.y = math::calc_angle ( local_player::m_data.pointer->origin ( ), aimbot::targets.front ( ).player->origin ( ) ).y - 180.f;
			}
			viewangle.angle_normalize ( ); viewangle.angle_clamp ( );
		}


	    
		cmd->viewangles.y = viewangle.y;

		if ( should_predict ( ) ) {
			if ( !desync_side )
				cmd->viewangles.y += 120;
			else
				cmd->viewangles.y -= 120;
			send_packet = false;
		}
		else if ( send_packet ) {
			if ( !desync_side )
				cmd->viewangles.y -= local_player::m_data.pointer->max_desync(false);
			else
				cmd->viewangles.y += local_player::m_data.pointer->max_desync ( false );
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
	
		
		if ( fabsf( cmd->sidemove ) < 2.0f ) {
			cmd->sidemove = cmd->tick_count % 2?1.10:-1.10;
		}

	}
	break;
	}
	

	
	cmd->viewangles.angle_normalize( );
	cmd->viewangles.angle_clamp( );

	//if ( c_config::get( ).leg )
	//cmd->buttons &= ~( in_forward | in_back | in_moveright | in_moveleft );

	

}
