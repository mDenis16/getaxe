#include "../features.hpp"
#include "../../helpers/helpers.h"
#include <cmath>
#include <cfloat>

void player_manager::simulate_movement ( player_t * pEntity, vec3_t & vecOrigin, vec3_t & vecVelocity, int & fFlags, bool bOnGround ) {
	vec3_t                start, end, normal;
	trace_t            trace;
	trace_world_only filter;

	// define trace start.
	start = vecOrigin;

	// move trace end one tick into the future using predicted velocity.
	end = start + ( vecVelocity * interfaces::globals->interval_per_tick );

	ray_t s;
	s.initialize ( start, end, pEntity->mins ( ), pEntity->maxs ( ) );

	// trace.
	interfaces::trace_ray->trace_ray ( s , CONTENTS_SOLID, &filter, &trace );

	// we hit shit
	// we need to fix shit.
	if ( trace.flFraction != 1.f ) {

		// fix sliding on planes.
		for ( int i { }; i < 2; ++i ) {
			ray_t b;
			
			vecVelocity -= trace.plane.normal * vecVelocity.dot ( trace.plane.normal );

			float adjust = vecVelocity.dot ( trace.plane.normal );
			if ( adjust < 0.f )
				vecVelocity -= ( trace.plane.normal * adjust );

			start = trace.end;
			end = start + ( vecVelocity * ( interfaces::globals->interval_per_tick * ( 1.f - trace.flFraction ) ) );
			b.initialize ( start, end, pEntity->mins ( ), pEntity->maxs ( ) );
			interfaces::trace_ray->trace_ray ( b, CONTENTS_SOLID, &filter, &trace );
			if ( trace.flFraction == 1.f )
				break;
		}
	}

	// set new final origin.
	start = end = vecOrigin = trace.end;

	// move endpos 2 units down.
	// this way we can check if we are in/on the ground.
	end.z -= 2.f;

	// trace.
	ray_t c;
	c.initialize ( start, end, pEntity->mins ( ), pEntity->maxs() );
	interfaces::trace_ray->trace_ray ( c, CONTENTS_SOLID, &filter, &trace );

	// strip onground flag.
	fFlags &= ~fl_onground;

	// add back onground flag if we are onground.
	if ( trace.flFraction != 1.f && trace.plane.normal.z > 0.7f )
		fFlags |= fl_onground;
}

void player_manager::event_logs::bullet_impact ( i_game_event * event ) {
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

	
}
void __declspec( naked ) feet_wobble_fix ( ) {
	__asm
	{
		push edi
		xchg dword ptr [ esp ], edi
		push eax
		mov eax, 77
		mov eax, dword ptr [ esp ]
		add esp, 4
		pop edi

		cmp esp, 0
		jne fixentity

		_emit 0x88
		_emit 0xFF

		invlpg dword ptr [ eax ]

		int 2

		fixentity:
		sub esp, 4
			mov dword ptr [ esp ], ebp

			call cleanup

			pop ebp

			ret

			cleanup :

		ret
	}
}
player_t* player_manager::util_player_by_index( int index )
{
	typedef player_t* ( __fastcall* player_index )( int );
	static player_index c_index = ( player_index ) utilities::pattern_scan( "server.dll", "85 C9 7E 2A A1" );

	if ( !c_index )
		return nullptr;

	return c_index( index );
}
void player_manager::get_rotated_matrix ( vec3_t origin, matrix3x4_t * from_matrix, float angle, matrix3x4_t * new_matrix ) {
	vec3_t BonePos;
	vec3_t OutPos;

	for ( int i = 0; i < MAXSTUDIOBONES; i++ ) {
		math::angle_matrix ( vec3_t ( 0, angle, 0 ), new_matrix [ i ] );
		math::matrix_multiply ( new_matrix [ i ], from_matrix [ i ] );
		BonePos = vec3_t ( from_matrix [ i ][ 0 ][ 3 ], from_matrix [ i ][ 1 ][ 3 ], from_matrix [ i ][ 2 ][ 3 ] ) - origin;
		math::vector_rotate ( BonePos, vec3_t ( 0, angle, 0 ), OutPos );
		OutPos += origin;
		new_matrix [ i ][ 0 ][ 3 ] = OutPos.x;
		new_matrix [ i ][ 1 ][ 3 ] = OutPos.y;
		new_matrix [ i ][ 2 ][ 3 ] = OutPos.z;
	}

}
namespace player_manager {

