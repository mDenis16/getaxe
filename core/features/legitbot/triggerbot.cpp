#include "../features.hpp"


namespace legit_bot {

	namespace trigger_bot {
		std::pair < player_manager::lagcomp_t , bool > should_attack ( player_t * player ) {

			std::pair < player_manager::lagcomp_t, bool > data;
			if ( !local_player::m_data.have_weapon )
				return data;

			auto end = math::angle_vector ( local_player::m_data.orig_viewangle + local_player::m_data.pointer->aim_punch_angle ( ) );
			end = local_player::m_data.eye_position + ( end * local_player::m_data.weapon_data->flRange );


			auto hdr = interfaces::model_info->get_studio_model ( player->model ( ) );
			if ( !hdr )
				return data;

			auto set = hdr->hitbox_set ( 0 );
			if ( !set )
				return data;
			bool found_record = false;
			for ( auto record : player_manager::records [ player->index ( ) ] ) {
				for ( int i = 0; i < set->hitbox_count; ++i ) {
					auto hitbox = set->hitbox ( i );
					if ( !hitbox || hitbox->radius == -1.f )
						continue;


					vec3_t mins = math::vector_transform ( hitbox->mins, record.bone [ hitbox->bone ] );
					vec3_t maxs = math::vector_transform ( hitbox->maxs, record.bone [ hitbox->bone ] );

					if ( auto intersection = aimbot::get_intersect_point ( local_player::m_data.eye_position, end, mins, maxs, hitbox->radius ); intersection ) {
						ray_t ray;
						trace_t trace;
						trace_filter filter;

						filter.skip = local_player::m_data.pointer;
						ray.initialize ( local_player::m_data.eye_position, *intersection );

						interfaces::trace_ray->trace_ray ( ray, MASK_SHOT | CONTENTS_GRATE, &filter, &trace );
						if ( !trace.did_hit ( ) ) {
							data.first = record;
							data.second = true;
							found_record = true;
						}
					}
					if ( found_record )
						break;
				}
				if ( found_record )
					break;
			}
			return data;
		}
		
		void run ( c_usercmd * cmd ) {
			if ( m_data.current_target ) {
				auto trigger_data = should_attack ( m_data.current_target );
				if ( trigger_data.second ) {
					if ( local_player::m_data.pointer->can_shoot ( false ) && (interfaces::globals->tick_count - m_data.last_shot_tickcount) > config.triggerbot_delay_between_shots )
						cmd->buttons |= in_attack;
				

					if ( cmd->buttons & ( int ) in_attack ) {
						cmd->tick_count = math::time_to_ticks ( trigger_data.first.simtime + player_manager::get_lerp_time ( ) );
						m_data.last_shot_tickcount = interfaces::globals->tick_count;
					}
				}
			}
		}

	}
}