#include "../features.hpp"
std::deque< player_manager::backtrack_records_t > player_manager::records [ 64 ];
player_manager::backtrack_records_t player_manager::extrapolate::extrapolate_records [ 64 ];
player_manager::backtrack_records_t player_manager::backup_data [ 64 ];
player_manager::backtrack_records_t player_manager::best_tick_global [ 65 ];
struct player_manager::backtrack_records_t;

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
void player_manager::extrapolate::extrapolate_player( player_t* entity, backtrack_records_t& record ) {

	auto nci = interfaces::engine->get_net_channel_info( );
	if ( !nci )
		return;
	record.simtime = entity->simulation_time( ) + nci->get_average_latency( 0 ) + nci->get_average_latency( 1 );
	record.entity = entity;
	record.obbmin = entity->collideable( )->mins( );
	record.obbmax = entity->collideable( )->maxs( );
	record.absangles = entity->abs_angles( );
	record.origin = entity->origin( );
	
	record.absorigin = entity->abs_origin( ) + ( entity->velocity( ) * ( interfaces::globals->interval_per_tick * ( float ) math::time_to_ticks( nci->get_average_latency( 0 ) + nci->get_average_latency( 1 ) ) ) );
	record.absangles = entity->abs_angles( );
	record.eyeangles = entity->eye_angles( );
	record.lby = entity->lower_body_yaw( );
	record.velocity = entity->velocity( );

	
	auto count = *( std::uint32_t* ) ( ( std::uint32_t ) entity->get_renderable( ) + 0x2918 );
	std::memcpy( &record.bone, *( void** ) ( ( std::uint32_t ) entity->get_renderable( ) + 0x290C ), sizeof( matrix_t ) * count );

	if ( variables::ragebot::extrapolate ) {
		for ( size_t i { }; i < 128; ++i ) {
			vec3_t predicted_position = ( record.bone [ i ].get_origin() + ( entity->velocity( ) * ( interfaces::globals->interval_per_tick * ( float ) math::time_to_ticks( nci->get_average_latency( 0 ) + nci->get_average_latency( 1 ) ) ))) ;
			record.bone [ i ].set_origin( predicted_position );
		}
		//interfaces::console->console_printf( "EXRTRAPOLATING  .. \n" );
	}

		

}

