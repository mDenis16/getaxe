#include "../features.hpp"
resolver::resolve_info resolver::resolver_data [ 65 ];
bool resolver::get_desync_side( player_t* entity ) {
	float Back, Right, Left;

	vec3_t src3D, dst3D, forward, right, up, src, dst;
	trace_t tr;
	ray_t ray, ray2, ray3, ray4, ray5;
	trace_filter_skip_one_entity filter( entity );




	math::angle_vectors( entity->eye_angles( ), &forward, &right, &up );

	src3D = entity->get_eye_pos( );
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
		return false;
	}
	else
	{
		return true;
	}

}
float resolver::server_feet_yaw( player_t* entity )
{
	auto animstate = entity->get_anim_state( );

	if ( !animstate )
		return 0.f;

	/* data */
	auto ground_fraction = *( float* ) ( animstate + 0x11C );
	auto duck_ammount = *( float* ) ( animstate + 0xA4 );
	auto ducking_speed = max( 0.f, min( 1.f, *reinterpret_cast< float* > ( animstate + 0xFC ) ) );
	auto running_speed = max( 0.f, min( *reinterpret_cast< float* > ( animstate + 0xF8 ), 1.f ) );
	/* offsets */
	auto backup_eflags = entity->get_eflags( );

	entity->get_eflags( ) = ( 1 << 12 );
	auto abs_velocity = *reinterpret_cast< vec3_t* > ( uintptr_t( entity ) + 0x94 );
	entity->get_eflags( ) = backup_eflags;

	auto speed = std::fmin( abs_velocity.length( ), 260.0f );

	auto goal_feet_yaw = animstate->m_flGoalFeetYaw;

	auto angle_diff = [ ] ( float destAngle, float srcAngle ) -> float
	{
		auto delta = 0.f;

		delta = fmodf( destAngle - srcAngle, 360.0f );

		if ( destAngle > srcAngle )
		{
			if ( delta >= 180 )
				delta -= 360;
		}
		else
		{
			if ( delta <= -180 )
				delta += 360;
		}

		return delta;
	};

	auto eye_feet_delta = angle_diff( animstate->m_flEyeYaw, goal_feet_yaw );

	auto flYawModifier = ( ( ( ground_fraction * -0.3f ) - 0.2f ) * running_speed ) + 1.0f;

	if ( duck_ammount > 0.0f )
		flYawModifier = flYawModifier + ( ( duck_ammount * ducking_speed ) * ( 0.5f - flYawModifier ) );

	auto flMaxYawModifier = flYawModifier * 58.f;
	auto flMinYawModifier = flYawModifier * -58.f;

	if ( eye_feet_delta <= flMaxYawModifier )
	{
		if ( flMinYawModifier > eye_feet_delta )
			goal_feet_yaw = fabs( flMinYawModifier ) + animstate->m_flEyeYaw;
	}
	else
		goal_feet_yaw = animstate->m_flEyeYaw - fabs( flMaxYawModifier );

	if ( goal_feet_yaw > 5000 || goal_feet_yaw < -5000 )
		return 0.f;

	math::normalize_yaw( goal_feet_yaw );

	if ( speed > 0.1f || fabs( abs_velocity.z ) > 100.0f )
	{
		goal_feet_yaw = math::fl_approach_angle(
			animstate->m_flEyeYaw,
			goal_feet_yaw,
			( ( ground_fraction * 20.0f ) + 30.0f )
			* animstate->m_flLastClientSideAnimationUpdateTime );
	}
	else
	{
		goal_feet_yaw = math::fl_approach_angle(
			entity->lower_body_yaw( ),
			goal_feet_yaw,
			animstate->m_flLastClientSideAnimationUpdateTime * 100.0f );
	}

	if ( goal_feet_yaw > 5000 || goal_feet_yaw < -5000 )
		return 0.f;

	math::normalize_yaw( goal_feet_yaw );

	return goal_feet_yaw;
}

