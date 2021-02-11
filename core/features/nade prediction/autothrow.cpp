#include "../features.hpp"
#include "nade_prediction.hpp"


namespace autothrow {
	void create_move ( c_usercmd * cmd ) {

		if ( !interfaces::inputsystem->is_button_down ( button_code_t::KEY_F ) )
			return;

		if ( aimbot::targets.empty ( ) )
			return;

		if ( !localdata.active_weapon )
			return;

		if ( !localdata.active_weapon->is_grenade ( ) )
			return;

		auto target = aimbot::targets.front ( );

		if ( target.player ) {
			vec3_t target_angle = math::calc_angle ( engine_prediction::unpredicted_eye, target.player->eye_pos ( ) );
			vec3_t best_grenade_angle = target_angle;

			std::pair < vec3_t, float > best;
			
			auto dmg_ray = ray_t ( ); dmg_ray.initialize ( engine_prediction::unpredicted_eye, target.player->get_hitbox_position (hitbox_body ) );
			trace_filter_skip_one_entity filter ( local_player::m_data.pointer );
			trace_t trace;
			ray_t ray;

			// check if our path was obstructed by anything using a trace.
			interfaces::trace_ray->trace_ray ( dmg_ray, MASK_SHOT, ( trace_filter * ) &filter, &trace );

			// something went wrong here.
			if ( !trace.entity || trace.entity != target.player )
			{

				for ( size_t i = 0; i < 35; i++ ) {
					float add_x = math::random_float ( 1.f, 6.f );
					float add_y = math::random_float ( 1.f, 6.f );
					best_grenade_angle = target_angle;
					best_grenade_angle.x *= add_x; best_grenade_angle.y *= add_y;

					auto current = nade_pred.custom_predict ( cmd, best_grenade_angle, target.player );

					float dist = math::calc_distance ( target.player->origin ( ), current.first, false );
					if ( dist < best.second ) {
						best.second = dist;
						best.first = current.first;
					}
				}
			}
			

			    cmd->viewangles.angle_normalize ( );

				//cmd->buttons |= in_attack;
				cmd->viewangles = best_grenade_angle;
				interfaces::engine->set_view_angles ( cmd->viewangles );

		}

	}
}