#pragma once
#include <cmath>
#include <xmmintrin.h>
#include <pmmintrin.h>

namespace math {

	static const float invtwopi = 0.1591549f;
	static const float twopi = 6.283185f;
	static const float threehalfpi = 4.7123889f;
	static const float pi = 3.141593f;
	static const float halfpi = 1.570796f;
	static const __m128 signmask = _mm_castsi128_ps ( _mm_set1_epi32 ( 0x80000000 ) );

	static const __declspec( align( 16 ) ) float null [ 4 ] = { 0.f, 0.f, 0.f, 0.f };
	static const __declspec( align( 16 ) ) float _pi2 [ 4 ] = { 1.5707963267948966192f, 1.5707963267948966192f, 1.5707963267948966192f, 1.5707963267948966192f };
	static const __declspec( align( 16 ) ) float _pi [ 4 ] = { 3.141592653589793238f, 3.141592653589793238f, 3.141592653589793238f, 3.141592653589793238f };

	typedef __declspec( align( 16 ) ) union {
		float f [ 4 ];
		__m128 v;
	} m128;

	__forceinline __m128 sqrt_ps ( const __m128 squared ) {
		return _mm_sqrt_ps ( squared );
	}

	__forceinline __m128 cos_52s_ps ( const __m128 x ) {
		const auto c1 = _mm_set1_ps ( 0.9999932946f );
		const auto c2 = _mm_set1_ps ( -0.4999124376f );
		const auto c3 = _mm_set1_ps ( 0.0414877472f );
		const auto c4 = _mm_set1_ps ( -0.0012712095f );
		const auto x2 = _mm_mul_ps ( x, x );
		return _mm_add_ps ( c1, _mm_mul_ps ( x2, _mm_add_ps ( c2, _mm_mul_ps ( x2, _mm_add_ps ( c3, _mm_mul_ps ( c4, x2 ) ) ) ) ) );
	}

	__forceinline __m128 cos_ps ( __m128 angle ) {
		angle = _mm_andnot_ps ( signmask, angle );
		angle = _mm_sub_ps ( angle, _mm_mul_ps ( _mm_cvtepi32_ps ( _mm_cvttps_epi32 ( _mm_mul_ps ( angle, _mm_set1_ps ( invtwopi ) ) ) ), _mm_set1_ps ( twopi ) ) );

		auto cosangle = angle;
		cosangle = _mm_xor_ps ( cosangle, _mm_and_ps ( _mm_cmpge_ps ( angle, _mm_set1_ps ( halfpi ) ), _mm_xor_ps ( cosangle, _mm_sub_ps ( _mm_set1_ps ( pi ), angle ) ) ) );
		cosangle = _mm_xor_ps ( cosangle, _mm_and_ps ( _mm_cmpge_ps ( angle, _mm_set1_ps ( pi ) ), signmask ) );
		cosangle = _mm_xor_ps ( cosangle, _mm_and_ps ( _mm_cmpge_ps ( angle, _mm_set1_ps ( threehalfpi ) ), _mm_xor_ps ( cosangle, _mm_sub_ps ( _mm_set1_ps ( twopi ), angle ) ) ) );

		auto result = cos_52s_ps ( cosangle );
		result = _mm_xor_ps ( result, _mm_and_ps ( _mm_and_ps ( _mm_cmpge_ps ( angle, _mm_set1_ps ( halfpi ) ), _mm_cmplt_ps ( angle, _mm_set1_ps ( threehalfpi ) ) ), signmask ) );
		return result;
	}

	__forceinline __m128 sin_ps ( const __m128 angle ) {
		return cos_ps ( _mm_sub_ps ( _mm_set1_ps ( halfpi ), angle ) );
	}

	__forceinline void sincos_ps ( __m128 angle, __m128 * sin, __m128 * cos ) {
		const auto anglesign = _mm_or_ps ( _mm_set1_ps ( 1.f ), _mm_and_ps ( signmask, angle ) );
		angle = _mm_andnot_ps ( signmask, angle );
		angle = _mm_sub_ps ( angle, _mm_mul_ps ( _mm_cvtepi32_ps ( _mm_cvttps_epi32 ( _mm_mul_ps ( angle, _mm_set1_ps ( invtwopi ) ) ) ), _mm_set1_ps ( twopi ) ) );

		auto cosangle = angle;
		cosangle = _mm_xor_ps ( cosangle, _mm_and_ps ( _mm_cmpge_ps ( angle, _mm_set1_ps ( halfpi ) ), _mm_xor_ps ( cosangle, _mm_sub_ps ( _mm_set1_ps ( pi ), angle ) ) ) );
		cosangle = _mm_xor_ps ( cosangle, _mm_and_ps ( _mm_cmpge_ps ( angle, _mm_set1_ps ( pi ) ), signmask ) );
		cosangle = _mm_xor_ps ( cosangle, _mm_and_ps ( _mm_cmpge_ps ( angle, _mm_set1_ps ( threehalfpi ) ), _mm_xor_ps ( cosangle, _mm_sub_ps ( _mm_set1_ps ( twopi ), angle ) ) ) );

		auto result = cos_52s_ps ( cosangle );
		result = _mm_xor_ps ( result, _mm_and_ps ( _mm_and_ps ( _mm_cmpge_ps ( angle, _mm_set1_ps ( halfpi ) ), _mm_cmplt_ps ( angle, _mm_set1_ps ( threehalfpi ) ) ), signmask ) );
		*cos = result;

		const auto sinmultiplier = _mm_mul_ps ( anglesign, _mm_or_ps ( _mm_set1_ps ( 1.f ), _mm_and_ps ( _mm_cmpgt_ps ( angle, _mm_set1_ps ( pi ) ), signmask ) ) );
		*sin = _mm_mul_ps ( sinmultiplier, sqrt_ps ( _mm_sub_ps ( _mm_set1_ps ( 1.f ), _mm_mul_ps ( result, result ) ) ) );
	}

