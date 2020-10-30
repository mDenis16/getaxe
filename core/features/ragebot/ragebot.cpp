#include "../features.hpp"
#include <optional>
std::deque<player_t*> ragebot::targets;
ragebot::target_data ragebot::get_last_target;
static int last_shot_tick = 0;
bool ragebot::m_shoot_next_tick;
int ragebot::last_target_index = -1;
vec3_t  ragebot::weapon_shoot_pos;
std::vector<ragebot::point> ragebot::hitscan_points;
std::optional<vec3_t> ragebot::get_intersect_point( vec3_t start, vec3_t end, vec3_t mins, vec3_t maxs, float radius )
{
	auto sphereRayIntersection = [ start, end, radius ] ( auto&& center ) -> std::optional<vec3_t>
	{
		auto direction = ( end - start ).normalizedvec( );

		auto q = vec3_t(center - start);
		auto v = q.dot( direction );
		auto d = radius * radius - ( q.length_sqr( ) - v * v );

		if ( d < FLT_EPSILON )
			return {};

		return start + direction * ( v - std::sqrt( d ) );
	};

	auto delta = ( maxs - mins ).normalizedvec( );
	for ( size_t i {}; i < std::floor( mins.distance_to( maxs ) ); ++i )
	{
		if ( auto intersection = sphereRayIntersection( mins + delta * float( i ) ); intersection )
			return intersection;
	}

	if ( auto intersection = sphereRayIntersection( maxs ); intersection )
		return intersection;

	return {};
}
bool ragebot::is_point_is_safe( player_t* entity, vec3_t from, vec3_t point, int hitbox, matrix_t new_matrix [ MAXSTUDIOBONES ] ) {
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
	studio_box_t* _hitbox = set->hitbox( hitbox );

	vec3_t max;
	vec3_t min;
	const auto mod = _hitbox->radius != -1.f?_hitbox->radius:0.f;
	VectorTransform_Wrapper( vec3_t( _hitbox->maxs.x + mod, _hitbox->maxs.y + mod, _hitbox->maxs.z + mod ), new_matrix [ _hitbox->bone ], max );
	VectorTransform_Wrapper( vec3_t( _hitbox->mins.x - mod, _hitbox->mins.y - mod, _hitbox->mins.z - mod ), new_matrix [ _hitbox->bone ], min );
	if ( auto intersection = get_intersect_point( from, point, min, max, _hitbox->radius ); intersection )
	{
		
		return true;
	}
	return false;
}
float g_CapsuleVertices [ ][ 3 ] =
{
	{ -0.01f, -0.01f, 1.00f },
	{ 0.51f, 0.00f, 0.86f },
	{ 0.44f, 0.25f, 0.86f },
	{ 0.25f, 0.44f, 0.86f },
	{ -0.01f, 0.51f, 0.86f },
	{ -0.26f, 0.44f, 0.86f },
	{ -0.45f, 0.25f, 0.86f },
	{ -0.51f, 0.00f, 0.86f },
	{ -0.45f, -0.26f, 0.86f },
	{ -0.26f, -0.45f, 0.86f },
	{ -0.01f, -0.51f, 0.86f },
	{ 0.25f, -0.45f, 0.86f },
	{ 0.44f, -0.26f, 0.86f },
	{ 0.86f, 0.00f, 0.51f },
	{ 0.75f, 0.43f, 0.51f },
	{ 0.43f, 0.75f, 0.51f },
	{ -0.01f, 0.86f, 0.51f },
	{ -0.44f, 0.75f, 0.51f },
	{ -0.76f, 0.43f, 0.51f },
	{ -0.87f, 0.00f, 0.51f },
	{ -0.76f, -0.44f, 0.51f },
	{ -0.44f, -0.76f, 0.51f },
	{ -0.01f, -0.87f, 0.51f },
	{ 0.43f, -0.76f, 0.51f },
	{ 0.75f, -0.44f, 0.51f },
	{ 1.00f, 0.00f, 0.01f },
	{ 0.86f, 0.50f, 0.01f },
	{ 0.49f, 0.86f, 0.01f },
	{ -0.01f, 1.00f, 0.01f },
	{ -0.51f, 0.86f, 0.01f },
	{ -0.87f, 0.50f, 0.01f },
	{ -1.00f, 0.00f, 0.01f },
	{ -0.87f, -0.50f, 0.01f },
	{ -0.51f, -0.87f, 0.01f },
	{ -0.01f, -1.00f, 0.01f },
	{ 0.49f, -0.87f, 0.01f },
	{ 0.86f, -0.51f, 0.01f },
	{ 1.00f, 0.00f, -0.02f },
	{ 0.86f, 0.50f, -0.02f },
	{ 0.49f, 0.86f, -0.02f },
	{ -0.01f, 1.00f, -0.02f },
	{ -0.51f, 0.86f, -0.02f },
	{ -0.87f, 0.50f, -0.02f },
	{ -1.00f, 0.00f, -0.02f },
	{ -0.87f, -0.50f, -0.02f },
	{ -0.51f, -0.87f, -0.02f },
	{ -0.01f, -1.00f, -0.02f },
	{ 0.49f, -0.87f, -0.02f },
	{ 0.86f, -0.51f, -0.02f },
	{ 0.86f, 0.00f, -0.51f },
	{ 0.75f, 0.43f, -0.51f },
	{ 0.43f, 0.75f, -0.51f },
	{ -0.01f, 0.86f, -0.51f },
	{ -0.44f, 0.75f, -0.51f },
	{ -0.76f, 0.43f, -0.51f },
	{ -0.87f, 0.00f, -0.51f },
	{ -0.76f, -0.44f, -0.51f },
	{ -0.44f, -0.76f, -0.51f },
	{ -0.01f, -0.87f, -0.51f },
	{ 0.43f, -0.76f, -0.51f },
	{ 0.75f, -0.44f, -0.51f },
	{ 0.51f, 0.00f, -0.87f },
	{ 0.44f, 0.25f, -0.87f },
	{ 0.25f, 0.44f, -0.87f },
	{ -0.01f, 0.51f, -0.87f },
	{ -0.26f, 0.44f, -0.87f },
	{ -0.45f, 0.25f, -0.87f },
	{ -0.51f, 0.00f, -0.87f },
	{ -0.45f, -0.26f, -0.87f },
	{ -0.26f, -0.45f, -0.87f },
	{ -0.01f, -0.51f, -0.87f },
	{ 0.25f, -0.45f, -0.87f },
	{ 0.44f, -0.26f, -0.87f },
	{ 0.00f, 0.00f, -1.00f },
};
std::vector<ragebot::point> ragebot::multi_point( player_t* entity, const int& hit_box ) {


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
	studio_box_t* hitbox = set->hitbox( hit_box );

	std::vector<ragebot::point> vecArray;

	vec3_t max;
	vec3_t min;
	const auto mod = hitbox->radius != -1.f?hitbox->radius:0.f;
	VectorTransform_Wrapper( vec3_t( hitbox->maxs.x + mod , hitbox->maxs.y + mod, hitbox->maxs.z + mod), csgo::player_bones [ entity->index( ) ][ hitbox->bone ], max );
	VectorTransform_Wrapper( vec3_t( hitbox->mins.x - mod, hitbox->mins.y - mod, hitbox->mins.z - mod ), csgo::player_bones [ entity->index( ) ][ hitbox->bone ], min );


	auto center_vec = ( min + max ) * 0.5f;
	matrix_t dmatrix, rmatrix;
	ragebot::point center;
	center.is_safe = false;
	center.point = center_vec;
	vec3_t CurrentAngles = math::calc_angle( center.point, engine_prediction::unpredicted_eye );

	vec3_t Forward = math::angle_vector( CurrentAngles );

	vec3_t Right = Forward.cross( vec3_t( 0, 0, 1 ) );
	vec3_t Left = vec3_t( -Right.x, -Right.y, Right.z );

	vec3_t Top = vec3_t( 0, 0, 1 );
	vec3_t Bot = vec3_t( 0, 0, -1 );
	float scale = 0.f;
	int iMaxPoints = 70;

	if ( hit_box == hitbox_head ) {
	
		for ( auto i = 0; i < 5; ++i )
		{
			vecArray.emplace_back( center );
		}
		vec3_t middle = ( Top - Bot );
		vecArray [ 0 ].point += vec3_t( Right.x, Right.y, Top.z) * ( hitbox->radius * ( variables::ragebot::head_scale / 100.f ) );
		vecArray [ 1 ].point += vec3_t( Left.x, Left.y, Top.z ) * ( hitbox->radius * ( variables::ragebot::head_scale / 100.f ) );
		vecArray [ 2 ].point += vec3_t( Right.x, Right.y, middle.z ) * ( hitbox->radius * ( variables::ragebot::head_scale / 100.f ) );
		vecArray [ 3 ].point += vec3_t( Left.x, Left.y, middle.z ) * ( hitbox->radius * ( variables::ragebot::head_scale / 100.f ) );
		vecArray [ 4 ].point += Top * ( hitbox->radius * ( variables::ragebot::head_scale / 100.f ) );
		

/*		for ( double angle = 0.0; angle <= 2 * M_PI; angle += ( 2 * M_PI ) / 30 ) {
			ragebot::point pp;
			pp.is_safe = false;
			auto vec_rotated = center_vec * Forward;

			pp.point = vec3_t( vec_rotated.x + cos( angle ) * hitbox->radius, vec_rotated.y + ( sin( angle ) * hitbox->radius ), vec_rotated.z );
			vecArray.emplace_back(
			pp
			);
		}*/
		
	}
	else if ( hit_box >= hitbox_pelvis && hit_box <= hitbox_upper_chest )
	{
		float body_scale = ( variables::ragebot::double_tap && tickbase_system::m_shift_data.m_needs_recharge == 0 )?5.f:variables::ragebot::point_scale;
		for ( auto i = 0; i < 3; ++i )
		{
			vecArray.emplace_back( center );
		}
		vecArray [ 1 ].point += Right * ( hitbox->radius * ( variables::ragebot::point_scale / 100.f ) );
		vecArray [ 2 ].point += Left * ( hitbox->radius * ( variables::ragebot::point_scale / 100.f ) );

	}
	else
		vecArray.emplace_back( center ); 

	
	
	
	for ( size_t i = 0; i < vecArray.size(); i++ )
	{
		vecArray.at( i ).is_safe = is_point_is_safe( entity, engine_prediction::unpredicted_eye, vecArray.at( i ).point, hit_box, csgo::left_player_bones [ entity->index( ) ] ) || is_point_is_safe( entity, engine_prediction::unpredicted_eye, vecArray.at( i ).point, hit_box, csgo::right_player_bones [ entity->index( ) ] );

		if ( vecArray.at( i ).is_safe ) {
			interfaces::console->console_printf( "FOUND SAFEPOINT FOR HITBOX %i \n", hit_box );
		}
	}

	return vecArray;
}

