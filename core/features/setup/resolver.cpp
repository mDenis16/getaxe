#include "../features.hpp"
resolver::resolve_info resolver::resolver_data [ 65 ];
std::deque<resolver::resolve_shot> resolver::shots;
void resolver::event_logs::weapon_fire ( i_game_event * event ) {
	if ( !event )
		return;

	if ( !interfaces::engine->is_connected ( ) && !interfaces::engine->is_in_game ( ) )
		return;

	auto local_player = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( interfaces::engine->get_local_player ( ) ) );
	if ( !local_player )
		return;

	auto entity = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( interfaces::engine->get_player_for_user_id ( event->get_int ( "userid" ) ) ) );
	if ( !entity )
		return;
	
	if ( entity == local_player )
		return;


	if ( !entity )
		return;


	if ( entity->is_enemy ( ) )
		return;

	

	resolve_shot shot;
     shot.hit = false; 
     shot.tick = interfaces::globals->tick_count;
	 shot.hurt = false;
	 shot.attacker_index = entity->index ( );
	 shot.enemy_index = 0;
 	shot.shotpos = entity->get_eye_pos();
	shot.approved = false;
	shot.curtime = interfaces::globals->cur_time;

	shots.push_back ( shot );
}
void resolver::event_logs::player_hurt ( i_game_event * event ) {
	if ( !event )
		return;

	if ( !interfaces::engine->is_connected ( ) && !interfaces::engine->is_in_game ( ) )
		return;

	auto local_player = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( interfaces::engine->get_local_player ( ) ) );
	if ( !local_player )
		return;

	auto victim_index = interfaces::engine->get_player_for_user_id ( event->get_int ( "userid" ));
	if ( !victim_index )
		return;

	auto victim = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( interfaces::engine->get_player_for_user_id ( event->get_int ( "userid" ) ) ) );
	if ( !victim )
		return;

	auto attacker = reinterpret_cast< player_t * >(interfaces::entity_list->get_client_entity ( interfaces::engine->get_player_for_user_id ( event->get_int ( "attacker" ) ) ));

	if ( !attacker )
		return;

	if ( attacker != local_player ) {
	


		auto hitbox = event->get_int ( "hitgroup" );
		if ( !hitbox )
			return;

		auto damage = event->get_int ( "dmg_health" );
		if ( !damage )
			return;

		auto health = event->get_int ( "health" );

		if ( !health && health != 0 )
			return;

		if ( attacker->team ( ) != local_player->team ( ) )
			return;


		auto shot = closest_shot ( interfaces::globals->tick_count, attacker );
		if ( shot ) {
			float angle_at_attacker = math::calc_angle ( victim->get_eye_pos ( ), attacker->get_eye_pos ( ) ).y - 180.f;
			angle_at_attacker = math::clamp_yaw ( angle_at_attacker );

			shot->hurt = true;
			shot->hit = true;
			shot->enemy_index = victim->index();
			shot->angle = victim->get_anim_state ( )->m_flGoalFeetYaw;

			shot->hit_info.damage = damage;
			shot->hit_info.hitgroup = hitbox;
			shot->hit_info.victim = victim_index;


		
		}


	}

}
player_t*  resolver::get_closest_player_by_point ( vec3_t from,  vec3_t point ) {
	player_t * closest = nullptr;
	auto distance = 6.5f;
	for ( int i = 1; i <= interfaces::globals->max_clients; i++ ) {
		player_t * entity = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( i ) );

		if ( !entity )
			continue;

		if ( entity->dormant ( ) )
			continue;

		if ( !entity->is_enemy ( ) )
			continue;

		if ( entity->health ( ) <= 0 )
			continue;

		auto angle_bullet = math::calc_angle ( from, point ); math::clamp ( angle_bullet );
		auto angle_enemy = math::calc_angle ( from, entity->get_hitbox_position(hitbox_body )); math::clamp ( angle_enemy );

		auto angDelta = ( angle_enemy - angle_bullet ).Length2D ( );
		
		if ( angDelta < distance ) {
			closest = entity;
			distance = angDelta;
		}


	}
	return closest;
}

