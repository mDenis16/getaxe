
/*#include "../../features.hpp"

#include <optional>

namespace aimbot {

	std::vector <seed> seed_list;



	std::optional<vec3_t> get_intersect_point ( vec3_t start, vec3_t end, vec3_t mins, vec3_t maxs, float radius ) {
		auto sphere_intesection = [ start, end, radius ] ( auto && center ) -> std::optional<vec3_t> {
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
			if ( auto intersection = sphere_intesection ( mins + delta * float ( i ) ); intersection )
				return intersection;
		}

		if ( auto intersection = sphere_intesection ( maxs ); intersection )
			return intersection;

		return {};
	}

	bool hitchance ( target & entity ) {
		constexpr int   SEED_MAX = 255;
		constexpr float HITCHANCE_MAX = 100.f;
		if ( !seed_list.size ( ) ) {
			for ( int i { }; i <= SEED_MAX; ++i )
				seed_list.push_back ( { math::random_float ( 0.f, 1.f ), math::random_float ( 0.f, 2.f * M_PI ),math::random_float ( 0.f, 1.f ),math::random_float ( 0.f, 2.f * M_PI ) } );

		}

		studio_box_t * _hitbox = entity.hitbox_set->hitbox ( entity.aimbot.best_point.hitbox );


		auto VectorTransform_Wrapper = [ ] ( const vec3_t & in1, const matrix3x4_t & in2, vec3_t & out ) {
			auto VectorTransform = [ ] ( const float * in1, const matrix3x4_t & in2, float * out ) {
				auto DotProducts = [ ] ( const float * v1, const float * v2 ) {
					return v1 [ 0 ] * v2 [ 0 ] + v1 [ 1 ] * v2 [ 1 ] + v1 [ 2 ] * v2 [ 2 ];
				};
				out [ 0 ] = DotProducts ( in1, in2 [ 0 ] ) + in2 [ 0 ][ 3 ];
				out [ 1 ] = DotProducts ( in1, in2 [ 1 ] ) + in2 [ 1 ][ 3 ];
				out [ 2 ] = DotProducts ( in1, in2 [ 2 ] ) + in2 [ 2 ][ 3 ];
			};
			VectorTransform ( &in1.x, in2, &out.x );
		};


		vec3_t mins = vec3_t ( ), maxs = vec3_t ( );
		float radius = _hitbox->radius != -1.f ? _hitbox->radius : 0.f;

		VectorTransform_Wrapper ( vec3_t ( _hitbox->maxs.x, _hitbox->maxs.y, _hitbox->maxs.z ), entity.aimbot.record.bone [ _hitbox->bone ], maxs );
		VectorTransform_Wrapper ( vec3_t ( _hitbox->mins.x, _hitbox->mins.y, _hitbox->mins.z ), entity.aimbot.record.bone [ _hitbox->bone ], mins );
		entity.aimbot.best_point.col.maxs = maxs;
		entity.aimbot.best_point.col.mins = mins;
		entity.aimbot.best_point.col.radius = radius;
		
		if ( !local_player::m_data.active_weapon )
			return false;


		local_player::m_data.active_weapon->update_accuracy_penalty ( );

		local_player::m_data.weapon_spread = local_player::m_data.active_weapon->get_spread ( );
		local_player::m_data.weapon_cone = local_player::m_data.active_weapon->inaccuracy ( );

		entity.aimbot.best_point.should_trace = true;
		bool gooten_value = false;
		int i = 0;
		while ( i <  5024 ) {
			if ( !aimbot::best_target.aimbot.best_point.should_trace ) {
				gooten_value = entity.aimbot.best_point.passed_hitchance;
				break;
			}
			i++;
		}


		return gooten_value;
	/*	auto angle = math::calc_angle ( engine_prediction::unpredicted_eye, best_target.aimbot.best_point.position );
		

		constexpr int   SEED_MAX = 255;
		constexpr float HITCHANCE_MAX = 100.f;

		auto local_player = local_player::m_data.pointer;
		float hitchance = config.ragebot_hitchance;

		if ( !local_player ) return false;
		int traces_hit = 0;
		int needed_hits = static_cast< int >( SEED_MAX * ( hitchance / 100.f ) );
		vec3_t forward, right, up;
		const vec3_t eye_position = engine_prediction::unpredicted_eye;
		math::angle_vectors ( angle, &forward, &right, &up );

		auto weapon = local_player->active_weapon ( );
		if ( !weapon )
			return false;


		weapon->update_accuracy_penalty ( );

		float weapon_spread = weapon->get_spread ( );
		float weapon_cone = weapon->inaccuracy ( );


		auto VectorTransform_Wrapper = [ ] ( const vec3_t & in1, const matrix3x4_t & in2, vec3_t & out ) {
			auto VectorTransform = [ ] ( const float * in1, const matrix3x4_t & in2, float * out ) {
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
		entity.aimbot.best_point.col.maxs = maxs;
		entity.aimbot.best_point.col.mins = mins;
		entity.aimbot.best_point.col.radius = radius;
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
		}*/

