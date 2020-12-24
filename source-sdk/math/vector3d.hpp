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


class quaternion_t {
public:
	float x, y, z, w;
};

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
	vec3_t operator+( const float &v ) {
		return vec3_t { x + v, y + v, z + v };
	}
	vec3_t operator-( const float & v ) {
		return vec3_t { x - v, y - v, z - v };
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

inline int UtlMemory_CalcNewAllocationCount ( int nAllocationCount, int nGrowSize, int nNewSize, int nBytesItem ) {
	if ( nGrowSize )
		nAllocationCount = ( ( 1 + ( ( nNewSize - 1 ) / nGrowSize ) ) * nGrowSize );
	else {
		if ( !nAllocationCount )
			nAllocationCount = ( 31 + nBytesItem ) / nBytesItem;

		while ( nAllocationCount < nNewSize )
			nAllocationCount *= 2;
	}

	return nAllocationCount;
}

template< class T, class I = int >
class CUtlMemory {
public:
	T & operator[]( I i ) {
		return m_pMemory [ i ];
	}

	T * Base ( ) {
		return m_pMemory;
	}

	int NumAllocated ( ) const {
		return m_nAllocationCount;
	}

	void Grow ( int num = 1 ) {
		if ( IsExternallyAllocated ( ) )
			return;

		int nAllocationRequested = m_nAllocationCount + num;
		int nNewAllocationCount = UtlMemory_CalcNewAllocationCount ( m_nAllocationCount, m_nGrowSize, nAllocationRequested, sizeof ( T ) );

		if ( ( int ) ( I ) nNewAllocationCount < nAllocationRequested ) {
			if ( ( int ) ( I ) nNewAllocationCount == 0 && ( int ) ( I ) ( nNewAllocationCount - 1 ) >= nAllocationRequested )
				--nNewAllocationCount;
			else {
				if ( ( int ) ( I ) nAllocationRequested != nAllocationRequested )
					return;

				while ( ( int ) ( I ) nNewAllocationCount < nAllocationRequested )
					nNewAllocationCount = ( nNewAllocationCount + nAllocationRequested ) / 2;
			}
		}

		m_nAllocationCount = nNewAllocationCount;

		if ( m_pMemory )
			m_pMemory = ( T * ) realloc ( m_pMemory, m_nAllocationCount * sizeof ( T ) );
		else
			m_pMemory = ( T * ) malloc ( m_nAllocationCount * sizeof ( T ) );
	}

	bool IsExternallyAllocated ( ) const {
		return m_nGrowSize < 0;
	}

protected:
	T * m_pMemory;
	int m_nAllocationCount;
	int m_nGrowSize;
};

template <class T>
inline T * Construct ( T * pMemory ) {
	return ::new( pMemory ) T;
}

template <class T>
inline void Destruct ( T * pMemory ) {
	pMemory->~T ( );
}

template< class T, class A = CUtlMemory<T> >
class CUtlVector {
	typedef A CAllocator;
public:
	CAllocator m_Memory;
	int m_Size;

	T & operator[]( int i ) {
		return m_Memory [ i ];
	}

	T & Element ( int i ) {
		return m_Memory [ i ];
	}

	T * Base ( ) {
		return m_Memory.Base ( );
	}

	int Count ( ) const {
		return m_Size;
	}

	void RemoveAll ( ) {
		for ( int i = m_Size; --i >= 0; )
			Destruct ( &Element ( i ) );

		m_Size = 0;
	}

	int AddToTail ( ) {
		return InsertBefore ( m_Size );
	}

	int InsertBefore ( int elem ) {
		GrowVector ( );
		ShiftElementsRight ( elem );
		Construct ( &Element ( elem ) );

		return elem;
	}

protected:
	void GrowVector ( int num = 1 ) {
		if ( m_Size + num > m_Memory.NumAllocated ( ) )
			m_Memory.Grow ( m_Size + num - m_Memory.NumAllocated ( ) );

		m_Size += num;
		ResetDbgInfo ( );
	}

	void ShiftElementsRight ( int elem, int num = 1 ) {
		int numToMove = m_Size - elem - num;
		if ( ( numToMove > 0 ) && ( num > 0 ) )
			memmove ( &Element ( elem + num ), &Element ( elem ), numToMove * sizeof ( T ) );
	}

	T * m_pElements;

	inline void ResetDbgInfo ( ) {
		m_pElements = Base ( );
	}
};

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

class matrix3x4_t {
public:
	__forceinline matrix3x4_t ( ) { }

	__forceinline matrix3x4_t ( float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23 ) {
		m_flMatVal [ 0 ][ 0 ] = m00;
		m_flMatVal [ 0 ][ 1 ] = m01;
		m_flMatVal [ 0 ][ 2 ] = m02;
		m_flMatVal [ 0 ][ 3 ] = m03;
		m_flMatVal [ 1 ][ 0 ] = m10;
		m_flMatVal [ 1 ][ 1 ] = m11;
		m_flMatVal [ 1 ][ 2 ] = m12;
		m_flMatVal [ 1 ][ 3 ] = m13;
		m_flMatVal [ 2 ][ 0 ] = m20;
		m_flMatVal [ 2 ][ 1 ] = m21;
		m_flMatVal [ 2 ][ 2 ] = m22;
		m_flMatVal [ 2 ][ 3 ] = m23;
	}

	__forceinline void Init ( const vec3_t & x, const vec3_t & y, const vec3_t & z, const vec3_t & origin ) {
		m_flMatVal [ 0 ][ 0 ] = x.x;
		m_flMatVal [ 0 ][ 1 ] = y.x;
		m_flMatVal [ 0 ][ 2 ] = z.x;
		m_flMatVal [ 0 ][ 3 ] = origin.x;
		m_flMatVal [ 1 ][ 0 ] = x.y;
		m_flMatVal [ 1 ][ 1 ] = y.y;
		m_flMatVal [ 1 ][ 2 ] = z.y;
		m_flMatVal [ 1 ][ 3 ] = origin.y;
		m_flMatVal [ 2 ][ 0 ] = x.z;
		m_flMatVal [ 2 ][ 1 ] = y.z;
		m_flMatVal [ 2 ][ 2 ] = z.z;
		m_flMatVal [ 2 ][ 3 ] = origin.z;
	}

	__forceinline matrix3x4_t ( const vec3_t & x, const vec3_t & y, const vec3_t & z, const vec3_t & origin ) {
		m_flMatVal [ 0 ][ 0 ] = x.x;
		m_flMatVal [ 0 ][ 1 ] = y.x;
		m_flMatVal [ 0 ][ 2 ] = z.x;
		m_flMatVal [ 0 ][ 3 ] = origin.x;
		m_flMatVal [ 1 ][ 0 ] = x.y;
		m_flMatVal [ 1 ][ 1 ] = y.y;
		m_flMatVal [ 1 ][ 2 ] = z.y;
		m_flMatVal [ 1 ][ 3 ] = origin.y;
		m_flMatVal [ 2 ][ 0 ] = x.z;
		m_flMatVal [ 2 ][ 1 ] = y.z;
		m_flMatVal [ 2 ][ 2 ] = z.z;
		m_flMatVal [ 2 ][ 3 ] = origin.z;
	}

	__forceinline void SetOrigin ( const vec3_t & p ) {
		m_flMatVal [ 0 ][ 3 ] = p.x;
		m_flMatVal [ 1 ][ 3 ] = p.y;
		m_flMatVal [ 2 ][ 3 ] = p.z;
	}

	__forceinline vec3_t GetOrigin ( ) {
		return { m_flMatVal [ 0 ][ 3 ], m_flMatVal [ 1 ][ 3 ], m_flMatVal [ 2 ][ 3 ] };
	}

	__forceinline float * operator[]( int i ) {
		return m_flMatVal [ i ];
	}

	__forceinline const float * operator[]( int i ) const {
		return m_flMatVal [ i ];
	}

	__forceinline float * Base ( ) {
		return &m_flMatVal [ 0 ][ 0 ];
	}

	__forceinline const float * Base ( ) const {
		return &m_flMatVal [ 0 ][ 0 ];
	}

public:
	float m_flMatVal [ 3 ][ 4 ];
};

class __declspec( align( 16 ) ) matrix3x4a_t : public matrix3x4_t {
public:
	__forceinline matrix3x4a_t & operator=( const matrix3x4_t & src ) {
		std::memcpy ( Base ( ), src.Base ( ), sizeof ( float ) * 3 * 4 );
		return *this;
	};
};

class BoneArray : public matrix3x4a_t {
public:
	bool get_bone ( vec3_t & out, int bone = 0 ) {
		if ( bone < 0 || bone >= 128 )
			return false;

		matrix3x4_t * bone_matrix = &this [ bone ];

		if ( !bone_matrix )
			return false;

		out = { bone_matrix->m_flMatVal [ 0 ][ 3 ], bone_matrix->m_flMatVal [ 1 ][ 3 ], bone_matrix->m_flMatVal [ 2 ][ 3 ] };

		return true;
	}
};
