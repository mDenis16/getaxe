#include "../features.hpp"


std::deque<resolver::resolve_shot> resolver::shots;
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


void resolver::frame_stage ( ) {
	if ( !interfaces::engine->is_connected ( ) )
		return;

	if ( !interfaces::engine->is_in_game ( ) )
		return;



	if ( !config.ragebot_enabled )
		return;

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

namespace player_manager {
	std::string side_name ( predicted_side side ) {
		switch ( side ) {
		case player_manager::UNKNOWN:
			return "unknown";
			break;
		case player_manager::LEFT:
			return "left";
			break;
		case player_manager::RIGHT:
			return "right";
			break;
		default:
			break;
		}
	}
	std::string method_name ( desync_type type ) {
		switch ( type ) {
		case NONE:
			return "unknown";
			break;
		case MICROMOVEMENTS:
			return "micromovements";
			break;
		case LBY_OPOSITE:
			return "lby oposite";
			break;
		case LBY_SIDEWAYS:
			return  "unknown";
			break;
		default:
			return "?";
			break;
		}
	}
	float anim_round ( float f ) {
		return floorf ( f * 100 ) / 100;
	}
	void lagcomp_t::wall_detection ( ) {

	}
	void lagcomp_t::resolve ( ) {

		if ( records [ this->entity->index ( ) ].empty ( ) )
			return;



		auto & previous = records [ this->entity->index ( ) ].back ( );
		const float speed = this->velocity.length ( );
		//interfaces::console->console_printf ( "speed : %f\n", speed );

		this->resolve_info = previous.resolve_info;
		if ( interfaces::inputsystem->is_button_down ( button_code_t::KEY_LSHIFT ) ) {
			this->resolve_info.side = predicted_side::LEFT;
		}
		/*
		to do system:
		if enemy changes side while is standing send data to server and wait predicted data from it.

		*/

		/* resolve shot */
		/*
		debug left

		eye angle  -173.704834
lowerbody  -113.709778

eye angle  -7.635498
lowerbody  52.360245



debug right

eye angle  -173.704834
lowerbody  126.290222

eye angle  -5.657959
lowerbody  -65.659729

		*/
		/*if ( static_cast< int >( speed * 1000 ) <= 802 ) {
			const auto lby = math::normalize_yaw ( this->entity->lower_body_yaw ( ) );
			const auto eye = math::normalize_yaw ( previous.eye_angles.y );
			const auto delta = math::angle_diff ( lby, eye );

			if ( delta > 14 ) /*make sure is desynced*/
		{
			/*		if ( lby >= previous.eye_angles.y ) {
						interfaces::console->console_printf ( "PREDICT LEFT SIDE  %f\n", eye );
					}else
						interfaces::console->console_printf ( "PREDICT RIGHT SIDE  %f\n", eye );
				}

				interfaces::console->console_printf ( "eye angle  %f\n", eye );
				interfaces::console->console_printf ( "lowerbody  %f\n", lby );

			}
			else {*/

			if ( static_cast< int >( speed * 1000 ) >= 252 ) {

				const float first_delta = std::fabs ( this->fake_left_layers [ 6 ].m_playback_rate * 1000.f - this->layer [ 6 ].m_playback_rate * 1000.f );
				const float second_delta = std::fabs ( this->fake_right_layers [ 6 ].m_playback_rate * 1000.f - this->layer [ 6 ].m_playback_rate * 1000.f );
				const float four_delta = std::fabs ( this->fake_right_layers [ 6 ].m_playback_rate * 1000.f - this->fake_left_layers [ 6 ].m_playback_rate * 1000.f );

				this->third_delta = std::fabs ( first_delta - second_delta ) * 1000;

				//	if ( this->m_flSpeed < 2.f )
					//	return;

				interfaces::console->console_printf ( "first delta %f second_delta %f third_delta %i four_delta %f \n", first_delta, second_delta, this->third_delta, four_delta );
				if ( first_delta > second_delta ) {
					this->resolve_info.side = predicted_side::RIGHT;

					//interfaces::console->console_printf ( "PREDICTED SIDE RIGHT\n" );
				}
				else if ( second_delta > first_delta ) {

					this->resolve_info.side = predicted_side::LEFT;

					//	interfaces::console->console_printf ( "PREDICTED SIDE LEFT\n" );
				}
			}
			//}

			/*debug*/
			const auto lean = this->predicted_layers [ 6 ];
			interfaces::console->console_printf ( "ANIMATION_LAYER_AIMMATRIX  \n" );
			interfaces::console->console_printf ( "eye angle  %f\n", math::normalize_yaw ( this->eye_angles.y ) );
			interfaces::console->console_printf ( "lowerbody  %f\n", math::normalize_yaw ( this->entity->lower_body_yaw ( ) ) );
			interfaces::console->console_printf ( "current feet  %f\n", math::normalize_yaw ( this->animstate.m_feet_yaw ) );
			interfaces::console->console_printf ( "feet yaw cycle %f\n", this->animstate.m_feet_cycle );
			interfaces::console->console_printf ( "m_abs_yaw %f\n", math::normalize_yaw ( this->animstate.m_abs_yaw ) );
			interfaces::console->console_printf ( "m_eye_yaw %f\n", math::normalize_yaw ( this->animstate.m_eye_yaw ) );
			interfaces::console->console_printf ( "m_max_yaw %f\n", this->animstate.m_max_yaw );
			interfaces::console->console_printf ( "m_max_yaw %f\n", this->animstate.m_min_yaw );

			interfaces::console->console_printf ( "m_anim_time  %f\n", lean.m_anim_time );
			interfaces::console->console_printf ( "m_fade_out_time  %f\n", lean.m_fade_out_time );
			interfaces::console->console_printf ( "m_flags  %i\n", lean.m_flags );
			interfaces::console->console_printf ( "m_activty %i \n", lean.m_activty );
			interfaces::console->console_printf ( "m_priority %i \n", lean.m_priority );
			interfaces::console->console_printf ( "m_order %i \n", lean.m_order );
			interfaces::console->console_printf ( "m_sequence %i \n", lean.m_sequence );
			interfaces::console->console_printf ( "m_prev_cycle %i \n", lean.m_prev_cycle );
			interfaces::console->console_printf ( "m_weight %f \n", lean.m_weight );
			interfaces::console->console_printf ( "m_weight_delta_rate %f \n", lean.m_weight_delta_rate );
			interfaces::console->console_printf ( "m_playback_rate %f \n", lean.m_playback_rate );
			interfaces::console->console_printf ( "m_cycle %f \n", lean.m_cycle );
		}
	}
}/*
			if ( static_cast< int >( previous.eye_angles.y ) == static_cast< int >( previous.eye_angles.y ) && this->velocity.Length2D() < 10 ) /*make sure he doesnt break lby*/
			

				/*	if ( this->layer [ 6 ].m_cycle > previous.layer [ 6 ].m_cycle && this->layer [ 6 ].m_cycle <= 1.f ) {
						this->resolve_info.type = MICROMOVEMENTS;/*sometimes when enemy chokes more than 6 it will fail*/
						//	if ( ( this->choked_ticks - 1 ) % 2 == 0 )  /*detection will fail if enemy chokes numere impare*/ {
							/*long int corrected_rate = this->layer [ 6 ].m_playback_rate * 1000000;
							long int previous_corrected_rate = previous.layer [ 6 ].m_playback_rate * 1000000;
							if ( corrected_rate > 100 || previous_corrected_rate > 100 )
								return;

							this->delta_rate = std::labs ( corrected_rate - previous_corrected_rate );
							//if ( this->delta_rate != previous.delta_rate ) {
							interfaces::console->console_printf ( "delta_rate  %i choke %i \n", delta_rate, this->choked_ticks - 1 );
							//}
							if ( corrected_rate >= 3 ){

							}else {
							}

						}

					}
					else {
						const bool is_jittering = math::angle_diff ( this->eye_angles.y, previous.eye_angles.y ); /* ensure this guy doesnt jitter*/
						//	const bool is_lby_oposite = math::angle_diff ( math::normalize_yaw ( this->lby ), math::normalize_yaw ( this->eye_angles.y - 180.f ) ) < 15.f;
							//interfaces::console->console_printf ( "lby %f yaw %f \n", this->lby, this->eye_angles.y );
						//	interfaces::console->console_printf ( " previous.layer[3].m_cycle %f this->layer[3].m_cycle %f \n", previous.layer [ 3 ].m_cycle, this->layer [ 3 ].m_cycle );


				//if ( this->lby != this->eye_angles.y && this->layer [ 3 ].m_cycle == 0.0 ) /*make sure he break lby at the oposite and he doesnt twitches otherwise this method wont work*/
				/*{
					if ( this->layer [ 6 ].m_cycle != previous.layer [ 6 ].m_cycle && this->layer [ 6 ].m_cycle != 0.0 ) {
						this->resolve_info.type = desync_type::LBY_OPOSITE;
						interfaces::console->console_printf ( " detected lby oposite cycle layer 6 %f prev cycle layer %f \n", this->layer [ 6 ].m_cycle, previous.layer [ 6 ].m_cycle );

						if ( this->layer [ 6 ].m_cycle < 0.6f && this->layer [ 6 ].m_cycle > 0.20f ) {
							this->resolve_info.side = predicted_side::LEFT;
							interfaces::console->console_printf ( " side  left\n" );
						}
						else if ( this->layer [ 6 ].m_cycle > 0.5f && this->layer [ 6 ].m_cycle < 1.f ) {

							this->resolve_info.side = predicted_side::RIGHT;
							interfaces::console->console_printf ( " side  right\n" );
						}

						// detected lby oposite cycle layer 6 0.570526 prev cycle layer 0.570496

					}
				}
				interfaces::console->console_printf ( "start \n" );
				interfaces::console->console_printf ( "left weight delta rate %f \n", this->fake_left_layers [ 6 ].m_weight_delta_rate );
				interfaces::console->console_printf ( "right weight delta rate %f \n", this->fake_right_layers [ 6 ].m_weight_delta_rate );
				interfaces::console->console_printf ( "original weight delta rate %f \n", this->layer [ 6 ].m_weight_delta_rate );
				interfaces::console->console_printf ( "predicted weight delta rate %f \n", this->predicted_layers [ 6 ].m_weight_delta_rate );
				interfaces::console->console_printf ( " \n" );
				interfaces::console->console_printf ( "left playback rate %f \n", this->fake_left_layers [ 6 ].m_playback_rate );
				interfaces::console->console_printf ( "right playback rate %f \n", this->fake_right_layers [ 6 ].m_playback_rate );
				interfaces::console->console_printf ( "original playback rate %f \n", this->layer [ 6 ].m_playback_rate );
				interfaces::console->console_printf ( "predicted playback rate %f \n", this->predicted_layers [ 6 ].m_playback_rate );
				interfaces::console->console_printf ( "end \n" );*/
				//	interfaces::console->console_printf ( "  lby oposite cycle layer 6 %f prev cycle layer %f \n", this->layer [ 6 ].m_cycle, previous.layer [ 6 ].m_cycle );


					//else
						//this->resolve_info.side = predicted_side::UNKNOWN;

				//}
				/*if ( !records [ entity->index ( ) ].empty ( ) ) {

				}*/
				//13
				/*const auto lean = this->resolve_layers [ 3 ];
				interfaces::console->console_printf ( "ANIMATION_LAYER_ADJUST  \n" );
				interfaces::console->console_printf ( "eye angle  %f\n", math::normalize_yaw ( this->eye_angles.y ) );
				interfaces::console->console_printf ( "lowerbody  %f\n", math::normalize_yaw ( this->entity->lower_body_yaw ( ) ) );
				interfaces::console->console_printf ( "current feet  %f\n", math::normalize_yaw ( this->animstate.m_feet_yaw ) );
				interfaces::console->console_printf ( "feet yaw cycle %f\n", this->animstate.m_feet_cycle );
				interfaces::console->console_printf ( "m_abs_yaw %f\n", math::normalize_yaw ( this->animstate.m_abs_yaw ) );
				interfaces::console->console_printf ( "m_eye_yaw %f\n", math::normalize_yaw ( this->animstate.m_eye_yaw ) );
				interfaces::console->console_printf ( "m_max_yaw %f\n", this->animstate.m_max_yaw );
				interfaces::console->console_printf ( "m_max_yaw %f\n", this->animstate.m_min_yaw );

				interfaces::console->console_printf ( "m_anim_time  %f\n", lean.m_anim_time );
				interfaces::console->console_printf ( "m_fade_out_time  %f\n", lean.m_fade_out_time );
				interfaces::console->console_printf ( "m_flags  %i\n", lean.m_flags );
				interfaces::console->console_printf ( "m_activty %i \n", lean.m_activty );
				interfaces::console->console_printf ( "m_priority %i \n", lean.m_priority );
				interfaces::console->console_printf ( "m_order %i \n", lean.m_order );
				interfaces::console->console_printf ( "m_sequence %i \n", lean.m_sequence );
				interfaces::console->console_printf ( "m_prev_cycle %i \n", lean.m_prev_cycle );
				interfaces::console->console_printf ( "m_weight %f \n", lean.m_weight );
				interfaces::console->console_printf ( "m_weight_delta_rate %f \n", lean.m_weight_delta_rate );
				interfaces::console->console_printf ( "m_playback_rate %f \n", lean.m_playback_rate );
				interfaces::console->console_printf ( "m_cycle %f \n", lean.m_cycle );*/



		/*
		float   m_anim_time;			// 0x0000
	float   m_fade_out_time;		// 0x0004
	int     m_flags;				// 0x0008
	int     m_activty;				// 0x000C
	int     m_priority;				// 0x0010
	int     m_order;				// 0x0014
	int     m_sequence;				// 0x0018
	float   m_prev_cycle;			// 0x001C
	float   m_weight;				// 0x0020
	float   m_weight_delta_rate;	// 0x0024
	float   m_playback_rate;		// 0x0028
	float   m_cycle;				// 0x002C
	void * m_owner;				// 0x0030
	int     m_bits;					// 0x0034
		*/
	
