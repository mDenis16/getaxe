#pragma once

class collideable_t {
public:
private:
	virtual void vtpad_0( void ) = 0;

public:
	virtual vec3_t& mins( void ) = 0;
	virtual vec3_t& maxs( void ) = 0;
};