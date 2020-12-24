#include "../features.hpp"
#include "../../helpers/helpers.h"
std::deque< player_manager::lagcomp_t > player_manager::records [ 64 ];

player_manager::lagcomp_t player_manager::backup_data [ 64 ];
player_manager::lagcomp_t player_manager::best_tick_global [ 65 ];
struct player_manager::lagcomp_t;

void player_manager::event_logs::bullet_impact( i_game_event* event ) {
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
	if ( records [ entity->index( ) ].size( ) < 0 )
		return;

	vec3_t bullet_impact = vec3_t( event->get_float( "x" ), event->get_float( "y" ), event->get_float( "z" ) );
	vec3_t enemy_angle = math::calc_angle( entity->get_eye_pos( ), bullet_impact );

/*	for ( size_t i = 0; i < records [ entity->index( ) ].size( ); i++ )
	{
		interfaces::console->console_printf( "SHOT TIME TICK simtime %i  and TICK %i\n", math::time_to_ticks( interfaces::globals->cur_time ), math::time_to_ticks( records [ entity->index( ) ].at( i ).simtime + player_manager::get_lerp_time()) );
	
		if ( math::time_to_ticks( records [ entity->index( ) ].at( i ).simtime + player_manager::get_lerp_time( ) ) == math::time_to_ticks(interfaces::globals->cur_time) )
		{
			records [ entity->index( ) ].at( i ).shoot = true;
			float flLeftFireYawDelta = fabsf( math::normalize_yaw( enemy_angle.y - ( records [ entity->index( ) ].at( i ).eyeangles.y + 58.f ) ) );
			float flRightFireYawDelta = fabsf( math::normalize_yaw( enemy_angle.y - ( records [ entity->index( ) ].at( i ).eyeangles.y - 58.f ) ) );


			float delta = flLeftFireYawDelta > flRightFireYawDelta?-resolver::resolver_data [ entity->index( ) ].max_desync_delta:resolver::resolver_data [ entity->index( ) ].max_desync_delta;

			records [ entity->index( ) ].at( i ).eyeangles.y = math::normalize_yaw( math::calc_angle( entity->get_eye_pos( ), bullet_impact ).y );

			entity->set_abs_angles( vec3_t( 0, records [ entity->index( ) ].at( i ).eyeangles.y, 0 ) );
			entity->update_animation_state( entity->get_anim_state( ), records [ entity->index( ) ].at( i ).eyeangles );
			entity->invalidate_bone_cache( );
			entity->setup_bones( records [ entity->index( ) ].at( i ).bone, 128, 256, interfaces::globals->cur_time );
			player_manager::restore_player( entity );
			interfaces::console->console_printf( "FOUND SHOT POS  simtime %f\ n", entity->simulation_time( ) );
			
		}
	}*/

}
player_t* player_manager::util_player_by_index( int index )
{
	typedef player_t* ( __fastcall* player_index )( int );
	static player_index c_index = ( player_index ) utilities::pattern_scan( "server.dll", "85 C9 7E 2A A1" );

	if ( !c_index )
		return nullptr;

	return c_index( index );
}