int ragebot::prioritize_hitbox( )
{
	int best_hitbox = 0;
	switch ( variables::ragebot::prioritize_hitbox )
	{
	case  0:
		return ( int ) hitbox_head;
		break;
	case  1:
		return ( int ) hitbox_neck;
		break;
	case  2:
		return ( int ) hitbox_upper_chest;
		break;
	case  3:
		return ( int ) hitbox_chest;
		break;
	case  4:
		return ( int ) hitbox_pelvis;
		break;
	case  5:
		return ( int ) hitbox_left_foot;
		break;
	default:
		return 0;
		break;
	}
}
std::vector< int > ragebot::hitscan_list( player_t* entity, bool extrapolation = false)
{
	auto best_hitbox = prioritize_hitbox( );
	std::vector<int> hitscan;
	if ( (variables::ragebot::double_tap && tickbase_system::m_shift_data.m_needs_recharge == 0) || extrapolation ) {

		hitscan.push_back( ( int ) hitboxes::hitbox_upper_chest );

		hitscan.push_back( ( int ) hitboxes::hitbox_chest );

		hitscan.push_back( ( int ) hitboxes::hitbox_pelvis );
	}
	else {
		hitscan.push_back( best_hitbox );

	//	if ( resolver::resolver_data [ entity->index( ) ].missed_shots == 0 )
	//	{
			hitscan.push_back( ( int ) hitboxes::hitbox_head );

			hitscan.push_back( ( int ) hitboxes::hitbox_neck );
	//	}

		hitscan.push_back( ( int ) hitboxes::hitbox_upper_chest );

		hitscan.push_back( ( int ) hitboxes::hitbox_chest );

		hitscan.push_back( ( int ) hitboxes::hitbox_pelvis );

		if ( ( !resolver::resolver_data [ entity->index( ) ].is_using_desync ) )
		{
			if ( entity->velocity( ).Length2D( ) < 0.25f )
			{
				hitscan.push_back( ( int ) hitboxes::hitbox_left_foot );
				hitscan.push_back( ( int ) hitboxes::hitbox_right_foot );
				hitscan.push_back( ( int ) hitboxes::hitbox_right_thigh );
				hitscan.push_back( ( int ) hitboxes::hitbox_left_thigh );
				hitscan.push_back( ( int ) hitboxes::hitbox_left_calf );
				hitscan.push_back( ( int ) hitboxes::hitbox_right_calf );

				hitscan.push_back( ( int ) hitboxes::hitbox_right_upper_arm );
				hitscan.push_back( ( int ) hitboxes::hitbox_left_upper_arm );
				hitscan.push_back( ( int ) hitboxes::hitbox_left_forearm );
				hitscan.push_back( ( int ) hitboxes::hitbox_right_forearm );
				hitscan.push_back( ( int ) hitboxes::hitbox_left_hand );
				hitscan.push_back( ( int ) hitboxes::hitbox_right_hand );
			}
		}

	}
	
	std::sort( hitscan.begin( ), hitscan.end( ) );
	hitscan.erase( std::unique( hitscan.begin( ), hitscan.end( ) ), hitscan.end( ) );

	return hitscan;
}

