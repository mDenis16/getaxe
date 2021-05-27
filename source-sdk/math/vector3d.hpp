#pragma once
#include <limits>
#include <cmath>
#include <algorithm>
#include <assert.h>

inline float bits_to_float( std::uint32_t i ) {
	union convertor_t
	{
		float f = 0.f; unsigned long ul;
	};
	convertor_t tmp = {};
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
	void Rotate2D ( const float & f );
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
	float  LengthSquared ( ) const {
		return ( ( x * x ) + ( y * y ) + ( z * z ) );
	}


};

inline vec3_t operator*( float lhs, const vec3_t& rhs ) {
	return vec3_t( rhs.x * lhs, rhs.x * lhs, rhs.x * lhs );
}


inline int UtlMemory_CalcNewAllocationcount ( int nAllocationcount, int nGrowSize, int nNewSize, int nBytesItem ) {
	if ( nGrowSize ) {
		nAllocationcount = ( ( 1 + ( ( nNewSize - 1 ) / nGrowSize ) ) * nGrowSize );
	}
	else {
		if ( !nAllocationcount ) {
			// Compute an allocation which is at least as big as a cache line...
			nAllocationcount = ( 31 + nBytesItem ) / nBytesItem;
		}

		while ( nAllocationcount < nNewSize ) {
			nAllocationcount *= 2;
		}
	}

	return nAllocationcount;
}

template< class T, class I = int >
class c_utl_memory {
public:
	bool is_valid_index ( I i ) const {
		long x = i;
		return ( x >= 0 ) && ( x < m_alloc_count );
	}

	T & operator[]( I i );
	const T & operator[]( I i ) const;

	T * base ( ) {
		return m_memory;
	}

	int num_allocated ( ) const {
		return m_alloc_count;
	}

	void grow ( int num ) {
		//assert ( num > 0 );

		auto old_alloc_count = m_alloc_count;
		// Make sure we have at least numallocated + num allocations.
		// Use the grow rules specified for this memory (in m_grow_size)
		int alloc_requested = m_alloc_count + num;

		int new_alloc_count = UtlMemory_CalcNewAllocationcount ( m_alloc_count, m_grow_size, alloc_requested, sizeof ( T ) );

		// if m_alloc_requested wraps index type I, recalculate
		if ( ( int ) ( I ) new_alloc_count < alloc_requested ) {
			if ( ( int ) ( I ) new_alloc_count == 0 && ( int ) ( I ) ( new_alloc_count - 1 ) >= alloc_requested ) {
				--new_alloc_count; // deal w/ the common case of m_alloc_count == MAX_USHORT + 1
			}
			else {
				if ( ( int ) ( I ) alloc_requested != alloc_requested ) {
					// we've been asked to grow memory to a size s.t. the index type can't address the requested amount of memory
				//	assert ( 0 );
					return;
				}
				while ( ( int ) ( I ) new_alloc_count < alloc_requested ) {
					new_alloc_count = ( new_alloc_count + alloc_requested ) / 2;
				}
			}
		}

		m_alloc_count = new_alloc_count;

		if ( m_memory ) {
			auto ptr = new unsigned char [ m_alloc_count * sizeof ( T ) ];

			memcpy ( ptr, m_memory, old_alloc_count * sizeof ( T ) );
			m_memory = ( T * ) ptr;
		}
		else {
			m_memory = ( T * )new unsigned char [ m_alloc_count * sizeof ( T ) ];
		}
	}

protected:
	T * m_memory;
	int m_alloc_count;
	int m_grow_size;
};

template< class T, class I >
T & c_utl_memory< T, I >::operator[]( I i ) {
	assert ( is_valid_index ( i ) );
	return m_memory [ i ];
}

template< class T, class I >
const T & c_utl_memory< T, I >::operator[]( I i ) const {
	assert ( is_valid_index ( i ) );
	return m_memory [ i ];
}

template< class T >
void destruct ( T * memory ) {
	memory->~T ( );
}

template< class T >
T * construct ( T * memory ) {
	return ::new( memory ) T;
}

template< class T >
T * copy_construct ( T * memory, T const & src ) {
	return ::new( memory ) T ( src );
}

template< class T, class A = c_utl_memory< T > >
class c_utl_vector {
	typedef A c_allocator;

	typedef T * iterator;
	typedef const T * const_iterator;
public:
	T & operator[]( int i );
	const T & operator[]( int i ) const;

	T & element ( int i ) {
		return m_memory [ i ];
	}

