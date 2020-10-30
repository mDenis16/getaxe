#include "../utilities/csgo.hpp"

//aimtux
void math::correct_movement( vec3_t old_angles, c_usercmd* cmd, float old_forwardmove, float old_sidemove ) {
	vec3_t vecForward, vecRight, vecUp, vecForwardOld, vecRightOld, vecUpOld;
   math::angle_vectors ( old_angles, &vecForward, &vecRight, &vecUp );
   math::angle_vectors( cmd->viewangles, &vecForwardOld, &vecRightOld, &vecUpOld );

	const float flFwdLenght = vecForward.Length2D( );
	const float flRightLenght = vecRight.Length2D( );
	const float flUpLenght = std::sqrtf( vecUp.z * vecUp.z );

	const vec3_t vecNormFwd = vec3_t( 1.f / flFwdLenght * vecForward.x, 1.f / flFwdLenght * vecForward.y, 0.f ) * cmd->forwardmove;
	const vec3_t vecNormRight = vec3_t( 1.f / flRightLenght * vecRight.x, 1.f / flRightLenght * vecRight.y, 0.f ) * cmd->sidemove;
	const vec3_t vecNormUp = vec3_t( 0.f, 0.f, 1.f / flUpLenght * vecUp.z ) * cmd->upmove;

	const float flFwdOldLenght = vecForwardOld.Length2D( );
	const float flRightOldLenght = vecRightOld.Length2D( );
	const float flUpOldLenght = std::sqrtf( vecUpOld.z * vecUpOld.z );

	const vec3_t vecNormFwdOld( 1.f / flFwdOldLenght * vecForwardOld.x, 1.f / flFwdOldLenght * vecForwardOld.y, 0.f );
	const vec3_t vecNormRightOld( 1.f / flRightOldLenght * vecRightOld.x, 1.f / flRightOldLenght * vecRightOld.y, 0.f );
	const vec3_t vecNormUpOld( 0.f, 0.f, 1.f / flUpOldLenght * vecUpOld.z );

	const float x = vecNormFwdOld.x * vecNormRight.x + vecNormFwdOld.y * vecNormRight.y + vecNormFwdOld.z * vecNormRight.z
		+ ( vecNormFwdOld.x * vecNormFwd.x + vecNormFwdOld.y * vecNormFwd.y + vecNormFwdOld.z * vecNormFwd.z )
		+ ( vecNormFwdOld.y * vecNormUp.x + vecNormFwdOld.x * vecNormUp.y + vecNormFwdOld.z * vecNormUp.z );

	const float y = vecNormRightOld.x * vecNormRight.x + vecNormRightOld.y * vecNormRight.y + vecNormRightOld.z * vecNormRight.z
		+ ( vecNormRightOld.x * vecNormFwd.x + vecNormRightOld.y * vecNormFwd.y + vecNormRightOld.z * vecNormFwd.z )
		+ ( vecNormRightOld.x * vecNormUp.y + vecNormRightOld.y * vecNormUp.x + vecNormRightOld.z * vecNormUp.z );

	const float z = vecNormUpOld.x * vecNormRight.y + vecNormUpOld.y * vecNormRight.x + vecNormUpOld.z * vecNormRight.z
		+ ( vecNormUpOld.x * vecNormFwd.y + vecNormUpOld.y * vecNormFwd.x + vecNormUpOld.z * vecNormFwd.z )
		+ ( vecNormUpOld.x * vecNormUp.x + vecNormUpOld.y * vecNormUp.y + vecNormUpOld.z * vecNormUp.z );

	// clamp and apply corrected movement
	cmd->forwardmove = std::clamp( x, -450.f, 450.f );
	cmd->sidemove = std::clamp( y, -450.f, 450.f );
	cmd->upmove = std::clamp( z, -450.f, 450.f );
}
 int math::time_to_ticks( float time ) {
	 return static_cast< int >( time / interfaces::globals->interval_per_tick + 0.5f );
}
 float math::clamp_yaw( float yaw ) {
	 auto angles = yaw;

	 while ( angles > 180.0f )
		 angles -= 360.0f;

	 while ( angles < -180.0f )
		 angles += 360.0f;

	 if ( angles > 180.0f )
		 angles = 180.0f;
	 else if ( angles < -180.0f )
		 angles = -180.0f;

	 return angles;
 }
 float math::ticks_to_time(  int ticks ) {
	return static_cast< float >( ticks * interfaces::globals->interval_per_tick );
}
 vec3_t math::get_matrix_position( const matrix_t& src ) {
	 return vec3_t( src [ 0 ][ 3 ], src [ 1 ][ 3 ], src [ 2 ][ 3 ] );
 }
 void math::set_matrix_position( vec3_t pos, matrix_t& matrix ) {
	 for ( size_t i { }; i < 3; ++i ) {
		 matrix [ i ][ 3 ] = pos[i];
	 }
	 matrix.invalidate( );

 }
 void math::angle_matrix( const vec3_t angles, matrix_t& matrix )
 {
	 float sr, sp, sy, cr, cp, cy;

	 sy = sin( DEG2RAD( angles [ 1 ] ) );
	 cy = cos( DEG2RAD( angles [ 1 ] ) );

	 sp = sin( DEG2RAD( angles [ 0 ] ) );
	 cp = cos( DEG2RAD( angles [ 0 ] ) );

	 sr = sin( DEG2RAD( angles [ 2 ] ) );
	 cr = cos( DEG2RAD( angles [ 2 ] ) );

	 //matrix = (YAW * PITCH) * ROLL
	 matrix [ 0 ][ 0 ] = cp * cy;
	 matrix [ 1 ][ 0 ] = cp * sy;
	 matrix [ 2 ][ 0 ] = -sp;

	 float crcy = cr * cy;
	 float crsy = cr * sy;
	 float srcy = sr * cy;
	 float srsy = sr * sy;

	 matrix [ 0 ][ 1 ] = sp * srcy - crsy;
	 matrix [ 1 ][ 1 ] = sp * srsy + crcy;
	 matrix [ 2 ][ 1 ] = sr * cp;

	 matrix [ 0 ][ 2 ] = ( sp * crcy + srsy );
	 matrix [ 1 ][ 2 ] = ( sp * crsy - srcy );
	 matrix [ 2 ][ 2 ] = cr * cp;

	 matrix [ 0 ][ 3 ] = 0.0f;
	 matrix [ 1 ][ 3 ] = 0.0f;
	 matrix [ 2 ][ 3 ] = 0.0f;
 }

 void  math::matrix_set_column( const vec3_t& in, int column, matrix_t& out )
 {
	 out [ 0 ][ column ] = in.x;
	 out [ 1 ][ column ] = in.y;
	 out [ 2 ][ column ] = in.z;
 }

 void math::angle_matrix ( const vec3_t& angles, const vec3_t& position, matrix_t& matrix_out )
 {
	 angle_matrix( angles, matrix_out );
	 matrix_set_column( position, 3, matrix_out );
 }

 void math::matrix_copy( const matrix_t& source, matrix_t& target )
 {
	 for ( int i = 0; i < 3; i++ ) {
		 for ( int j = 0; j < 4; j++ ) {
			 target [ i ][ j ] = source [ i ][ j ];
		 }
	 }
 }

 void math::matrix_multiply( matrix_t& in1, const matrix_t& in2 )
 {
	 matrix_t out;
	 if ( &in1 == &out )
	 {
		 matrix_t in1b;
		 matrix_copy( in1, in1b );
		 matrix_multiply( in1b, in2 );
		 return;
	 }
	 if ( &in2 == &out )
	 {
		 matrix_t in2b;
		 matrix_copy( in2, in2b );
		 matrix_multiply( in1, in2b );
		 return;
	 }
	 out [ 0 ][ 0 ] = in1 [ 0 ][ 0 ] * in2 [ 0 ][ 0 ] + in1 [ 0 ][ 1 ] * in2 [ 1 ][ 0 ] +
		 in1 [ 0 ][ 2 ] * in2 [ 2 ][ 0 ];
	 out [ 0 ][ 1 ] = in1 [ 0 ][ 0 ] * in2 [ 0 ][ 1 ] + in1 [ 0 ][ 1 ] * in2 [ 1 ][ 1 ] +
		 in1 [ 0 ][ 2 ] * in2 [ 2 ][ 1 ];
	 out [ 0 ][ 2 ] = in1 [ 0 ][ 0 ] * in2 [ 0 ][ 2 ] + in1 [ 0 ][ 1 ] * in2 [ 1 ][ 2 ] +
		 in1 [ 0 ][ 2 ] * in2 [ 2 ][ 2 ];
	 out [ 0 ][ 3 ] = in1 [ 0 ][ 0 ] * in2 [ 0 ][ 3 ] + in1 [ 0 ][ 1 ] * in2 [ 1 ][ 3 ] +
		 in1 [ 0 ][ 2 ] * in2 [ 2 ][ 3 ] + in1 [ 0 ][ 3 ];
	 out [ 1 ][ 0 ] = in1 [ 1 ][ 0 ] * in2 [ 0 ][ 0 ] + in1 [ 1 ][ 1 ] * in2 [ 1 ][ 0 ] +
		 in1 [ 1 ][ 2 ] * in2 [ 2 ][ 0 ];
	 out [ 1 ][ 1 ] = in1 [ 1 ][ 0 ] * in2 [ 0 ][ 1 ] + in1 [ 1 ][ 1 ] * in2 [ 1 ][ 1 ] +
		 in1 [ 1 ][ 2 ] * in2 [ 2 ][ 1 ];
	 out [ 1 ][ 2 ] = in1 [ 1 ][ 0 ] * in2 [ 0 ][ 2 ] + in1 [ 1 ][ 1 ] * in2 [ 1 ][ 2 ] +
		 in1 [ 1 ][ 2 ] * in2 [ 2 ][ 2 ];
	 out [ 1 ][ 3 ] = in1 [ 1 ][ 0 ] * in2 [ 0 ][ 3 ] + in1 [ 1 ][ 1 ] * in2 [ 1 ][ 3 ] +
		 in1 [ 1 ][ 2 ] * in2 [ 2 ][ 3 ] + in1 [ 1 ][ 3 ];
	 out [ 2 ][ 0 ] = in1 [ 2 ][ 0 ] * in2 [ 0 ][ 0 ] + in1 [ 2 ][ 1 ] * in2 [ 1 ][ 0 ] +
		 in1 [ 2 ][ 2 ] * in2 [ 2 ][ 0 ];
	 out [ 2 ][ 1 ] = in1 [ 2 ][ 0 ] * in2 [ 0 ][ 1 ] + in1 [ 2 ][ 1 ] * in2 [ 1 ][ 1 ] +
		 in1 [ 2 ][ 2 ] * in2 [ 2 ][ 1 ];
	 out [ 2 ][ 2 ] = in1 [ 2 ][ 0 ] * in2 [ 0 ][ 2 ] + in1 [ 2 ][ 1 ] * in2 [ 1 ][ 2 ] +
		 in1 [ 2 ][ 2 ] * in2 [ 2 ][ 2 ];
	 out [ 2 ][ 3 ] = in1 [ 2 ][ 0 ] * in2 [ 0 ][ 3 ] + in1 [ 2 ][ 1 ] * in2 [ 1 ][ 3 ] +
		 in1 [ 2 ][ 2 ] * in2 [ 2 ][ 3 ] + in1 [ 2 ][ 3 ];

	 in1 = out;
 }
  float math::dot_product( const float* v1, const float* v2 ) {
	 return v1 [ 0 ] * v2 [ 0 ] + v1 [ 1 ] * v2 [ 1 ] + v1 [ 2 ] * v2 [ 2 ];
 }
 void math::vector_rotate( const float* in1, const matrix_t& in2, float* out )
 {
	 out [ 0 ] = math::dot_product( in1, in2 [ 0 ] );
	 out [ 1 ] = math::dot_product( in1, in2 [ 1 ] );
	 out [ 2 ] = math::dot_product( in1, in2 [ 2 ] );
 }

 void math::vector_rotate( const vec3_t& in1, const matrix_t& in2, vec3_t& out )
 {
	 vector_rotate( &in1.x, in2, &out.x );
 }

 void  math::vector_rotate( const vec3_t& in1, const vec3_t& in2, vec3_t& out )
 {
	 matrix_t matRotate;
     math::angle_matrix ( in2, matRotate );
	 math::vector_rotate( in1, matRotate, out );
 }
  vec3_t math::lerp( const vec3_t& a, const vec3_t& b, const float t ) {

	  return vec3_t( std::lerp( a.x, b.x, t ), std::lerp( a.y, b.y, t ), std::lerp( a.z, b.z, t ));
  }