/*
	}
}*/

#include "../../features.hpp"

#include <optional>

namespace aimbot {

	std::optional<vec3_t> get_intersect_point ( vec3_t start, vec3_t end, vec3_t mins, vec3_t maxs, float radius ) {
		auto sphere_intesection = [ start, end, radius ] ( auto && center ) -> std::optional<vec3_t> {
					
			auto direction = ( end - start );

			math::fast_vec_normalize ( direction );

			auto q = vec3_t ( center - start );
			auto v = q.dot ( direction );
			auto d = radius * radius - ( q.length_sqr ( ) - v * v );
			
			if ( d < FLT_EPSILON )
				return {};

			return start + direction * ( v - std::sqrt ( d ) );
		};

		auto delta = ( maxs - mins );
		math::fast_vec_normalize(delta );
		auto dist = mins.distance_to ( maxs );
		if ( dist > 100.f ) {
			//interfaces::console->console_printf ( "DEBUG: CRITICAL ERROR IN GET INTERSECT POINT ADDRESS %p hitchance.cpp %\n", _ReturnAddress ( ) );
			return {};
		}
		for ( size_t i {}; i < std::floor ( mins.distance_to ( maxs ) ); ++i ) {
			if ( auto intersection = sphere_intesection ( mins + delta * float ( i ) ); intersection )/*de aici da crash pentru ca nu are mins maxs*/
				return intersection;
		}

		if ( auto intersection = sphere_intesection ( maxs ); intersection )
			return intersection;

		return {};
	}
	float get_innacuracy_level (  ) 
	{

		float hitchance = 0; 
		float inaccuracy = local_player::m_data.active_weapon->inaccuracy ( );

		if ( inaccuracy == 0 )
			inaccuracy = 0.0000001;

		inaccuracy = 1 / inaccuracy;
		hitchance = inaccuracy;

		return hitchance;
	}
	bool hitchance ( target & entity ) {
		auto angle = math::calc_angle ( localdata.eye_position, best_target.aimbot.best_point.position );


		constexpr int   SEED_MAX = 255;
		constexpr float HITCHANCE_MAX = 100.f;

		auto local_player = local_player::m_data.pointer;
		float hitchance = config.ragebot_hitchance;

		if ( !local_player ) return false;

		const auto round_acc = [ ] ( const float accuracy ) { return roundf ( accuracy * 1000.f ) / 1000.f; };

		const auto crouched = local_pointer->flags ( ) & fl_ducking;
		const auto weapon_inaccuracy = localdata.active_weapon->inaccuracy ( );

		if ( crouched ) {
			if ( round_acc ( weapon_inaccuracy ) == round_acc ( localdata.active_weapon->is_sniper() ? localdata.weapon_data->flInaccuracyCrouchAlt : localdata.weapon_data->flInaccuracyCrouch ) )
				return true;
		}
		else {
			if ( round_acc ( weapon_inaccuracy ) == round_acc ( localdata.active_weapon->is_sniper ( ) ? localdata.weapon_data->flInaccuracyStandAlt : localdata.weapon_data->flInaccuracyStand ) )
				return true;

		}
		int traces_hit = 0;
		int needed_hits = static_cast< int >( SEED_MAX * ( hitchance / 100.f ) );
		vec3_t forward, right, up;
		const vec3_t eye_position = engine_prediction::unpredicted_eye;
		math::angle_vectors ( angle, &forward, &right, &up );

		auto weapon = local_player->active_weapon ( );
		if ( !weapon )
			return false;


		float weapon_spread = weapon->get_spread ( );
		float weapon_cone = weapon->inaccuracy ( );


		auto VectorTransform_Wrapper = [ ] ( const vec3_t & in1, const matrix3x4_t & in2, vec3_t & out ) {
			auto VectorTransform = [ ] ( const float * in1, const matrix3x4_t & in2, float * out ) {
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

		bool should_use_resolved = entity.aimbot.best_point.hitbox == hitbox_head && entity.aimbot.record.resolved;

		VectorTransform_Wrapper ( vec3_t ( _hitbox->maxs.x, _hitbox->maxs.y, _hitbox->maxs.z ),  entity.aimbot.record.bone [ _hitbox->bone ], maxs );
		VectorTransform_Wrapper ( vec3_t ( _hitbox->mins.x, _hitbox->mins.y, _hitbox->mins.z ), entity.aimbot.record.bone [ _hitbox->bone ], mins );
		entity.aimbot.best_point.col.maxs = maxs;
		entity.aimbot.best_point.col.mins = mins;
		entity.aimbot.best_point.col.radius = radius;
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
