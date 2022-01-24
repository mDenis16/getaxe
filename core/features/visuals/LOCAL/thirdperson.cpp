
#include "../../features.hpp"

void visuals::thirdperson ( ) {
	if ( !local_player::m_data.pointer )
		return;
	if ( !interfaces::engine->is_in_game ( ) )
		return;
	if ( !interfaces::engine->is_connected ( ) )
		return;




	static auto current_fraction = 0.0f;
	static vec3_t vecAngles;
	static vec3_t oldVecAngles;
	static vec3_t interpolated;

	interfaces::engine->get_view_angles ( vecAngles );
	if ( oldVecAngles.is_zero ( ) )
		oldVecAngles = vecAngles;
	float ideal_distance = config.local_visual.thirdperson_distance;

	interfaces::input->m_camera_in_thirdperson = config.local_visual.thirdperson;

	if ( interfaces::input->m_camera_in_thirdperson && local_player::m_data.pointer->health ( ) > 0 ) {

		vec3_t inverse_angles = vecAngles;

		inverse_angles.x *= -1.f, inverse_angles.y += 180.f;

		vec3_t direction = math::angle_vector ( inverse_angles );

		trace_world_only filter;
		trace_t trace;
		ray_t ray;


		ray.initialize ( local_player::m_data.pointer->abs_origin ( ) + local_player::m_data.pointer->view_offset ( ), ( local_player::m_data.pointer->abs_origin ( ) + local_player::m_data.pointer->view_offset ( ) ) + ( direction * ( ideal_distance + 25.f ) ) );
		interfaces::trace_ray->trace_ray ( ray, MASK_SHOT_HULL, &filter, &trace );


		if ( current_fraction > trace.flFraction )
			current_fraction = trace.flFraction;
		else if ( current_fraction > 0.9999f )
			current_fraction = 1.0f;

		current_fraction = math::interpolate ( current_fraction, trace.flFraction, interfaces::globals->frame_time /  0.3f );
		
		if ( config.local_visual.interpolate_angles ) {
			math::interpolate_angles ( oldVecAngles, vecAngles, interpolated, interfaces::globals->frame_time * config.local_visual.interpolation_amount );
		}
		else {
			interpolated = vec3_t ( vecAngles.x, vecAngles.y, 0 );
		}
		ideal_distance *= current_fraction;

		interfaces::input->m_camera_offset = vec3_t ( interpolated.x, interpolated.y, ideal_distance );
		oldVecAngles = interpolated;
	}
	else {
		current_fraction = 0.f;
		interfaces::input->m_camera_offset = vec3_t ( vecAngles.x, vecAngles.y, 0 );
	}
}