	/*void interpolate_records (player_t* entity ) { never use this shit again

		const auto index = entity->index ( );
		auto & anim = animations::player_data [ entity->index ( ) ];

		if ( !records [ index ].empty ( ) && records [index].size() >= 2) {


			auto & current = records [ index ].back ( );
		

			auto & previous = records [ index ].at ( records [ index ].size ( ) - 2 );


			auto velocity_per_tick = ( current.velocity - previous.velocity ) / current.choked_ticks;

			auto origin_per_tick = ( current.origin - previous.origin ) / current.choked_ticks;

	
			for ( size_t i = 0; i < current.choked_ticks; i++ ) {

				if ( i % 2 != 0 )
					continue;

				lagcomp_t interpolated = { };
				std::memcpy ( &interpolated, &current, sizeof ( interpolated ) );
				

				if ( !origin_per_tick.is_zero ( ) )
					interpolated.origin = previous.origin + origin_per_tick * ( float ) ( i + 1 ); 


				interpolated.simtime = previous.simtime + math::ticks_to_time ( i );

				auto backup = entity->read<char> ( 0x274 );
				auto old_ik = entity->read<void *> ( 0x2670 );

				anim.update_bones = true;

				entity->invalidate_bone_cache ( );

				entity->set_abs_origin ( interpolated.origin ); vec3_t backup_abs = entity->abs_origin ( );
			
				entity->read<char> ( 0x274 ) = 0;
				entity->read<void *> ( 0x2670 ) = nullptr;

				entity->invalidate_bone_cache ( );
				vec3_t b = entity->abs_angles ( );
				entity->get_bone_accessor ( )->m_WritableBones = entity->get_bone_accessor ( )->m_ReadableBones = 0;
				entity->set_abs_angles ( vec3_t ( 0, current.animstate.m_abs_yaw, 0 ) );
				entity->setup_bones ( interpolated.bones ( predicted_side::UNKNOWN ), 128, 0x0007FF00, current.simtime );
				entity->set_abs_angles ( b );

				entity->read<void *> ( 0x2670 ) = old_ik;
				entity->read<char> ( 0x274 ) = backup;



				anim.update_bones = false;


				entity->set_abs_origin ( backup_abs );
				current.apply_anims ( entity );
				current.apply ( entity, false, true );
				generated_records [ index ].push_back ( interpolated );
				
			}
		
		}
	}*/
	void lagcomp_t::apply_anims ( player_t * entity ) {
		std::memcpy ( entity->get_animoverlays(), this->layer, sizeof(animationlayer) * 15);
		std::memcpy ( entity->get_anim_state ( ), &this->animstate, sizeof ( anim_state ) );
		std::memcpy ( entity->m_flPoseParameter ( ).data ( ), this->pos_params.data(), sizeof ( float ) * 24 );
	}
	void lagcomp_t::receive_anims ( player_t * entity ) {
		std::memcpy ( &this->layer, entity->get_animoverlays (), sizeof ( animationlayer ) * 15 );
		std::memcpy ( &this->animstate, entity->get_anim_state ( ), sizeof ( anim_state ) );
		std::memcpy ( this->pos_params.data ( ), entity->m_flPoseParameter ( ).data ( ), sizeof ( float ) * 24 );
	}