void math::clamp( vec3_t& angles ) {
	while ( angles.x > 89.0f )
		angles.x -= 180.f;

	while ( angles.x < -89.0f )
		angles.x += 180.f;

	while ( angles.y > 180.f )
		angles.y -= 360.f;

	while ( angles.y < -180.f )
		angles.y += 360.f;

	if ( angles.x > 89.0f )
		angles.x = 89.0f;
	else if ( angles.x < -89.0f )
		angles.x = -89.0f;

	if ( angles.y > 180.0f )
		angles.y = 180.0f;
	else if ( angles.y < -180.0f )
		angles.y = -180.0f;

	angles.z = 0.0f;
}
void math::random_seed( int seed )
{
	static auto random_seed = reinterpret_cast< void( * )( int ) >( GetProcAddress( GetModuleHandleA( "vstdlib.dll" ), "RandomSeed" ) );

	random_seed( seed );
}
float math::random_float( float min, float max ) {
	static auto random_float_fn = ( float( * )( float, float ) ) GetProcAddress( GetModuleHandleA( "vstdlib.dll" ), "RandomFloat" );

	return random_float_fn( min, max );
}

int math::random_int( int min, int max ) {
	static auto random_int_fn = ( int( * )( int, int ) ) GetProcAddress( GetModuleHandleA( "vstdlib.dll" ), "RandomInt" );

	return random_int_fn( min, max );
}
vec3_t math::cubic_bezier( const vec3_t& a, const vec3_t& b, const vec3_t& c, const vec3_t& d, const float t ) {
	vec3_t ab, bc, cd, abbc, bccd;
	vec3_t result;

	ab = lerp( a, b, t );
	bc = lerp( b, c, t );
	cd = lerp( c, d, t );
	abbc = lerp( ab, bc, t );
	bccd = lerp( bc, cd, t );

	result = lerp( abbc, bccd, t );
	return result;
}
vec3_t math::calc_angle( vec3_t src, vec3_t dst ) {
	auto angle = vec3_t( );
	auto direction = src - dst;

	math::normalize_num( direction, direction );
	math::vector_angles( direction, angle );
	return angle;
}
void math::vector_vectors( const vec3_t& forward, vec3_t& right, vec3_t& up )
{

	vec3_t tmp;

	if ( fabs( forward [ 0 ] ) < 1e-6 && fabs( forward [ 1 ] ) < 1e-6 )
	{
		// pitch 90 degrees up/down from identity
		right [ 0 ] = 0;
		right [ 1 ] = -1;
		right [ 2 ] = 0;
		up [ 0 ] = -forward [ 2 ];
		up [ 1 ] = 0;
		up [ 2 ] = 0;
	}
	else
	{
		tmp [ 0 ] = 0; tmp [ 1 ] = 0; tmp [ 2 ] = 1.0;


		right.crossproduct( forward, tmp, right );
		right.normalize( );

		up.crossproduct( right, forward, up );
		up.normalize( );
	}
}

