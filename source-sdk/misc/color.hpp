#pragma once
#include <cstdint>
#include <d3d9.h>
#include <cmath>

struct color {
	int a, r, g, b;
	color() = default;
	color(int r, int g, int b, int a = 255) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
	color(uint32_t color) {
		this->a = (color >> 24) & 0xff;
		this->r = (color >> 16) & 0xff;
		this->g = (color >> 8) & 0xff;
		this->b = (color & 0xff);
	}
	color from_uint(uint32_t uint) {
		return color(uint);
	}
	D3DCOLOR from_color(color col) {
		return D3DCOLOR_ARGB(col.a, col.r, col.g, col.b);
	}
    constexpr color& from_hsv( float h, float s, float v ) {
        float colOut[ 3 ]{ };
        if ( s == 0.0f ) {
            r = g = b = static_cast< int >( v * 255 );
            return *this;
        }

        h = std::fmodf( h, 1.0f ) / ( 60.0f / 360.0f );
        int   i = static_cast< int >( h );
        float f = h - static_cast< float >( i );
        float p = v * ( 1.0f - s );
        float q = v * ( 1.0f - s * f );
        float t = v * ( 1.0f - s * ( 1.0f - f ) );

        switch ( i ) {
        case 0:
            colOut[ 0 ] = v;
            colOut[ 1 ] = t;
            colOut[ 2 ] = p;
            break;
        case 1:
            colOut[ 0 ] = q;
            colOut[ 1 ] = v;
            colOut[ 2 ] = p;
            break;
        case 2:
            colOut[ 0 ] = p;
            colOut[ 1 ] = v;
            colOut[ 2 ] = t;
            break;
        case 3:
            colOut[ 0 ] = p;
            colOut[ 1 ] = q;
            colOut[ 2 ] = v;
            break;
        case 4:
            colOut[ 0 ] = t;
            colOut[ 1 ] = p;
            colOut[ 2 ] = v;
            break;
        case 5: default:
            colOut[ 0 ] = v;
            colOut[ 1 ] = p;
            colOut[ 2 ] = q;
            break;
        }

        r = static_cast< int >( colOut[ 0 ] * 255 );
        g = static_cast< int >( colOut[ 1 ] * 255 );
        b = static_cast< int >( colOut[ 2 ] * 255 );
        return *this;
    }

	static color black(int a = 255) { return { 0, 0, 0, a }; }
	static color white(int a = 255) { return { 255, 255, 255, a }; }
	static color red(int   a = 255) { return { 255, 0, 0, a }; }
	static color green(int a = 255) { return { 0, 255, 0, a }; }
	static color blue(int  a = 255) { return { 0, 0, 255, a }; }
};