	void lagcomp_t::apply (player_t* entity, bool collision, bool restore ) {
		

		if ( !entity ) {
			this->failed = true;
			return;
		}

		if (!entity->m_CachedBoneData ( ).m_Size )
		{
			this->failed = true;
			return;
		}
		if ( entity->m_CachedBoneData ( ).Base() == nullptr) {
			this->failed = true;
			return;
		}
		if ( !entity->is_alive ( ) ) {
			this->failed = true;
			return;
		}

		if ( !entity->collideable ( ) ) {
			this->failed = true;
			return;
		}
		

		entity->set_abs_angles (  this->abs_angles );
		entity->set_abs_origin (  this->origin );

		entity->collideable ( )->mins ( ) = this->obbmin;
		entity->collideable ( )->maxs ( ) = this->obbmax;
	
	//	interfaces::console->console_printf ( "scanning side %i \n", this->resolve_info.side );
		memcpy ( entity->m_CachedBoneData ( ).Base ( ), this->bones ( restore ? UNKNOWN : this->resolve_info.side ), entity->m_CachedBoneData ( ).m_Size * sizeof ( matrix3x4_t ) );

	}
	void lagcomp_t::restore ( player_t * entity ) {
		auto & anim = animations::player_data [ entity->index ( ) ];

		if ( this->failed ) {

			return;
			
		}
		if ( records [ entity->index ( ) ].empty ( ) )
			return;


		auto current = records[ entity->index ( ) ].back ( );

		current.apply ( entity, true, true );


	}
	bool lagcomp_t::is_valid ( ) {
		const auto nci = interfaces::engine->get_net_channel_info ( );

		if ( !nci )
			return false;

		if ( invalid )
			return false;


		static auto sv_maxunlag = interfaces::console->get_convar (  ( "sv_maxunlag" ) );

		auto outgoing = nci->get_latency ( 0 );
		auto incoming = nci->get_latency ( 1 );

		auto correct = std::clamp ( outgoing + incoming + get_lerp_time ( ), 0.0f, sv_maxunlag->get_float ( ) );

		auto curtime = math::ticks_to_time ( localdata.fixed_tickbase );
		auto delta_time = correct - ( curtime - this->simtime );

		if ( fabs ( delta_time ) > 0.2f )
			return false;

		auto extra_choke = 0;

		if ( localdata.fakeducking )
			extra_choke = 14 - interfaces::clientstate->m_choked_commands;

		auto server_tickcount = extra_choke + interfaces::globals->tick_count + math::time_to_ticks ( outgoing + incoming );
		int dead_time = static_cast<int>( ( math::ticks_to_time ( server_tickcount ) - sv_maxunlag->get_float ( ) ));

		if ( this->simtime < static_cast<float>(dead_time)) 
			return false;

		return true;
	}

	void lagcomp_t::predict ( player_t * entity ) {

		if ( records.at ( entity->index ( ) ).empty ( ) )
			return;

		auto predict_origin = [ ] ( vec3_t origin, vec3_t velocity, int ticks ) {
			return origin + ( velocity * ( interfaces::globals->interval_per_tick * static_cast< float >( ticks ) ) );
		};

		auto predicted_origin = predict_origin ( this->origin, this->velocity, this->choked_ticks );

	

		this->predicted = true;

		this->simtime += math::ticks_to_time ( 1 );


		for ( auto & i : this->bone ) {
			vec3_t original = vec3_t ( i [ 0 ][ 3 ], i [ 1 ][ 3 ], i [ 2 ][ 3 ] );
			vec3_t predicted = predict_origin ( original, this->velocity, this->choked_ticks );
		
			i [ 0 ][ 3 ] = predicted.x;
			i [ 1 ][ 3 ] = predicted.y;
			i [ 2 ][ 3 ] = predicted.z;
		}

		this->abs_origin = this->origin = predicted_origin;
	}

