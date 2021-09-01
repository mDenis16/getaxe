#pragma once
#include <cstdint>
#include <string>
#include "../../source-sdk/math/vector3d.hpp"

namespace utilities {
	template<typename FuncType>
	__forceinline static FuncType call_virtual( void* ppClass, int index ) {
		int* pVTable = *( int** ) ppClass;
		int dwAddress = pVTable [ index ];
		return ( FuncType ) ( dwAddress );
	}
	template <typename T>
	constexpr auto relativeToAbsolute ( uintptr_t address ) noexcept;
	template < typename T > static T get_virtual_function( void* base, const std::uint16_t index ) {
		return ( *reinterpret_cast< T** >( base ) ) [ index ];
	}
	inline unsigned int get_vfunc(void* pp_class, unsigned int index) {
		return (unsigned int)(*(int**)pp_class)[index];
	}
	std::uint8_t* pattern_scan( const char* module_name, const char* signature ) noexcept;


	std::string unicode_to_ascii( const std::wstring& unicode );
	std::uintptr_t code_style_pattern ( const char * moduleName, std::string_view pattern, bool reportNotFound = true ) noexcept;

	void* grab_interface( const char* module_name, const char* interf, bool bruteforce );

	float get_distance( vec3_t src, vec3_t dest );
#define in_range( x, a, b )	\
	( x >= a && x <= b ) 

#define get_bits( x ) \
	( in_range( x, '0', '9' ) ? ( x - '0' ) : ( ( x & ( ~0x20 ) ) - 'A' + 0xA ) )

#define get_byte( x ) \
	( get_bits( x[ 0 ] ) << 4 | get_bits( x[ 1 ] ) )

#define virtual_fn( function_name, index, type, ... ) \
	auto function_name { \
		return utilities::get_virtual_function< type >( this, index )( this, __VA_ARGS__ ); \
	};
}