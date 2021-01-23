#include "../../../features.hpp"
#include "../../../../helpers/helpers.h"

namespace aimbot {
#define scale(a, b)  hitbox->radius * (a / b )

	void multi_point ( target & entity, const int & hit_box, player_manager::lagcomp_t & record, multipoint & points, bool should_disable ) {

		auto model = entity.player->model ( );
		if ( !model )
			return;

		auto studio_model = interfaces::model_info->get_studio_model ( model );
		if ( !studio_model )
			return;

		auto hitbox_set = studio_model->hitbox_set ( 0 );
		if ( !hitbox_set )
			return;

		studio_box_t * hitbox = hitbox_set->hitbox ( hit_box );

		const auto mod = hitbox->radius != -1.f ? hitbox->radius : 0.f;
		auto maxs = math::vector_transform ( hitbox->maxs + mod, record.bone_aim [ hitbox->bone ] );
		auto  mins = math::vector_transform ( hitbox->mins - mod, record.bone_aim [ hitbox->bone ] );

		points.center = ( mins + maxs ) * 0.5f;

		point point;
		point.safe = true;
		point.pos = points.center;
		points.points.push_back ( point );

		if ( should_disable && hit_box != hitbox_head )
			return;

		vec3_t current_angles = math::calc_angle ( points.center, engine_prediction::unpredicted_eye );

		vec3_t forward = math::angle_vector ( current_angles );

		vec3_t right = forward.cross ( vec3_t ( 0, 0, 0.95f ) );
		vec3_t left = vec3_t ( -right.x, -right.y, right.z );

		vec3_t top = vec3_t ( 0, 0, 0.95f );
		vec3_t bot = vec3_t ( 0, 0, -0.95f );


		if ( hit_box == hitbox_head ) {

			for ( auto i = 0; i < 5; ++i ) {
				points.points.push_back ( point );
			}
			vec3_t middle = ( top - bot );
			points.points [ 0 ].pos.z += middle.z;

			points.points [ 1 ].pos += vec3_t ( right.x * scale ( config.ragebot_head_scale, 100.f ), right.y * scale ( config.ragebot_head_scale, 100.f ), top.z * hitbox->radius * 0.95f );
			points.points [2 ].pos += vec3_t ( left.x * scale ( config.ragebot_head_scale, 100.f ), left.y * scale ( config.ragebot_head_scale, 100.f ), top.z * hitbox->radius * 0.95f );
			points.points [ 3 ].pos += vec3_t ( right.x * scale ( config.ragebot_head_scale, 100.f ), right.y * scale ( config.ragebot_head_scale, 100.f ), middle.z );
			points.points [ 4 ].pos += vec3_t ( left.x * scale ( config.ragebot_head_scale, 100.f ), left.y * scale ( config.ragebot_head_scale, 100.f ), middle.z );
			points.points [ 5 ].pos = vec3_t ( points.center.x, points.center.y, points.center.z + top.z * hitbox->radius * (entity.player->is_moving() ? 0.65f : 0.99f ) );
			

		}
		else if ( hit_box >= hitbox_pelvis && hit_box <= hitbox_upper_chest ) {
		
			for ( auto i = 0; i < 3; ++i ) {
				points.points.emplace_back ( point );
			}
			points.points [ 1 ].pos += right * ( hitbox->radius * ( config.ragebot_point_scale / 100.f ) );
			points.points [ 2 ].pos += left * ( hitbox->radius * ( config.ragebot_point_scale / 100.f ) );

		}
		else
			points.points.emplace_back ( point );




		if ( hit_box == hitbox_head ) { /*head upper chest intersection*/
			for ( size_t i = 0; i < points.points.size ( ); i++ )
				if ( does_point_intersect ( entity, points.points.at ( i ).pos, hitbox_upper_chest, record.bone ) )
					points.points.erase ( points.points.begin ( ) + i );
		}
		/*safepoint implementation after i sleep*/
		for ( size_t i = 0; i < points.points.size ( ); i++ ) {
		

			if ( record.max_delta <= 32 || record.shoot ) {
				points.points.at ( i ).safe = does_point_intersect ( entity, points.points.at ( i ).pos, hit_box, record.bone_left ) && does_point_intersect ( entity, points.points.at ( i ).pos, hit_box, record.bone_right );
			}
			else if (config.ragebot_safe_point > 0) {
				
				if ( record.resolved && record.max_delta > 32 && record.max_delta <= 50 )
					points.points.at ( i ).safe = does_point_intersect ( entity, points.points.at ( i ).pos, hit_box, record.bone_resolved ) && does_point_intersect ( entity, points.points.at ( i ).pos, hit_box, record.bone_aim );
				else if ( record.resolved )
					points.points.at ( i ).safe = true;
			}
		     
			
		
		
		}
		aimbot::m_visual_debug.points = points.points;

	}

}