	void lagcomp_t::receive ( player_t * entity ) {
		


		this->obbmin = entity->collideable ( )->mins ( );
		this->obbmax = entity->collideable ( )->maxs ( );
		this->entity = entity;
		this->lby = entity->lower_body_yaw ( );
		this->flags = entity->flags ( );
		this->simtime = entity->simulation_time ( );
		this->origin = entity->origin ( );
		this->eye_angles = entity->eye_angles ( );
		this->velocity = entity->velocity ( );
		this->old_vec_origin = entity->old_origin ( );
		this->origin = entity->origin ( );
		this->abs_origin = entity->origin ( );
		this->duck_amount = entity->duck_amount ( );
		if ( entity->active_weapon ( ) )
			this->shoot = entity->active_weapon ( )->m_fLastShotTime ( ) >= this->simtime;
		this->update_animations ( );

		
		this->abs_angles = entity->abs_angles ( );
		
	
		this->lby = entity->lower_body_yaw ( );



		this->bone_count = entity->m_CachedBoneData ( ).Count ( );





		this->speed = entity->velocity ( ).length_2d_sqr ( );
		
		this->flags = entity->flags ( );
		

		this->in_air = entity->is_in_air ( );
	
		this->goal_feet = entity->get_anim_state ( )->m_feet_yaw;


		if ( entity->simulation_time ( ) < entity->get_old_simulation_time ( ) )
			this->invalid = true;

	
	}
	void lagcomp_t::matrix_resolve ( player_t * entity ) {
		if ( this->in_air || this->max_delta < 36 )
			return;

	/*	auto closest_wall = vec3_t ( );
		float step = M_PI * 2.0 / 8;
		float radius = 130.f;
		float closest_dist = 999.f;

		auto position = this->origin +  entity->view_offset();
		for ( float a = 0; a < ( M_PI * 2.0 ); a += step ) {
			vec3_t location ( radius * cos ( a ) + position.x, radius * sin ( a ) + position.y, position.z );

			ray_t ray;
			trace_t tr;
			ray.initialize ( position, location );
			trace_filter_one_entity traceFilter;
			traceFilter.pEntity = entity;
			interfaces::trace_ray->trace_ray ( ray, MASK_SHOT, &traceFilter, &tr );
			auto dist = tr.end.distance_to ( position );

			if ( dist < closest_dist ) {
				closest_wall = tr.end;
				closest_dist = dist;
			}

		}

		if ( closest_dist < 120.f ) {
			if ( !closest_wall.is_zero ( ) ) {
				auto left_head = entity->get_hitbox_position ( hitbox_head, this->bone_left );
				auto right_head = entity->get_hitbox_position ( hitbox_head, this->bone_right );

				auto left_dist = left_head.distance_to ( closest_wall );
				auto right_dist = right_head.distance_to ( closest_wall );
			
				bool le = right_dist <= left_dist;
				if ( localdata.force_invert_resolver_on_key )
					le = !le;

				if ( le ) {
					std::memcpy ( this->bone_resolved, this->bone_left, sizeof ( this->bone_left ) );
					this->resolved = true;
					this->side = resolver::desync_side::left;
				}
				else {

					std::memcpy ( this->bone_resolved, this->bone_right, sizeof ( this->bone_right ) );
					this->resolved = true;
					this->side = resolver::desync_side::right;

				}
			}
		}
		
		

		*/
		
	}

