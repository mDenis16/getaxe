#include "../features.hpp"
#include "../../helpers/helpers.h"



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

	auto entity = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( engine_userid ) );

	if ( !entity )
		return;
	if ( !entity->is_alive ( ) )
		return;
	if ( entity->dormant ( ) )
		return;
	if ( records [ entity->index ( ) ].size ( ) < 0 )
		return;

	vec3_t bullet_impact = vec3_t ( event->get_float ( "x" ), event->get_float ( "y" ), event->get_float ( "z" ) );
	vec3_t enemy_angle = math::calc_angle ( entity->get_eye_pos ( ), bullet_impact );


	float
		event_time = math::ticks_to_time ( interfaces::globals->tick_count ),
		player_time = entity->simulation_time ( );

	float shot_time = event_time;

	for ( auto & record : records [ entity->index ( ) ] ) {

		if ( math::ticks_to_time ( record.tick_count ) <= event_time ) {
			shot_time = record.simtime + ( event_time - math::ticks_to_time ( record.tick_count ) ); // also get choked from this

		//	interfaces::console->console_printf ( "Found exact shot time: %f, ticks choked to get here: %d\n", shot_time, math::time_to_ticks ( event_time - math::ticks_to_time ( record.tick_count ) ) );
			record.shoot = true;
			record.resolved = true;

			float flPseudoFireYaw = math::normalize_yaw ( math::calc_angle ( entity->get_eye_pos ( ), local_pointer->eye_pos() ).y );

		
				float flLeftFireYawDelta = fabsf ( math::normalize_yaw ( flPseudoFireYaw - ( record.eye_angles.y + 29.f ) ) );
				float flRightFireYawDelta = fabsf ( math::normalize_yaw ( flPseudoFireYaw - ( record.eye_angles.y - 29.f ) ) );

				//g_notify.add( tfm::format( XOR( "found shot record on %s: [ yaw: %i ]" ), game::GetPlayerName( record->m_player->index( ) ), int( flLeftFireYawDelta > flRightFireYawDelta ? -29.f : 29.f ) ) );

				if ( flLeftFireYawDelta > flRightFireYawDelta )
					record.side = resolver::desync_side::left;
				else
					record.side = resolver::desync_side::right;
					
			//}
			//const float delta = math::normalize_yaw ( enemy_angle.y - entity->eye_angles ( ).y );

			//get_rotated_matrix ( record.origin, record.bone, delta, record.bone );

			break;
		}

		//else
		//	interfaces::console->console_printf ( "Bad curtime difference, EVENT: %f, RECORD: %f\n", event_time, math::ticks_to_time ( record.tick_count ) );

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
namespace player_manager {

	std::array<std::vector< lagcomp_t >, 64> records;

	void lagcomp_t::apply (player_t* entity ) {
		

		if ( !entity ) {
			this->failed = true;
			return;
		}

		animations::player_data [ entity->index ( ) ].modify_bonecache = true;
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


		/*if ( entity->simulation_time ( ) == this->simtime )
			return;*/

		if ( !entity->collideable ( ) ) {
			this->failed = true;
			return;
		}
		entity->collideable ( )->mins ( ) = this->obbmin;
		entity->collideable ( )->maxs ( ) = this->obbmax;
		entity->set_abs_angles ( this->abs_angles );
		entity->set_abs_origin ( this->abs_origin );

		memcpy ( entity->m_CachedBoneData ( ).Base ( ), this->bone, entity->m_CachedBoneData ( ).m_Size * sizeof ( matrix3x4_t ) );
	}
	void lagcomp_t::restore ( player_t * entity ) {
		auto & anim = animations::player_data [ entity->index ( ) ];

		if ( this->failed ) {
			anim.modify_bonecache = false;
			return;
			
		}

		auto & current = records.at ( entity->index ( ) ).back ( );

/*		if ( entity->simulation_time ( ) == this->simtime ) {
			anim.modify_bonecache = false; 
			return;
		}*/

		entity->collideable ( )->mins ( ) = current.obbmin;
		entity->collideable ( )->maxs ( ) = current.obbmax;
		entity->set_abs_angles ( current.abs_angles );
		entity->set_abs_origin ( current.abs_origin );


		memcpy ( entity->m_CachedBoneData ( ).Base ( ), current.bone, entity->m_CachedBoneData ( ).m_Size * sizeof ( matrix3x4_t ) );

		anim.modify_bonecache = false;
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

		auto curtime = local_pointer->is_alive ( ) ? math::ticks_to_time ( localdata.fixed_tickbase ) : interfaces::globals->cur_time; //-V807
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
	/*
	bool lagcomp_t::is_valid ( ) {
		if ( !this ) //-V704
			return false;


		if ( this->invalid )
			return false;

		auto net_channel_info = interfaces::engine->get_net_channel_info ( );

		if ( !net_channel_info )
			return false;

		static auto sv_maxunlag = interfaces::console->get_convar (   "sv_maxunlag"  );

		auto outgoing = net_channel_info->get_latency ( 0 );
		auto incoming = net_channel_info->get_latency ( 1 );

		auto correct = std::clamp ( outgoing + incoming + get_lerp_time ( ), 0.0f, sv_maxunlag->get_float ( ) );

		auto curtime = local_pointer->is_alive ( ) ? math::ticks_to_time ( localdata.fixed_tickbase ) : interfaces::globals->cur_time; //-V807
		auto delta_time = correct - ( curtime - this->simtime );

		if ( fabs ( delta_time ) > 0.2f )
			return false;

		auto extra_choke = 0;

		if ( localdata.fakeducking )
			extra_choke = 14 - interfaces::clientstate->m_choked_commands;

		auto server_tickcount = extra_choke + interfaces::globals->tick_count + math::time_to_ticks ( outgoing + incoming );
		auto dead_time = ( int ) ( math::time_to_ticks ( server_tickcount ) - sv_maxunlag->get_float ( ) );

		if ( this->simtime < ( float ) dead_time )
			return false;

		return true;
	}
	*/
	void lagcomp_t::predict ( player_t * entity ) {

		if ( records.at ( entity->index ( ) ).empty ( ) )
			return;

		auto predict_origin = [ ] ( vec3_t origin, vec3_t velocity, int ticks ) {
			return origin + ( velocity * ( interfaces::globals->interval_per_tick * static_cast< float >( ticks ) ) );
		};

		auto predicted_origin = predict_origin ( this->origin, this->velocity, this->choked );

	

		this->predicted = true;

		this->simtime += math::ticks_to_time ( 1 );


		for ( auto & i : this->bone ) {
			vec3_t original = vec3_t ( i [ 0 ][ 3 ], i [ 1 ][ 3 ], i [ 2 ][ 3 ] );
			vec3_t predicted = predict_origin ( original, this->velocity, this->choked );
		
			i [ 0 ][ 3 ] = predicted.x;
			i [ 1 ][ 3 ] = predicted.y;
			i [ 2 ][ 3 ] = predicted.z;
		}

		this->abs_origin = this->origin = predicted_origin;
	}

	void lagcomp_t::receive ( player_t * entity ) {

		this->simtime = entity->simulation_time ( );
		/*animation system*/
		animations::update_player_animation (entity );

		/*autowall related stuff*/
		this->obbmin = entity->collideable ( )->mins ( );
		this->obbmax = entity->collideable ( )->maxs ( );
		this->origin = entity->origin ( );
		this->abs_origin = entity->abs_origin ( );
		this->abs_angles = entity->abs_angles ( );
		this->eye_angles = entity->eye_angles ( );
		this->origin = entity->origin ( );

		/*bone cache*/
		memcpy ( this->bone, entity->m_CachedBoneData ( ).Base ( ), entity->m_CachedBoneData ( ).m_Size * sizeof ( matrix3x4_t ) );

		/*record selection*/
		this->tick_count = interfaces::globals->tick_count;
		this->speed = entity->velocity ( ).length_2d_sqr ( );
		this->choked = math::time_to_ticks ( std::fabs(entity->simulation_time ( ) - entity->get_old_simulation_time ( )) );
		this->flags = entity->flags ( );
		this->max_delta = resolver::max_desync_delta ( entity );
		this->in_air = entity->is_in_air ( );
		this->lby = entity->lower_body_yaw ( );
		this->goal_feet = entity->get_anim_state ( )->m_feet_yaw;

		/*resolver*/
		std::memcpy ( this->anim_layer, entity->get_animoverlays ( ), sizeof ( animationlayer ) * 15 );



		//{"autowall_side":2,"goal_feet":0.0,"id":0,"lby":-94.9395523071289,"left_yaw":0,"map":"maps\\workshop\\308490450\\xhair_v3.bsp","max_desync_delta":58.0,"missed":false,"position":{"x":-1024.0,"y":32.0,"z":0.031249992549419403},"right_yaw":0,"side":0,"yaw":0}

//	if ( this->choked > 2 )
	//	this->predict ( entity );

		this->manage_matrix ( entity );
		this->matrix_resolve ( entity );

		if ( entity->simulation_time ( ) < entity->get_old_simulation_time ( ) )
			this->invalid = true;

	}
	void lagcomp_t::matrix_resolve ( player_t * entity ) {
		if ( this->in_air || this->max_delta < 36 )
			return;

		auto closest_wall = vec3_t ( );
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

		if ( closest_dist < 100.f ) {
			if ( !closest_wall.is_zero ( ) ) {
				auto left_head = entity->get_hitbox_position ( hitbox_head, this->bone_left );
				auto right_head = entity->get_hitbox_position ( hitbox_head, this->bone_right );

				auto left_dist = left_head.distance_to ( closest_wall );
				auto right_dist = right_head.distance_to ( closest_wall );
			
				bool le = right_dist >= left_dist;
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
		
		

		
	}
	void lagcomp_t::manage_matrix ( player_t * entity ) {

		auto angle_to_me = math::calc_angle ( entity->get_eye_pos ( ), local_player::m_data.pointer->get_eye_pos ( ) ).y;
		angle_to_me = math::normalize_yaw ( angle_to_me );
		const auto at_target_yaw = math::normalize_yaw ( math::calc_angle ( entity->origin ( ), local_player::m_data.pointer->origin ( ) ).y - 180.f );
		const float delta = math::normalize_yaw ( at_target_yaw - entity->eye_angles ( ).y );
		create_fake_matrix ( entity, this->bone_left, localdata.force_low_delta_on_key  ? -28 : -resolver::max_desync_delta ( entity ) );
		create_fake_matrix ( entity, this->bone_right, localdata.force_low_delta_on_key ? 28 : resolver::max_desync_delta ( entity ) );

		//std::memcpy ( this->bone_left, csgo::left_player_bones [ entity->index ( ) ], sizeof ( csgo::left_player_bones [ entity->index ( ) ] ) );
		//std::memcpy ( this->bone_right, csgo::right_player_bones [ entity->index ( ) ], sizeof ( csgo::right_player_bones [ entity->index (  ) ] ) );
		get_rotated_matrix ( entity->origin ( ), this->bone, delta, this->bone_at_me );
	}
}

void player_manager::manage_bones( ) {
	if ( !config.ragebot_enabled )
		return;

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

		animations::post_data_end ( entity );
	


	}


}
void player_manager::setup_players( ) {
	

	if ( !local_player::m_data.pointer )
		return;


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


void player_manager::create_fake_matrix ( player_t* pl, matrix3x4_t bones [ 128 ], float delta ) {
	auto & anim = animations::player_data [ pl->index ( ) ];

	std::array< animationlayer, 13 > backup_layers;
	std::memcpy ( backup_layers.data ( ), pl->get_animoverlays ( ), sizeof ( backup_layers ) );

	anim_state backup_state;
	auto animstate = pl->get_anim_state ( );
	if ( !animstate ) {
		pl->GetBoneAccessor ( )->m_ReadableBones = pl->GetBoneAccessor ( )->m_WritableBones = 0;

		pl->invalidate_bone_cache ( );
		pl->setup_bones ( bones, 128, bone_used_by_anything, pl->simulation_time ( ) );
		return;
	}
	memcpy ( &backup_state, animstate, sizeof ( anim_state ) );


	*reinterpret_cast< bool * >( uintptr_t ( pl->get_anim_state ( )) + 0x5 ) = true;
	animstate->m_last_clientside_anim_framecount = 0;

	
	pl->update_client_side_animations ( );
	auto angle = pl->eye_angles ( );
	angle.y += delta;
	animations::update_anim_angle ( animstate, angle );
	if (config.abs_angles )
	pl->set_abs_angles (vec3_t(0, pl->get_anim_state ( )->m_abs_yaw ,0) );
	pl->GetBoneAccessor ( )->m_ReadableBones = pl->GetBoneAccessor ( )->m_WritableBones = 0;

	pl->invalidate_bone_cache ( );
	anim.update_bones = true;
	pl->setup_bones ( bones, 128, bone_used_by_anything, pl->simulation_time() );
	anim.update_bones = false;
	
	std::memcpy ( pl->get_animoverlays ( ), backup_layers.data ( ), sizeof ( backup_layers ) );

	memcpy ( animstate, &backup_state, sizeof ( anim_state ) );

}
void player_manager::create_fake_matrix_based_on_angle ( player_t * pl, matrix3x4_t bones [ 128 ], vec3_t angle ) {
	std::array< animationlayer, 13 > backup_layers;
	std::memcpy ( backup_layers.data ( ), pl->get_animoverlays ( ), sizeof ( backup_layers ) );

	anim_state backup_state;
	auto animstate = pl->get_anim_state ( );
	if ( !animstate ) {
		pl->GetBoneAccessor ( )->m_ReadableBones = pl->GetBoneAccessor ( )->m_WritableBones = 0;

		pl->invalidate_bone_cache ( );
		pl->setup_bones ( bones, 128, bone_used_by_anything, pl->simulation_time ( ) );
		return;
	}
	memcpy ( &backup_state, animstate, sizeof ( anim_state ) );


	*reinterpret_cast< bool * >( uintptr_t ( pl->get_anim_state ( ) ) + 0x5 ) = true;
	animstate->m_last_clientside_anim_framecount = 0;


	pl->update_client_side_animations ( );
	animations::update_anim_angle ( animstate, angle );
	/*crash*/
	pl->set_abs_angles ( vec3_t ( 0, pl->get_anim_state ( )->m_abs_yaw, 0 ) );
	pl->GetBoneAccessor ( )->m_ReadableBones = pl->GetBoneAccessor ( )->m_WritableBones = 0;

	pl->invalidate_bone_cache ( );
	pl->setup_bones ( bones, 128, bone_used_by_anything, pl->simulation_time ( ) );


	std::memcpy ( pl->get_animoverlays ( ), backup_layers.data ( ), sizeof ( backup_layers ) );

	memcpy ( animstate, &backup_state, sizeof ( anim_state ) );

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


void player_manager::get_rotated_matrix( vec3_t origin, matrix3x4_t from_matrix [ MAXSTUDIOBONES ], float angle, matrix3x4_t new_matrix [ MAXSTUDIOBONES ] ) {
	vec3_t BonePos;
	vec3_t OutPos;

	for ( int i = 0; i < MAXSTUDIOBONES; i++ )
	{
		math::angle_matrix( vec3_t( 0, angle, 0 ), new_matrix [ i ] );
		math::matrix_multiply( new_matrix [ i ], from_matrix [ i ] );
		BonePos = vec3_t( from_matrix [ i ][ 0 ][ 3 ], from_matrix [ i ][ 1 ][ 3 ], from_matrix [ i ][ 2 ][ 3 ] ) - origin;
		math::vector_rotate( BonePos, vec3_t( 0, angle, 0 ), OutPos );
		OutPos += origin;
		new_matrix [ i ][ 0 ][ 3 ] = OutPos.x;
		new_matrix [ i ][ 1 ][ 3 ] = OutPos.y;
		new_matrix [ i ][ 2 ][ 3 ] = OutPos.z;
	}

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

		if ( records.at ( i ).size ( ) ) {

			if ( player->dormant ( ) || !player->is_alive ( ) ) {
				records [ i ].clear ( );
				continue;
			}
			for ( size_t t = 0; t < records.at ( i ).size ( ); t++ ) {
				auto & rec = records.at ( i ).at ( t );
				if ( !rec.is_valid ( ) )
					records [ i ].erase ( records.at ( i ).begin ( ) + t );
			}
		}

	}

}
void player_manager::setup_records ( client_frame_stage_t stage ) {

	if ( stage != FRAME_NET_UPDATE_END )
		return;

	for ( int i = 1; i <= interfaces::globals->max_clients; i++ ) {
		player_t * player = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( i ) );

		if ( !player )
			continue;

		if ( player == local_player::m_data.pointer )
			continue;

		if ( player->team ( ) == local_player::m_data.pointer->team ( ) )
			continue;

		auto time_delta = abs ( math::time_to_ticks ( player->simulation_time ( ) ) - interfaces::globals->tick_count );

		if ( time_delta > 1.0f / interfaces::globals->interval_per_tick )
			continue;

		auto update = records [ i ].empty ( ) || player->simulation_time ( ) != player->get_old_simulation_time ( ); //-V550

		if ( update && !records [ i ].empty ( ) ) {
			auto server_tick = interfaces::clientstate->m_iServerTick - i % interfaces::globals->timestamp_randomize_window;
			auto current_tick = server_tick - server_tick % interfaces::globals->timestamp_networking_base;

			if ( math::time_to_ticks ( player->get_old_simulation_time ( ) ) < current_tick && math::time_to_ticks ( player->simulation_time ( ) ) == current_tick ) {
				auto layer = &player->get_animoverlays ( ) [ 11 ];
				auto previous_layer = &records [ i ].front ( ).anim_layer [ 11 ];

				if ( layer->m_cycle == previous_layer->m_cycle ) //-V550
				{
					player->simulation_time ( ) = player->get_old_simulation_time ( );
					update = false;
				}
			}
		}


		if ( !player->dormant ( ) && player->is_alive ( ) ) {


			if ( !player->m_CachedBoneData ( ).m_Size )
				continue;

			if ( !player->m_CachedBoneData ( ).Base ( ) )
				continue;

			if ( !update )
				continue;

			if ( !records [ i ].empty ( ) && ( player->origin ( ) - records [ i ].front ( ).origin ).length_sqr ( ) > 4096.0f )
				for ( auto & record : records [ i ] )
					record.invalid = true;

			lagcomp_t record;

			record.receive ( player );
			records.at ( i ).push_back ( record );

		}
	}
}
	