void math::matrix_get_column( const matrix_t& in, int column, vec3_t& out )
{
	out.x = in [ 0 ][ column ];
	out.y = in [ 1 ][ column ];
	out.z = in [ 2 ][ column ];
}

void math::vector_matrix( const vec3_t& forward, matrix_t& matrix )
{
	
	vec3_t right, up;
	math::vector_vectors( forward, right, up );

	matrix_set_column( forward, 0, matrix );
	matrix_set_column( vec3_t( -right.x, -right.y, right.z ), 1, matrix );
	matrix_set_column( up, 2, matrix );
}

void math::smooth_angles( vec3_t view_angles, vec3_t aim_angles, vec3_t& out_angles, float smoothing_x, float smoothing_y ) {
	out_angles = aim_angles - view_angles;

	if ( smoothing_x < 1.f )
		smoothing_x = 10.f;

	if ( smoothing_y < 10.f )
		smoothing_y = 10.f;

	smoothing_y = smoothing_y / 10.f;
	smoothing_x = smoothing_x / 10.f;

	auto normalize = [ & ]( )
	{
		while ( out_angles.x > 89.0f )
			out_angles.x -= 180.f;

		while ( out_angles.x < -89.0f )
			out_angles.x += 180.f;

		while ( out_angles.y > 180.f )
			out_angles.y -= 360.f;

		while ( out_angles.y < -180.f )
			out_angles.y += 360.f;
	};
	normalize( );

	out_angles.x = out_angles.x / smoothing_x + view_angles.x;
	out_angles.y = out_angles.y / smoothing_y + view_angles.y;

	normalize( );
}
float math::get_fov( const vec3_t& va, const vec3_t& eyepos, const vec3_t& aimpos )
{
	auto forward = vec3_t {};
	auto delta = ( aimpos - eyepos ).normalized( );

	forward = math::angle_vector( va );

	return RAD2DEG( acos( delta.dot( forward ) ) );
}
float math::get_fov( vec3_t viewangle, vec3_t aim_angle ) {
	vec3_t delta = aim_angle - viewangle;
	clamp( delta );
	return sqrtf( powf( delta.x, 2.0f ) + powf( delta.y, 2.0f ) );
}
float math::get_estimate_server_time( c_usercmd* cmd )
{
	const auto v1 = interfaces::engine->get_net_channel_info();

	const auto v3 = v1->get_average_latency( 0);
	const auto v4 = v1->get_average_latency( 1);

	return v3 + v4 + math::ticks_to_time( 1 ) + math::ticks_to_time( cmd->tick_count );
}