	void lagcomp_t::manage_matrix ( player_t * entity ) {
		const auto setup_bones_temp = [ ] ( anim_state * state, player_manager::lagcomp_t* rec, float delta, predicted_side side ) {
			animationlayer backup_layers [ 15 ];
			std::memcpy ( backup_layers, rec->entity->get_animoverlays ( ), ( sizeof ( animationlayer ) * 15 ) );

			animations::backup ( );
			animations::update_globals ( rec->entity, rec->entity->simulation_time ( ) );
			rec->entity->get_clientside_animation ( ) = true;
			feet_wobble_fix ( );
			state->m_abs_yaw += delta;
			animations::update_anim_angle ( state,  vec3_t( rec->eye_angles.x, rec->eye_angles.y, 0 ));
			rec->entity->get_clientside_animation ( ) = false;
			animations::restore ( );

			switch ( side ) {
			case player_manager::UNKNOWN:
				break;
			case player_manager::LEFT:
				std::memcpy ( &rec->fake_left_layers, rec->entity->get_animoverlays ( ), ( sizeof ( animationlayer ) * 15 ) );
				break;
			case player_manager::RIGHT:
				std::memcpy ( &rec->fake_right_layers, rec->entity->get_animoverlays ( ), ( sizeof ( animationlayer ) * 15 ) );
				break;
			default:
				break;
			}
			
			std::memcpy ( rec->entity->get_animoverlays ( ), backup_layers, ( sizeof ( animationlayer ) * 15 ) );



			auto backup = rec->entity->read<char> ( 0x274 );
			auto old_ik = rec->entity->read<void *> ( 0x2670 );

			animations::player_data [ rec->entity->index ( ) ].update_bones = true;

			rec->entity->read<char> ( 0x274 ) = 0;
			rec->entity->read<void *> ( 0x2670 ) = nullptr;
			rec->entity->invalidate_bone_cache ( );
			rec->entity->get_bone_accessor ( )->m_WritableBones = rec->entity->get_bone_accessor ( )->m_ReadableBones = 0;
			vec3_t abs_angles = rec->entity->abs_angles ( );
		
			rec->entity->set_abs_angles ( vec3_t(0, state->m_abs_yaw, 0) );
			rec->entity->set_abs_origin ( rec->origin );
			rec->entity->setup_bones ( rec->bones ( side ), 128, 0x0007FF00, rec->simtime + static_cast<int>( side) );
			rec->entity->set_abs_angles ( abs_angles );
			rec->entity->read<void *> ( 0x2670 ) = old_ik;
			rec->entity->read<char> ( 0x274 ) = backup;

			
			animations::player_data [ rec->entity->index ( ) ].update_bones = false;
			rec->apply ( rec->entity, false, true );

		};

		setup_bones_temp ( &this->fake_left_state, this, -this->max_delta, predicted_side::LEFT );
		this->apply_anims ( this->entity  );
		setup_bones_temp ( &this->fake_right_state, this, this->max_delta, predicted_side::RIGHT );
		this->apply_anims ( this->entity );
	}
	void lagcomp_t::delta ( ) {
		if ( this->entity  && this->entity->is_alive() && &this->animstate ) {
			try {


				this->m_flSpeed = std::fmin ( this->velocity.length ( ), 260.0f );

			//	interfaces::console->console_printf ( "m_flSpeed %f \n", m_flSpeed );
				float flMaxMovementSpeed = 260.0f;
				if ( this->entity->active_weapon ( ) && this->entity->active_weapon ( )->get_weapon_data ( ) )
					flMaxMovementSpeed = std::fmax ( this->entity->active_weapon ( )->get_weapon_data ( )->flMaxPlayerSpeed, 0.001f );

				const float m_flRunningSpeed = std::clamp ( m_flSpeed / ( flMaxMovementSpeed * 0.520f ), 0.0f, 1.f );
				const float m_flDuckingSpeed = std::clamp ( m_flSpeed / ( flMaxMovementSpeed * 0.340f ), 0.0f, 1.f );


				float flYawModifier = ( ( this->animstate.m_ground_fraction * -0.3f ) - 0.2f ) * m_flRunningSpeed + 1.0f;


				if ( this->duck_amount > 0 )
					flYawModifier += ( ( this->duck_amount * m_flDuckingSpeed ) * ( 0.5f - flYawModifier ) );

				this->max_delta = localdata.force_low_delta_on_key ? 28 : this->animstate.m_max_yaw * flYawModifier;
			}
			catch ( int ex ) {
				interfaces::console->console_printf ( "ai luat muie  ti-a dat crash \n" );
			}
		}
	}
	void lagcomp_t::update_animations (  ) {
		try {
			auto & anim = animations::player_data [ entity->index ( ) ];



			if ( !this->entity )
				return;
			if ( !this->entity->is_alive ( ) )
				return;

			if ( this->entity->dormant ( ) )
				return;

			auto animstate = this->entity->get_anim_state ( );

			if ( !animstate )
				return;

			const auto nci = interfaces::engine->get_net_channel_info ( );

			if ( !nci )
				return;


			this->choked_ticks = 3;


			const auto index = entity->index ( );


			const auto o_eflags = entity->get_ieflags ( );
			this->network_update = true;


			bool failed_animating = true;

			this->receive_anims ( entity );


			animationlayer backup_layers [ 15 ];
			std::memcpy ( backup_layers, entity->get_animoverlays ( ), ( sizeof ( animationlayer ) * 15 ) );

			if ( !records [ index ].empty ( ) ) {


				auto & previous = records [ index ].back ( );


				this->network_update = records [ entity->index ( ) ].empty ( ) || this->simtime > previous.simtime;
				/*if ( this->network_update && !records [ entity->index ( ) ].empty ( ) ) {
					auto server_tick = interfaces::clientstate->m_iServerTick - entity->index ( ) % interfaces::globals->timestamp_randomize_window;
					auto current_tick = server_tick - server_tick % interfaces::globals->timestamp_networking_base;

					if ( math::time_to_ticks ( entity->get_old_simulation_time ( ) ) < current_tick && math::time_to_ticks ( entity->simulation_time ( ) ) == current_tick ) {
						auto layer = &records [ entity->index ( ) ].back ( ).layer [ 11 ];
						auto previous_layer = &records [ entity->index ( ) ].front ( ).layer [ 11 ];
						//interfaces::console->console_printf ( "layer 11  m_cycle %f  old m_cycle %f \n", layer->m_cycle, previous_layer->m_cycle );
						if ( layer->m_cycle == previous_layer->m_cycle ) //-V550
						{
							entity->simulation_time ( ) = entity->get_old_simulation_time ( );
							this->simtime = previous.simtime;
							this->network_update = false;
						}
					}
				}*/
				if ( this->network_update ) {

					this->choked_ticks = math::time_to_ticks ( this->simtime - previous.simtime );

					if ( !this->choked_ticks )
						this->choked_ticks = 1;

					//if ( this->choked_ticks > 2 )
						//this->choked_ticks--;

					//interfaces::console->console_printf ( "network update choked ticks %i  simtime %i, old simtime %i \n", this->choked_ticks, math::time_to_ticks ( this->simtime ), math::time_to_ticks ( previous.simtime ) );


					auto duck_amount_per_tick = ( entity->duck_amount ( ) - previous.duck_amount ) / this->choked_ticks;
					previous.apply_anims ( entity );

					entity->get_ieflags ( ) &= ~0x1000;


					float backup_duck_amount = this->entity->duck_amount ( );

					auto velocity_per_tick = ( this->velocity - previous.velocity ) / this->choked_ticks;

					auto origin_per_tick = ( this->origin - previous.origin ) / this->choked_ticks;


					for ( size_t i = 0; i < this->choked_ticks; i++ ) {

						if ( duck_amount_per_tick ) {
							this->entity->duck_amount ( ) = previous.duck_amount + duck_amount_per_tick * ( float ) i;
						}

						if ( !velocity_per_tick.is_zero ( ) )
							this->velocity = previous.velocity + velocity_per_tick * ( float ) i;

						if ( !origin_per_tick.is_zero ( ) )
							this->origin = previous.origin + origin_per_tick * ( float ) ( i + 1 ); /*predict 1 tick future*/

						if ( entity->flags ( ) & fl_onground && entity->velocity ( ).length ( ) > 0.0f && backup_layers [ 6 ].m_weight <= 0.0f )
							this->velocity = vec3_t ( 0, 0, 0 ); /*player suddenly stopped*/


						auto simulated_time = previous.simtime + math::ticks_to_time ( i );

						//if ( this->choked_ticks <= 1 ) 
							//simulated_time = this->simtime; /*don't simulate 1 tick future*/


						entity->get_abs_velocity ( ) = this->velocity;



						animations::backup ( );
						animations::update_globals ( this->entity, simulated_time );
						this->entity->get_clientside_animation ( ) = true;
						feet_wobble_fix ( );
						animations::update_anim_angle ( animstate, this->eye_angles );
						this->entity->get_clientside_animation ( ) = false;
						animations::restore ( );


						std::memcpy ( this->predicted_layers, this->entity->get_animoverlays ( ), ( sizeof ( animationlayer ) * 15 ) );
						//interfaces::console->console_printf ( "this->resolve_layers[3] %f", this->resolve_layers[3].m_playback_rate );

						failed_animating = false;
					}

					std::memcpy ( this->entity->get_animoverlays ( ), backup_layers, ( sizeof ( animationlayer ) * 15 ) );
					this->entity->duck_amount ( ) = backup_duck_amount;


				}
			}
			else if ( entity->simulation_time ( ) != entity->get_old_simulation_time ( ) ) {
				animations::backup ( );
				animations::update_globals ( this->entity, this->simtime );
				this->entity->get_clientside_animation ( ) = true;
				feet_wobble_fix ( );
				if ( entity->flags ( ) & fl_onground && entity->velocity ( ).length ( ) > 0.0f && backup_layers [ 6 ].m_weight <= 0.0f )
					this->velocity = vec3_t ( 0, 0, 0 ); /*player suddenly stopped*/
				this->entity->get_abs_velocity ( ) = this->velocity;
				animations::update_anim_angle ( this->entity->get_anim_state ( ), this->eye_angles );
				this->entity->get_clientside_animation ( ) = false;
				animations::restore ( );

				std::memcpy ( this->entity->get_animoverlays ( ), backup_layers, ( sizeof ( animationlayer ) * 15 ) );
				//interfaces::console->console_printf ( "game update animations %i \n", this->choked_ticks );
				this->network_update = true;
			}


			if ( this->network_update ) {
				std::memcpy ( this->layer, entity->get_animoverlays ( ), ( sizeof ( animationlayer ) * 15 ) );
				std::memcpy ( &this->animstate, entity->get_anim_state ( ), sizeof ( anim_state ) );
				std::memcpy ( &this->fake_left_state, entity->get_anim_state ( ), sizeof ( anim_state ) );
				std::memcpy ( &this->fake_right_state, entity->get_anim_state ( ), sizeof ( anim_state ) );
				std::memcpy ( this->pos_params.data ( ), entity->m_flPoseParameter ( ).data ( ), sizeof ( float ) * 24 );

				this->delta ( );

				//interfaces::console->console_printf ( "predicted delta %f \n", this->max_delta );

				//this->body_rotation ( );



				auto predict_origin = [ ] ( vec3_t origin, vec3_t velocity, int ticks ) {
					return origin + ( velocity * ( interfaces::globals->interval_per_tick * static_cast< float >( ticks ) ) );
				};




				auto backup = this->entity->read<char> ( 0x274 );
				auto old_ik = this->entity->read<void *> ( 0x2670 );

				anim.update_bones = true;

				entity->invalidate_bone_cache ( );

				entity->set_abs_origin ( entity->is_in_air ( ) ? predict_origin ( this->origin, this->velocity, 1 ) : this->origin ); vec3_t backup_abs = entity->abs_origin ( );

				this->entity->read<char> ( 0x274 ) = 0;
				this->entity->read<void *> ( 0x2670 ) = nullptr;

				this->entity->invalidate_bone_cache ( );
				this->entity->get_bone_accessor ( )->m_WritableBones = this->entity->get_bone_accessor ( )->m_ReadableBones = 0;
				this->entity->setup_bones ( this->bones ( predicted_side::UNKNOWN ), 128, 0x0007FF00, this->simtime );


				this->entity->read<void *> ( 0x2670 ) = old_ik;
				this->entity->read<char> ( 0x274 ) = backup;



				anim.update_bones = false;

				this->apply_anims ( this->entity );

				this->manage_matrix ( this->entity );
				this->resolve ( );

				entity->set_abs_origin ( backup_abs );

			}

			entity->get_ieflags ( ) = o_eflags;
		}
		catch ( int ex ) {

		}
	}
}

