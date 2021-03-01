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


	if ( !entity )
		return;


	if ( !entity->is_alive ( ) )
		return;
	
	if ( entity->dormant() )
		return;


	resolve_shot shot;
     shot.hit = false; 
     shot.tick = interfaces::globals->tick_count;
	 shot.hurt = false;
	 shot.attacker_index = entity->index ( );
	 shot.enemy_index = 0;
	 shot.desync_side = 0;
 	shot.shotpos = entity->eye_pos ();
	if ( shot.shotpos.is_zero ( ) )
		interfaces::console->console_printf ( "shot.shotpos is zero \n" );
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
	if ( attacker->dormant ( ) )
		return;

	if ( victim->dormant ( ) )
		return;
	

		auto hitbox = event->get_int ( "hitgroup" );
		if ( !hitbox )
			return;

		auto damage = event->get_int ( "dmg_health" );
		if ( !damage )
			return;

		auto health = event->get_int ( "health" );

		if ( !health && health != 0 )
			return;

		//if ( attacker->team ( ) != local_player->team ( ) )
		//	return;


		auto shot = closest_shot ( interfaces::globals->tick_count, attacker );
		if ( shot ) {
			float angle_at_attacker = math::calc_angle ( victim->eye_pos ( ), attacker->eye_pos ( ) ).y - 180.f;
			angle_at_attacker = math::clamp_yaw ( angle_at_attacker );

			shot->hurt = true;
			shot->enemy_index = victim->index();
			shot->angle = victim->get_anim_state ( )->m_abs_yaw;

			shot->hit_info.damage = damage;
			shot->hit_info.hitgroup = hitbox;
			shot->hit_info.victim = victim_index;


		
		}


//	}
	

}
player_t*  resolver::get_closest_player_by_point ( vec3_t from,  vec3_t point ) {
	player_t * closest = nullptr;
	auto distance = 12.5f;
	for ( int i = 1; i <= interfaces::globals->max_clients; i++ ) {
		player_t * entity = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( i ) );

		if ( !entity )
			continue;

		if ( entity->dormant ( ) )
			continue;

		if ( entity->health ( ) <= 0 )
			continue;

		auto angle_bullet = math::calc_angle ( from, point ); angle_bullet.angle_normalize ( );

		auto angle_enemy = math::calc_angle ( from, entity->eye_pos ( ) ); angle_enemy.angle_normalize ( );


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

void resolver::event_logs::bullet_impact ( i_game_event * event ) {

	
	auto userid = event->get_int ( "userid" );
	if ( !userid )
		return;

	if ( !interfaces::engine->is_connected ( ) && !interfaces::engine->is_in_game ( ) )
		return;

	auto engine_userid = interfaces::engine->get_player_for_user_id ( userid );
	if ( !engine_userid )
		return;



	auto entity = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( engine_userid ) );

	if ( !entity )
		return;


	if ( entity->dormant ( ) )
		return;




	vec3_t bullet_impact = vec3_t ( event->get_float ( "x" ), event->get_float ( "y" ), event->get_float ( "z" ) );


	auto shot = closest_shot ( interfaces::globals->tick_count, entity );

	if ( shot && !shot->bullat_impact_approved ) {
		auto target = get_closest_player_by_point ( entity->eye_pos ( ), bullet_impact );

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
			auto limit = shot->shotpos + direction * weapon_data->flRange;


			shot->hitpos = bullet_impact;
	
			shot->bullat_impact_approved = true;
			shot->desync_side = get_desync_side ( shot->shotpos, shot->hitpos, target, 0 );
		}

	}
}