resolver::resolve_shot * resolver::closest_shot ( int tickcount, player_t * player ) {
	resolve_shot * closest_shot = nullptr;
	auto closest_diff = 64;
	for ( auto & shot :  shots ) {
		if ( shot.attacker_index == player->index ( ) ) {
			const auto diff = abs ( tickcount - shot.tick );
			if ( diff <= closest_diff ) {
				//	interfaces::console->console_printf( "DIF TICKCOUNT %i \n", diff );
				closest_shot = &shot;
				closest_diff = diff;
			}
		}

	}

	return closest_shot;
}
std::string resolver::side_name ( int side ) {
	switch ( side ) {
	case 0:
		return "low delta";
	case 1:
		return "left";
	case 2:
		return "right";
	}
}
int resolver::get_desync_side ( vec3_t from, vec3_t to, player_t* entity, int hitbox ) {
	auto VectorTransform_Wrapper = [ ] ( const vec3_t & in1, const matrix_t & in2, vec3_t & out ) {
		auto VectorTransform = [ ] ( const float * in1, const matrix_t & in2, float * out ) {
			auto DotProducts = [ ] ( const float * v1, const float * v2 ) {
				return v1 [ 0 ] * v2 [ 0 ] + v1 [ 1 ] * v2 [ 1 ] + v1 [ 2 ] * v2 [ 2 ];
			};
			out [ 0 ] = DotProducts ( in1, in2 [ 0 ] ) + in2 [ 0 ][ 3 ];
			out [ 1 ] = DotProducts ( in1, in2 [ 1 ] ) + in2 [ 1 ][ 3 ];
			out [ 2 ] = DotProducts ( in1, in2 [ 2 ] ) + in2 [ 2 ][ 3 ];
		};
		VectorTransform ( &in1.x, in2, &out.x );
	};

	studio_hdr_t * pStudioModel = interfaces::model_info->get_studio_model ( entity->model ( ) );
	studio_hitbox_set_t * set = pStudioModel->hitbox_set ( 0 );
	studio_box_t * _hitbox = set->hitbox ( hitbox );

	vec3_t max;
	vec3_t min;
	const auto mod = _hitbox->radius != -1.f ? _hitbox->radius : 0.f;
	VectorTransform_Wrapper ( vec3_t ( _hitbox->maxs.x + mod, _hitbox->maxs.y + mod, _hitbox->maxs.z + mod ), csgo::right_player_bones [entity->index()][ _hitbox->bone ], max );
	VectorTransform_Wrapper ( vec3_t ( _hitbox->mins.x - mod, _hitbox->mins.y - mod, _hitbox->mins.z - mod ), csgo::right_player_bones [ entity->index ( ) ][ _hitbox->bone ], min );


	vec3_t max2;
	vec3_t min2;

	VectorTransform_Wrapper ( vec3_t ( _hitbox->maxs.x + mod, _hitbox->maxs.y + mod, _hitbox->maxs.z + mod ), csgo::left_player_bones [ entity->index ( ) ][ _hitbox->bone ], max2 );
	VectorTransform_Wrapper ( vec3_t ( _hitbox->mins.x - mod, _hitbox->mins.y - mod, _hitbox->mins.z - mod ), csgo::left_player_bones [ entity->index ( ) ][ _hitbox->bone ], min2 );

	//if ( auto intersection = ragebot::get_intersect_point ( from, to, min, max, _hitbox->radius ); intersection ) 
	//	return 2;
	///if ( auto intersection = ragebot::get_intersect_point ( from, to, min2, max2, _hitbox->radius ); intersection )
	//	return 1;
	return 0;
}
void resolver::event_logs::bullet_impact ( i_game_event * event ) {
	static int last_time_shoot [ 64 ];
	auto userid = event->get_int ( "userid" );
	if ( !userid )
		return;

	if ( !interfaces::engine->is_connected ( ) && !interfaces::engine->is_in_game ( ) )
		return;

	auto engine_userid = interfaces::engine->get_player_for_user_id ( userid );
	if ( !engine_userid )
		return;

	if ( engine_userid == interfaces::engine->get_local_player ( ) )
		return;

	auto entity = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( engine_userid ) );

	if ( !entity )
		return;


	if ( entity->dormant ( ) )
		return;

	if ( entity->is_enemy ( ) )
		return;


	vec3_t bullet_impact = vec3_t ( event->get_float ( "x" ), event->get_float ( "y" ), event->get_float ( "z" ) );

	if (last_time_shoot [ entity->index ( ) ] != interfaces::globals->tick_count ) {
		auto shot = closest_shot ( interfaces::globals->tick_count, entity );
		if ( shot ) {
			auto target = get_closest_player_by_point ( entity->get_eye_pos ( ), bullet_impact );

			auto weapon = entity->active_weapon ( );
			if ( !weapon )
				return;

			auto weapon_data = weapon->get_weapon_data ( );
			if ( !weapon_data )
				return;

			if ( target ) {
				shot->enemy_index = target->index ( );
				
				trace_t tr;
				ray_t ray;

				const auto angles = math::calc_angle ( shot->shotpos, bullet_impact );
				vec3_t direction = math::angle_vector ( angles );
				direction.normalize ( );


				ray.initialize ( shot->shotpos, shot->shotpos + direction * weapon_data->flRange );
				interfaces::trace_ray->clip_ray_to_entity ( ray, MASK_SHOT_HULL | CONTENTS_HITBOX, target, &tr );
				if ( tr.entity == target ) {
					shot->hitpos = tr.end;
					shot->hit = true;
				}


				shot->desync_side = get_desync_side ( shot->shotpos, shot->shotpos + direction * weapon_data->flRange, target, hitbox_head );
				resolver_data [ shot->enemy_index ].brute_side = shot->desync_side;
				if ( shot->desync_side > 0 ) {
					shot->hit = true;
				}

				




			}
			last_time_shoot [ entity->index ( ) ] = interfaces::globals->tick_count;
		}
	}

}
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
float resolver::server_feet_yaw( player_t* entity, vec3_t angle )
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
	auto backup_eflags = entity->get_ieflags ( );

	entity->get_ieflags( ) = ( 1 << 12 );
	auto abs_velocity = *reinterpret_cast< vec3_t* > ( uintptr_t( entity ) + 0x94 );
	entity->get_ieflags ( ) = backup_eflags;

	auto speed = std::fmin( abs_velocity.length( ), 260.0f );

	auto goal_feet_yaw = animstate->m_flGoalFeetYaw;
	/*static float shot [ 64 ];
	if ( entity->active_weapon ( ) ) {
		if ( shot [ entity->index ( ) ] != entity->active_weapon ( )->m_fLastShotTime ( ) ) {
			
			shot [ entity->index ( ) ] = entity->active_weapon ( )->m_fLastShotTime ( );
			printf ( "DETECteD SHOOT STOpeD goal feeet calculation" );
			return goal_feet_yaw;
		}
	}*/
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

	auto eye_feet_delta = angle_diff( angle.y, goal_feet_yaw );

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
			angle.y,
			goal_feet_yaw,
			( ( ground_fraction * 20.0f ) + 30.0f )
			* animstate->m_flLastClientSideAnimationUpdateTime );
	}
	else
	{
		goal_feet_yaw = math::fl_approach_angle (
			entity->lower_body_yaw ( ),
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
	
}
float resolver::get_safe_point_angle ( int i ) {
	
	float safe_point_angle = 0.f;
	switch ( variables::ragebot::safe_point ) {
	case 0:
		safe_point_angle = 0.f;
		break;
	case 1:
		safe_point_angle = resolver::resolver_data [ i ].max_desync_delta / 2.5f;
		break;
	case 2:
		safe_point_angle =  resolver::resolver_data [ i ].extended_desync ?  34.f :  28.f;
		break;
	case 3:
		safe_point_angle = resolver::resolver_data [ i ].max_desync_delta;
		break;
	default:
		break;
	}
	std::clamp ( safe_point_angle, 0.f, resolver::resolver_data [ i ].max_desync_delta );
	return safe_point_angle;
}

void resolver::manage_shots ( ) {
	for ( size_t i = 0; i < shots.size ( ); i++ ) {
		auto shot = &shots.at ( i );
		if ( !shot->approved && abs ( shot->tick - interfaces::globals->tick_count ) < 64 ) {
			if ( ( interfaces::globals->cur_time - shot->curtime ) > 5.f )
				shots.erase ( shots.begin ( ) + i );

			if ( !shot->enemy_index )
				continue;


			auto enemy = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( shot->enemy_index ) );

			if ( !enemy )
				return;

			auto attacker = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( shot->attacker_index ) );

			if ( !attacker )
				return;

			std::string name = attacker->get_info ( ).name;

			if ( name.length ( ) > 17 )
				name = name.substr ( 0, 17 ) + "...";

			std::transform ( name.begin ( ), name.end ( ), name.begin ( ), tolower );



			std::string victim_name = enemy->get_info ( ).name;

			if ( victim_name.length ( ) > 17 )
				victim_name = victim_name.substr ( 0, 17 ) + "...";

			std::transform ( victim_name.begin ( ), victim_name.end ( ), victim_name.begin ( ), tolower );


			if ( shot->hit && !shot->hurt ) {

				float angle_at_teammate = math::calc_angle ( enemy->get_hitbox_position ( hitbox_head ), attacker->get_eye_pos() ).y - 180.f; math::normalize_yaw ( angle_at_teammate );
				float dif = math::angle_diff ( enemy->get_anim_state ( )->m_flGoalFeetYaw, angle_at_teammate );
				std::clamp ( dif, -58.f, 58.f );

				std::stringstream ss;
				ss << "Teammate " << name << "misssed  " << victim_name << " due resolver side ( " << side_name ( shot->desync_side ) << " ) dif (" << dif << " ).";

				resolver::resolver_data [ shot->enemy_index ].last_hit_angle_related_to_me.push_back ( dif );
				visuals::notifications::add ( ss.str ( ) );
				resolver::resolver_data [ shot->enemy_index ].last_side_change = interfaces::globals->cur_time;
				resolver_data [ enemy->index ( ) ].desync_side = shot->desync_side;

			}
			else if ( shot->hurt && shot->hit ) {


				std::stringstream ss;
				ss << "Teammate " << name << "hit  " << name << " in " << " for " << shot->hit_info.damage << " damage.";

				visuals::notifications::add ( ss.str ( ) );
				//visuals::event_.push_front( visuals::loginfo( ss.str( ), color( 255, 255, 255 ), interfaces::globals->cur_time ) );
			}
			//	interfaces::debug_overlay->add_line_overlay( shot->shotpos, shot->hitpos, 255, 0, 0, false, 0.5f );
			shot->approved = true;
		}
	

	}

}
void resolver::frame_stage( ) {
	if ( !interfaces::engine->is_connected( ) )
		return;

	if ( !interfaces::engine->is_in_game( ) )
		return;

	if ( !csgo::local_player )
		return;
	if ( !variables::ragebot::enabled )
		return;
	manage_shots ( );

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
		std::string entity_name = entity->get_info ( ).name;

		if ( entity_name.length ( ) > 17 )
			entity_name = entity_name.substr ( 0, 17 ) + "...";

		std::transform ( entity_name.begin ( ), entity_name.end ( ), entity_name.begin ( ), tolower );

		if ( resolver::resolver_data [ i ].last_hit_angle_related_to_me.size ( ) > 16 )resolver::resolver_data [ i ].last_hit_angle_related_to_me.pop_back ( );

		resolver_data [ i ].max_desync_delta = max_desync_delta( entity );
		//resolver_data [ i ].server_goal_feet = server_feet_yaw( entity );
		if ( ( interfaces::globals->cur_time - resolver_data [ i ].last_reset_time ) > 20.f ) {
			resolver_data [ i ].is_using_desync = false;
			resolver_data [ i ].last_reset_time = interfaces::globals->cur_time;


		}
		resolver_data [ i ].goal_feet_yaw = entity->get_anim_state ( )->m_flGoalFeetYaw;
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
			resolver_data [ i ].safe_point_angle = get_safe_point_angle ( i );

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
		
	
		

			

			resolver_data [ i ].extended_desync = currentLayer.cycle == 0.f && currentLayer.weight == 0.f;
			resolver::resolver_data [ i ].brute_angle = resolver_data [ i ].extended_desync ? resolver_data [ i ].max_desync_delta : resolver_data [ i ].max_desync_delta / 1.9f;
		     
			if ( resolver::resolver_data [ i ].last_hit_angle_related_to_me.size ( ) > 0 )
				resolver::resolver_data [ i ].first_shoot_correction = resolver::resolver_data [ i ].last_hit_angle_related_to_me.at(0);
		
			if ( std::fabs ( resolver::resolver_data [ i ].last_side_change - interfaces::globals->cur_time ) > 3.f ) 				{
				resolver::resolver_data [ i ].brute_side = 0;
			}


		
		}
	}
}