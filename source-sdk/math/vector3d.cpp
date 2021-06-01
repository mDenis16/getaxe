#include "vector3d.hpp"
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <random>

vec3_t::vec3_t( void ) {
	x = y = z = 0.0f;
}

vec3_t::vec3_t( float fx, float fy, float fz ) {
	x = fx;
	y = fy;
	z = fz;
}
bool vec3_t::operator==( const vec3_t& src ) const {
	return ( src.x == x ) && ( src.y == y ) && ( src.z == z );
}
void shitsin_cos ( float r, float * s, float * c ) {
	*s = sin ( r );
	*c = cos ( r );
}
vec3_t::~vec3_t( void ) {
};
 void vec3_t::Rotate2D ( const float & f ) {
	float _x, _y;

	float s, c;

	shitsin_cos ( DEG2RAD ( f ), &s, &c );

	_x = x;
	_y = y;

	x = ( _x * c ) - ( _y * s );
	y = ( _x * s ) + ( _y * c );
}

void vec3_t::init( float ix, float iy, float iz ) {
	x = ix; y = iy; z = iz;
}
bool vec3_t::is_zero( void ) {

	return vec3_t( x, y, z ) == vec3_t( 0, 0, 0 );
}
void vec3_t::clamp( void ) {
	x = std::clamp( x, -89.0f, 89.0f );
	y = std::clamp( std::remainder( y, 360.0f ), -180.0f, 180.0f );
	z = std::clamp( z, -50.0f, 50.0f );
}

vec3_t vec3_t::clamped( ) {
	vec3_t clamped = *this;
	clamped.clamp( );
	return clamped;
}

float vec3_t::distance_to( const vec3_t& other ) {
	vec3_t delta;
	delta.x = x - other.x;
	delta.y = y - other.y;
	delta.z = z - other.z;

	return delta.length( );
}


void vec3_t::normalize( ) {

	*this = normalized( );
}
float vec3_t::normalize_in_place( )
{
	vec3_t vecOut = *this;

	float flLength = vecOut.length( );
	float flRadius = 1.0f / ( flLength + std::numeric_limits<float>::epsilon( ) );

	vecOut.x *= flRadius;
	vecOut.y *= flRadius;
	vecOut.z *= flRadius;

	return flLength;
}
vec3_t vec3_t::normalized( ) const {
	vec3_t res = *this;
	float l = res.length( );

	if ( l != 0.0f )
		res /= l;
	else
		res.x = res.y = res.z = 0.0f;

	return res;
}
vec3_t vec3_t::normalizedvec( ) const {
	vec3_t res = *this;
	float l = res.length( );

	if ( l != 0.0f )
		res /= l;
	else
		res.x = res.y = res.z = 0.0f;

	return res;
}
void vec3_t::angle_normalize( ) {

	if ( std::isinf ( y ) ) {
		throw std::runtime_error ( "Error: vec3_t::angle_normalize, Message: y is inf" );
	}
	if ( std::isinf ( x ) ) {
		throw std::runtime_error ( "Error: vec3_t::angle_normalize, Message: y is inf" );
	}
	if ( x > 89.0f )
		x = 89.0f;

	if ( x < -89.0f )
		x = -89.0f;

	while ( y > 180 )
		y -= 360;

	while ( y < -180 )
		y += 360;

	z = 0.f;
}
void vec3_t::angle_clamp( void ) {
	x = std::clamp( x, -89.0f, 89.0f );
	y = std::clamp( y, -180.0f, 180.0f );
	z = std::clamp( z, -50.0f, 50.0f );
}
float vec3_t::length( void ) {
	return std::sqrt( length_sqr( ) );
}

float vec3_t::length_sqr( void ) {
	auto sqr = [ ]( float n ) {
		return static_cast< float >( n * n );
	};

	return ( sqr( x ) + sqr( y ) + sqr( z ) );
}

float vec3_t::length_2d_sqr( void ) const {
	return ( x * x + y * y );
}