void ragebot::get_rotated_matrix( player_t* entity, float angle, matrix_t new_matrix [ MAXSTUDIOBONES ] ) {
	vec3_t BonePos;
	vec3_t OutPos;
	
	for ( int i = 0; i < MAXSTUDIOBONES; i++ )
	{
	math::angle_matrix ( vec3_t( 0, angle, 0 ), new_matrix [ i ] );
	math::matrix_multiply( new_matrix [ i ], csgo::player_bones[entity->index()] [i] );
		BonePos = vec3_t( csgo::player_bones [ entity->index( ) ][ i ][ 0 ][ 3 ], csgo::player_bones [ entity->index( ) ][ i ][ 1 ][ 3 ], csgo::player_bones [ entity->index( ) ][ i ][ 2 ][ 3 ] ) - entity->origin();
		math::vector_rotate( BonePos, vec3_t( 0, angle, 0 ), OutPos );
		OutPos += entity->origin( );
		new_matrix [ i ][ 0 ][ 3 ] = OutPos.x;
		new_matrix [ i ][ 1 ][ 3 ] = OutPos.y;
		new_matrix [ i ][ 2 ][ 3 ] = OutPos.z;
	}

}
void ragebot::modify_eye_pos( anim_state* animstate, vec3_t* pos )
{


	static auto lookup_bone_fn = ( int( __thiscall* )( void*, const char* ) ) utilities::pattern_scan( ( "client.dll" ), "55 8B EC 53 56 8B F1 57 83 BE ?? ?? ?? ?? ?? 75 14" );

	if ( animstate->m_bInHitGroundAnimation || animstate->m_fDuckAmount != 0.f )
	{
		auto base_entity = animstate->m_pBaseEntity;

		//	auto bone_pos = get_bone_pos( reinterpret_cast< player_t* >( base_entity ), lookup_bone_fn( base_entity, "head_0" ) );
		auto ent = reinterpret_cast< player_t* >( base_entity );
		auto 	bone_pos = ent->get_bone_position( lookup_bone_fn( base_entity, "head_0" ) );

		bone_pos.z += 1.7f;

		if ( ( *pos ).z > bone_pos.z )
		{
			float some_factor = 0.f;

			float delta = ( *pos ).z - bone_pos.z;

			float some_offset = ( delta - 4.f ) / 6.f;
			if ( some_offset >= 0.f )
				some_factor = std::fminf( some_offset, 1.f );

			( *pos ).z += ( ( bone_pos.z - ( *pos ).z ) * ( ( ( some_factor * some_factor ) * 3.f ) - ( ( ( some_factor * some_factor ) * 2.f ) * some_factor ) ) );
		}
	}
}
inline void UpdateAnimationState( anim_state* state, vec3_t ang ) {
	using fn = void( __vectorcall* )( void*, void*, float, float, float, void* );
	static auto ret = reinterpret_cast< fn >( utilities::pattern_scan( "client.dll", ( "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24" ) ) );

	if ( !ret || !state )
		return;

	ret( state, nullptr, 0.f, ang.y, ang.x, nullptr );
}
vec3_t ragebot::weapon_shot_pos( vec3_t target )
{
	/*vec3_t relative_angle = math::calc_angle( csgo::local_player->get_eye_pos( ), target );

	for ( int i = 0; i < MAXSTUDIOBONES; i++ )
	{
		math::angle_matrix( relative_angle, csgo::player_bones [ csgo::local_player->index( ) ][ i ] );

	}

	weapon_shoot_pos = csgo::local_player->get_bone_position( 8, csgo::player_bones [ csgo::local_player->index( ) ] );
	*/
	return vec3_t( );
}

