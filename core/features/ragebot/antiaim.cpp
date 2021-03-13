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

static int last_bruteforce_change = 0;
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
		last_bruteforce_change = interfaces::globals->tick_count + 256;
		desync_side = !desync_side;
		last_time_shoot [ entity->index ( ) ] = interfaces::globals->tick_count;
	}

}

bool will_lby_update ( ) {
	static float next_update = 0.f;
	const auto nci = interfaces::engine->get_net_channel_info ( );

	float curtime = interfaces::globals->cur_time;

	if ( local_pointer->velocity ( ).Length2D ( ) >= .1f ) {
		next_update = curtime + .22f;
	}

	else {
		if ( next_update < curtime ) {
			next_update = curtime + 1.1f;
			return true;
		}
	}

	return false;
}

void anti_aim::lowerbody_desync (c_usercmd* cmd, bool& send_packet ) {
	const float desync_delta = config.ragebot_double_tap_ticks;
	if ( will_lby_update ( ) ) {
	
		cmd->viewangles.y = localdata.antiaim_yaw + config.lby_offset;


		localdata.last_lby_angle.y = cmd->viewangles.y;

		send_packet = false;

		localdata.break_lby = true;
	}

	else if ( !send_packet ) {

		cmd->viewangles.y = localdata.antiaim_yaw + config.fake_offset;
	

		cmd->viewangles.angle_normalize ( );
	}

	else {
		cmd->viewangles.y += 180.f;
		cmd->viewangles.angle_normalize ( );
		localdata.antiaim_yaw = cmd->viewangles.y;
	}
}