	template <typename t>
	static t interpolate ( const t & t1, const t & t2, float progress ) {
		if ( t1 == t2 )
			return t1;

		return t2 * progress + t1 * ( 1.0f - progress );
	}

	int time_to_ticks ( float time );
	float ticks_to_time ( int tick );
	vec3_t get_matrix_position ( const matrix3x4_t & src );
	int ticks_to_stop ( vec3_t velocity );
	float clamp_yaw ( float yaw );


	void vector_rotate ( const vec3_t & in1, const vec3_t & in2, vec3_t & out );

	vec3_t lerp ( const vec3_t & a, const vec3_t & b, const float t );
	void clamp ( vec3_t & angles );
	void random_seed ( int seed );
	void set_matrix_position ( vec3_t pos, matrix3x4_t & matrix );
	float fast_sin ( float x );
	void angle_matrix ( const vec3_t angles, matrix3x4_t & matrix );
	float fast_cos ( const float x );

	float random_float ( float min, float max );
	int random_int ( int min, int max );
	vec3_t cubic_bezier ( const vec3_t & a, const vec3_t & b, const vec3_t & c, const vec3_t & d, const float t );
	vec3_t calc_angle ( vec3_t a, vec3_t b );
	void vector_vectors ( const vec3_t & forward, vec3_t & right, vec3_t & up );

	void vector_vectors ( const vec3_t & forward, vec3_t & right, vec3_t & up );
	void matrix_set_column ( const vec3_t & in, int column, matrix3x4_t & out );
	
	void angle_matrix ( const vec3_t & angles, const vec3_t & position, matrix3x4_t & matrix_out );

	void matrix_copy ( const matrix3x4_t & source, matrix3x4_t & target );

	void matrix_multiply ( matrix3x4_t & in1, const matrix3x4_t & in2 );
	float dot_product ( const float * v1, const float * v2 );
	void vector_rotate ( const float * in1, const matrix3x4_t & in2, float * out );
	void vector_rotate ( const vec3_t & in1, const matrix3x4_t & in2, vec3_t & out );
	void matrix_get_column ( const matrix3x4_t & in, int column, vec3_t & out );


	void vector_matrix ( const vec3_t & forward, matrix3x4_t & matrix );
	void smooth_angles ( vec3_t view_angles, vec3_t aim_angles, vec3_t & out_angles, float smoothing_x, float smoothing_y );
	float get_fov ( const vec3_t & va, const vec3_t & eyepos, const vec3_t & aimpos );
	float get_fov ( vec3_t viewangle, vec3_t aim_angle );
	float get_estimate_server_time ( c_usercmd * cmd );


	void vector_substract ( const vec3_t & a, const vec3_t & b, vec3_t & c );
	void smooth_angle ( vec3_t src, vec3_t & dst, float factor );
	float calc_distance ( const vec3_t src, const vec3_t dst, bool _2d );

	void sin_cos ( float r, float * s, float * c );

	float distance_to_ray ( const vec3_t & pos, const vec3_t & ray_start, const vec3_t & ray_end, float * along = 0, vec3_t * point_on_ray = 0 );

	vec3_t angle_vector ( vec3_t angle );
	void vector_angles ( vec3_t & forward, vec3_t & angles );
	void VectorAnglesAwall ( const vec3_t & vecForward, vec3_t & angView );
	void transform_vector ( vec3_t &, matrix3x4_t &, vec3_t & );

	float segment_to_segment ( const vec3_t s1, const vec3_t s2, const vec3_t k1, const vec3_t k2 );

	inline void fast_rsqrt ( float a, float * out );

	float fast_vec_normalize ( vec3_t & vec );

	 void fast_sqrt ( float * __restrict p_out, float * __restrict p_in );

	void angle_vectors ( vec3_t &, vec3_t *, vec3_t *, vec3_t * );
	vec3_t vector_add ( const vec3_t a, const vec3_t b );
	vec3_t vector_add ( vec3_t &, vec3_t & );
	vec3_t vector_subtract ( vec3_t &, vec3_t & );
	vec3_t vector_multiply ( vec3_t &, vec3_t & );
	void angle_vectors ( vec3_t & angles, vec3_t & forward );
	void vector_ma ( const vec3_t & start, float scale, const vec3_t & direction, vec3_t & dest );
	vec3_t vector_divide ( vec3_t &, vec3_t & );
	bool screen_transform ( const vec3_t & point, vec3_t & screen );
	void normalize_num ( vec3_t & vIn, vec3_t & vOut );
	float fl_angle_mod ( float fl_angle );
	float angle_diff ( float destAngle, float srcAngle );
	float fl_approach_angle ( float fl_target, float fl_value, float fl_speed );
	float normalize_yaw ( float f );
	vec3_t vector_transform ( const vec3_t & in1, const matrix3x4_t & in2 );
	bool world_to_screen ( const vec3_t & origin, vec3_t & screen );



	template <typename t> t clamp_value ( t value, t min, t max ) {
		if ( value > max ) {
			return max;
		}
		if ( value < min ) {
			return min;
		}
		return value;
	}
};