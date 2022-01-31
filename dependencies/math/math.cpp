#include "../utilities/csgo.hpp"


#define sin math::fast_sin
#define cos math::fast_cos

int math::time_to_ticks ( float time ) {
	return static_cast< int >( time / interfaces::globals->interval_per_tick + 0.5f );
}
float math::clamp_yaw ( float yaw ) {
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
void math::quaternion_align ( const quaternion & p, const quaternion & q, quaternion & qt ) {

	// FIXME: can this be done with a quat dot product? hai mars ca tot as ae 

	int i;
	// decide if one of the quaternions is backwards
	float a = 0;
	float b = 0;
	for ( i = 0; i < 4; i++ ) {
		a += ( p [ i ] - q [ i ] ) * ( p [ i ] - q [ i ] );
		b += ( p [ i ] + q [ i ] ) * ( p [ i ] + q [ i ] );
	}
	if ( a > b ) {
		for ( i = 0; i < 4; i++ ) {
			qt [ i ] = -q [ i ];
		}
	}
	else if ( &qt != &q ) {
		for ( i = 0; i < 4; i++ ) {
			qt [ i ] = q [ i ];
		}
	}
}

/*de la cretinii de la valve*/
void math::angle_quaterinion ( const vec3_t & angles, quaternion & outQuat ) {
	float sr, sp, sy, cr, cp, cy;

	sin_cos ( DEG2RAD ( angles [ 1 ] ) * 0.5f, &sy, &cy );
	sin_cos ( DEG2RAD ( angles [ 0 ] ) * 0.5f, &sp, &cp );
	sin_cos ( DEG2RAD ( angles [ 2 ] ) * 0.5f, &sr, &cr );

	// NJS: for some reason VC6 wasn't recognizing the common subexpressions:
	float srXcp = sr * cp, crXsp = cr * sp;
	outQuat [ 0 ] = srXcp * cy - crXsp * sy; // X
	outQuat [ 1 ] = crXsp * cy + srXcp * sy; // Y

	float crXcp = cr * cp, srXsp = sr * sp;
	outQuat [ 2 ] = crXcp * sy - srXsp * cy; // Z
	outQuat [ 3 ] = crXcp * cy + srXsp * sy; // W (real component)
}

void math::quaternion_slerp ( const quaternion & p, const quaternion & q, float t, quaternion & qt ) {
	quaternion q2;
	// 0.0 returns p, 1.0 return q.

	// decide if one of the quaternions is backwards
	quaternion_align ( p, q, q2 );

	quaternion_slerp_no_align ( p, q2, t, qt );
}


void math::quaternion_slerp_no_align ( const quaternion & p, const quaternion & q, float t, quaternion & qt ) {

	float omega, cosom, sinom, sclp, sclq;
	int i;

	// 0.0 returns p, 1.0 return q.

	cosom = p [ 0 ] * q [ 0 ] + p [ 1 ] * q [ 1 ] + p [ 2 ] * q [ 2 ] + p [ 3 ] * q [ 3 ];

	if ( ( 1.0f + cosom ) > 0.000001f ) {
		if ( ( 1.0f - cosom ) > 0.000001f ) {
			omega = acos ( cosom );
			sinom = sin ( omega );
			sclp = sin ( ( 1.0f - t ) * omega ) / sinom;
			sclq = sin ( t * omega ) / sinom;
		}
		else {
			// TODO: add short circuit for cosom == 1.0f?
			sclp = 1.0f - t;
			sclq = t;
		}
		for ( i = 0; i < 4; i++ ) {
			qt [ i ] = sclp * p [ i ] + sclq * q [ i ];
		}
	}
	else {
		Assert ( &qt != &q );

		qt [ 0 ] = -q [ 1 ];
		qt [ 1 ] = q [ 0 ];
		qt [ 2 ] = -q [ 3 ];
		qt [ 3 ] = q [ 2 ];
		sclp = sin ( ( 1.0f - t ) * ( 0.5f * M_PI ) );
		sclq = sin ( t * ( 0.5f * M_PI ) );
		for ( i = 0; i < 3; i++ ) {
			qt [ i ] = sclp * p [ i ] + sclq * qt [ i ];
		}
	}

}
void QuaternionMatrix ( const quaternion & q, matrix3x4_t & matrix ) {

	matrix [ 0 ][ 0 ] = 1.0 - 2.0 * q [ 1 ] * q [ 1 ] - 2.0 * q [ 2 ] * q [ 2 ];
	matrix [ 1 ][ 0 ] = 2.0 * q [ 0 ] * q [ 1 ] + 2.0 * q [ 3 ] * q [ 2 ];
	matrix [ 2 ][ 0 ] = 2.0 * q [ 0 ] * q [ 2 ] - 2.0 * q [ 3 ] * q [ 1 ];

	matrix [ 0 ][ 1 ] = 2.0f * q [ 0 ] * q [ 1 ] - 2.0f * q [ 3 ] * q [ 2 ];
	matrix [ 1 ][ 1 ] = 1.0f - 2.0f * q [ 0 ] * q [ 0 ] - 2.0f * q [ 2 ] * q [ 2 ];
	matrix [ 2 ][ 1 ] = 2.0f * q [ 1 ] * q [ 2 ] + 2.0f * q [ 3 ] * q [ 0 ];

	matrix [ 0 ][ 2 ] = 2.0f * q [ 0 ] * q [ 2 ] + 2.0f * q [ 3 ] * q [ 1 ];
	matrix [ 1 ][ 2 ] = 2.0f * q [ 1 ] * q [ 2 ] - 2.0f * q [ 3 ] * q [ 0 ];
	matrix [ 2 ][ 2 ] = 1.0f - 2.0f * q [ 0 ] * q [ 0 ] - 2.0f * q [ 1 ] * q [ 1 ];

	matrix [ 0 ][ 3 ] = 0.0f;
	matrix [ 1 ][ 3 ] = 0.0f;
	matrix [ 2 ][ 3 ] = 0.0f;


}
void MatrixAngles ( const matrix3x4_t & matrix, vec3_t & angles ) {

	float forward [ 3 ];
	float left [ 3 ];
	float up [ 3 ];

	//
	// Extract the basis vectors from the matrix. Since we only need the Z
	// component of the up vector, we don't get X and Y.
	//
	forward [ 0 ] = matrix [ 0 ][ 0 ];
	forward [ 1 ] = matrix [ 1 ][ 0 ];
	forward [ 2 ] = matrix [ 2 ][ 0 ];
	left [ 0 ] = matrix [ 0 ][ 1 ];
	left [ 1 ] = matrix [ 1 ][ 1 ];
	left [ 2 ] = matrix [ 2 ][ 1 ];
	up [ 2 ] = matrix [ 2 ][ 2 ];

	float xyDist = sqrtf ( forward [ 0 ] * forward [ 0 ] + forward [ 1 ] * forward [ 1 ] );

	// enough here to get angles?
	if ( xyDist > 0.001f ) {
		// (yaw)	y = ATAN( forward[1], forward[0] );		-- in our space, forward is the X axis
		angles [ 1 ] = RAD2DEG ( atan2f ( forward [ 1 ], forward [ 0 ] ) );

		// (pitch)	x = ATAN( -forward[2], sqrt(forward[0]*forward[0]+forward[1]*forward[1]) );
		angles [ 0 ] = RAD2DEG ( atan2f ( -forward [ 2 ], xyDist ) );

		// (roll)	z = ATAN( left[2], up[2] );
		angles [ 2 ] = RAD2DEG ( atan2f ( left [ 2 ], up [ 2 ] ) );
	}
	else	// forward is mostly Z, gimbal lock-
	{
		// (yaw)	y = ATAN( -left[0], left[1] );			-- forward is mostly z, so use right for yaw
		angles [ 1 ] = RAD2DEG ( atan2f ( -left [ 0 ], left [ 1 ] ) );

		// (pitch)	x = ATAN( -forward[2], sqrt(forward[0]*forward[0]+forward[1]*forward[1]) );
		angles [ 0 ] = RAD2DEG ( atan2f ( -forward [ 2 ], xyDist ) );

		// Assume no roll in this case as one degree of freedom has been lost (i.e. yaw == roll)
		angles [ 2 ] = 0;
	}

}
void math::quaterinion_angles ( const quaternion & q, vec3_t & angles ) {




	//#if 1
		// FIXME: doing it this way calculates too much data, needs to do an optimized version...
	matrix3x4_t matrix;
	QuaternionMatrix ( q, matrix );
	MatrixAngles ( matrix, angles );
	/*#else
		float m11, m12, m13, m23, m33;
		m11 = ( 2.0f * q[3] * q[3] ) + ( 2.0f * q[0] * q[0] ) - 1.0f;
		m12 = ( 2.0f * q[0] * q[1] ) + ( 2.0f * q[3] * q[2] );
		m13 = ( 2.0f * q[0] * q[2] ) - ( 2.0f * q[3] * q[1] );
		m23 = ( 2.0f * q[1] * q[2] ) + ( 2.0f * q[3] * q[0] );
		m33 = ( 2.0f * q[3] * q[3] ) + ( 2.0f * q[2] * q[2] ) - 1.0f;
		// FIXME: this code has a singularity near PITCH +-90
		angles [ YAW ] = RAD2DEG ( atan2 ( m12, m11 ) );
		angles [ PITCH ] = RAD2DEG ( asin ( -m13 ) );
		angles [ ROLL ] = RAD2DEG ( atan2 ( m23, m33 ) );
	#endif*/

	Assert ( angles.IsValid ( ) );
}

void math::interpolate_angles ( const vec3_t & start, const vec3_t & end, vec3_t & output, float frac ) {
	quaternion src, dest;

	// Convert to quaternions
	angle_quaterinion ( start, src );
	angle_quaterinion ( end, dest );

	quaternion result;

	// Slerp
	quaternion_slerp ( src, dest, frac, result );

	// Convert to euler
	quaterinion_angles ( result, output );
}


float math::ticks_to_time ( int ticks ) {
	return static_cast< float >( ticks * interfaces::globals->interval_per_tick );
}
vec3_t math::get_matrix_position ( const matrix3x4_t & src ) {
	return vec3_t ( src [ 0 ][ 3 ], src [ 1 ][ 3 ], src [ 2 ][ 3 ] );
}
int math::ticks_to_stop ( vec3_t velocity ) {

	const float max_speed = 320.f;
	const float acceleration = 5.5f;
	const float max_accelspeed = acceleration * max_speed * interfaces::globals->interval_per_tick;

	return static_cast< int >( velocity.length ( ) / max_accelspeed );
}
void math::set_matrix_position ( vec3_t pos, matrix3x4_t & matrix ) {
	for ( size_t i { }; i < 3; ++i ) {
		matrix [ i ][ 3 ] = pos [ i ];
	}
	///matrix.invalidate ( );

}
float math::fast_sin ( float x ) {
	x *= float ( 0.159155 );
	x -= floor ( x );
	const auto xx = x * x;
	auto y = -6.87897;
	y = y * xx + 33.7755;
	y = y * xx - 72.5257;
	y = y * xx + 80.5874;
	y = y * xx - 41.2408;
	y = y * xx + 6.28077;
	return float ( x * y );
}

float math::fast_cos ( const float x ) {
	return fast_sin ( x + 1.5708f );
}

void math::angle_matrix ( const vec3_t angles, matrix3x4_t & matrix ) {
	float sr, sp, sy, cr, cp, cy;

	sy = sin ( DEG2RAD ( angles [ 1 ] ) );
	cy = cos ( DEG2RAD ( angles [ 1 ] ) );

	sp = sin ( DEG2RAD ( angles [ 0 ] ) );
	cp = cos ( DEG2RAD ( angles [ 0 ] ) );

	sr = sin ( DEG2RAD ( angles [ 2 ] ) );
	cr = cos ( DEG2RAD ( angles [ 2 ] ) );

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


void  math::matrix_set_column ( const vec3_t & in, int column, matrix3x4_t & out ) {
	out [ 0 ][ column ] = in [ 0 ];
	out [ 1 ][ column ] = in [ 1 ];
	out [ 2 ][ column ] = in [ 2 ];
}

void math::angle_matrix ( const vec3_t & angles, const vec3_t & position, matrix3x4_t & matrix_out ) {
	angle_matrix ( angles, matrix_out );
	matrix_set_column ( position, 3, matrix_out );
}

void math::matrix_copy ( const matrix3x4_t & source, matrix3x4_t & target ) {
	for ( int i = 0; i < 3; i++ ) {
		for ( int j = 0; j < 4; j++ ) {
			target [ i ][ j ] = source [ i ][ j ];
		}
	}
}

void math::matrix_multiply ( matrix3x4_t & in1, const matrix3x4_t & in2 ) {
	matrix3x4_t out;
	if ( &in1 == &out ) {
		matrix3x4_t in1b;
		matrix_copy ( in1, in1b );
		matrix_multiply ( in1b, in2 );
		return;
	}
	if ( &in2 == &out ) {
		matrix3x4_t in2b;
		matrix_copy ( in2, in2b );
		matrix_multiply ( in1, in2b );
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
float math::dot_product ( const float * v1, const float * v2 ) {
	return v1 [ 0 ] * v2 [ 0 ] + v1 [ 1 ] * v2 [ 1 ] + v1 [ 2 ] * v2 [ 2 ];
}
float math::dot_product ( vec3_t& v1, vec3_t & v2 ) {
	return v1 [ 0 ] * v2 [ 0 ] + v1 [ 1 ] * v2 [ 1 ] + v1 [ 2 ] * v2 [ 2 ];
}
void math::vector_rotate ( const float * in1, const matrix3x4_t & in2, float * out ) {
	out [ 0 ] = math::dot_product ( in1, in2 [ 0 ] );
	out [ 1 ] = math::dot_product ( in1, in2 [ 1 ] );
	out [ 2 ] = math::dot_product ( in1, in2 [ 2 ] );
}

void math::vector_rotate ( const vec3_t & in1, const matrix3x4_t & in2, vec3_t & out ) {
	vector_rotate ( &in1.x, in2, &out.x );
}

void  math::vector_rotate ( const vec3_t & in1, const vec3_t & in2, vec3_t & out ) {
	matrix3x4_t matRotate;
	math::angle_matrix ( in2, matRotate );
	math::vector_rotate ( in1, matRotate, out );
}
vec3_t math::lerp ( const vec3_t & a, const vec3_t & b, const float t ) {

	return vec3_t ( std::lerp ( a [ 0 ], b [ 0 ], t ), std::lerp ( a [ 1 ], b [ 1 ], t ), std::lerp ( a [ 2 ], b [ 2 ], t ) );
}

void math::clamp ( vec3_t & angles ) {
	while ( angles [ 0 ] > 89.0f )
		angles [ 0 ] -= 180.f;

	while ( angles [ 0 ] < -89.0f )
		angles [ 0 ] += 180.f;

	while ( angles [ 1 ] > 180.f )
		angles [ 1 ] -= 360.f;

	while ( angles [ 1 ] < -180.f )
		angles [ 1 ] += 360.f;

	if ( angles [ 0 ] > 89.0f )
		angles [ 0 ] = 89.0f;
	else if ( angles [ 0 ] < -89.0f )
		angles [ 0 ] = -89.0f;

	if ( angles [ 1 ] > 180.0f )
		angles [ 1 ] = 180.0f;
	else if ( angles [ 1 ] < -180.0f )
		angles [ 1 ] = -180.0f;

	angles [ 2 ] = 0.0f;
}
void math::random_seed ( int seed ) {
	static auto random_seed = reinterpret_cast< void( * )( int ) >( GetProcAddress ( GetModuleHandleA ( "vstdlib.dll" ), "RandomSeed" ) );

	random_seed ( seed );
}
float math::random_float ( float min, float max ) {
	static auto random_float_fn = ( float( * )( float, float ) ) GetProcAddress ( GetModuleHandleA ( "vstdlib.dll" ), "RandomFloat" );

	return random_float_fn ( min, max );
}

int math::random_int ( int min, int max ) {
	static auto random_int_fn = ( int( * )( int, int ) ) GetProcAddress ( GetModuleHandleA ( "vstdlib.dll" ), "RandomInt" );

	return random_int_fn ( min, max );
}
vec3_t math::cubic_bezier ( const vec3_t & a, const vec3_t & b, const vec3_t & c, const vec3_t & d, const float t ) {
	vec3_t ab, bc, cd, abbc, bccd;
	vec3_t result;

	ab = lerp ( a, b, t );
	bc = lerp ( b, c, t );
	cd = lerp ( c, d, t );
	abbc = lerp ( ab, bc, t );
	bccd = lerp ( bc, cd, t );

	result = lerp ( abbc, bccd, t );
	return result;
}
vec3_t math::calc_angle ( vec3_t src, vec3_t dst ) {
	auto angle = vec3_t ( );
	auto direction = src - dst;

	math::normalize_num ( direction, direction );
	math::vector_angles ( direction, angle );
	return angle;
}
void math::vector_vectors ( const vec3_t & forward, vec3_t & right, vec3_t & up ) {

	vec3_t tmp;

	if ( fabs ( forward [ 0 ] ) < 1e-6 && fabs ( forward [ 1 ] ) < 1e-6 ) {
		// pitch 90 degrees up/down from identity
		right [ 0 ] = 0;
		right [ 1 ] = -1;
		right [ 2 ] = 0;
		up [ 0 ] = -forward [ 2 ];
		up [ 1 ] = 0;
		up [ 2 ] = 0;
	}
	else {
		tmp [ 0 ] = 0; tmp [ 1 ] = 0; tmp [ 2 ] = 1.0;


		right.crossproduct ( forward, tmp, right );
		right.normalize ( );

		up.crossproduct ( right, forward, up );
		up.normalize ( );
	}
}

void math::matrix_get_column ( const matrix3x4_t & in, int column, vec3_t & out ) {
	out [ 0 ] = in [ 0 ][ column ];
	out [ 1 ] = in [ 1 ][ column ];
	out [ 2 ] = in [ 2 ][ column ];
}

void math::vector_matrix ( const vec3_t & forward, matrix3x4_t & matrix ) {

	vec3_t right, up;
	math::vector_vectors ( forward, right, up );

	matrix_set_column ( forward, 0, matrix );
	matrix_set_column ( vec3_t ( -right [ 0 ], -right [ 1 ], right [ 2 ] ), 1, matrix );
	matrix_set_column ( up, 2, matrix );
}

void math::smooth_angles ( vec3_t view_angles, vec3_t aim_angles, vec3_t & out_angles, float smoothing_x, float smoothing_y ) {
	out_angles = aim_angles - view_angles;

	if ( smoothing_x < 1.f )
		smoothing_x = 10.f;

	if ( smoothing_y < 10.f )
		smoothing_y = 10.f;

	smoothing_y = smoothing_y / 10.f;
	smoothing_x = smoothing_x / 10.f;

	auto normalize = [ & ] ( ) {
		while ( out_angles [ 0 ] > 89.0f )
			out_angles [ 0 ] -= 180.f;

		while ( out_angles [ 0 ] < -89.0f )
			out_angles [ 0 ] += 180.f;

		while ( out_angles [ 1 ] > 180.f )
			out_angles [ 1 ] -= 360.f;

		while ( out_angles [ 1 ] < -180.f )
			out_angles [ 1 ] += 360.f;
	};
	normalize ( );

	out_angles [ 0 ] = out_angles [ 0 ] / smoothing_x + view_angles [ 0 ];
	out_angles [ 1 ] = out_angles [ 1 ] / smoothing_y + view_angles [ 1 ];

	normalize ( );
}
float math::get_fov ( const vec3_t & va, const vec3_t & eyepos, const vec3_t & aimpos ) {
	auto forward = vec3_t {};
	auto delta = ( aimpos - eyepos ).normalized ( );

	forward = math::angle_vector ( va );

	return RAD2DEG ( acos ( delta.dot ( forward ) ) );
}
float math::get_fov ( vec3_t viewangle, vec3_t aim_angle, float distance) {
	auto pitch = sin ( DEG2RAD ( viewangle.x - aim_angle.x ) ) * distance;
	auto yaw = sin ( DEG2RAD ( viewangle.y - aim_angle.y ) ) * distance;

	return sqrt ( powf ( pitch, 2.0 ) + powf ( yaw, 2.0 ) );
}
float math::get_estimate_server_time ( c_usercmd * cmd ) {
	const auto v1 = interfaces::engine->get_net_channel_info ( );

	const auto v3 = v1->get_average_latency ( 0 );
	const auto v4 = v1->get_average_latency ( 1 );

	return v3 + v4 + math::ticks_to_time ( 1 ) + math::ticks_to_time ( cmd->tick_count );
}

vec3_t math::bezier ( float t, vec3_t & a, vec3_t & b, vec3_t & c, vec3_t & d ) {
	vec3_t dst;

	dst.x =
		a.x * ( 1.f - t ) * ( 1.f - t ) * ( 1.f - t ) +
		b.x * 3.f * t * ( 1.f - t ) * ( 1.f - t ) +
		c.x * 3.f * t * t * ( 1.f - t ) +
		d.x * t * t * t;
	dst.y =
		a.y * ( 1.f - t ) * ( 1.f - t ) * ( 1.f - t ) +
		b.y * 3.f * t * ( 1.f - t ) * ( 1.f - t ) +
		c.y * 3.f * t * t * ( 1 - t ) +
		d.y * t * t * t;
	dst.z =
		a.z * ( 1.f - t ) * ( 1.f - t ) * ( 1.f - t ) +
		b.z * 3.f * t * ( 1.f - t ) * ( 1 - t ) +
		c.z * 3.f * t * t * ( 1.f - t ) +
		d.z * t * t * t;

	return dst;

}

void math::vector_substract ( const vec3_t & a, const vec3_t & b, vec3_t & c ) {
	c [ 0 ] = a [ 0 ] - b [ 0 ];
	c [ 1 ] = a [ 1 ] - b [ 1 ];
	c [ 2 ] = a [ 2 ] - b [ 2 ];
}

void math::smooth_angle ( vec3_t src, vec3_t & dst, float factor ) {
	if ( factor == 0 ) return;

	factor *= 3;
	vec3_t diff = dst - src;

	auto normalize = [ & ] ( ) {
		while ( diff [ 0 ] > 89.0f )
			diff [ 0 ] -= 180.f;

		while ( diff [ 0 ] < -89.0f )
			diff [ 0 ] += 180.f;

		while ( diff [ 1 ] > 180.f )
			diff [ 1 ] -= 360.f;

		while ( diff [ 1 ] < -180.f )
			diff [ 1 ] += 360.f;
	};
	normalize ( );
	auto calced = diff / std::powf ( factor, 0.8f );
	dst = src + calced;
}

float math::calc_distance ( const vec3_t src, const vec3_t dst, bool _2d = false ) {


	return sqrtf ( powf ( ( src [ 1 ] - dst [ 1 ] ), 2 ) + powf ( ( src [ 0 ] - dst [ 0 ] ), 2 ) + ( _2d ? 1.0f : powf ( ( src [ 2 ] - dst [ 2 ] ), 2 ) ) );
}
/*optimize out math functions with precomputed values*/
void math::sin_cos ( float r, float * s, float * c ) {
	*s = sin ( r );
	*c = cos ( r );
}

float math::distance_to_ray ( const  vec3_t & pos, const vec3_t & ray_start, const  vec3_t & ray_end, float * along, vec3_t * point_on_ray ) {
	vec3_t to = pos - ray_start;
	vec3_t dir = ray_end - ray_start;
	float length = dir.normalized ( ).length ( );

	float range_along = dir.dot ( to );
	if ( along )
		*along = range_along;

	float range;

	if ( range_along < 0.0f ) {
		range = -( pos - ray_start ).length ( );

		if ( point_on_ray )
			*point_on_ray = ray_start;
	}
	else if ( range_along > length ) {
		range = -( pos - ray_end ).length ( );

		if ( point_on_ray )
			*point_on_ray = ray_end;
	}
	else {
		vec3_t on_ray = ray_start + range_along * dir;
		range = ( pos - on_ray ).length ( );

		if ( point_on_ray )
			*point_on_ray = on_ray;
	}

	return range;
}
vec3_t math::angle_vector ( vec3_t angle ) {
	auto sy = sin ( angle [ 1 ] / 180.f * static_cast< float >( M_PI ) );
	auto cy = cos ( angle [ 1 ] / 180.f * static_cast< float >( M_PI ) );

	auto sp = sin ( angle [ 0 ] / 180.f * static_cast< float >( M_PI ) );
	auto cp = cos ( angle [ 0 ] / 180.f * static_cast< float >( M_PI ) );

	return vec3_t ( cp * cy, cp * sy, -sp );
}

/*void math::vector_angles( const vec3_t& vecForward, vec3_t& angView )
{
	float flPitch, flYaw;

	if ( vecForward [0] == 0.f && vecForward [1] == 0.f )
	{
		flPitch = ( vecForward [2] > 0.f )?270.f:90.f;
		flYaw = 0.f;
	}
	else
	{
		flPitch = std::atan2f( -vecForward [2], vecForward.Length2D( ) ) * 180.f / M_PI;

		if ( flPitch < 0.f )
			flPitch += 180.f;

		flYaw = std::atan2f( vecForward [1], vecForward [0] ) * 180.f / M_PI;

		if ( flYaw < 0.f )
			flYaw += 180.f;
	}

	angView [0] = flPitch;
	angView [1] = flYaw;
	angView [2] = 0.f;
}*/
void math::vector_angles ( vec3_t & forward, vec3_t & angles ) {
	if ( forward [ 1 ] == 0.0f && forward [ 0 ] == 0.0f ) {
		angles [ 0 ] = ( forward [ 2 ] > 0.0f ) ? 270.0f : 90.0f;
		angles [ 1 ] = 0.0f;
	}
	else {
		angles [ 0 ] = atan2 ( -forward [ 2 ], vec2_t ( forward ).length ( ) ) * -180 / static_cast< float >( M_PI );
		angles [ 1 ] = atan2 ( forward [ 1 ], forward [ 0 ] ) * 180 / static_cast< float >( M_PI );

		if ( angles [ 1 ] > 90 )
			angles [ 1 ] -= 180;
		else if ( angles [ 1 ] < 90 )
			angles [ 1 ] += 180;
		else if ( angles [ 1 ] == 90 )
			angles [ 1 ] = 0;
	}

	angles [ 2 ] = 0.0f;
}
void math::VectorAnglesAwall ( const vec3_t & vecForward, vec3_t & angView ) {
	float flPitch, flYaw;

	if ( vecForward [ 0 ] == 0.f && vecForward [ 1 ] == 0.f ) {
		flPitch = ( vecForward [ 2 ] > 0.f ) ? 270.f : 90.f;
		flYaw = 0.f;
	}
	else {
		flPitch = std::atan2f ( -vecForward [ 2 ], vecForward.Length2D ( ) ) * 57.2957795131f;

		if ( flPitch < 0.f )
			flPitch += 360.f;

		flYaw = std::atan2f ( vecForward [ 1 ], vecForward [ 0 ] ) * 57.2957795131f;

		if ( flYaw < 0.f )
			flYaw += 360.f;
	}

	angView [ 0 ] = flPitch;
	angView [ 1 ] = flYaw;
	angView [ 2 ] = 0.f;
}
void math::transform_vector ( vec3_t & a, matrix3x4_t & b, vec3_t & out ) {
	out [ 0 ] = a.dot ( b.m_flMatVal [ 0 ] ) + b.m_flMatVal [ 0 ][ 3 ];
	out [ 1 ] = a.dot ( b.m_flMatVal [ 1 ] ) + b.m_flMatVal [ 1 ][ 3 ];
	out [ 2 ] = a.dot ( b.m_flMatVal [ 2 ] ) + b.m_flMatVal [ 2 ][ 3 ];
}

float math::segment_to_segment ( const vec3_t s1, const vec3_t s2, const vec3_t k1, const vec3_t k2 ) {
	static auto constexpr epsilon = 0.00000001;

	auto u = s2 - s1;
	auto v = k2 - k1;
	const auto w = s1 - k1;

	const auto a = u.dot ( u );
	const auto b = u.dot ( v );
	const auto c = v.dot ( v );
	const auto d = u.dot ( w );
	const auto e = v.dot ( w );
	const auto D = a * c - b * b;
	float sn, sd = D;
	float tn, td = D;

	if ( D < epsilon ) {
		sn = 0.0f;
		sd = 1.0f;
		tn = e;
		td = c;
	}
	else {
		sn = b * e - c * d;
		tn = a * e - b * d;

		if ( sn < 0.0f ) {
			sn = 0.0f;
			tn = e;
			td = c;
		}
		else if ( sn > sd ) {
			sn = sd;
			tn = e + b;
			td = c;
		}
	}

	if ( tn < 0.0f ) {
		tn = 0.0f;

		if ( -d < 0.0f )
			sn = 0.0f;
		else if ( -d > a )
			sn = sd;
		else {
			sn = -d;
			sd = a;
		}
	}
	else if ( tn > td ) {
		tn = td;

		if ( -d + b < 0.0f )
			sn = 0;
		else if ( -d + b > a )
			sn = sd;
		else {
			sn = -d + b;
			sd = a;
		}
	}

	const float sc = abs ( sn ) < epsilon ? 0.0f : sn / sd;
	const float tc = abs ( tn ) < epsilon ? 0.0f : tn / td;

	m128 n = {};
	auto dp = w + u * sc - v * tc;
	n.f [ 0 ] = dp.dot ( dp );
	const auto calc = sqrt_ps ( n.v );
	return reinterpret_cast< const m128 * >( &calc )->f [ 0 ];
}
inline void math::fast_rsqrt ( float a, float * out ) {
	const auto xx = _mm_load_ss ( &a );
	auto xr = _mm_rsqrt_ss ( xx );
	auto xt = _mm_mul_ss ( xr, xr );
	xt = _mm_mul_ss ( xt, xx );
	xt = _mm_sub_ss ( _mm_set_ss ( 3.f ), xt );
	xt = _mm_mul_ss ( xt, _mm_set_ss ( 0.5f ) );
	xr = _mm_mul_ss ( xr, xt );
	_mm_store_ss ( out, xr );
}

float math::fast_vec_normalize ( vec3_t & vec ) {
	const auto sqrlen = vec.length_sqr ( ) + 1.0e-10f;
	float invlen;
	fast_rsqrt ( sqrlen, &invlen );
	vec [ 0 ] *= invlen;
	vec [ 1 ] *= invlen;
	vec [ 2 ] *= invlen;
	return sqrlen * invlen;
}
void math::fast_sqrt ( float * __restrict p_out, float * __restrict p_in ) {
	_mm_store_ss ( p_out, _mm_sqrt_ss ( _mm_load_ss ( p_in ) ) );
	// compiles to movss, sqrtss, movss
}


void math::angle_vectors ( vec3_t & angles, vec3_t * forward, vec3_t * right, vec3_t * up ) {
	float sp, sy, sr, cp, cy, cr;

	sin_cos ( DEG2RAD ( angles [ 0 ] ), &sp, &cp );
	sin_cos ( DEG2RAD ( angles [ 1 ] ), &sy, &cy );
	sin_cos ( DEG2RAD ( angles [ 2 ] ), &sr, &cr );

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

void math::angle_vectors ( vec3_t & angles, vec3_t & forward ) {
	float sp, sy, cp, cy;

	sin_cos ( DEG2RAD ( angles [ 1 ] ), &sy, &cy );
	sin_cos ( DEG2RAD ( angles [ 0 ] ), &sp, &cp );

	forward [ 0 ] = cp * cy;
	forward [ 1 ] = cp * sy;
	forward [ 2 ] = -sp;
}
void math::vector_ma ( const vec3_t & start, float scale, const vec3_t & direction, vec3_t & dest ) {
	dest [ 0 ] = start [ 0 ] + direction [ 0 ] * scale;
	dest [ 1 ] = start [ 1 ] + direction [ 1 ] * scale;
	dest [ 2 ] = start [ 2 ] + direction [ 2 ] * scale;
}
vec3_t math::vector_add ( const vec3_t a, const vec3_t  b ) {
	return vec3_t ( a [ 0 ] + b [ 0 ],
		a [ 1 ] + b [ 1 ],
		a [ 2 ] + b [ 2 ] );
}

vec3_t math::vector_subtract ( vec3_t & a, vec3_t & b ) {
	return vec3_t ( a [ 0 ] - b [ 0 ],
		a [ 1 ] - b [ 1 ],
		a [ 2 ] - b [ 2 ] );
}

vec3_t math::vector_multiply ( vec3_t & a, vec3_t & b ) {
	return vec3_t ( a [ 0 ] * b [ 0 ],
		a [ 1 ] * b [ 1 ],
		a [ 2 ] * b [ 2 ] );
}

vec3_t math::vector_divide ( vec3_t & a, vec3_t & b ) {
	return vec3_t ( a [ 0 ] / b [ 0 ],
		a [ 1 ] / b [ 1 ],
		a [ 2 ] / b [ 2 ] );
}

bool math::screen_transform ( const vec3_t & point, vec3_t & screen ) {
	static std::uint32_t pmatrix = *( std::uint32_t * ) ( ( std::uint32_t ) utilities::pattern_scan ( "client.dll", "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9" ) + 3 ) + 176;
	
	auto & matrix = *( view_matrix3x4_t * ) pmatrix;

	float w = matrix [ 3 ][ 0 ] * point [ 0 ] + matrix [ 3 ][ 1 ] * point [ 1 ] + matrix [ 3 ][ 2 ] * point [ 2 ] + matrix [ 3 ][ 3 ];
	screen [ 0 ] = matrix [ 0 ][ 0 ] * point [ 0 ] + matrix [ 0 ][ 1 ] * point [ 1 ] + matrix [ 0 ][ 2 ] * point [ 2 ] + matrix [ 0 ][ 3 ];
	screen [ 1 ] = matrix [ 1 ][ 0 ] * point [ 0 ] + matrix [ 1 ][ 1 ] * point [ 1 ] + matrix [ 1 ][ 2 ] * point [ 2 ] + matrix [ 1 ][ 3 ];
	screen [ 2 ] = 0.0f;

	int inverse_width = static_cast< int >( ( w < 0.001f ) ? -1.0f / w :
		1.0f / w );

	screen [ 0 ] *= inverse_width;
	screen [ 1 ] *= inverse_width;
	return ( w < 0.001f );
}

void math::normalize_num ( vec3_t & vIn, vec3_t & vOut ) {
	float flLen = vIn.length ( );
	if ( flLen == 0 ) {
		vOut.init ( 0, 0, 1 );
		return;
	}
	flLen = 1 / flLen;
	vOut.init ( vIn [ 0 ] * flLen, vIn [ 1 ] * flLen, vIn [ 2 ] * flLen );
}
float math::fl_angle_mod ( float fl_angle ) {
	return( ( 360.0f / 65536.0f ) * ( ( int32_t ) ( fl_angle * ( 65536.0f / 360.0f ) ) & 65535 ) );
}
float math::angle_diff ( float destAngle, float srcAngle ) {
	float delta = 0.f;

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
}
float math::fl_approach_angle ( float fl_target, float fl_value, float fl_speed ) {
	float fl_adjusted_speed = fl_speed;
	if ( fl_adjusted_speed < 0.0f )
		fl_adjusted_speed *= -1.0f;


	float fl_angle_mod_target = fl_angle_mod ( fl_target );
	float fl_angle_mod_value = fl_angle_mod ( fl_value );

	float fl_delta = ( fl_angle_mod_target - fl_angle_mod_value );
	if ( fl_delta >= -180.0f ) {
		if ( fl_delta >= 180.0f )
			fl_delta -= 360.0f;
	}
	else {
		if ( fl_delta <= -180.0f )
			fl_delta += 360.0f;
	}

	float fl_return;

	if ( fl_delta <= fl_adjusted_speed ) {
		if ( ( fl_adjusted_speed * -1.0f ) <= fl_delta )
			fl_return = fl_angle_mod_target;
		else
			fl_return = ( fl_angle_mod_value - fl_adjusted_speed );
	}
	else
		fl_return = ( fl_angle_mod_value + fl_adjusted_speed );

	return fl_return;
}
float math::normalize_yaw ( float f ) {
	if ( std::isnan ( f ) || std::isinf ( f ) )
		f = 0;

	if ( f > 9999999 )
		f = 0;

	if ( f < -9999999 )
		f = 0;

	while ( f < -180.0f ) f += 360.0f;
	while ( f > 180.0f ) f -= 360.0f;

	return f;
}
float math::normalize_degree ( float f ) {
	if ( std::isnan ( f ) || std::isinf ( f ) )
		f = 0;

	if ( f > 9999999 )
		f = 0;

	if ( f < -9999999 )
		f = 0;

	while ( f < 0.f ) f += 360.f;
	while ( f > 360.0f ) f -= 360.f;

	return f;
}
/*	auto VectorTransform_Wrapper = [ ] ( const vec3_t & in1, const matrix3x4_t & in2, vec3_t & out ) {
			auto VectorTransform = [ ] ( const float * in1, const matrix3x4_t & in2, float * out ) {
				auto DotProducts = [ ] ( const float * v1, const float * v2 ) {
					return v1 [ 0 ] * v2 [ 0 ] + v1 [ 1 ] * v2 [ 1 ] + v1 [ 2 ] * v2 [ 2 ];
				};
				out [ 0 ] = DotProducts ( in1, in2 [ 0 ] ) + in2 [ 0 ][ 3 ];
				out [ 1 ] = DotProducts ( in1, in2 [ 1 ] ) + in2 [ 1 ][ 3 ];
				out [ 2 ] = DotProducts ( in1, in2 [ 2 ] ) + in2 [ 2 ][ 3 ];
			};
			VectorTransform ( &in1[0], in2, &out[0] );
		};*/
vec3_t math::vector_transform ( const vec3_t & in1, const matrix3x4_t & in2 ) {
	auto out = vec3_t ( in1.dot_product ( vec3_t ( in2 [ 0 ][ 0 ], in2 [ 0 ][ 1 ], in2 [ 0 ][ 2 ] ) ) + in2 [ 0 ][ 3 ], in1.dot_product ( vec3_t ( in2 [ 1 ][ 0 ], in2 [ 1 ][ 1 ], in2 [ 1 ][ 2 ] ) ) + in2 [ 1 ][ 3 ], in1.dot_product ( vec3_t ( in2 [ 2 ][ 0 ], in2 [ 2 ][ 1 ], in2 [ 2 ][ 2 ] ) ) + in2 [ 2 ][ 3 ] );
	return out;
}