bool ragebot::hit_chance( vec3_t point, int hit_box, vec3_t angle, player_t* ent, float hitchance, int hitbox, matrix_t matrix [ MAXSTUDIOBONES ] ) {

	if (m_shoot_next_tick)
		hitchance = 25;
	auto local_player = csgo::local_player;


	if ( !local_player ) return false;
	int traces_hit = 0;
	int needed_hits = static_cast< int >( 150.f * ( hitchance / 100.f ) );
	vec3_t forward, right, up;
	const vec3_t eye_position = engine_prediction::unpredicted_eye;
	math::angle_vectors( angle, &forward, &right, &up );

	auto weapon = local_player->active_weapon( );
	if ( !weapon )
		return false;

	weapon->update_accuracy_penalty( );
	float weapon_spread = weapon->get_spread( );
	float weapon_cone = weapon->inaccuracy( );


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

	studio_hdr_t* pStudioModel = interfaces::model_info->get_studio_model( ent->model( ) );
	studio_hitbox_set_t* set = pStudioModel->hitbox_set( 0 );
	studio_box_t* _hitbox = set->hitbox( hitbox );

	vec3_t max;
	vec3_t min;
	const auto mod = _hitbox->radius != -1.f?_hitbox->radius:0.f;
	VectorTransform_Wrapper( vec3_t( _hitbox->maxs.x + mod, _hitbox->maxs.y + mod, _hitbox->maxs.z + mod ), matrix [ _hitbox->bone ], max );
	VectorTransform_Wrapper( vec3_t( _hitbox->mins.x - mod, _hitbox->mins.y - mod, _hitbox->mins.z - mod ), matrix [ _hitbox->bone ], min );
	const auto get_bullet_location = [ & ] ( int seed ) {

		math::random_seed( seed );

		float a = math::random_float( 0.f, 1.f );
		float b = math::random_float( 0.f, 2.f * M_PI );
		float c = math::random_float( 0.f, 1.f );
		float d = math::random_float( 0.f, 2.f * M_PI );

		const float generated_spread = a * weapon_spread;
		const float generated_cone = c * weapon_cone;

		const vec3_t spread = vec3_t(
			std::cos( b ) * generated_spread + std::cos( d ) * generated_cone,
			std::sin( b ) * generated_spread + std::sin( d ) * generated_cone,
			0.f
		);

		return vec3_t( forward + right * -spread.x + up * -spread.y ).normalized( );
	};

	for ( int i = 1; i < 150; i++ ) {
		vec3_t spread_angle;
		vec3_t bullet_end;
		auto a = get_bullet_location( i );
		math::VectorAnglesAwall( a, spread_angle );
		bullet_end = math::angle_vector( angle - ( spread_angle - angle ) );

	

		if ( auto intersection = get_intersect_point( eye_position, eye_position + bullet_end * weapon->get_weapon_data( )->flRange, min, max, _hitbox->radius ); intersection )
			++traces_hit;


		if ( static_cast< int >( ( static_cast< float >( traces_hit ) / 150.f ) * 100.f ) >= hitchance )
			return true;

		if ( ( 150 - i + traces_hit ) < needed_hits )
			return false;
	}

	return false;
}