	const T & element ( int i ) const {
		assert ( is_valid_index ( i ) );
		return m_memory [ i ];
	}

	T * base ( ) {
		return m_memory.base ( );
	}

	int count ( ) const {
		return m_size;
	}

	void removeAll ( ) {
		for ( int i = m_size; --i >= 0; )
			destruct ( &element ( i ) );

		m_size = 0;
	}

	bool is_valid_index ( int i ) const {
		return ( i >= 0 ) && ( i < m_size );
	}

	void shift_elements_right ( int elem, int num = 1 ) {
		assert ( is_valid_index ( elem ) || ( m_size == 0 ) || ( num == 0 ) );
		int num_to_move = m_size - elem - num;
		if ( ( num_to_move > 0 ) && ( num > 0 ) )
			memmove ( &element ( elem + num ), &element ( elem ), num_to_move * sizeof ( T ) );
	}

	void shift_elements_left ( int elem, int num = 1 ) {
		assert ( is_valid_index ( elem ) || ( m_size == 0 ) || ( num == 0 ) );
		int numToMove = m_size - elem - num;
		if ( ( numToMove > 0 ) && ( num > 0 ) )
			memmove ( &element ( elem ), &element ( elem + num ), numToMove * sizeof ( T ) );
	}

	void grow_vector ( int num = 1 ) {
		if ( m_size + num > m_memory.num_allocated ( ) ) {
			m_memory.grow ( m_size + num - m_memory.num_allocated ( ) );
		}

		m_size += num;
	}

	int insert_before ( int elem ) {
		// Can insert at the end
		assert ( ( elem == count ( ) ) || is_valid_index ( elem ) );

		grow_vector ( );
		shift_elements_right ( elem );
		construct ( &element ( elem ) );
		return elem;
	}

	int add_to_head ( ) {
		return insert_before ( 0 );
	}

	int add_to_tail ( ) {
		return insert_before ( m_size );
	}

	int insert_before ( int elem, const T & src ) {
		// Can't insert something that's in the list... reallocation may hose us
		assert ( ( base ( ) == NULL ) || ( &src < base ( ) ) || ( &src >= ( base ( ) + count ( ) ) ) );

		// Can insert at the end
		assert ( ( elem == count ( ) ) || is_valid_index ( elem ) );

		grow_vector ( );
		shift_elements_right ( elem );
		copy_construct ( &element ( elem ), src );
		return elem;
	}

	int add_to_tail ( const T & src ) {
		// Can't insert something that's in the list... reallocation may hose us
		return insert_before ( m_size, src );
	}

	int find ( const T & src ) const {
		for ( int i = 0; i < count ( ); ++i ) {
			if ( element ( i ) == src )
				return i;
		}
		return -1;
	}

	void remove ( int elem ) {
		destruct ( &element ( elem ) );
		shift_elements_left ( elem );
		--m_size;
	}

	bool find_and_remove ( const T & src ) {
		int elem = find ( src );
		if ( elem != -1 ) {
			remove ( elem );
			return true;
		}
		return false;
	}

	iterator begin ( ) {
		return base ( );
	}

	const_iterator begin ( ) const {
		return base ( );
	}

	iterator end ( ) {
		return base ( ) + count ( );
	}

	const_iterator end ( ) const {
		return base ( ) + count ( );
	}

protected:
	c_allocator m_memory;
	int m_size;
	T * m_elements;
};

template< typename T, class A >
T & c_utl_vector< T, A >::operator[]( int i ) {
	//assert ( i < m_size );
	return m_memory [ i ];
}

template< typename T, class A >
const T & c_utl_vector< T, A >::operator[]( int i ) const {
	//assert ( i < m_size );
	return m_memory [ i ];
}

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

class CBoneAccessor {
public:
	matrix3x4_t * GetBoneArrayForWrite ( ) {
		return m_pBones;
	}

	void SetBoneArrayForWrite ( matrix3x4_t * bone_array ) {
		m_pBones = bone_array;
	}

	int GetReadableBones ( ) {
		return m_ReadableBones;
	}

	void SetReadableBones ( int flags ) {
		m_ReadableBones = flags;
	}

	int GetWritableBones ( ) {
		return m_WritableBones;
	}

	void SetWritableBones ( int flags ) {
		m_WritableBones = flags;
	}

	alignas( 16 ) matrix3x4_t * m_pBones;
	int m_ReadableBones;
	int m_WritableBones;
};