void player_manager::manage_bones( ) {
	//if ( !config.ragebot_enabled )
		//return;

	if ( !local_player::m_data.pointer )
		return;

	

	for ( int i = 1; i <= interfaces::globals->max_clients; i++ ) {
		player_t* entity = reinterpret_cast< player_t* >( interfaces::entity_list->get_client_entity( i ) );

		if ( !entity )
			continue;

	

		if ( entity->dormant( ) )
			continue;

		if ( !entity->is_alive() )
			continue;

		if ( entity == local_pointer )
			continue;

		static auto set_interpolation_flags = [ ] ( player_t* e, int flag ) {
			const auto var_map = ( uintptr_t ) e + 36;
			const auto sz_var_map = *( int* ) ( var_map + 20 );

			for ( auto index = 0; index < sz_var_map; index++ )
				*( uintptr_t* ) ( ( *( uintptr_t* ) var_map ) + index * 12 ) = flag;
		};
		
		  set_interpolation_flags ( entity, 0 );


	
	}


}

void player_manager::setup_players( ) {
	
	
	

	for ( int i = 1; i <= interfaces::globals->max_clients; i++ ) {
		player_t* player = reinterpret_cast< player_t* >( interfaces::entity_list->get_client_entity( i ) );

		if ( !player )
			continue;

		if ( player->dormant( ) )
			continue;

		if ( !player->is_alive() )
			continue;


		float fDuration = -1.f;
		static uintptr_t pCall = ( uintptr_t ) utilities::pattern_scan( "server.dll", "55 8B EC 81 EC ? ? ? ? 53 56 8B 35 ? ? ? ? 8B D9 57 8B CE" );
		PVOID pEntity  = util_player_by_index( i );

		if ( pEntity )
		{
			__asm
			{
				pushad
				movss xmm1, fDuration
				push 0 //bool monoColor
				mov ecx, pEntity
				call pCall
				popad
			}
		}

	}
}