void math::vector_substract( const vec3_t& a, const vec3_t& b, vec3_t& c ) {
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
}

void math::smooth_angle( vec3_t src, vec3_t& dst, float factor ) {
	if ( factor == 0 ) return;

	factor *= 3;
	vec3_t diff = dst - src;

	auto normalize = [ & ]( )
	{
		while ( diff.x > 89.0f )
			diff.x -= 180.f;

		while ( diff.x < -89.0f )
			diff.x += 180.f;

		while ( diff.y > 180.f )
			diff.y -= 360.f;

		while ( diff.y < -180.f )
			diff.y += 360.f;
	};
	normalize( );
	auto calced = diff / std::powf( factor, 0.8f );
	dst = src + calced;
}

float math::calc_distance( const vec3_t src, const vec3_t dst, bool _2d = false ) {
	return sqrtf( pow( ( src.y - dst.y ), 2 ) + pow( ( src.x - dst.x ), 2 ) + (_2d ? 1.0 :pow( ( src.z - dst.z ), 2 ) ) );
}

void math::sin_cos( float r, float* s, float* c ) {
	*s = sin( r );
	*c = cos( r );
}

float math::distance_to_ray( const  vec3_t& pos, const vec3_t& ray_start, const  vec3_t& ray_end, float* along, vec3_t* point_on_ray )
{
	vec3_t to = pos - ray_start;
	vec3_t dir = ray_end - ray_start;
	float length = dir.normalized( ).length( );

	float range_along = dir.dot( to );
	if ( along )
		*along = range_along;

	float range;

	if ( range_along < 0.0f )
	{
		range = -( pos - ray_start ).length( );

		if ( point_on_ray )
			*point_on_ray = ray_start;
	}
	else if ( range_along > length )
	{
		range = -( pos - ray_end ).length( );

		if ( point_on_ray )
			*point_on_ray = ray_end;
	}
	else
	{
		vec3_t on_ray = ray_start + range_along * dir;
		range = ( pos - on_ray ).length( );

		if ( point_on_ray )
			*point_on_ray = on_ray;
	}

	return range;
}
vec3_t math::angle_vector( vec3_t angle ) {
	auto sy = sin( angle.y / 180.f * static_cast< float >( M_PI ) );
	auto cy = cos( angle.y / 180.f * static_cast< float >( M_PI ) );

	auto sp = sin( angle.x / 180.f * static_cast< float >( M_PI ) );
	auto cp = cos( angle.x / 180.f * static_cast< float >( M_PI ) );

	return vec3_t( cp * cy, cp * sy, -sp );
}

