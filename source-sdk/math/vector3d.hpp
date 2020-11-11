#pragma once
#include <limits>
#include <cmath>
#include <algorithm>

inline float bits_to_float( std::uint32_t i ) {
	union convertor_t
	{
		float f; unsigned long ul;
	} tmp;

	tmp.ul = i;
	return tmp.f;
}

constexpr double M_PI = 3.14159265358979323846;
constexpr float M_RADPI = 57.295779513082f;
constexpr float M_PI_F = static_cast< float >( M_PI );

constexpr float RAD2DEG( const float x ) {
	return ( float ) ( x ) * ( float ) ( 180.f / M_PI_F );
}
constexpr float DEG2RAD( const float x ) {
	return ( float ) ( x ) * ( float ) ( M_PI_F / 180.f );
}

constexpr std::uint32_t FLOAT32_NAN_BITS = 0x7FC00000;
const float FLOAT32_NAN = bits_to_float( FLOAT32_NAN_BITS );
#define VEC_T_NAN FLOAT32_NAN
#define ASSERT( _exp ) ( (void ) 0 )


class vec3_t
{
public:
	vec3_t( );
	vec3_t( float, float, float );
	bool operator==( const vec3_t& src ) const;
	~vec3_t( );

	float x, y, z;

	vec3_t& operator+=( const vec3_t& v ) {
		x += v.x; y += v.y; z += v.z; return *this;
	}
	vec3_t& operator-=( const vec3_t& v ) {
		x -= v.x; y -= v.y; z -= v.z; return *this;
	}
	vec3_t& operator*=( const float& v ) {
		x *= v; y *= v; z *= v; return *this;
	}

	vec3_t& operator+=( const float& v ) {
		x += v; y += v; z += v; return *this;
	}

	vec3_t operator+( const vec3_t& v ) {
		return vec3_t{ x + v.x, y + v.y, z + v.z };
	}
	vec3_t operator+( const vec3_t& v ) const {
		return vec3_t( x + v.x, y + v.y, z + v.z );
	}
	float& operator[]( int i )
	{
		return ( ( float* ) this ) [ i ];
	}

	float operator[]( int i ) const
	{
		return ( ( float* ) this ) [ i ];
	}
	vec3_t operator-( const vec3_t& v ) {
		return vec3_t{ x - v.x, y - v.y, z - v.z };
	}
	vec3_t operator*( float fl ) const {
		return vec3_t( x * fl, y * fl, z * fl );
	}

	vec3_t operator*( const vec3_t& v ) const {
		return vec3_t( x * v.x, y * v.y, z * v.z );
	}
	
	vec3_t& operator+=( float fl ) {
		x += fl;
		y += fl;
		z += fl;
		return *this;
	}

	vec3_t& operator/=( float fl ) {
		x /= fl;
		y /= fl;
		z /= fl;
		return *this;
	}

	vec3_t& operator-=( float fl ) {
		x -= fl;
		y -= fl;
		z -= fl;
		return *this;
	}

	auto operator-( const vec3_t& other ) const -> vec3_t {
		auto buf = *this;

		buf.x -= other.x;
		buf.y -= other.y;
		buf.z -= other.z;

		return buf;
	}

	auto operator/( float other ) const {
		vec3_t vec;
		vec.x = x / other;
		vec.y = y / other;
		vec.z = z / other;
		return vec;
	}

	

	inline float Length2D( ) const {
		return std::sqrt ( ( x * x ) + ( y * y ) );
	}
	float dot_product( const vec3_t& vec ) const {
		return ( x * vec.x + y * vec.y + z * vec.z );
	}
	void crossproduct( vec3_t v1, vec3_t v2, vec3_t cross_p ) const {
		cross_p.x = ( v1.y * v2.z ) - ( v1.z * v2.y ); //i
		cross_p.y = -( ( v1.x * v2.z ) - ( v1.z * v2.x ) ); //j
		cross_p.z = ( v1.x * v2.y ) - ( v1.y * v2.x ); //k
	}

	vec3_t cross( const vec3_t& b ) const {
		return vec3_t( y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x );
	}

