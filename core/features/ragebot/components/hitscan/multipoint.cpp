#include "../../../features.hpp"
#include "../../../../helpers/helpers.h"

namespace aimbot {
#define scale(a, b)  hitbox->radius * (a / b )

	void multi_point ( target & entity, const int & hit_box, player_manager::lagcomp_t & record, multipoint & points, bool should_disable ) {



		studio_box_t * hitbox = entity.hitbox_set->hitbox ( hit_box );

		const auto mod = hitbox->radius != -1.f ? hitbox->radius : 0.f;
		auto maxs = math::vector_transform ( hitbox->maxs + mod, record.bone [ hitbox->bone ] );
		auto  mins = math::vector_transform ( hitbox->mins - mod, record.bone [ hitbox->bone ] );

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

		vec3_t top = vec3_t ( 0, 0, 0.99f );
		vec3_t bot = vec3_t ( 0, 0, -0.95f );


		if ( hit_box == hitbox_head ) {

			for ( auto i = 0; i < 5; ++i ) {
				points.points.push_back ( point );
			}
			vec3_t middle = ( top - bot );

			points.points [ 1 ].pos += vec3_t ( right.x * scale ( variables::ragebot::head_scale, 100.f ), right.y * scale ( variables::ragebot::head_scale, 100.f ), top.z * hitbox->radius * 0.95f );
			points.points [ 2 ].pos += vec3_t ( left.x * scale ( variables::ragebot::head_scale, 100.f ), left.y * scale ( variables::ragebot::head_scale, 100.f ), top.z * hitbox->radius * 0.95f );
			points.points [ 3 ].pos += vec3_t ( right.x * scale ( variables::ragebot::head_scale, 100.f ), right.y * scale ( variables::ragebot::head_scale, 100.f ), middle.z );
			points.points [ 4 ].pos += vec3_t ( left.x * scale ( variables::ragebot::head_scale, 100.f ), left.y * scale ( variables::ragebot::head_scale, 100.f ), middle.z );
			//points.points [ 5 ].pos = vec3_t ( points.center.x, points.center.y, points.center.z + top.z * hitbox->radius * (entity.player->is_moving() ? 0.85f : 0.99f ) );


		}
		else if ( hit_box >= hitbox_pelvis && hit_box <= hitbox_upper_chest ) {
			float body_scale = ( variables::ragebot::double_tap && tickbase_system::m_shift_data.m_needs_recharge == 0 ) ? 5.f : variables::ragebot::point_scale;
			for ( auto i = 0; i < 3; ++i ) {
				points.points.emplace_back ( point );
			}
			points.points [ 1 ].pos += right * ( hitbox->radius * ( variables::ragebot::point_scale / 100.f ) );
			points.points [ 2 ].pos += left * ( hitbox->radius * ( variables::ragebot::point_scale / 100.f ) );

		}
		else
			points.points.emplace_back ( point );

		if ( hit_box == hitbox_head ) { /*head upper chest intersection*/
			for ( size_t i = 0; i < points.points.size ( ); i++ )
				if ( does_point_intersect ( entity, points.points.at ( i ).pos, hitbox_upper_chest, record.bone ) )
					points.points.erase ( points.points.begin ( ) + i );
		}
		/*safepoint implementation after sleep*/
		for ( size_t i = 0; i < points.points.size ( ); i++ ) {

			if ( variables::ragebot::safe_point > 0 ) {
				if ( variables::ragebot::safe_point == 1 && hit_box == hitboxes::hitbox_head && !record.shoot ) {
					auto left_head = entity.player->get_hitbox_position ( hitbox_head, record.bone_left, entity.hitbox_set->hitbox ( hitbox_head ) );
					auto right_head = entity.player->get_hitbox_position ( hitbox_head, record.bone_right, entity.hitbox_set->hitbox ( hitbox_head ) );
					auto left_head_w2s = vec3_t ( ), right_head_w2s = vec3_t ( );
					auto center = vec3_t ( csgo::screen_width / 2.f, csgo::screen_height / 2.f, 0.f );
					if ( interfaces::debug_overlay->world_to_screen ( left_head, left_head_w2s ) && interfaces::debug_overlay->world_to_screen ( right_head, right_head_w2s ) ) {

						auto dist_to_left = math::calc_distance ( center, left_head_w2s, true );
						auto dist_to_right = math::calc_distance ( center, right_head_w2s, true );

						points.points.at ( i ).safe = does_point_intersect ( entity, points.points.at ( i ).pos, hit_box, dist_to_left <= dist_to_right ? record.bone_left : record.bone_right );


					}
				}
				else
					points.points.at ( i ).safe = does_point_intersect ( entity, points.points.at ( i ).pos, hit_box, record.bone_left ) && does_point_intersect ( entity, points.points.at ( i ).pos, hit_box, record.bone_right );
			}

		}

	}

}