bool anti_aim::allow( c_usercmd* ucmd, bool& send_packet ) {

	if ( shifting::_shift.shift_ticks )
		return false;

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
 int anti_aim::freestanding_angle ( ) {

	 vec3_t view_angle = vec3_t ( );  interfaces::engine->get_view_angles ( view_angle );
	 vec3_t freestand_angle = vec3_t(csgo::real_angle.x, math::normalize_yaw ( view_angle.y - 180.f ), 0 );

	 if ( anti_aim::desync_side )
		 freestand_angle.y -= 90.f;
	 else
		 freestand_angle.y += 90.f;

	 return freestand_angle.y;

	 if ( aimbot::targets.empty ( ) )
		 return freestand_angle.y;

	 if ( !local_pointer->m_CachedBoneData ( ).m_Size )
		 return freestand_angle.y;

	 auto target = aimbot::targets.front ( );
	 if ( target.player) {
		 freestand_angle = vec3_t ( csgo::real_angle.x, math::normalize_yaw( math::calc_angle ( local_pointer->origin ( ), target.player->origin ( ) ).y - 180.f), 0);
		 if ( target.player->active_weapon ( ) ) {
			 autowall::FireBulletData_t awall = { };

			 matrix3x4_t old_cache [ 128 ];

			 std::memcpy ( old_cache, local_pointer->m_CachedBoneData ( ).Base ( ), local_pointer->m_CachedBoneData ( ).m_Size * sizeof ( matrix3x4_t ) );

			 vec3_t old_abs_angles = local_pointer->abs_angles ( );
			 vec3_t left_angles = vec3_t ( csgo::real_angle.x, math::normalize_yaw ( freestand_angle.y - 90.f ), 0 );
			 vec3_t right_angles = vec3_t ( csgo::real_angle.x, math::normalize_yaw ( freestand_angle.y + 90.f ), 0 );

			 //player_manager::create_fake_matrix_based_on_angle ( local_pointer, localdata.left_matrix, left_angles );
			 //player_manager::create_fake_matrix_based_on_angle ( local_pointer, localdata.right_matrix, right_angles );
			 const float delta_left = math::normalize_yaw ( left_angles.y -csgo::real_angle.y );
			 const float delta_right = math::normalize_yaw ( right_angles.y - csgo::real_angle.y );
			 vec3_t extrapolated_origin ( ( local_pointer->origin ( ) + ( local_pointer->velocity ( ) * math::ticks_to_time(4) ) ) );

			// player_manager::get_rotated_matrix ( local_pointer->origin ( ), old_cache, delta_left, localdata.left_matrix );
			// player_manager::get_rotated_matrix ( local_pointer->origin ( ), old_cache, delta_right, localdata.right_matrix );

			 local_pointer->set_abs_angles ( left_angles );
			 std::memcpy ( local_pointer->m_CachedBoneData ( ).Base ( ), localdata.left_matrix, local_pointer->m_CachedBoneData ( ).m_Size * sizeof ( matrix3x4_t ) );
			 auto bone_left = local_pointer->get_hitbox_position ( hitbox_head, localdata.left_matrix );
		
			 int left_dmg = autowall::get_damage ( target.player, target.player->get_eye_pos ( ), bone_left, awall );
		 
			 local_pointer->set_abs_angles ( right_angles );
			 std::memcpy ( local_pointer->m_CachedBoneData ( ).Base ( ), localdata.right_matrix, local_pointer->m_CachedBoneData ( ).m_Size * sizeof ( matrix3x4_t ) );
			 auto bone_right = local_pointer->get_hitbox_position ( hitbox_head, localdata.right_matrix );

			 int right_dmg = autowall::get_damage ( target.player, target.player->get_eye_pos ( ), bone_right, awall );


			// interfaces::console->console_printf ( "FREESTANDING LEFT DAMAGE: %i | RIGHT DAMAGE: %i \n", left_dmg, right_dmg );


			 local_pointer->set_abs_angles ( old_abs_angles );
			 std::memcpy ( local_pointer->m_CachedBoneData ( ).Base ( ), old_cache, local_pointer->m_CachedBoneData ( ).m_Size * sizeof ( matrix3x4_t ) );


			 if ( left_dmg == right_dmg || left_dmg == 0 && right_dmg == 0 || ((left_dmg > 40 && right_dmg > 30 && std::abs(left_dmg - right_dmg) < 30)) || (left_dmg > 20 && right_dmg  > 20 ))
				 return freestand_angle.y;

			 if ( left_dmg > right_dmg )
				 return right_angles.y;
			 else if ( right_dmg > left_dmg )
				 return left_angles.y;
			 else
				 return freestand_angle.y;

		 }
	 }
}
void anti_aim::calculate_peek_real ( ) {
	if ( interfaces::globals->tick_count <= last_bruteforce_change )
		return;

	if ( aimbot::targets.empty ( ) )
		return;

	auto target = aimbot::targets.front ( );

	float Back, Right, Left;

	vec3_t src3D, dst3D, forward, right, up, src, dst;
	trace_t tr;
	ray_t ray, ray2, ray3, ray4, ray5;
	trace_filter filter;

	vec3_t engineViewAngles;
	interfaces::engine->get_view_angles ( engineViewAngles );

	engineViewAngles.x = 0;

	math::angle_vectors ( engineViewAngles, &forward, &right, &up );

	filter.skip = local_pointer;
	src3D = engine_prediction::unpredicted_eye;
	dst3D = src3D + ( forward * 384 );

	ray.initialize ( src3D, dst3D );

	interfaces::trace_ray->trace_ray ( ray, MASK_SHOT, &filter, &tr );

	Back = ( tr.end - tr.start ).length ( );

	ray2.initialize ( src3D + right * 35, dst3D + right * 35 );

	interfaces::trace_ray->trace_ray ( ray2, MASK_SHOT, &filter, &tr );

	Right = ( tr.end - tr.start ).length ( );

	ray3.initialize ( src3D - right * 35, dst3D - right * 35 );

	interfaces::trace_ray->trace_ray ( ray3, MASK_SHOT, &filter, &tr );

	Left = ( tr.end - tr.start ).length ( );

	if ( Left > Right ) {
		anti_aim::desync_side = true;
	}
	else if ( Right > Left ) {
		anti_aim::desync_side = false;
	}
	//else if ( Back > Right || Back > Left ) {
	//	return ( yaw - 180 );
//	}

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
			//player_manager::get_rotated_matrix ( local_origin, local_pointer->m_CachedBoneData ( ).Base ( ), angle, rotated );
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
	if ( interfaces::inputsystem->is_button_down( button_code_t::KEY_V ) && ( interfaces::globals->tick_count  - tick) > 2 )
	{
		desync_side = !desync_side;
		//send_packet  = true;
		tick = interfaces::globals->tick_count;
	}

	if ( localdata.freestanding_on_key ) {
	
		cmd->viewangles.y = math::normalize_yaw ( freestanding_angle ( ) );
	
		return;
	}

	switch ( config.antiaim_yaw ) {
	case 1:
	{
		/*vec3_t viewangle = vec3_t ( );
		static vec3_t temp_angle = vec3_t ( );
		interfaces::engine->get_view_angles ( viewangle );
		if ( !( cmd->buttons & in_use ) ) {

			viewangle.y -= 180.f;
			if ( config.antiaim_at_targets && aimbot::targets.size ( ) && aimbot::targets.front ( ).player ) {
				viewangle.y = math::calc_angle ( local_player::m_data.pointer->origin ( ), aimbot::targets.front ( ).player->origin ( ) ).y - 180.f;
			}
			viewangle.angle_normalize ( ); viewangle.angle_clamp ( );
		}

		float fakelag_percent = interfaces::clientstate->m_choked_commands ? static_cast<float>(interfaces::clientstate->m_choked_commands) / 14.0 : 0.f;
		float desync_delta = 58.f;

		//interfaces::console->console_printf ( "DESYNC DELTA %f \n", desync_delta );
		lowerbody_desync ( cmd, send_packet );*/
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
		
		 calculate_peek_real ( );
		 float delta = 120.f;

		 if ( send_packet ) {
			     cmd->viewangles.angle_normalize ( );
				 cmd->viewangles.y = viewangle.y;
				 localdata.antiaim_yaw = cmd->viewangles.y;
		 }
		 else {
			 cmd->viewangles.angle_normalize ( );
			 if ( anti_aim::desync_side )
				 cmd->viewangles.y = localdata.antiaim_yaw - delta;
			 else
				 cmd->viewangles.y = localdata.antiaim_yaw + delta;
		 }

		if ( fabsf ( cmd->sidemove ) < 2.0f ) {
			cmd->sidemove = cmd->tick_count % 2 ? 1.10f : -1.10f;
		}

		if ( will_lby_update ( )  ) {
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