void player_manager::manage_bones( ) {
	
	if ( !local_player::m_data.pointer )
		return;

	/*if ( local_player::m_data.pointer && local_player::m_data.pointer->is_alive() && local_player::m_data.pointer->active_weapon() ) {
		
		interfaces::console->console_printf ( "shott time  %f \n", local_player::m_data.pointer->active_weapon ( )->m_fLastShotTime ( ) );
	}*/

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

static float shot [ 65 ];

void player_manager::recieve_record( player_t* entity, lagcomp_t& record ) {

	auto index = entity->index( );
	static float lby [ 65 ];
	record.simtime = entity->simulation_time( );
	record.duckamount = entity->duck_amount( );

	record.obbmin = entity->collideable( )->mins( );
	record.obbmax = entity->collideable( )->maxs( );
	record.absangles = entity->abs_angles( );
	record.absorigin = entity->abs_origin( );
	record.origin = entity->abs_origin( );
	record.eyeangles = entity->eye_angles( );
	record.lby = entity->lower_body_yaw( );
	record.flags = entity->flags( );
	record.moving = entity->is_moving( );
	record.velocity = entity->velocity( );
	
	//record.pose_params = entity->get_poseparameter( );
	record.speed = entity->velocity ( ).length_2d_sqr ( );
	record.resolver = resolver::resolver_data [ entity->index( ) ];
	std::memcpy ( record.anim_layer, entity->get_animoverlays ( ), sizeof ( animationlayer ) * 15 );

	auto count = *( std::uint32_t* ) ( ( std::uint32_t ) entity->get_renderable( ) + 0x2918 );
	std::memcpy( &record.bone, *( void** ) ( ( std::uint32_t ) entity->get_renderable( ) + 0x290C ), sizeof( matrix_t ) * count );


	record.shoot = false;
	if ( entity->active_weapon( ) ) {
		if ( shot [ index ] != entity->active_weapon( )->m_fLastShotTime( ) )
		{
			record.shoot = true;
			shot [ index ] = entity->active_weapon( )->m_fLastShotTime( );
		}
	}
	if ( record.shoot ) {
		auto angle_to_me = math::calc_angle ( entity->get_eye_pos(), local_player::m_data.pointer->get_eye_pos()).y;
		angle_to_me = math::normalize_yaw ( angle_to_me );

		get_rotated_matrix ( record, angle_to_me, record.bone );
	}
	get_rotated_matrix( record, -record.resolver.safe_point_angle, record.bone_left );
	get_rotated_matrix( record, record.resolver.safe_point_angle, record.bone_right );
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
bool player_manager::is_tick_valid( lagcomp_t record )
{
	const auto nci = interfaces::engine->get_net_channel_info( );

	if ( !nci )
		return false;


	float correct = 0;

	correct += nci->get_latency( 1 );
	correct += get_lerp_time( );

	const auto delta_time = correct - ( interfaces::globals->cur_time - record.simtime );

	return fabsf( delta_time ) < 0.2f && correct < 0.2f;

}

static float sim_time [ 65 ];
void player_manager::get_rotated_matrix( lagcomp_t record, float angle, matrix_t new_matrix [ MAXSTUDIOBONES ] ) {
	vec3_t BonePos;
	vec3_t OutPos;

	for ( int i = 0; i < MAXSTUDIOBONES; i++ )
	{
		math::angle_matrix( vec3_t( 0, angle, 0 ), new_matrix [ i ] );
		math::matrix_multiply( new_matrix [ i ], record.bone[ i ] );
		BonePos = vec3_t( record.bone [ i ][ 0 ][ 3 ], record.bone [ i ][ 1 ][ 3 ], record.bone [ i ][ 2 ][ 3 ] ) - record.origin;
		math::vector_rotate( BonePos, vec3_t( 0, angle, 0 ), OutPos );
		OutPos += record.origin;
		new_matrix [ i ][ 0 ][ 3 ] = OutPos.x;
		new_matrix [ i ][ 1 ][ 3 ] = OutPos.y;
		new_matrix [ i ][ 2 ][ 3 ] = OutPos.z;
	}

}

void player_manager::rotate_matrix ( matrix_t bones[128], float angle ) {
	vec3_t BonePos;
	vec3_t OutPos;
	matrix_t new_matrix [ 128 ];
	for ( int i = 0; i < MAXSTUDIOBONES; i++ ) {
		math::angle_matrix ( vec3_t ( 0, angle, 0 ), new_matrix [ i ] );
		math::matrix_multiply ( new_matrix [ i ], bones [ i ] );
		BonePos = vec3_t ( bones [ i ][ 0 ][ 3 ], bones [ i ][ 1 ][ 3 ], bones [ i ][ 2 ][ 3 ] ) - local_player::m_data.pointer->abs_origin();
		math::vector_rotate ( BonePos, vec3_t ( 0, angle, 0 ), OutPos );
		OutPos += local_player::m_data.pointer->abs_origin ( );
		new_matrix [ i ][ 0 ][ 3 ] = OutPos.x;
		new_matrix [ i ][ 1 ][ 3 ] = OutPos.y;
		new_matrix [ i ][ 2 ][ 3 ] = OutPos.z;
	}
	std::memcpy ( bones, new_matrix, sizeof ( new_matrix ) );

}

void player_manager::setup_records ( ) {



	if ( !local_player::m_data.alive )
		return;

	for ( int i = 1; i <= interfaces::globals->max_clients; i++ ) {
		player_t * player = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( i ) );

		if ( !player )
			continue;

		if ( player == local_player::m_data.pointer )
			continue;

		if ( player->team ( ) == local_player::m_data.pointer->team ( ) )
			continue;

		if ( !player->dormant ( ) && player->is_alive() ) {
			


				player_manager::backup_player ( player );
				lagcomp_t record;

				recieve_record ( player, record );
				records [ i ].push_back ( record );
			
		}

		if ( records [ i ].size ( ) > 0 ) {

			if ( player->dormant ( ) || !player->is_alive ( ) ) {
				records [ i ].clear ( );
				continue;
			}
			for ( int t = 0; t < records [ i ].size ( ); t++ )
					if ( !is_tick_valid ( records [ i ].at ( t ) ) )
						records [ i ].erase ( records [ i ].begin ( ) + t );
		}
	}
}
void player_manager::restore_record( player_t* entity, lagcomp_t record ) {
	
	entity->flags( ) = record.flags;

	entity->eye_angles( ) = record.eyeangles;
	entity->set_abs_angles( record.absangles );
	entity->velocity( ) = record.velocity;
	entity->set_abs_origin( record.absorigin );
	entity->origin( ) = record.origin;
	entity->lower_body_yaw( ) = record.lby;
	entity->duck_amount( ) = record.duckamount;
	const auto collideable = entity->collideable( );
	collideable->maxs( ) = record.obbmax;
	collideable->mins( ) = record.obbmin;

	auto count = *( std::uint32_t* ) ( ( std::uint32_t ) entity->get_renderable( ) + 0x2918 );
	std::memcpy( *( void** ) ( ( std::uint32_t ) entity->get_renderable( ) + 0x290C ), record.bone, sizeof( matrix_t ) * count );
}
void player_manager::restore_player( player_t* entity ) {
	
	auto& record = backup_data [ entity->index( ) ];
	entity->flags( ) = record.flags;
	entity->eye_angles( ) = record.eyeangles;
	entity->set_abs_angles( record.absangles );
	entity->velocity( ) = record.velocity;
	entity->set_abs_origin( record.origin );
	entity->origin( ) = record.origin;
	entity->lower_body_yaw( ) = record.lby;
	entity->duck_amount( ) = record.duckamount;
	const auto collideable = entity->collideable( );
	collideable->maxs( ) = record.obbmax;
	collideable->mins( ) = record.obbmin;
	auto count = *( std::uint32_t* ) ( ( std::uint32_t ) entity->get_renderable( ) + 0x2918 );
	std::memcpy( *( void** ) ( ( std::uint32_t ) entity->get_renderable( ) + 0x290C ), record.bone, sizeof( matrix_t ) * count );
}
void player_manager::backup_player( player_t* entity )
{
	auto index = entity->index( );
	auto& record = backup_data [ index ];
	record.simtime = entity->simulation_time( );
	record.duckamount = entity->duck_amount( );
	record.entity = entity;
	record.obbmin = entity->collideable( )->mins( );
	record.obbmax = entity->collideable( )->maxs( );
	record.absangles = entity->abs_angles( );
	record.absorigin = entity->abs_origin( );
	record.origin = entity->origin( );
	record.eyeangles = entity->eye_angles( );
	record.lby = entity->lower_body_yaw( );
	record.flags = entity->flags( );
	record.moving = entity->is_moving( );
	record.velocity = entity->velocity( );
	auto count = *( std::uint32_t* ) ( ( std::uint32_t ) entity->get_renderable( ) + 0x2918 );
	std::memcpy( &record.bone, *( void** ) ( ( std::uint32_t ) entity->get_renderable( ) + 0x290C ), sizeof( matrix_t ) * count );
  
	std::memcpy ( &csgo::player_bones [ index ], *( void ** ) ( ( std::uint32_t ) entity->get_renderable ( ) + 0x290C ), sizeof ( matrix_t ) * count );
}