ragebot::target_data ragebot::find_best_point_backtrack( player_t* entity ) {
	auto index = entity->index( );
	ragebot::target_data data;
	data.backtrack = true;
	data.extrapolation = false;

	data.dmg = 0.0; data.aimpos = vec3_t( ); data.entity = entity;
	auto best_record = player_manager::find_best_tick( entity );
	if ( best_record.simtime <= 0.0f )
		return data;
	player_manager::best_tick_global [ index ] = best_record;
	autowall::FireBulletData_t awall = { };
	player_manager::restore_record( entity, best_record );
	bool ignore_onshot = variables::ragebot::double_tap && (&tickbase_system::shift_data::m_needs_recharge == 0);

	if ( best_record.shoot && !ignore_onshot ) {
		auto onshot_possible_points = player_manager::on_shot_safe( entity, best_record );
		for ( auto point : onshot_possible_points ) {
			float dmg = autowall::GetDamage( csgo::local_player, point, awall );

			if ( dmg >= entity->health( ) / 2 )
			{
				data.aimpos = point;
				data.entity = entity;
				data.dmg = dmg;
				data.hitbox = hitbox_head;
				player_manager::restore_player( entity );
				return data;
			}
		}
		

	}

	for ( auto hitbox : hitscan_list( entity ) )
	{
		auto point = entity->get_hitbox_position( hitbox, best_record.bone );
	
			 
		bool is_safe = is_point_is_safe( entity, engine_prediction::unpredicted_eye, point, hitbox, best_record.bone_left) || is_point_is_safe( entity, engine_prediction::unpredicted_eye, point, hitbox, best_record.bone_right  );

		

		float dmg = autowall::GetDamage( csgo::local_player, point, awall );


		if ( is_safe && ( dmg >= ( data.dmg + 10 ) ) && dmg >= ( variables::ragebot::double_tap && tickbase_system::m_shift_data.m_needs_recharge == 0 && entity->health( ) < 50?50.f:variables::ragebot::min_dmg ) )
		{
			data.aimpos = point;
			data.entity = entity;
			data.dmg = dmg;
			data.hitbox = hitbox;

		}

	}
	player_manager::restore_player( entity );
	return data;
}
ragebot::target_data ragebot::find_best_point( player_t* entity ) {
	ragebot::target_data data;
	ragebot::hitscan_points.clear( );
	data.backtrack = false;
	data.extrapolation = false;
	data.dmg = 0.0; data.aimpos = vec3_t( ); data.entity = nullptr;
	autowall::FireBulletData_t awall = { };

	for ( auto hitbox : hitscan_list( entity ) )
	{
		for ( auto point : multi_point( entity, hitbox ) )
		{

			 ragebot::hitscan_points.push_back( point );
			
			int dmg = autowall::GetDamage( csgo::local_player, point.point, awall );

			if ( ( dmg >= ( data.dmg + 10 ) ) && dmg >= ( variables::ragebot::double_tap && tickbase_system::m_shift_data.m_needs_recharge == 0 && entity->health() < 50 ?  50.f :variables::ragebot::min_dmg )  && point.is_safe )
			{
				data.aimpos = point.point;
				data.entity = entity;
				data.dmg = dmg;
				data.hitbox = hitbox;
				if ( static_cast< int32_t >( dmg ) >= entity->health( ) )
				{
					return data;
					break;
				}
			}
		}
	}

	return data;
}

