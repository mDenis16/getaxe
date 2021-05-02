#pragma once
#include "../../source-sdk/math/vector3d.hpp"

class glow_object_definition_t {
public:
	void set(const float& r, const float & g, const float & b, const float & a, const float& _bloom_amount, const bool& _render_when_occluded, const bool& _render_when_unoccluded ) {
		color = vec3_t(r, g, b);
		alpha = a;
		render_when_occluded = _render_when_occluded;
		render_when_unoccluded = _render_when_unoccluded;
		bloom_amount = _bloom_amount;
	}
	bool unused() {
		return next_free_slot != -2;
	}

	void* entity;
	vec3_t color;
	float alpha;
	char unknown0[8];
	float bloom_amount;
	char unknown1[4];
	bool render_when_occluded;
	bool render_when_unoccluded;
	bool full_bloom_render;
	char unknown2[13];
	int next_free_slot;
};

class glow_manager_t {
public:
	glow_object_definition_t* objects;
	char pad[8];
	int size;
};