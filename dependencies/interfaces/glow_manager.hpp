#pragma once
#include "../../source-sdk/math/vector3d.hpp"

class glow_object_definition_t {
public:
	void set(const float& r, const float & g, const float & b, const float & a, const float& _bloom_amount, const bool& _render_when_occluded, const bool& _render_when_unoccluded ) {
		m_vGlowColor = vec3_t(r, g, b);
		m_flGlowAlpha = a;
		m_bRenderWhenOccluded = _render_when_occluded;
		m_bRenderWhenUnoccluded = _render_when_unoccluded;
		
	}
	bool unused() {
		return m_nNextFreeSlot != -2;
	}

	int m_nNextFreeSlot;
	void* m_pEntity;
	vec3_t m_vGlowColor;
	float m_flGlowAlpha;
	bool m_bGlowAlphaCappedByRenderAlpha;
	float m_flGlowAlphaFunctionOfMaxVelocity;
	float m_flGlowAlphaMax;
	float m_flGlowPulseOverdrive;
	bool m_bRenderWhenOccluded;
	bool m_bRenderWhenUnoccluded;
	bool m_bFullBloomRender;
	int m_nFullBloomStencilTestValue;
	int m_nRenderStyle;
	int m_nSplitScreenSlot;
};

class glow_manager_t {
public:
	glow_object_definition_t* objects;
	char pad[8];
	int size;
};