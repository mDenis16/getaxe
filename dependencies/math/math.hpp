#pragma once

namespace math {
	void correct_movement(vec3_t old_angles, c_usercmd* cmd, float old_forwardmove, float old_sidemove);
	int time_to_ticks(float time );
	float ticks_to_time( int tick );
	vec3_t get_matrix_position( const matrix_t& src );
	float clamp_yaw( float yaw );

	
	void vector_rotate( const vec3_t& in1, const vec3_t& in2, vec3_t& out );

	vec3_t lerp( const vec3_t& a, const vec3_t& b, const float t );
	void clamp( vec3_t& angles );
	void random_seed( int seed );
	void set_matrix_position( vec3_t pos, matrix_t& matrix );
	void angle_matrix( const vec3_t angles, matrix_t& matrix );
	float random_float( float min, float max );
	int random_int( int min, int max );
	vec3_t cubic_bezier( const vec3_t& a, const vec3_t& b, const vec3_t& c, const vec3_t& d, const float t );
	vec3_t calc_angle( vec3_t a, vec3_t b );
	void vector_vectors( const vec3_t& forward, vec3_t& right, vec3_t& up );

	void vector_vectors( const vec3_t& forward, vec3_t& right, vec3_t& up );
	void matrix_set_column( const vec3_t& in, int column, matrix_t& out );
	void angle_matrix( const vec3_t& angles, const vec3_t& position, matrix_t& matrix_out );
	void matrix_copy( const matrix_t& source, matrix_t& target );
	void matrix_multiply( matrix_t& in1, const matrix_t& in2 );
	float dot_product( const float* v1, const float* v2 );
	void vector_rotate( const float* in1, const matrix_t& in2, float* out );
	void vector_rotate( const vec3_t& in1, const matrix_t& in2, vec3_t& out );
	void matrix_get_column( const matrix_t& in, int column, vec3_t& out );

	void vector_rotate( const vec3_t& in1, const matrix3x4_t& in2, vec3_t& out );
	void vector_matrix( const vec3_t& forward, matrix_t& matrix );
	void smooth_angles( vec3_t view_angles, vec3_t aim_angles, vec3_t& out_angles, float smoothing_x, float smoothing_y );
	float get_fov( const vec3_t& va, const vec3_t& eyepos, const vec3_t& aimpos );
	float get_fov( vec3_t viewangle, vec3_t aim_angle );
	float get_estimate_server_time( c_usercmd* cmd );
	float get_fov_player( vec3_t ViewOffSet, vec3_t View, player_t* entity, int hitbox );


	void vector_substract( const vec3_t& a, const vec3_t& b, vec3_t& c );
	void smooth_angle( vec3_t src, vec3_t& dst, float factor );
	float calc_distance( const vec3_t src, const vec3_t dst, bool _2d );

	void sin_cos(float r, float* s, float* c);

	float distance_to_ray( const vec3_t& pos, const vec3_t& ray_start, const vec3_t& ray_end, float* along = 0, vec3_t* point_on_ray = 0 );
	
	vec3_t angle_vector(vec3_t angle);
	void VectorAnglesAwall( const vec3_t& vecForward, vec3_t& angView );
	void transform_vector(vec3_t&, matrix_t&, vec3_t&);
	vec3_t vector_transform( const vec3_t& in, const matrix_t& matrix );
	void vector_angles(vec3_t&, vec3_t&);
	void angle_vectors(vec3_t&, vec3_t*, vec3_t*, vec3_t*);
	vec3_t vector_add( const vec3_t a, const vec3_t b );
	vec3_t vector_add(vec3_t&, vec3_t&);
	vec3_t vector_subtract(vec3_t&, vec3_t&);
	vec3_t vector_multiply(vec3_t&, vec3_t&);
	void angle_vectors( vec3_t& angles, vec3_t& forward );
	vec3_t vector_divide(vec3_t&, vec3_t&);
	bool screen_transform(const vec3_t& point, vec3_t& screen);
	void normalize_num( vec3_t& vIn, vec3_t& vOut );
	float fl_angle_mod( float fl_angle );
	float angle_diff( float destAngle, float srcAngle );
	float fl_approach_angle( float fl_target, float fl_value, float fl_speed );
	float normalize_yaw( float f );
	bool world_to_screen(const vec3_t& origin, vec3_t& screen);

	template <typename t> t clamp_value( t value, t min, t max ) {
		if ( value > max ) {
			return max;
		}
		if ( value < min ) {
			return min;
		}
		return value;
	}
};