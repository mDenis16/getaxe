
#include "../../features.hpp"
#include "../../../helpers/helpers.h"
#include <optional>

namespace aimbot {



	struct seed {
		float a, b, c, d;
	};

	std::optional<vec3_t> get_intersect_point ( vec3_t start, vec3_t end, vec3_t mins, vec3_t maxs, float radius ) {
		auto sphereRayIntersection = [ start, end, radius ] ( auto && center ) -> std::optional<vec3_t> {
			auto direction = ( end - start ).normalizedvec ( );

			auto q = vec3_t ( center - start );
			auto v = q.dot ( direction );
			auto d = radius * radius - ( q.length_sqr ( ) - v * v );

			if ( d < FLT_EPSILON )
				return {};

			return start + direction * ( v - std::sqrt ( d ) );
		};

		auto delta = ( maxs - mins ).normalizedvec ( );
		for ( size_t i {}; i < std::floor ( mins.distance_to ( maxs ) ); ++i ) {
			if ( auto intersection = sphereRayIntersection ( mins + delta * float ( i ) ); intersection )
				return intersection;
		}

		if ( auto intersection = sphereRayIntersection ( maxs ); intersection )
			return intersection;

		return {};
	}

	bool hitchance ( target & entity ) {
		auto angle = math::calc_angle ( engine_prediction::unpredicted_eye, best_target.aimbot.best_point.center );
		

		constexpr int   SEED_MAX = 255;
		constexpr float HITCHANCE_MAX = 100.f;

		auto local_player = csgo::local_player;
		float hitchance = variables::ragebot::hitchance;

		if ( !local_player ) return false;
		int traces_hit = 0;
		int needed_hits = static_cast< int >( SEED_MAX * ( hitchance / 100.f ) );
		vec3_t forward, right, up;
		const vec3_t eye_position = engine_prediction::unpredicted_eye;
		math::angle_vectors ( angle, &forward, &right, &up );

		auto weapon = local_player->active_weapon ( );
		if ( !weapon )
			return false;

		auto backupvel = csgo::local_player->velocity ( );
		auto backupabsvel = csgo::local_player->get_abs_velocity ( );
		csgo::local_player->get_abs_velocity ( ) = csgo::local_player->velocity ( ) = engine_prediction::unpredicted_velocity;
		weapon->update_accuracy_penalty ( );
		csgo::local_player->velocity ( ) = backupvel;
		csgo::local_player->get_abs_velocity ( ) = backupabsvel;
		float weapon_spread = weapon->get_spread ( );
		float weapon_cone = weapon->inaccuracy ( );


		auto VectorTransform_Wrapper = [ ] ( const vec3_t & in1, const matrix_t & in2, vec3_t & out ) {
			auto VectorTransform = [ ] ( const float * in1, const matrix_t & in2, float * out ) {
				auto DotProducts = [ ] ( const float * v1, const float * v2 ) {
					return v1 [ 0 ] * v2 [ 0 ] + v1 [ 1 ] * v2 [ 1 ] + v1 [ 2 ] * v2 [ 2 ];
				};
				out [ 0 ] = DotProducts ( in1, in2 [ 0 ] ) + in2 [ 0 ][ 3 ];
				out [ 1 ] = DotProducts ( in1, in2 [ 1 ] ) + in2 [ 1 ][ 3 ];
				out [ 2 ] = DotProducts ( in1, in2 [ 2 ] ) + in2 [ 2 ][ 3 ];
			};
			VectorTransform ( &in1.x, in2, &out.x );
		};

		studio_box_t * _hitbox = entity.hitbox_set->hitbox ( entity.aimbot.best_point.hitbox );
		vec3_t mins = vec3_t ( ), maxs = vec3_t ( );
		float radius = _hitbox->radius != -1.f ? _hitbox->radius : 0.f;

		VectorTransform_Wrapper ( vec3_t ( _hitbox->maxs.x , _hitbox->maxs.y , _hitbox->maxs.z  ), entity.aimbot.record.bone [ _hitbox->bone ], maxs );
		VectorTransform_Wrapper ( vec3_t ( _hitbox->mins.x , _hitbox->mins.y , _hitbox->mins.z  ), entity.aimbot.record.bone [ _hitbox->bone ], mins );
		static std::vector<seed> preseed;
		if ( preseed.size ( ) == 0 ) {
			for ( int i { }; i <= SEED_MAX; ++i )
				preseed.push_back ( { math::random_float ( 0.f, 1.f ), math::random_float ( 0.f, 2.f * M_PI ),math::random_float ( 0.f, 1.f ),math::random_float ( 0.f, 2.f * M_PI ) } );

		}

		const auto get_bullet_location = [ & ] ( int seed ) {


			float a = preseed.at ( seed ).a;
			float b = preseed.at ( seed ).b;
			float c = preseed.at ( seed ).c;
			float d = preseed.at ( seed ).d;

			const float generated_spread = a * weapon_spread;
			const float generated_cone = c * weapon_cone;

			const vec3_t spread = vec3_t (
				std::cos ( b ) * generated_spread + std::cos ( d ) * generated_cone,
				std::sin ( b ) * generated_spread + std::sin ( d ) * generated_cone,
				0.f
			);

			return vec3_t ( forward + right * -spread.x + up * -spread.y ).normalized ( );
		};

		for ( int i { }; i <= SEED_MAX; ++i ) {
			vec3_t spread_angle;
			vec3_t bullet_end;
			auto a = get_bullet_location ( i );
			math::VectorAnglesAwall ( a, spread_angle );
			bullet_end = math::angle_vector ( angle - ( spread_angle - angle ) );

			vec3_t end_point = eye_position + bullet_end * weapon->get_weapon_data ( )->flRange;
			m_visual_debug.from = eye_position;
			m_visual_debug.end = end_point;
			if ( auto intersection = get_intersect_point ( eye_position, end_point, mins, maxs, _hitbox->radius ); intersection )
				++traces_hit;


			if ( traces_hit >= needed_hits )
				return true;

			if ( ( SEED_MAX - i + traces_hit ) < needed_hits )
				return false;
		}

		return false;
	}
}