ragebot::target_data ragebot::get_target_data( player_t* entity ) {
	
	ragebot::target_data backtrack = find_best_point_backtrack( entity );
	backtrack.record = player_manager::best_tick_global [ entity->index( ) ];
	backtrack.backtrack = true;
	backtrack.extrapolation = false;

	if ( backtrack.record.shoot  ) {

		
		return backtrack;
	}

	ragebot::target_data hitscan = find_best_point( entity );
	hitscan.backtrack = false;
	hitscan.extrapolation = false;

	
	if ( backtrack.dmg > ( hitscan.dmg + 10.f ) )
		return backtrack;
	else
		return hitscan;
	




}
void ragebot::sort_targets( )
{
	targets.clear( );
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

		targets.push_back( entity );

	}

	struct {
		bool operator()( struct player_t* a, struct player_t* b ) const
		{
			auto local = csgo::local_player->get_eye_pos( );
			vec3_t viewangle = vec3_t( ); interfaces::engine->get_view_angles( viewangle );
			switch ( variables::ragebot::target_selection )
			{
			case 0:
				return  math::get_fov(viewangle, local, a->get_eye_pos() ) < math::get_fov( viewangle, local, b->get_eye_pos( ) );
			case 1:
				return a->origin( ).distance_to( csgo::local_player->origin( ) ) < b->origin( ).distance_to( csgo::local_player->origin( ) );
			}

			return false;
		}
	} targetSort;

	std::sort( targets.begin( ), targets.end( ), targetSort );
	if ( targets.size( ) > variables::ragebot::max_targets )
		targets.erase( targets.begin( ) + variables::ragebot::max_targets, targets.end( ) );
}

