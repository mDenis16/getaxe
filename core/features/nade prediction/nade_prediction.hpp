#pragma once
#include "./../features.hpp"

class c_nadepoint
{
public:
	c_nadepoint( ) {
		m_valid = false;
	}

	c_nadepoint( vec3_t start, vec3_t end, bool plane, bool valid, vec3_t normal, bool detonate ) {
		m_start = start;
		m_end = end;
		m_plane = plane;
		m_valid = valid;
		m_normal = normal;
		m_detonate = detonate;
	}

	vec3_t m_start, m_end, m_normal;
	vec3_t m_start_2, m_end_2, m_normal_2;
	bool m_valid, m_plane, m_detonate;
};

enum nade_throw_act
{
	ACT_NONE,
	ACT_THROW,
	ACT_LOB,
	ACT_DROP
};

class c_nade_prediction
{
	std::array< c_nadepoint, 500 >	_points{ };
	bool		 _predicted = false;
	bool         _w2s = false;
	void predict( c_usercmd* user_cmd );
	bool detonated( weapon_t* weapon, float time, trace_t& trace );
public:
	void trace( c_usercmd* user_cmd );
	void paint_traverse ( );
	void draw( );
};

extern c_nade_prediction nade_pred;