int resolver::get_desync_side ( vec3_t from, vec3_t to, player_t * entity, int hitbox ) {
	auto VectorTransform_Wrapper = [ ] ( const vec3_t & in1, const matrix3x4_t & in2, vec3_t & out ) {
		auto VectorTransform = [ ] ( const float * in1, const matrix3x4_t & in2, float * out ) {
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
	int left_intersect = 0;
	int right_intersect = 0;
	for ( size_t i {}; i < set->hitbox_count; ++i ) {
		studio_box_t * _hitbox = set->hitbox ( i );
		if ( !hitbox || _hitbox->radius == -1.f )
			continue;
		vec3_t max;
		vec3_t min;
		const auto mod = _hitbox->radius != -1.f ? _hitbox->radius : 0.f;
		VectorTransform_Wrapper ( _hitbox->maxs, csgo::right_player_bones [ entity->index ( ) ][ _hitbox->bone ], max );
		VectorTransform_Wrapper ( _hitbox->mins, csgo::right_player_bones [ entity->index ( ) ][ _hitbox->bone ], min );


		vec3_t max2;
		vec3_t min2;

		VectorTransform_Wrapper ( _hitbox->maxs, csgo::left_player_bones [ entity->index ( ) ][ _hitbox->bone ], max2 );
		VectorTransform_Wrapper ( _hitbox->mins, csgo::left_player_bones [ entity->index ( ) ][ _hitbox->bone ], min2 );



		if ( auto intersection = aimbot::get_intersect_point ( from, to, min, max, _hitbox->radius ); intersection )
			left_intersect++;
	   if ( auto intersection = aimbot::get_intersect_point ( from, to, min2, max2, _hitbox->radius ); intersection )
			right_intersect++;
	}

	if ( left_intersect > right_intersect )
		return 1;
	else if ( right_intersect > left_intersect )
		return 2;

	return 0;
}

float resolver::server_feet_yaw ( player_t * entity ) {
	auto animstate = entity->get_anim_state ( );

	if ( !animstate )
		return 0.f;

	/* data */
	auto ground_fraction = *( float * ) ( animstate + 0x11C );
	auto duck_ammount = *( float * ) ( animstate + 0xA4 );
	auto ducking_speed = max ( 0.f, min ( 1.f, *reinterpret_cast< float * > ( animstate + 0xFC ) ) );
	auto running_speed = max ( 0.f, min ( *reinterpret_cast< float * > ( animstate + 0xF8 ), 1.f ) );
	/* offsets */
	auto backup_eflags = entity->get_ieflags ( );

	entity->get_ieflags ( ) = ( 1 << 12 );
	auto abs_velocity = *reinterpret_cast< vec3_t * > ( uintptr_t ( entity ) + 0x94 );
	entity->get_ieflags ( ) = backup_eflags;

	auto speed = std::fmin ( abs_velocity.length ( ), 260.0f );

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

	auto eye_feet_delta = angle_diff ( animstate->m_eye_yaw, goal_feet_yaw );

	auto flYawModifier = ( ( ( ground_fraction * -0.3f ) - 0.2f ) * running_speed ) + 1.0f;

	if ( duck_ammount > 0.0f )
		flYawModifier = flYawModifier + ( ( duck_ammount * ducking_speed ) * ( 0.5f - flYawModifier ) );

	auto flMaxYawModifier = flYawModifier * 58.f;
	auto flMinYawModifier = flYawModifier * -58.f;

	if ( eye_feet_delta <= flMaxYawModifier ) {
		if ( flMinYawModifier > eye_feet_delta )
			goal_feet_yaw = fabs ( flMinYawModifier ) + animstate->m_eye_yaw;
	}
	else
		goal_feet_yaw = animstate->m_eye_yaw - fabs ( flMaxYawModifier );

	if ( goal_feet_yaw > 5000 || goal_feet_yaw < -5000 )
		return 0.f;

	math::normalize_yaw ( goal_feet_yaw );

	if ( speed > 0.1f || fabs ( abs_velocity.z ) > 100.0f ) {
		goal_feet_yaw = math::fl_approach_angle (
			animstate->m_eye_yaw,
			goal_feet_yaw,
			( ( ground_fraction * 20.0f ) + 30.0f )
			* animstate->m_last_clientside_anim_update );
	}
	else {
		goal_feet_yaw = entity->lower_body_yaw ( );
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
	
	for ( size_t i = 0; i < shots.size ( ); i++ ) {
		auto& shot = shots.at ( i );
		if ( shot.bullat_impact_approved  && !shot.approved ) {
		

			if ( !shot.enemy_index )
				continue;


			auto enemy = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( shot.enemy_index ) );

			if ( !enemy )
				return;

			auto attacker = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( shot.attacker_index ) );

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


		     if ( shot.hurt && !shot.hit ) {


				std::stringstream ss;
				ss << "Player " << name << "hit  " << victim_name << " for " << shot.hit_info.damage << " damage. side ( " << shot.desync_side << " )";

				
				connection::telemetry send_data;
					send_data.lby = enemy->lower_body_yaw ( );
					send_data.autowall_side = 0;
					send_data.goal_feet = enemy->get_anim_state ( )->m_feet_yaw;
					send_data.left_yaw = enemy->eye_angles().y - 58.f;
					send_data.yaw = enemy->eye_angles ( ).y;
					send_data.right_yaw = enemy->eye_angles ( ).y + 58.f;
					send_data.map = interfaces::engine->get_level_name ( );
					send_data.hit_side = shot.desync_side;
					if ( send_data.hit_side == 1 )
						send_data.hit_side = 2; /*desync side gives real side*/
					else if ( send_data.hit_side == 2 )
						send_data.hit_side = 1;
					send_data.max_desync_delta = resolver::max_desync_delta ( enemy );
					send_data.missed = false;
					send_data.position = enemy->origin ( );
					send_data.send ( );
				

				visuals::notifications::add ( ss.str ( ) );

			
			 }
			 else if ( shot.desync_side > 0 && !shot.hit ) {
				 std::stringstream ss;
				 ss << "Player missed " << name << " side " << shot.desync_side;
				 connection::telemetry send_data ;
				 send_data.lby = enemy->lower_body_yaw ( );
				 send_data.autowall_side = 0;
				 send_data.goal_feet = enemy->get_anim_state ( )->m_feet_yaw;
				 send_data.left_yaw = enemy->eye_angles ( ).y - 58.f;
				 send_data.yaw = enemy->eye_angles ( ).y;
				 send_data.right_yaw = enemy->eye_angles ( ).y + 58.f;
				 send_data.map = interfaces::engine->get_level_name ( );
				 send_data.hit_side = static_cast<int>(shot.desync_side); /*this gives inverted side*/
				 send_data.max_desync_delta = resolver::max_desync_delta ( enemy );
				 send_data.missed = true;
				 send_data.position = enemy->origin ( );
				 send_data.send ( );

				 visuals::notifications::add ( ss.str ( ) );
			}

			shot.approved = true;
		}else 	if ( ( interfaces::globals->cur_time - shot.curtime ) > 15.f )
			shots.erase ( shots.begin ( ) + i );
	

	}
	
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
//	data.server_goal_feet = server_feet_yaw ( player, animations::player_data [ i ].last_networked_angle );
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



	if ( !config.ragebot_enabled )
		return;


	manage_shots ( );
	/*
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
		std::string entity_name = entity->get_info ( ).name;

		if ( entity_name.length ( ) > 17 )
			entity_name = entity_name.substr ( 0, 17 ) + "...";

		std::transform ( entity_name.begin ( ), entity_name.end ( ), entity_name.begin ( ), tolower );

		resolver_data [ i ].max_desync_delta = max_desync_delta ( entity );
	
		resolver_data [ i ].goal_feet_yaw = entity->get_anim_state ( )->m_abs_yaw;
	
		if ( player_manager::records [ i ].size ( ) > 0 ) {
			auto currentLayer = player_manager::records [ i ].back ( ).anim_layer [ 3 ];
			auto previousLayer = player_manager::records [ i ].front ( ).anim_layer [ 3 ];



			const int current_layer_activity = entity->get_sequence_act ( currentLayer.m_sequence );
			const int previous_layer_activity = entity->get_sequence_act ( previousLayer.m_sequence );
			float currentLayerflcycle = currentLayer.m_cycle, currentLayerflprevcycle = currentLayer.m_prev_cycle, currentLayerflweight = currentLayer.m_weight, currentLayerflweightdatarate = currentLayer.m_weight_delta_rate;
			float previousLayerflcycle = previousLayer.m_cycle, previousLayerflprevcycle = previousLayer.m_prev_cycle, previousLayerflweight = previousLayer.m_weight, previousLayerflweightdatarate = previousLayer.m_weight_delta_rate;
			uint32_t norder = currentLayer.m_order;

			if ( current_layer_activity == 979 ) // the current layer must be triggering 979
			{
				if ( previous_layer_activity == 979 ) // the previous layer must be trigerring 979
				{
					// we can tell now that he is surely breaking lby in some sort

					if ( ( previousLayer.m_cycle != currentLayer.m_cycle ) || currentLayer.m_weight == 1.f ) {
						resolver_data [ i ].lby_desync = true;
						float lby =  entity->lower_body_yaw ( ) ;
					//	float yaw =  animations::player_data [ entity->index ( ) ].last_networked_angle.y;
					//	interfaces::console->console_printf ( "HIGH LBY BREAK DETECTED FOR PLAYER %c LBY %f  YAW %f DELTA %f\n", entity_name.data ( ), lby, yaw, std::fabs(lby - yaw) );
					
					}
					else if ( currentLayer.m_weight == 0.f && ( previousLayer.m_cycle > 0.92f && currentLayer.m_cycle > 0.92f ) ) // breaking lby with delta < 120; can fakewalk here aswell
					{
						float lby = math::normalize_yaw ( entity->lower_body_yaw ( ) );
					//	float yaw = math::normalize_yaw ( animations::player_data [ entity->index ( ) ].last_networked_angle.y );
						//interfaces::console->console_printf ( "LOW DELTA LBY BREAK DETECTED FOR PLAYER %c LBY %f  YAW %f \n", entity_name.data ( ), lby, yaw );
					}
				}

			}
			//float orig_goal_feet = server_feet_yaw ( entity, animations::player_data[i].last_networked_angle);
		
			resolver_data [ i ].extended_desync = currentLayer.m_cycle == 0.f && currentLayer.m_weight == 0.f;
		}
	}
	*/

}