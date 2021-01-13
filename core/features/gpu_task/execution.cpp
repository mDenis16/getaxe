#include <deque>
#include "../features.hpp"
#include "../../menu/render/menu_render.hpp"

namespace gpu_task {


	void execute_gpu_trace (  ) {
	/*	if ( !aimbot::best_target.aimbot.best_point.should_trace )
			return;
		
		static int traces_hit = 0;
		static int tick_count = 0;
	/*	if ( tick_count == interfaces::globals->tick_count ) {
			aimbot::best_target.aimbot.best_point.should_trace = false;
			tick_count = interfaces::globals->tick_count;
			return;
		}*//*
		tick_count = interfaces::globals->tick_count;
		bool passed = false;
		constexpr int   SEED_MAX = 255;
		constexpr float HITCHANCE_MAX = 100.f;



		auto angle = math::calc_angle ( engine_prediction::unpredicted_eye, aimbot::best_target.aimbot.best_point.position );


		int needed_hits = static_cast< int >( SEED_MAX * ( config.ragebot_hitchance / 100.f ) );
		vec3_t forward, right, up;
		const vec3_t eye_position = engine_prediction::unpredicted_eye;
		math::angle_vectors ( angle, &forward, &right, &up );


		const auto get_bullet_location = [ & ] ( int seed ) {


		/*	float a = aimbot::seed_list.at ( seed ).a;
			float b = aimbot::seed_list.at ( seed ).b;
			float c = aimbot::seed_list.at ( seed ).c;
			float d = aimbot::seed_list.at ( seed ).d;

			const float generated_spread = a * local_player::m_data.weapon_spread;
			const float generated_cone = c * local_player::m_data.weapon_cone;

			const vec3_t spread = vec3_t (
				std::cos ( b ) * generated_spread + std::cos ( d ) * generated_cone,
				std::sin ( b ) * generated_spread + std::sin ( d ) * generated_cone,
				0.f
			);*/

		//	return vec3_t ( /*forward + right * -spread.x + up * -spread.y*/ ).normalized ( );
	//	};

	//	for ( int i { }; i <= SEED_MAX; ++i ) {
		/*	vec3_t spread_angle;
			vec3_t bullet_end;
			auto a = get_bullet_location ( i );
			math::VectorAnglesAwall ( a, spread_angle );
			bullet_end = math::angle_vector ( angle - ( spread_angle - angle ) );

			vec3_t end_point = eye_position + bullet_end * 8192;
		
		

			if ( auto intersection = aimbot::get_intersect_point ( eye_position, end_point, aimbot::best_target.aimbot.best_point.col.mins, aimbot::best_target.aimbot.best_point.col.maxs, aimbot::best_target.aimbot.best_point.col.radius ); intersection ) {
				traces_hit++;
				auto pos = ImVec2 ( );
				if ( visuals::world_to_screen ( *intersection, pos ) )

					c_menu::get ( ).draw->AddCircleFilled ( pos, 3.f, ImColor ( 0, 255, 0 ));
			}

			if ( traces_hit >= needed_hits )
			{
				passed = true;
				traces_hit = 0;
				break;
			}

			if ( ( SEED_MAX - i + traces_hit ) < needed_hits ) {
				traces_hit = 0;
				break;
			}
		}
		c_menu::get ( ).draw->AddText ( ImVec2 ( 500, 500 ), ImColor ( 255, 0, 0, 255 ), std::to_string( traces_hit).c_str() );
		aimbot::best_target.aimbot.best_point.passed_hitchance = passed;

		aimbot::best_target.aimbot.best_point.should_trace = false;*/
	}
}