void math::VectorAnglesAwall( const vec3_t& vecForward, vec3_t& angView )
{
	float flPitch, flYaw;

	if ( vecForward .x == 0.f && vecForward .y == 0.f )
	{
		flPitch = ( vecForward .z > 0.f )?270.f:90.f;
		flYaw = 0.f;
	}
	else
	{
		flPitch = std::atan2f( -vecForward .z, vecForward.Length2D( ) ) * 180.f / M_PI;

		if ( flPitch < 0.f )
			flPitch += 360.f;

		flYaw = std::atan2f( vecForward .y, vecForward .x ) * 180.f / M_PI;

		if ( flYaw < 0.f )
			flYaw += 360.f;
	}

	angView .x = flPitch;
	angView .y = flYaw;
	angView .z = 0.f;
}
void math::transform_vector( vec3_t& a, matrix_t& b, vec3_t& out ) {
	out.x = a.dot( b.mat_val[ 0 ] ) + b.mat_val[ 0 ][ 3 ];
	out.y = a.dot( b.mat_val[ 1 ] ) + b.mat_val[ 1 ][ 3 ];
	out.z = a.dot( b.mat_val[ 2 ] ) + b.mat_val[ 2 ][ 3 ];
}

void math::vector_angles( vec3_t& forward, vec3_t& angles ) {
	if ( forward.y == 0.0f && forward.x == 0.0f ) {
		angles.x = ( forward.z > 0.0f )?270.0f:90.0f;
		angles.y = 0.0f;
	} else {
		angles.x = atan2( -forward.z, vec2_t( forward ).length( ) ) * -180 / static_cast< float >( M_PI );
		angles.y = atan2( forward.y, forward.x ) * 180 / static_cast< float >( M_PI );

		if ( angles.y > 90 )
			angles.y -= 180;
		else if ( angles.y < 90 )
			angles.y += 180;
		else if ( angles.y == 90 )
			angles.y = 0;
	}

	angles.z = 0.0f;
}