	void init( float ix, float iy, float iz );
	bool is_zero( void );
	void clamp( );
	vec3_t clamped( );
	
	vec3_t normalized( ) const;
	vec3_t normalizedvec( ) const;
	void angle_normalize( );
	void angle_clamp( void );
	float distance_to( const vec3_t& other );
	void normalize( );
	float length( );
	float length_sqr( );
	float length_2d_sqr( void ) const;
	float dot( const vec3_t other ) {
		return ( x * other.x + y * other.y + z * other.z );
	}
	float normalize_in_place( );
	float dot( const float* other ) {
		const vec3_t& a = *this;

		return( a.x * other [ 0 ] + a.y * other [ 1 ] + a.z * other [ 2 ] );
	}
};

inline vec3_t operator*( float lhs, const vec3_t& rhs ) {
	return vec3_t( rhs.x * lhs, rhs.x * lhs, rhs.x * lhs );
}

struct matrix_t
{
	matrix_t( ) { }
	matrix_t(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23 ) {
		mat_val[ 0 ][ 0 ] = m00;	mat_val[ 0 ][ 1 ] = m01; mat_val[ 0 ][ 2 ] = m02; mat_val[ 0 ][ 3 ] = m03;
		mat_val[ 1 ][ 0 ] = m10;	mat_val[ 1 ][ 1 ] = m11; mat_val[ 1 ][ 2 ] = m12; mat_val[ 1 ][ 3 ] = m13;
		mat_val[ 2 ][ 0 ] = m20;	mat_val[ 2 ][ 1 ] = m21; mat_val[ 2 ][ 2 ] = m22; mat_val[ 2 ][ 3 ] = m23;
	}

	//-----------------------------------------------------------------------------
	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	//-----------------------------------------------------------------------------
	void init( const vec3_t& xAxis, const vec3_t& yAxis, const vec3_t& zAxis, const vec3_t& vecOrigin ) {
		mat_val[ 0 ][ 0 ] = xAxis.x; mat_val[ 0 ][ 1 ] = yAxis.x; mat_val[ 0 ][ 2 ] = zAxis.x; mat_val[ 0 ][ 3 ] = vecOrigin.x;
		mat_val[ 1 ][ 0 ] = xAxis.y; mat_val[ 1 ][ 1 ] = yAxis.y; mat_val[ 1 ][ 2 ] = zAxis.y; mat_val[ 1 ][ 3 ] = vecOrigin.y;
		mat_val[ 2 ][ 0 ] = xAxis.z; mat_val[ 2 ][ 1 ] = yAxis.z; mat_val[ 2 ][ 2 ] = zAxis.z; mat_val[ 2 ][ 3 ] = vecOrigin.z;
	}

	//-----------------------------------------------------------------------------
	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	//-----------------------------------------------------------------------------
	matrix_t( const vec3_t& xAxis, const vec3_t& yAxis, const vec3_t& zAxis, const vec3_t& vecOrigin ) {
		init( xAxis, yAxis, zAxis, vecOrigin );
	}

	
	inline void set_origin( vec3_t const& origin ) {
		for ( size_t i { }; i < 3; ++i ) {
			mat_val [ i ][ 3 ] = origin [ i ];
		}

	}
	inline vec3_t get_origin(  ) {
		return vec3_t( mat_val [ 0 ][ 3 ], mat_val [ 1 ][ 3 ], mat_val [ 2 ][ 3 ] );
	
	}
	

	inline void invalidate( void ) {
		for ( int i = 0; i < 3; i++ ) {
			for ( int j = 0; j < 4; j++ ) {
				mat_val[ i ][ j ] = VEC_T_NAN;
			}
		}
	}

	float* operator[]( int i ) { ASSERT( ( i >= 0 ) && ( i < 3 ) ); return mat_val[ i ]; }
	const float* operator[]( int i ) const { ASSERT( ( i >= 0 ) && ( i < 3 ) ); return mat_val[ i ]; }
	float* base( ) { return &mat_val[ 0 ][ 0 ]; }
	const float* base( ) const { return &mat_val[ 0 ][ 0 ]; }

	float mat_val[ 3 ][ 4 ];
};