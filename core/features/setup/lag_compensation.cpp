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

			const float delta = math::normalize_yaw ( enemy_angle.y - entity->eye_angles ( ).y );

			get_rotated_matrix ( entity->origin ( ), record.bone, delta, record.bone );

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


		float correct = 0.f;

		correct += nci->get_latency ( 1 );
		correct += get_lerp_time ( );

		const auto delta_time = correct - ( interfaces::globals->cur_time - this->simtime );

		return fabsf ( delta_time ) < 0.2f;
	}
	void lagcomp_t::predict ( player_t * entity ) {

		if ( records.at ( entity->index ( ) ).empty ( ) )
			return;

		auto predict_origin = [ ] ( vec3_t origin, vec3_t velocity, int ticks ) {
			return origin + ( velocity * ( interfaces::globals->interval_per_tick * static_cast< float >( ticks ) ) );
		};

		auto predicted_origin = predict_origin ( this->origin, this->velocity, this->choked );

		static auto sv_gravity = interfaces::console->get_convar ( "sv_gravity" );
		static auto sv_jump_impulse = interfaces::console->get_convar ( "sv_jump_impulse" );

		//if ( !( flags & fl_onground ) )
		//	predicted_origin.z -= ( interfaces::globals->frame_time * sv_gravity->get_float ( ) );
		//else if ( ( records.at ( entity->index ( ) ).front ( ).flags & fl_onground ) )
		//	this->origin.z = sv_jump_impulse->get_float ( );


		this->predicted = true;

		this->simtime += math::ticks_to_time ( 1 );


		for ( auto & i : this->bone ) {
			vec3_t original = vec3_t ( i [ 0 ][ 3 ], i [ 1 ][ 3 ], i [ 2 ][ 3 ] );
			vec3_t predicted = predict_origin ( original, this->velocity, this->choked );
		//	if ( !( flags & fl_onground ) )
			//	predicted.z -= ( interfaces::globals->frame_time * sv_gravity->get_float ( ) );
			i [ 0 ][ 3 ] = predicted.x;
			i [ 1 ][ 3 ] = predicted.y;
			i [ 2 ][ 3 ] = predicted.z;
		}

		this->abs_origin = this->origin = predicted_origin;
	}

	void lagcomp_t::receive ( player_t * entity ) {

		this->simtime = entity->simulation_time ( );
		
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
		if ( this->choked > 2 )
			this->predict ( entity );

		this->manage_matrix ( entity );
	}
	void lagcomp_t::manage_matrix ( player_t * entity ) {

		auto angle_to_me = math::calc_angle ( entity->get_eye_pos ( ), local_player::m_data.pointer->get_eye_pos ( ) ).y;
		angle_to_me = math::normalize_yaw ( angle_to_me );
		const auto at_target_yaw = math::normalize_yaw ( math::calc_angle ( entity->origin ( ), local_player::m_data.pointer->origin ( ) ).y - 180.f );
		const float delta = math::normalize_yaw ( at_target_yaw - entity->eye_angles ( ).y );
		create_fake_matrix ( entity, this->bone_left, -resolver::max_desync_delta ( entity ) );
		create_fake_matrix ( entity, this->bone_right, resolver::max_desync_delta ( entity ) );
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

		if ( entity == local_player::m_data.pointer )
			continue;

		if ( entity->dormant( ) )
			continue;

		if ( !entity->is_alive() )
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
	/*crash*/
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


void player_manager::setup_records ( ) {



	for ( int i = 1; i <= interfaces::globals->max_clients; i++ ) {
		player_t * player = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( i ) );

		if ( !player )
			continue;

		if ( player == local_player::m_data.pointer )
			continue;

		if ( player->team ( ) == local_player::m_data.pointer->team ( ) )
			continue;

		if ( !player->dormant ( ) && player->is_alive ( ) ) {


			if ( !player->m_CachedBoneData ( ).m_Size )
				continue;

			if ( !player->m_CachedBoneData ( ).Base ( ) )
				continue;

			lagcomp_t record;

			record.receive ( player  );
			records.at ( i ).push_back ( record );
		}

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