float player_manager::get_lerp_time( )
{


	static auto cl_updaterate = interfaces::console->get_convar( "cl_updaterate" );
	static auto cl_interp = interfaces::console->get_convar( "cl_interp" );

	const auto update_rate = cl_updaterate->get_int( );
	const auto interp_ratio = cl_interp->get_float( );

	auto lerp = interp_ratio / update_rate;

	if ( lerp <= cl_interp->get_float( ) )
		lerp = cl_interp->get_float( );

	return lerp;
}



void player_manager::filter_records ( ) {
	for ( int i = 1; i <= interfaces::globals->max_clients; i++ ) {
		player_t * player = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( i ) );


		if ( !player )
			continue;

		if ( player == local_player::m_data.pointer )
			continue;

		if ( player->team ( ) == local_player::m_data.pointer->team ( ) )
			continue;

		if ( records [ i ].size ( ) ) {

			if ( player->dormant ( ) || !player->is_alive ( ) ) {
				records [ i ].clear ( );
				continue;
			}

			for ( size_t t = 0; t < records.at ( i ).size ( ); t++ ) {
				auto & rec = records [ i ].at ( t );

				if ( !rec.is_valid ( ) )
					rec.invalid = true;
			}
		}
	}
}
void player_manager::setup_records ( client_frame_stage_t stage ) {

	if ( stage != FRAME_NET_UPDATE_END )
		return;

	if ( update_tick != interfaces::globals->tick_count ) {
		local_player::m_data.pointer = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( interfaces::engine->get_local_player ( ) ) );
		for ( int i = 1; i <= interfaces::globals->max_clients; i++ ) {
			player_t * player = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( i ) );

			if ( !player )
				continue;

			if ( player == local_player::m_data.pointer )
				continue;

			if ( player->team ( ) == local_player::m_data.pointer->team ( ) )
				continue;

			if ( !player->dormant ( ) && player->is_alive ( ) ) {


				if ( !records [ i ].empty ( ) && ( player->origin ( ) - records [ i ].back ( ).origin ).length_sqr ( ) > 4096.0f )
					for ( auto & record : records [ i ] )
						record.invalid = true;

				lagcomp_t record;


				record.receive ( player );



				if ( record.network_update ) {
					records [ i ].push_back ( record );
					for ( size_t t = 0; t < records.at ( i ).size ( ); t++ ) {
						auto & rec = records [ i ].at ( t );

						if ( rec.invalid )
							records [ i ].erase ( records [ i ].begin ( ) + t );
					}
				}
				
			}
		
			/*if ( !records [ i ].empty ( ) ) {
				const auto move = player->get_animoverlays ( ) [ 6 ];
				interfaces::console->console_printf ( "LAYER 6  \n" );


				interfaces::console->console_printf ( "m_weight %f \n", move.m_weight );
				interfaces::console->console_printf ( "m_weight_delta_rate %f \n", move.m_weight_delta_rate );
				interfaces::console->console_printf ( "m_playback_rate %f \n", move.m_playback_rate );
				interfaces::console->console_printf ( "m_cycle %f \n", move.m_cycle );

				interfaces::console->console_printf ( "========================= \n" );


				interfaces::console->console_printf ( "END \n" );


			}*/
		}
		update_tick = interfaces::globals->tick_count;
	}
	
}