int sleep_ticks = 0;
int since_use = 0;
void ragebot::createmove( c_usercmd* cmd, bool& send_packet ) {

	
	static bool shot = false;
	static player_t* last_target = nullptr;

	if ( !variables::ragebot::enabled )
		return;

	if (!tickbase_system::m_shift_data.m_did_shift_before && !tickbase_system::m_shift_data.m_should_be_ready)
		m_shoot_next_tick = false;
	sort_targets( );

	ragebot::target_data best_target; best_target.aimpos = vec3_t( ); best_target.dmg = 0.f; best_target.entity = nullptr;
	best_target.backtrack = false;
	best_target.extrapolation = false;
	for ( auto ent : targets )
	{

		auto target_data = get_target_data( ent );

		if (  !target_data.aimpos.is_zero( ) && best_target.dmg <= target_data.dmg  )
		{
			best_target.dmg = target_data.dmg;
			best_target.aimpos = target_data.aimpos;
			best_target.entity = ent;
			best_target.hitbox = target_data.hitbox;
			best_target.backtrack = target_data.backtrack;
			best_target.record = target_data.record;
			best_target.extrapolation = target_data.extrapolation;
		}
	}


	if ( !best_target.aimpos.is_zero( ) && best_target.dmg != float {} ) {



		//	printf( "SET BT SIMTIME %f (%f) PLAYER SIMTIME  TICKS: %i\n", player_manager::best_tick_global [ best_target.entity->index( ) ].simtime, sim_time,  math::time_to_ticks(variables::cheat:interval_per_tick, best_target.entity->simulation_time( ) - player_manager::best_tick_global [ best_target.entity->index( ) ].simtime ) );

		//printf( "SIMTIME %f \n", player_manager::best_tick_global [ best_target.entity->index//( ) ].simtime );
		bool using_scout = csgo::local_player->active_weapon( )->item_definition_index( ) == weapon_ssg08;
		if ( variables::ragebot::autostop && csgo::local_player->flags( ) & fl_onground )
		{
			if ( using_scout )
			{
				if ( best_target.dmg >= best_target.entity->health( ) && csgo::local_player->can_shoot( false ) )
				{
					cmd->sidemove = 0;
					cmd->forwardmove = csgo::local_player->velocity( ).Length2D( ) > 20.f?450.f:0.f;


					autostop::rotate_movement( cmd, math::calc_angle( vec3_t( 0, 0, 0 ), csgo::local_player->velocity( ) ).y + 180.f );
				}
			}
			else
			{
				cmd->sidemove = 0;
				cmd->forwardmove = csgo::local_player->velocity( ).Length2D( ) > 20.f?450.f:0.f;

				autostop::rotate_movement( cmd, math::calc_angle( vec3_t( 0, 0, 0 ), csgo::local_player->velocity( ) ).y + 180.f );
			}
		}




		auto ang = math::calc_angle( engine_prediction::unpredicted_eye, best_target.aimpos );
		ang -= csgo::local_player->aim_punch_angle( ) * 2.f;
		math::clamp( ang );

		float sim_time = 0.f;

		if ( best_target.backtrack || best_target.extrapolation )
			player_manager::restore_record( best_target.entity, best_target.record );
		
		best_target.simtime = best_target.backtrack || best_target.extrapolation ? best_target.record.simtime :best_target.entity->simulation_time( );
		if (!best_target.extrapolation )
	     	best_target.simtime += player_manager::get_lerp_time( );
		else
	    	interfaces::console->console_printf( "EXTRAPOLATING ... %b", best_target.extrapolation );
		if ( hit_chance( best_target.aimpos, best_target.hitbox,  ang, best_target.entity, variables::ragebot::hitchance, best_target.hitbox, (best_target.backtrack || best_target.extrapolation) ? best_target.record.bone :  csgo::player_bones [ best_target.entity->index( ) ] ))
		{

			sleep_ticks++;
			if ( !variables::ragebot::silent_aim )
				interfaces::engine->set_view_angles( ang );
			/*if ( !tickbase_system::m_shift_data.m_should_attempt_shift || ( ( csgo::m_goal_shift == 12 || tickbase_system::m_shift_data.m_should_disable ) && tickbase_system::m_shift_data.m_should_attempt_shift ) || ( csgo::m_goal_shift == 7 && tickbase_system::m_shift_data.m_should_attempt_shift && !( tickbase_system::m_shift_data.m_prepare_recharge || tickbase_system::m_shift_data.m_did_shift_before && !tickbase_system::m_shift_data.m_should_be_ready ) ) ) {
				{
					if (variables::ragebot::auto_fire && csgo::local_player->can_shoot(false))
					cmd->buttons |= (int)in_attack;
				else if (variables::ragebot::auto_fire && !csgo::local_player->can_shoot( false ))
					cmd->buttons &= ~(int)in_attack;

				}
			}*/
			
			if ( variables::ragebot::auto_fire && csgo::local_player->can_shoot( variables::ragebot::double_tap ) )
				cmd->buttons |= ( int ) in_attack;
			else if ( variables::ragebot::auto_fire && !csgo::local_player->can_shoot( variables::ragebot::double_tap ) )
				cmd->buttons &= ~( int ) in_attack;

			if ( cmd->buttons & ( int ) in_attack )
			{

				cmd->tick_count = math::time_to_ticks( best_target.simtime );
				//interfaces::console->console_printf( "SIMTIME BETWEEN SHOTS %f \n", fabs( ragebot::get_last_target.simtime - best_target.simtime ) );
				cmd->viewangles = ang;
				send_packet = true;
				ragebot::get_last_target = best_target;
				ragebot::last_target_index = best_target.entity->index();
				if (m_shoot_next_tick)
					m_shoot_next_tick = false;

				if ( sleep_ticks > 20 )
				{
					sleep_ticks = 0;
					//	if ( variables::ragebot::double_tap )
						//	tickbase_system::m_shift_data.m_ticks_to_shift = math::time_to_ticks(variables::cheat:interval_per_tick, 0.2f );
					visuals::capsule_overlay( best_target.entity, 1.7f, best_target.backtrack?player_manager::best_tick_global [ best_target.entity->index( ) ].bone:csgo::player_bones [ best_target.entity->index( ) ] );

				}
				if ( best_target.extrapolation )
				interfaces::console->console_printf( "EXTRAPOLATED %b \n", best_target.extrapolation );
				csgo::choke_next_tick = true;
				if (!m_shoot_next_tick && csgo::m_goal_shift == 12 && tickbase_system::m_shift_data.m_should_attempt_shift && !(tickbase_system::m_shift_data.m_prepare_recharge || tickbase_system::m_shift_data.m_did_shift_before && !tickbase_system::m_shift_data.m_should_be_ready)) {
					m_shoot_next_tick = true;
				}
				if ( autostop::m_autostop_data.state == 1 )
					autostop::m_autostop_data.state = 2;
			}
		}

		if ( best_target.backtrack ) player_manager::restore_player( best_target.entity );


	}
}