float resolver::max_desync_delta( player_t* entity ) {
	if ( !entity->get_anim_state( ) )
		return 0.f;
	auto animstate = uintptr_t( entity->get_anim_state( ) );

	float
		rate = 180,
		duckammount = *( float* ) ( animstate + 0xA4 ),
		speedfraction = max( 0.f, min( *( float* ) ( animstate + 0xF8 ), 1.f ) ),
		speedfactor = max( 0.f, min( 1.f, *( float* ) ( animstate + 0xFC ) ) );

	float
		unk1 = ( ( *( float* ) ( animstate + 0x11C ) * -0.30000001 ) - 0.19999999 ) * speedfraction,
		unk2 = unk1 + 1.f;

	if ( duckammount > 0 )
		unk2 += ( ( duckammount * speedfactor ) * ( 0.5f - unk2 ) );

	return *( float* ) ( animstate + 0x334 ) * unk2;

}
bool resolver::is_player_peeking( player_t* player ) {
	bool peeking = false;
	if ( !(fabs( csgo::local_player->velocity( ).Length2D( ) ) > .1f ))
		return false;

	int choked = player->get_old_simulation_time( ) - player->simulation_time( );
	int ticks = 5;
	vec3_t enemy_head_extrapolated = player->get_hitbox_position( hitbox_head ) + ( player->velocity( ) * math::ticks_to_time( ticks ));
	vec3_t local_body_extrapolated = csgo::local_player->get_bone_position( hitbox_upper_chest ) + ( csgo::local_player->velocity( ) * math::ticks_to_time( ticks ) );

	float radius = 50.f;
	float step = M_PI * 2.0 / 16;

	if ( fabs( csgo::local_player->velocity( ).Length2D( ) ) > .1f ) {
		peeking = autowall::can_hit_float_point( local_body_extrapolated, enemy_head_extrapolated );

	}
	else {
		for ( float a = 0; a < ( M_PI * 2.0 ); a += step )
		{
			vec3_t location( radius * cos( a ) + local_body_extrapolated.x, radius * sin( a ) + local_body_extrapolated.y, local_body_extrapolated.z );
			peeking = autowall::can_hit_float_point( location, enemy_head_extrapolated );

			if ( peeking )
				break;
		}
	}


	return peeking;
}
void resolver::create_move( c_usercmd* cmd ) {
	for ( int i = 1; i <= interfaces::globals->max_clients; i++ ) {
		player_t* entity = reinterpret_cast< player_t* >( interfaces::entity_list->get_client_entity( i ) );

		if ( !entity )
			continue;

		if ( entity->dormant( ) )
			continue;
		if ( !entity->is_enemy( ) )
			continue;
		if ( entity->health( ) <= 0 )
			continue;
		//resolver_data [ i ].real_head = get_real_head( entity );
		//resolver_data [ i ].desync_side = get_desync_side( entity );
	//   resolver_data [ i ].will_peek = is_player_peeking( entity );

	}
}
float resolver::get_real_head( player_t* entity ) {
	float step = M_PI * 2.0 / 8;
	float lowest_dmg = 99999.F;
	float final_angle = 0.f;
	for ( float a = 0; a < ( M_PI * 2.0 ); a += step )
	{
		float angle = RAD2DEG( a );
		vec3_t BonePos;
		vec3_t OutPos;
		matrix_t BoneMatrix [ MAXSTUDIOBONES ];
		for ( int i = 0; i < MAXSTUDIOBONES; i++ )
		{
			math::angle_matrix( vec3_t( 0, angle, 0 ), BoneMatrix [ i ] );
			math::matrix_multiply( BoneMatrix [ i ], csgo::player_bones [ entity->index( ) ][ i ] );
			BonePos = vec3_t( csgo::player_bones [ entity->index( ) ][ i ][ 0 ][ 3 ], csgo::player_bones [ entity->index( ) ][ i ][ 1 ][ 3 ], csgo::player_bones [ entity->index( ) ][ i ][ 2 ][ 3 ] ) - entity->origin( );
			math::vector_rotate( BonePos, vec3_t( 0, angle, 0 ), OutPos );
			OutPos += entity->origin( );
			BoneMatrix [ i ][ 0 ][ 3 ] = OutPos.x;
			BoneMatrix [ i ][ 1 ][ 3 ] = OutPos.y;
			BoneMatrix [ i ][ 2 ][ 3 ] = OutPos.z;
		}

		auto count = *( std::uint32_t* ) ( ( std::uint32_t ) entity->get_renderable( ) + 0x2918 );
		std::memcpy( *( void** ) ( ( std::uint32_t ) entity->get_renderable( ) + 0x290C ), BoneMatrix, sizeof( matrix_t ) * count );
		entity->set_abs_angles( vec3_t(0, angle, 0) );
		autowall::FireBulletData_t awall = { };
		autowall::GetDamage( csgo::local_player, entity->get_hitbox_position( hitbox_head, BoneMatrix ), awall );
		
		player_manager::restore_player( entity );

		if ( lowest_dmg < awall.flCurrentDamage )
		{
			lowest_dmg = awall.flCurrentDamage;
			final_angle = angle;
		}
	}
	return final_angle;
}
void resolver::frame_stage( ) {
	if ( !interfaces::engine->is_connected( ) )
		return;

	if ( !interfaces::engine->is_in_game( ) )
		return;

	if ( !csgo::local_player )
		return;
	if ( !variables::ragebot::resolver )
		return;

	for ( int i = 1; i <= interfaces::globals->max_clients; i++ ) {
		player_t* entity = reinterpret_cast< player_t* >( interfaces::entity_list->get_client_entity( i ) );

		if ( !entity )
			continue;

		if ( entity->dormant( ) )
			continue;
		if ( !entity->is_enemy( ) )
			continue;
		if ( entity->health( ) <= 0 )
			continue;

		resolver_data [ i ].max_desync_delta = max_desync_delta( entity );
		resolver_data [ i ].server_goal_feet = server_feet_yaw( entity );
		if ( ( interfaces::globals->cur_time - resolver_data [ i ].last_reset_time ) > 20.f ) {
			resolver_data [ i ].is_using_desync = false;
			resolver_data [ i ].last_reset_time = interfaces::globals->cur_time;
			resolver_data [ i ].missed_shots = 0;

		}
		//entity->get_anim_state( )->m_flGoalFeetYaw = entity->get_anim_state( )->m_flGoalFeetYaw + 58.f;
		
		if ( player_manager::records [ i ].size( ) > 0 )
		{
			auto currentLayer = player_manager::records [ i ].front( ).anim_layer [ 3 ];
			auto previousLayer = player_manager::records [ i ].back( ).anim_layer [ 3 ];



			const int current_layer_activity = entity->get_sequence_act( currentLayer.sequence );
			const int previous_layer_activity = entity->get_sequence_act( previousLayer.sequence );
			float currentLayerflcycle = currentLayer.cycle, currentLayerflprevcycle = currentLayer.previous_cycle, currentLayerflweight = currentLayer.weight, currentLayerflweightdatarate = currentLayer.weight_delta_rate;
			float previousLayerflcycle = previousLayer.cycle, previousLayerflprevcycle = previousLayer.previous_cycle, previousLayerflweight = previousLayer.weight, previousLayerflweightdatarate = previousLayer.weight_delta_rate;
			uint32_t norder = currentLayer.order;

			if ( current_layer_activity == 979 ) // the current layer must be triggering 979
			{
				if ( previous_layer_activity == 979 ) // the previous layer must be trigerring 979
				{
					// we can tell now that he is surely breaking lby in some sort

					if ( ( previousLayer.cycle != currentLayer.cycle ) || currentLayer.weight == 1.f )
					{
						resolver_data [ i ].type = desync_type::balance;
						resolver_data [ i ].is_using_desync = true;
						resolver_data [ i ].last_desync_detection = interfaces::globals->cur_time;

					}
					else if ( currentLayer.weight == 0.f && ( previousLayer.cycle > 0.92f && currentLayer.cycle > 0.92f ) ) // breaking lby with delta < 120; can fakewalk here aswell
					{
						resolver_data [ i ].is_using_desync = true;
						resolver_data [ i ].type = desync_type::balance;
						resolver_data [ i ].last_desync_detection = interfaces::globals->cur_time;

					}
				}

			}
			float orig_goal_feet = entity->get_anim_state( )->m_flGoalFeetYaw;
			float angle_at_me = math::calc_angle( entity->get_eye_pos( ), csgo::local_player->get_eye_pos( ) ).y - 180.f; math::normalize_yaw( angle_at_me );
			resolver_data [ i ].extended_desync = currentLayer.cycle == 0.f && currentLayer.weight == 0.f;
			if ( resolver_data [ i ].is_using_desync  && resolver_data [ i ].missed_shots  > 0) {
				switch ( resolver_data [ i ].missed_shots % 3 )
				{
				case 0:
					resolver_data [ i ].final_goal_feet = angle_at_me;
					break;
				case 1:
					resolver_data [ i ].final_goal_feet = orig_goal_feet + (resolver_data[i].extended_desync ?58.f : 28.f);
					break;
				case 2:
					resolver_data [ i ].final_goal_feet = orig_goal_feet - ( resolver_data [ i ].extended_desync?58.f:28.f );
					break;

				default:
					break;
				}
				entity->get_anim_state( )->m_flGoalFeetYaw = resolver_data [ i ].final_goal_feet;
			}
		}
	}
}