player_manager::backtrack_records_t player_manager::find_best_tick( player_t* entity )
{
	player_manager::backtrack_records_t best_tick;// MAX );
	float dmg = 0.0f;

	auto index = entity->index( );
	for ( int i = player_manager::records [ index ].size( ) - 1; i >= 0; i-- ) {
		auto& record = player_manager::records [ index ][ i ];


		autowall::FireBulletData_t awall = { };



		if ( record.shoot )
		{
			best_tick = record;
			return best_tick;

		}
		auto records = player_manager::records [ index ];
		switch ( variables::ragebot::prioritize_record )
		{
		case 0:
			best_tick = records.at( 0 );
			break;
		case 1:
		{
			best_tick = records.back( );
			break;
		}
		case 2:
		{
			float velocity = 0.f;
			if ( record.velocity.Length2D( ) > velocity ) {
				best_tick = record;
				velocity = record.velocity.Length2D( );
			}
			break;
		}
		case 3:
		{
			player_manager::restore_record( entity, record );

			auto cur_damage = autowall::GetDamage( csgo::local_player, record.bone [ 8 ].get_origin( ), awall );

			if ( cur_damage > dmg ) {
				best_tick = record;
				dmg = cur_damage;
			}

			if ( dmg >= entity->health( ) ) {
				return best_tick;
			}

			cur_damage = autowall::GetDamage( csgo::local_player, record.bone [ 0 ].get_origin( ), awall );

			if ( cur_damage > dmg ) {
				best_tick = record;
				dmg = cur_damage;
			}
		}
		break;
		default:
			break;
		}

	}

	return best_tick;
}
void player_manager::update_animations( player_t* entity ) /* thx to xsharcs*/
{
	auto state = entity->get_anim_state( ); if ( !state ) return;
	auto index = entity->index( );
	static float sim_time [ 65 ];



	//entity->invalidate_bone_cache( );
	//entity->setup_bones( nullptr, -1, 0x7FF00, interfaces::globals->cur_time );
}
void player_manager::manage_bones( ) {

	if ( !csgo::local_player )
		return;

	for ( int i = 1; i <= interfaces::globals->max_clients; i++ ) {
		player_t* entity = reinterpret_cast< player_t* >( interfaces::entity_list->get_client_entity( i ) );

		if ( !entity )
			continue;

		if ( entity->dormant( ) )
			continue;

		if ( entity->health( ) <= 0 )
			continue;


		static auto set_interpolation_flags = [ ] ( player_t* e, int flag ) {
			const auto var_map = ( uintptr_t ) e + 36;
			const auto sz_var_map = *( int* ) ( var_map + 20 );

			for ( auto index = 0; index < sz_var_map; index++ )
				*( uintptr_t* ) ( ( *( uintptr_t* ) var_map ) + index * 12 ) = flag;
		};


		set_interpolation_flags( entity, 0 );


	}


}
void player_manager::setup_players( ) {
	if ( !csgo::local_player )
		return;

	for ( int i = 1; i <= interfaces::globals->max_clients; i++ ) {
		player_t* player = reinterpret_cast< player_t* >( interfaces::entity_list->get_client_entity( i ) );

		if ( !player )
			continue;

		if ( player->dormant( ) )
			continue;

		if ( player->health( ) <= 0 )
			continue;


		float fDuration = -1.f;
		static uintptr_t pCall = ( uintptr_t ) utilities::pattern_scan( "server.dll", "55 8B EC 81 EC ? ? ? ? 53 56 8B 35 ? ? ? ? 8B D9 57 8B CE" );
		PVOID pEntity = nullptr;
		pEntity = util_player_by_index( i );

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
std::vector< vec3_t> player_manager::on_shot_safe( player_t* entity, backtrack_records_t& record ) {


	auto VectorTransform_Wrapper = [ ] ( const vec3_t& in1, const matrix_t& in2, vec3_t& out )
	{
		auto VectorTransform = [ ] ( const float* in1, const matrix_t& in2, float* out )
		{
			auto DotProducts = [ ] ( const float* v1, const float* v2 )
			{
				return v1 [ 0 ] * v2 [ 0 ] + v1 [ 1 ] * v2 [ 1 ] + v1 [ 2 ] * v2 [ 2 ];
			};
			out [ 0 ] = DotProducts( in1, in2 [ 0 ] ) + in2 [ 0 ][ 3 ];
			out [ 1 ] = DotProducts( in1, in2 [ 1 ] ) + in2 [ 1 ][ 3 ];
			out [ 2 ] = DotProducts( in1, in2 [ 2 ] ) + in2 [ 2 ][ 3 ];
		};
		VectorTransform( &in1.x, in2, &out.x );
	};

	studio_hdr_t* pStudioModel = interfaces::model_info->get_studio_model( entity->model( ) );
	studio_hitbox_set_t* set = pStudioModel->hitbox_set( 0 );
	studio_box_t* hitbox = set->hitbox( 0 );

	std::vector<vec3_t> vecArray;

	vec3_t max;
	vec3_t min;
	const auto mod = hitbox->radius != -1.f?hitbox->radius:0.f;
	VectorTransform_Wrapper( vec3_t( hitbox->maxs.x + mod, hitbox->maxs.y + mod, hitbox->maxs.z + mod ), csgo::player_bones [ entity->index( ) ][ hitbox->bone ], max );
	VectorTransform_Wrapper( vec3_t( hitbox->mins.x - mod, hitbox->mins.y - mod, hitbox->mins.z - mod ), csgo::player_bones [ entity->index( ) ][ hitbox->bone ], min );


	auto center_vec = ( min + max ) * 0.5f;
	matrix_t dmatrix, rmatrix;
	vec3_t center = center_vec;

	vec3_t CurrentAngles = math::calc_angle( center, engine_prediction::unpredicted_eye );

	vec3_t Forward = math::angle_vector( CurrentAngles );

	vec3_t Right = Forward.cross( vec3_t( 0, 0, 1 ) );
	vec3_t Left = vec3_t( -Right.x, -Right.y, Right.z );

	vec3_t Top = vec3_t( 0, 0, 1 );
	vec3_t Bot = vec3_t( 0, 0, -1 );


	for ( auto i = 0; i < 5; ++i )
	{
		vecArray.emplace_back( center );
	}
	vec3_t middle = ( Top - Bot );
	vecArray [ 0 ] += vec3_t( Right.x, Right.y, Top.z ) * ( hitbox->radius * ( 60.f / 100.f ) );
	vecArray [ 1 ] += vec3_t( Left.x, Left.y, Top.z ) * ( hitbox->radius * ( 60.f / 100.f ) );
	vecArray [ 2 ] += vec3_t( Right.x, Right.y, middle.z ) * ( hitbox->radius * ( 60.f / 100.f ) );
	vecArray [ 3 ] += vec3_t( Left.x, Left.y, middle.z ) * ( hitbox->radius * ( 60.f / 100.f ) );
	vecArray [ 4 ] += Top * ( hitbox->radius * ( variables::ragebot::head_scale / 100.f ) );

	for ( size_t i = 0; i < vecArray.size( ); i++ )
	{
		bool safe = ragebot::is_point_is_safe( entity, engine_prediction::unpredicted_eye, vecArray.at( i ), 0, record.bone_left ) || ragebot::is_point_is_safe( entity, engine_prediction::unpredicted_eye, vecArray.at( i ), 0, record.bone_right );

		if ( !safe )
			vecArray.erase( vecArray.begin( ) + i );
	}

	return vecArray;
}

void player_manager::recieve_record( player_t* entity, backtrack_records_t& record ) {

	auto index = entity->index( );
	static float lby [ 65 ];
	record.simtime = entity->simulation_time( );
	record.duckamount = entity->duck_amount( );
	record.entity = entity;
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

	record.pose_params = entity->get_poseparameter( );

	record.resolver = resolver::resolver_data [ entity->index( ) ];

	std::memcpy( record.anim_layer, entity->get_anim_overlays( ), sizeof( animationlayer ) * 15 );

	auto count = *( std::uint32_t* ) ( ( std::uint32_t ) entity->get_renderable( ) + 0x2918 );
	std::memcpy( &record.bone, *( void** ) ( ( std::uint32_t ) entity->get_renderable( ) + 0x290C ), sizeof( matrix_t ) * count );

	if ( lby [ index ] != entity->lower_body_yaw( ) || ( entity->flags( ) & ( int ) fl_onground && entity->velocity( ).Length2D( ) > 29.f ) )
	{
		record.lby_update = true;
		lby [ index ] = entity->lower_body_yaw( );
	}
	record.shoot = false;
	if ( entity->active_weapon( ) ) {
		if ( shot [ index ] != entity->active_weapon( )->m_fLastShotTime( ) )
		{
			record.shoot = true;
			shot [ index ] = entity->active_weapon( )->m_fLastShotTime( );
		}
	}
	if ( record.shoot ) {
		auto angle_to_me = math::calc_angle ( entity->get_hitbox_position(hitbox_head), csgo::local_player->get_hitbox_position (hitbox_head ) ).y;
		angle_to_me = math::normalize_yaw ( angle_to_me );

		get_rotated_matrix ( record, angle_to_me, record.bone );
	}
	get_rotated_matrix( record, -25.f, record.bone_left );
	get_rotated_matrix( record, 25.f, record.bone_right );
}

float player_manager::interpolation_time( ) {

	static	auto ud_rate = interfaces::console->get_convar( "cl_updaterate" )->get_int( );
	static	auto min_ud_rate = interfaces::console->get_convar( "sv_minupdaterate" );
	static	auto max_ud_rate = interfaces::console->get_convar( "sv_maxupdaterate" );

	if ( min_ud_rate && max_ud_rate )
		ud_rate = max_ud_rate->get_int( );

	static auto ratio = interfaces::console->get_convar( "cl_interp_ratio" )->get_float( );

	if ( ratio == 0.0f )
		ratio = 1.0f;

	static auto lerp = interfaces::console->get_convar( "cl_interp" )->get_float( );
	static auto c_min_ratio = interfaces::console->get_convar( "sv_client_min_interp_ratio" );
	static auto c_max_ratio = interfaces::console->get_convar( "sv_client_max_interp_ratio" );

	if ( c_min_ratio && c_max_ratio && c_min_ratio->get_float( ) != 1.0f )
		ratio = std::clamp( ratio, c_min_ratio->get_float( ), c_max_ratio->get_float( ) );

	return max( lerp, ( ratio / ud_rate ) );
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
bool player_manager::is_tick_valid( backtrack_records_t record )
{
	const auto nci = interfaces::engine->get_net_channel_info( );

	if ( !nci )
		return false;
	if ( !csgo::cmd )
		return false;

	float correct = 0;

	correct += nci->get_latency( 1 );
	correct += get_lerp_time( );

	const auto delta_time = correct - ( interfaces::globals->cur_time - record.simtime );

	return fabsf( delta_time ) < 0.2f && correct < 0.2f;

}

static float sim_time [ 65 ];
void player_manager::get_rotated_matrix( backtrack_records_t record, float angle, matrix_t new_matrix [ MAXSTUDIOBONES ] ) {
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
void player_manager::setup_records( ) {

	if ( !csgo::local_player )
		return;
	static int old_tick = 0;
	for ( int i = 1; i <= interfaces::globals->max_clients; i++ ) {
		player_t* player = reinterpret_cast< player_t* >( interfaces::entity_list->get_client_entity( i ) );

		if ( !player )
			continue;

		if ( player == csgo::local_player )
			continue;

		if ( player->team( ) == csgo::local_player->team( ) )
			continue;

	





		auto count = *( std::uint32_t* ) ( ( std::uint32_t ) player->get_renderable( ) + 0x2918 );
		std::memcpy( csgo::player_bones [ player->index( ) ], *( void** ) ( ( std::uint32_t ) player->get_renderable( ) + 0x290C ), sizeof( matrix_t ) * count );

		ragebot::get_rotated_matrix( player, -resolver::resolver_data [ player->index ( ) ].brute_angle, csgo::left_player_bones [ player->index( ) ] );
		ragebot::get_rotated_matrix( player, resolver::resolver_data [ player->index ( ) ].brute_angle, csgo::right_player_bones [ player->index( ) ] );
	
		player_manager::backup_player( player );
		backtrack_records_t record;
		backtrack_records_t extrapolate_record;

		recieve_record( player, record );
		records [ i ].push_back( record );
		//printf ( "BRUTE SIDE %i \n", resolver::resolver_data [ player->index ( ) ].brute_side );

		/*switch ( resolver::resolver_data [ player->index ( ) ].brute_side ) {
		case 1:
			std::memcpy ( csgo::player_bones [ player->index ( ) ], csgo::left_player_bones [ player->index ( ) ], sizeof ( csgo::left_player_bones [ player->index ( ) ] ) );
			break;

		case 2:
			std::memcpy ( csgo::player_bones [ player->index ( ) ], csgo::right_player_bones [ player->index ( ) ], sizeof ( csgo::right_player_bones [ player->index ( ) ] ) );
			break;
		default:
			break;
		}*/


		if ( records [ i ].size ( ) > 0 ) {
			for ( int t = 0; t < records [ i ].size ( ); t++ )
				if ( player->dormant ( ) && !player->health ( ) > 0 )
					records [ i ].clear ( );
				else
					if ( !is_tick_valid ( records [ i ].at ( t ) ) )
						records [ i ].erase ( records [ i ].begin ( ) + t );



		}



	}

}
void player_manager::restore_record( player_t* entity, backtrack_records_t record ) {
	entity->flags( ) = record.flags;
	//entity->simulation_time( ) = record.simtime;
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
	//entity->simulation_time( ) = record.simtime;
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

}