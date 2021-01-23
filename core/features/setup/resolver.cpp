#include "../features.hpp"

resolver::resolve_info resolver::resolver_data [ 65 ];
std::deque<resolver::resolve_shot> resolver::shots;
void resolver::event_logs::weapon_fire ( i_game_event *  ) {
	return;
	/*if ( !event )
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

	shots.push_back ( shot );*/
}
void resolver::event_logs::player_hurt ( i_game_event *  ) {
	/*
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
			shot->angle = victim->get_anim_state ( )->m_abs_yaw;

			shot->hit_info.damage = damage;
			shot->hit_info.hitgroup = hitbox;
			shot->hit_info.victim = victim_index;


		
		}


	}
	*/

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
std::string resolver::side_name ( desync_side side ) {
	std::string side_name = "unknown";
	switch ( side ) {
	case desync_side::dodge:
		side_name =  "dodge";
		break;
	case desync_side::left:
		side_name = "left";
		break;
	case desync_side::right:
		side_name = "right";
		break;
	}
	return side_name;
}

void resolver::event_logs::bullet_impact ( i_game_event *  ) {
	/*return;
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


				//shot->desync_side = get_desync_side ( shot->shotpos, shot->shotpos + direction * weapon_data->flRange, target, hitbox_head );
			
				




			}
			last_time_shoot [ entity->index ( ) ] = interfaces::globals->tick_count;
		}
	}
	*/
}
void resolver::side_think ( player_t * entity ) {
	if ( !local_player::m_data.pointer )
		return;
	if ( !local_player::m_data.pointer->is_alive() )
		return;

	auto & resolve_record = resolver_data [ entity->index() ];

	const float height = 64;

	vec3_t direction_1 = math::angle_vector ( vec3_t ( 0.f, math::calc_angle ( local_player::m_data.pointer->origin ( ), entity->origin ( ) ).y - 90.f, 0.f ));
	vec3_t direction_2 = math::angle_vector ( vec3_t ( 0.f, math::calc_angle ( local_player::m_data.pointer->origin ( ), entity->origin ( ) ).y + 90.f, 0.f ));

	const auto left_eye_pos = entity->origin ( ) + vec3_t ( 0, 0, height ) + ( direction_1 * 16.f );
	const auto right_eye_pos = entity->origin ( ) + vec3_t ( 0, 0, height ) + ( direction_2 * 16.f );
/*	autowall::FireBulletData_t awall = { };

	resolve_record.left_damage =  autowall::GetDamage ( local_player::m_data.pointer, left_eye_pos, awall );
	resolve_record.right_damage = autowall::GetDamage ( local_player::m_data.pointer, right_eye_pos, awall );

	ray_t ray;
	trace_t trace;
	trace_world_only filter;

	ray.initialize ( local_player::m_data.eye_position, left_eye_pos );
	interfaces::trace_ray->trace_ray ( ray, MASK_ALL, &filter, &trace );
	resolve_record.left_fraction =  trace.flFraction;

	ray.initialize ( local_player::m_data.eye_position, right_eye_pos );
	interfaces::trace_ray->trace_ray ( ray, MASK_ALL, &filter, &trace );
	resolve_record.right_fraction = trace.flFraction;*/
}
float resolver::server_feet_yaw( player_t* entity, vec3_t angle )
{
	auto animstate = entity->get_anim_state ( );

	if ( !animstate )
		return 0.f;

	/* data */
	auto ground_fraction = *( float * ) ( animstate + 0x11C );
	auto duck_ammount = *( float * ) ( animstate + 0xA4 );
	auto ducking_speed = max ( 0.f, min ( 1.f, *reinterpret_cast< float * > ( animstate + 0xFC ) ) );
	auto running_speed = max ( 0.f, min ( *reinterpret_cast< float * > ( animstate + 0xF8 ), 1.f ) );
	/* offsets */

	auto speed = std::fmin ( animations::player_data[entity->index()].perdicted_velocity.length ( ), 260.0f );

	auto goal_feet_yaw = animstate->m_abs_yaw;
	/*static float shot [ 64 ];
	if ( entity->active_weapon ( ) ) {
		if ( shot [ entity->index ( ) ] != entity->active_weapon ( )->m_fLastShotTime ( ) ) {

			shot [ entity->index ( ) ] = entity->active_weapon ( )->m_fLastShotTime ( );
			printf ( "DETECteD SHOOT STOpeD goal feeet calculation" );
			return goal_feet_yaw;
		}
	}*/
	auto angle_diff = [ ] ( float destAngle, float srcAngle ) -> float {
		auto delta = 0.f;

		delta = fmodf ( destAngle - srcAngle, 360.0f );

		if ( destAngle > srcAngle ) {
			if ( delta >= 180 )
				delta -= 360;
		}
		else {
			if ( delta <= -180 )
				delta += 360;
		}

		return delta;
	};

	auto eye_feet_delta = angle_diff ( angle.y, goal_feet_yaw );

	auto flYawModifier = ( ( ( ground_fraction * -0.3f ) - 0.2f ) * running_speed ) + 1.0f;

	if ( duck_ammount > 0.0f )
		flYawModifier = flYawModifier + ( ( duck_ammount * ducking_speed ) * ( 0.5f - flYawModifier ) );

	auto flMaxYawModifier = flYawModifier * 58.f;
	auto flMinYawModifier = flYawModifier * -58.f;

	if ( eye_feet_delta <= flMaxYawModifier ) {
		if ( flMinYawModifier > eye_feet_delta )
			goal_feet_yaw = fabs ( flMinYawModifier ) + animstate->m_abs_yaw;
	}
	else
		goal_feet_yaw = animstate->m_abs_yaw - fabs ( flMaxYawModifier );

	if ( goal_feet_yaw > 5000 || goal_feet_yaw < -5000 )
		return 0.f;

	math::normalize_yaw ( goal_feet_yaw );

	if ( speed > 0.1f || fabs ( animations::player_data [ entity->index ( ) ].perdicted_velocity.z ) > 100.0f ) {
		goal_feet_yaw = math::fl_approach_angle (
			angle.y,
			goal_feet_yaw,
			( ( ground_fraction * 20.0f ) + 30.0f )
			* animstate->m_last_clientside_anim_update );
	}
	else {
		goal_feet_yaw = math::fl_approach_angle (
			entity->lower_body_yaw ( ),
			goal_feet_yaw,
			animstate->m_last_clientside_anim_update * 100.0f );
	}

	if ( goal_feet_yaw > 5000 || goal_feet_yaw < -5000 )
		return 0.f;

	math::normalize_yaw ( goal_feet_yaw );

	return goal_feet_yaw;
}

float resolver::max_desync_delta( player_t* entity ) {
	if ( !entity->get_anim_state( ) )
		return 0.f;
	auto animstate = uintptr_t( entity->get_anim_state( ) );

	float
		duckammount = *( float* ) ( animstate + 0xA4 ),
		speedfraction = max( 0.f, min( *( float* ) ( animstate + 0xF8 ), 1.f ) ),
		speedfactor = max( 0.f, min( 1.f, *( float* ) ( animstate + 0xFC ) ) );

	float
		unk1 = ( ( *( float* ) ( animstate + 0x11C ) * -0.30000001f ) - 0.19999999f ) * speedfraction,
		unk2 = unk1 + 1.f;

	if ( duckammount > 0 )
		unk2 += ( ( duckammount * speedfactor ) * ( 0.5f - unk2 ) );

	return *( float* ) ( animstate + 0x334 ) * unk2;

}


float resolver::get_safe_point_angle ( int i ) {
	
	float safe_point_angle = 0.f;
	switch ( config.ragebot_safe_point ) {
	case 0:
		safe_point_angle = 0.f;
		break;
	case 1:
		safe_point_angle = resolver::resolver_data [ i ].max_desync_delta / 2.f;
		break;
	case 2:
		safe_point_angle = 48.f;
		break;
	case 3:
		safe_point_angle = resolver::resolver_data [ i ].max_desync_delta;
		break;
	default:
		break;
	}
	//std::clamp ( safe_point_angle, 0.f, resolver::resolver_data [ i ].max_desync_delta );
	return safe_point_angle;
}
float resolver::get_brute_angle ( player_t * player ) {
	
	if ( resolver::resolver_data [ player->index ( ) ].side > 0 ) 		{
		switch ( resolver::resolver_data [ player->index ( ) ].side ) {
		case desync_side::left:
			return resolver::resolver_data [ player->index ( ) ].goal_feet_yaw - resolver::resolver_data [ player->index ( ) ].brute_angle;
			break;

		case desync_side::right:
			return resolver::resolver_data [ player->index ( ) ].goal_feet_yaw + resolver::resolver_data [ player->index ( ) ].brute_angle;
			break;
		default:
			break;
		}
	}

	return 0.f;
}
void resolver::manage_shots ( ) {
	/*return;
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
				float dif = math::angle_diff ( enemy->get_anim_state ( )->m_abs_yaw, angle_at_teammate );
				std::clamp ( dif, -58.f, 58.f );


			}
			else if ( shot->hurt && shot->hit ) {


				std::stringstream ss;
				ss << "Teammate " << name << "hit  " << name << " in " << " for " << shot->hit_info.damage << " damage.";

				visuals::notifications::add ( ss.str ( ) );
			
			}

			shot->approved = true;
		}
	

	}
	*/
}

std::string resolver::antiaim_name ( int index ) {
	auto data = resolver_data [ index ];
	switch ( data.antiaim_type ) {
	case none:
		return "none";
		break;
	case backwards:
		return "backwards";
		break;
	case sideways:
		return "sideways";
		break;
	default	:
		break;
	}
	return "none";
}
void resolver::resolve_player ( player_t * player ) {
	auto i = player->index ( );
	auto & data = resolver_data [ i ];
	auto yaw = player->eye_angles ( ).y;
	data.max_desync_delta = max_desync_delta ( player );

	data.safe_point_angle = resolver::resolver_data [ i ].max_desync_delta / 2.f;
	data.server_goal_feet = server_feet_yaw ( player, animations::player_data [ i ].last_networked_angle );
	data.desync_delta = fabs ( math::normalize_yaw ( yaw - data.server_goal_feet ) );
	

	const auto at_target_yaw = math::calc_angle ( local_player::m_data.pointer->origin ( ), player->origin ( ) ).y;
	const float delta = fabs ( math::normalize_yaw ( at_target_yaw - yaw ) );
	data.angle_at_me = at_target_yaw;
	if ( delta < 75)
		data.antiaim_type = backwards;
	else
		data.antiaim_type = sideways;
	
	if ( data.antiaim_type && player_manager::records [ player->index ( ) ].size() ) {
		/*auto origin = player->get_hitbox_position ( hitbox_head, player_manager::records [ player->index ( ) ].back ( ).bone );
		auto left = player->get_hitbox_position ( hitbox_head, player_manager::records [ player->index ( ) ].back ( ).bone_left );
		auto right = player->get_hitbox_position ( hitbox_head, player_manager::records [ player->index ( ) ].back ( ).bone_right );
		auto head_at_target = player->get_hitbox_position ( hitbox_head, player_manager::records [ player->index ( ) ].back ( ).bone_at_me );

		
		auto dist_left = math::calc_distance ( origin, left, false );
		auto dist_right = math::calc_distance ( origin, right, false );
		auto dist_exteremetis = math::calc_distance ( left, right, false );

		float dist_hitable = math::calc_distance ( origin, head_at_target, false );
		float dist_hitable_left = math::calc_distance ( left, head_at_target, false );
		float dist_hitable_right = math::calc_distance ( right, head_at_target, false );

		data.standing_hitable = dist_hitable > 1.7f && data.antiaim_type == backwards;

		bool is_left = fabs ( dist_left - dist_right ) < 1.5f;
		if ( !is_left ) {
			data.side = desync_side::right;
		}
		else {
			data.side = desync_side::left;
		}
	//	interfaces::console->console_printf ( "left distance %f  right distance %f  \n", dist_left, dist_right );
		interfaces::console->console_printf ( "data.standing_hitable %f  \n", dist_hitable );
		*/
	
	}

	/*if ( data.left_damage <= 0 && data.right_damage <= 0 ) {
		if ( data.right_fraction > data.left_fraction )
			data.side = desync_side::right;
		else
			data.side = desync_side::left;
	}
	else {
		if ( data.left_damage > data.right_damage )
			data.side = desync_side::right;
		else
			data.side = desync_side::left;
	}*/

}

void resolver::frame_stage ( ) {
	if ( !interfaces::engine->is_connected ( ) )
		return;

	if ( !interfaces::engine->is_in_game ( ) )
		return;

	if ( !local_player::m_data.alive )
		return;

	if ( !config.ragebot_enabled )
		return;


	//manage_shots ( );

	/*for ( int i = 1; i <= interfaces::globals->max_clients; i++ ) {
		player_t * entity = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( i ) );

		if ( !entity )
			continue;

		if ( entity->dormant ( ) )
			continue;
		//if ( !entity->is_enemy ( ) )
		//	continue;
		if ( !entity->is_alive())
			continue;
		

		

	}*/
}