void math::angle_vectors( vec3_t& angles, vec3_t* forward, vec3_t* right, vec3_t* up ) {
	float sp, sy, sr, cp, cy, cr;

	sin_cos( DEG2RAD( angles.x ), &sp, &cp );
	sin_cos( DEG2RAD( angles.y ), &sy, &cy );
	sin_cos( DEG2RAD( angles.z ), &sr, &cr );

	if ( forward ) {
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if ( right ) {
		right->x = -1 * sr * sp * cy + -1 * cr * -sy;
		right->y = -1 * sr * sp * sy + -1 * cr * cy;
		right->z = -1 * sr * cp;
	}

	if ( up ) {
		up->x = cr * sp * cy + -sr * -sy;
		up->y = cr * sp * sy + -sr * cy;
		up->z = cr * cp;
	}
}

void math::angle_vectors( vec3_t& angles, vec3_t& forward ) {
	float sp, sy, cp, cy;

	sin_cos( DEG2RAD( angles.y ), &sy, &cy );
	sin_cos( DEG2RAD( angles.x ), &sp, &cp );

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
}
vec3_t math::vector_add( const vec3_t a, const vec3_t  b ) {
	return vec3_t( a.x + b.x,
		a.y + b.y,
		a.z + b.z );
}

vec3_t math::vector_subtract( vec3_t& a, vec3_t& b ) {
	return vec3_t( a.x - b.x,
		a.y - b.y,
		a.z - b.z );
}

vec3_t math::vector_multiply( vec3_t& a, vec3_t& b ) {
	return vec3_t( a.x * b.x,
		a.y * b.y,
		a.z * b.z );
}

vec3_t math::vector_divide( vec3_t& a, vec3_t& b ) {
	return vec3_t( a.x / b.x,
		a.y / b.y,
		a.z / b.z );
}

bool math::screen_transform( const vec3_t& point, vec3_t& screen ) {
	auto matrix = interfaces::engine->world_to_screen_matrix( );

	float w = matrix[ 3 ][ 0 ] * point.x + matrix[ 3 ][ 1 ] * point.y + matrix[ 3 ][ 2 ] * point.z + matrix[ 3 ][ 3 ];
	screen.x = matrix[ 0 ][ 0 ] * point.x + matrix[ 0 ][ 1 ] * point.y + matrix[ 0 ][ 2 ] * point.z + matrix[ 0 ][ 3 ];
	screen.y = matrix[ 1 ][ 0 ] * point.x + matrix[ 1 ][ 1 ] * point.y + matrix[ 1 ][ 2 ] * point.z + matrix[ 1 ][ 3 ];
	screen.z = 0.0f;

	int inverse_width = static_cast< int >( ( w < 0.001f )?-1.0f / w:
		1.0f / w );

	screen.x *= inverse_width;
	screen.y *= inverse_width;
	return ( w < 0.001f );
}

void math::normalize_num( vec3_t& vIn, vec3_t& vOut ) {
	float flLen = vIn.length( );
	if ( flLen == 0 ) {
		vOut.init( 0, 0, 1 );
		return;
	}
	flLen = 1 / flLen;
	vOut.init( vIn.x * flLen, vIn.y * flLen, vIn.z * flLen );
}
float math::fl_angle_mod( float fl_angle )
{
	return( ( 360.0f / 65536.0f ) * ( ( int32_t ) ( fl_angle * ( 65536.0f / 360.0f ) ) & 65535 ) );
}
float math::angle_diff( float destAngle, float srcAngle )
{
	float delta = 0.f;

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
}
float math::fl_approach_angle( float fl_target, float fl_value, float fl_speed )
{
	float fl_adjusted_speed = fl_speed;
	if ( fl_adjusted_speed < 0.0f )
		fl_adjusted_speed *= -1.0f;


	float fl_angle_mod_target = fl_angle_mod( fl_target );
	float fl_angle_mod_value = fl_angle_mod( fl_value );

	float fl_delta = ( fl_angle_mod_target - fl_angle_mod_value );
	if ( fl_delta >= -180.0f )
	{
		if ( fl_delta >= 180.0f )
			fl_delta -= 360.0f;
	}
	else
	{
		if ( fl_delta <= -180.0f )
			fl_delta += 360.0f;
	}

	float fl_return;

	if ( fl_delta <= fl_adjusted_speed )
	{
		if ( ( fl_adjusted_speed * -1.0f ) <= fl_delta )
			fl_return = fl_angle_mod_target;
		else
			fl_return = ( fl_angle_mod_value - fl_adjusted_speed );
	}
	else
		fl_return = ( fl_angle_mod_value + fl_adjusted_speed );

	return fl_return;
}
float math::normalize_yaw( float f ) {
	if ( std::isnan( f ) || std::isinf( f ) )
		f = 0;

	if ( f > 9999999 )
		f = 0;

	if ( f < -9999999 )
		f = 0;

	while ( f < -180.0f ) f += 360.0f;
	while ( f > 180.0f ) f -= 360.0f;

	return f;
}

bool math::world_to_screen( const vec3_t& origin, vec3_t& screen ) {
	auto matrix = interfaces::engine->world_to_screen_matrix( );

	auto find_point = [ ]( vec3_t& point, int screen_w, int screen_h, int degrees ) -> void {
		float x2 = screen_w * 0.5f;
		float y2 = screen_h * 0.5f;

		float d = sqrt( pow( ( point.x - x2 ), 2 ) + ( pow( ( point.y - y2 ), 2 ) ) ); //Distance
		float r = degrees / d; //Segment ratio

		point.x = r * point.x + ( 1 - r ) * x2; //find point that divides the segment
		point.y = r * point.y + ( 1 - r ) * y2; //into the ratio (1-r):r
	};

	float w = matrix[ 3 ][ 0 ] * origin.x + matrix[ 3 ][ 1 ] * origin.y + matrix[ 3 ][ 2 ] * origin.z + matrix[ 3 ][ 3 ];

	int screen_width, screen_height;
	interfaces::engine->get_screen_size( screen_width, screen_height );

	float inverse_width = -1.0f / w;
	bool behind = true;

	if ( w > 0.01 ) {
		inverse_width = 1.0f / w;
		behind = false;
	}

	screen.x = ( float ) ( ( screen_width / 2 ) + ( 0.5 * ( ( matrix[ 0 ][ 0 ] * origin.x
		+ matrix[ 0 ][ 1 ] * origin.y
		+ matrix[ 0 ][ 2 ] * origin.z
		+ matrix[ 0 ][ 3 ] ) * inverse_width ) * screen_width + 0.5 ) );

	screen.y = ( float ) ( ( screen_height / 2 ) - ( 0.5 * ( ( matrix[ 1 ][ 0 ] * origin.x
		+ matrix[ 1 ][ 1 ] * origin.y
		+ matrix[ 1 ][ 2 ] * origin.z
		+ matrix[ 1 ][ 3 ] ) * inverse_width ) * screen_height + 0.5 ) );

	if ( screen.x > screen_width || screen.x < 0 || screen.y > screen_height || screen.y < 0 || behind ) {
		find_point( screen, screen_width, screen_height, screen_height / 2 );
		return false;
	}

	return !( behind );
}