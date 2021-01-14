#include "../features.hpp"

bool need_to_flip = false;

 static float last_hp = 100;

 bool anti_aim::desync_side = false;
std::vector<anti_aim::freestand_point> anti_aim::points;
float get_curtime( c_usercmd* ucmd ) {
	auto local_player = local_player::m_data.pointer;

	if ( !local_player )
		return 0;

	static int g_tick = 0;
	c_usercmd* g_pLastCmd = nullptr;
	if ( !g_pLastCmd || g_pLastCmd->hasbeenpredicted ) {
		g_tick = local_player->get_tick_base( );
	}
	else {
		++g_tick;
	}
	g_pLastCmd = ucmd;
	float curtime = g_tick * interfaces::globals->interval_per_tick;
	return curtime;
}


static int last_user_id = -1;
static int shots_on_me = 0;
static float last_angle_bullet_impact = 0.0f;
bool anti_aim::choke_next_tick =false;
void anti_aim::event_logs::bullet_impact( i_game_event* event ) {
	if ( !config.antiaim_antibruteforce )
		return;

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


bool anti_aim::allow( c_usercmd* ucmd, bool& send_packet ) {


	auto local = local_player::m_data.pointer;
	if ( !local && !local->is_alive( ) )
		return false;

	const auto move_type = local->move_type( );
	if ( move_type == ( int ) movetype_ladder || move_type == ( int ) movetype_noclip )
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

int anti_aim::best_freestanding_angle ( ) {
	anti_aim::points.clear ( );
	auto local_origin = local_pointer->origin ( );
	vec3_t view_angle = vec3_t ( );  interfaces::engine->get_view_angles ( view_angle );
	int freestand_angle = static_cast< int >( math::normalize_yaw ( view_angle.y - 180.f ) );
	if ( aimbot::targets.empty ( ) )
		return freestand_angle;

	if ( !local_pointer->m_CachedBoneData ( ).m_Size )
		return freestand_angle;

	auto target = aimbot::targets.front ( );
	int lowest_damage = 9999;
	


	
	if ( target.player && target.player->active_weapon() ) {
		autowall::FireBulletData_t awall = { };
		awall.target = local_pointer;
		vec3_t old_abs_angles = target.player->abs_angles ( );
		matrix3x4_t old_cache [ 128 ];

		std::memcpy (old_cache, local_pointer->m_CachedBoneData ( ).Base ( ), local_pointer->m_CachedBoneData ( ).m_Size * sizeof ( matrix3x4_t ) );

		for ( int angle = -180; angle <= 180; angle += 45 ) {

			matrix3x4_t rotated [ 128 ];
			player_manager::get_rotated_matrix ( local_origin, local_pointer->m_CachedBoneData ( ).Base ( ), angle, rotated );
			//local_pointer->set_abs_angles ( vec3_t(0, angle, 0) );
			std::memcpy ( local_pointer->m_CachedBoneData ( ).Base ( ), rotated, local_pointer->m_CachedBoneData ( ).m_Size * sizeof ( matrix3x4_t ) );
			auto bone = local_pointer->get_hitbox_position ( hitbox_head, rotated );
			int dmg = autowall::get_damage ( target.player, target.player->get_eye_pos ( ), bone, awall );

			if ( dmg <= lowest_damage ) {
				lowest_damage = dmg;
				freestand_angle = angle;
			}
			anti_aim::points.push_back ( { dmg, bone } );
	  
		}
		local_pointer->set_abs_angles ( old_abs_angles );
		
	/*	vec3_t bone = local_pointer->get_hitbox_position ( hitbox_chest, local_pointer->m_CachedBoneData ( ).Base ( ) );
	
		int dmg = autowall::get_damage ( target.player, target.player->get_eye_pos ( ), bone, awall );

		interfaces::console->console_printf ( "dmg %i \n", dmg );// , ( bone, 2, 0.1f, 255, 0, 0, 0, std::to_string ( dmg ).c_str ( ) );*/
	}

	
	
	return freestand_angle;
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


	if ( !weapon )
		return;

	if ( !allow( cmd, send_packet ) )
		return;

	switch ( c_config::get().antiaim_pitch) {
	case 1: cmd->viewangles.x = 89.9f; break;
	case 2: cmd->viewangles.x = 0; break;
	case 3: cmd->viewangles.x = -89.9f; break;
	}
	if ( cmd->buttons & in_use )
		cmd->viewangles.x = 0;

	
	static int tick = 0;
	if ( interfaces::inputsystem->is_button_down( button_code_t::KEY_V ) && ( interfaces::globals->tick_count  - tick) > 3 )
	{
		desync_side = !desync_side;
		//send_packet  = true;
		tick = interfaces::globals->tick_count;
	}


	switch ( config.antiaim_yaw ) {
	case 1:
	{
		cmd->viewangles.y = best_freestanding_angle ( );
	}

	break;
	case 2:
	{
		vec3_t viewangle = vec3_t ( );
		static vec3_t temp_angle = vec3_t ( );
		interfaces::engine->get_view_angles ( viewangle );
		if ( !( cmd->buttons & in_use ) ) {

			viewangle.y -= 180.f;
			if ( config.antiaim_at_targets && aimbot::targets.size ( ) && aimbot::targets.front ( ).player ) {
				viewangle.y = math::calc_angle ( local_player::m_data.pointer->origin ( ), aimbot::targets.front ( ).player->origin ( ) ).y - 180.f;
			}
			viewangle.angle_normalize ( ); viewangle.angle_clamp ( );
		}
		
	

		 if ( send_packet ) {
				 cmd->viewangles.y = viewangle.y;
		 }
		 else {
			 if ( anti_aim::desync_side )
				 cmd->viewangles.y = localdata.antiaim_yaw - 120.f;
			 else
				 cmd->viewangles.y = localdata.antiaim_yaw + 120.f;
		 }
		if ( fabsf ( cmd->sidemove ) < 2.0f ) {
			cmd->sidemove = cmd->tick_count % 2 ? 1.10f : -1.10f;
		}

		if ( should_predict ( ) ) {
			if ( anti_aim::desync_side )
				cmd->viewangles.y = localdata.antiaim_yaw - 120.f;
			else
				cmd->viewangles.y = localdata.antiaim_yaw + 120.f;
			send_packet = false;
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
			cmd->sidemove = cmd->tick_count % 2?1.10f:-1.10f;
		}

	}
	break;
	}
	


}
