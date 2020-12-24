#include "../features.hpp"
float Distance ( vec3_t a, vec3_t b ) {
	return sqrt ( pow ( a.x - b.x, 2 ) + pow ( a.y - b.y, 2 ) + pow ( a.z - b.z, 2 ) );
}
namespace autopeek {
	/*peek_data m_data;
	void key_check ( ) {

		if ( interfaces::inputsystem->is_button_down ( button_code_t::KEY_F ) ) {
			m_data.pressed_last_tick = true;
			m_data.run = true;
		}
		else {
			if ( m_data.pressed_last_tick ) {
				reset ( );

			}
		}
	}
	void run_cm ( c_usercmd * cmd ) {
		key_check ( );
		if ( !m_data.run )
			return;
		if ( m_data.finished )
			return;
		m_data.origin = local_player::m_data.pointer->abs_origin ( );
		if ( m_data.state == peek_state::standby ) {
			search_position ( );

		}else {
			moving_check ( );
			if ( m_data.finished )
				return;
			move ( cmd );
		}
	}

	void search_position ( ) {
		if ( !ragebot::targets.size ( ) )
			return;

		auto target = ragebot::targets.back();
		if ( !target.player )
			return;
		/* getting the first element of targets vector ( this should me the closest related to fov)*/
		/*vec3_t position = local_player::m_data.pointer->get_eye_pos ( );

		float radius = 120;
		int radius_steps = 6;
		float step = M_PI * 2.0 / 16;
		autowall::FireBulletData_t awall = { };
		auto target_head = target.player->get_hitbox_position ( hitbox_head );
		m_data.backward_position = m_data.origin;
		/*creating a circle around head*/
		/*
				   ---------------------------- (peek position)
				 -                     `      -
				-                     ` step 3  -
			   -                  ` step 2  -    -
			  -                 `  step 1       -
			 -               ()                 -
			 -            our head             -
			  -                               -
			   -                             -
				-                           -
				 -                         -
				  --------------------------



		*/
		/*	float closest = 100.f;
			for ( float a = 0; a < ( M_PI * 2.0 ); a += step ) {
				for ( int i = 1; i <= radius_steps; i++ ) {


					float temp_radius = ( 120 / 6 ) * i;
					vec3_t location ( temp_radius * cos ( a ) + position.x, temp_radius * sin ( a ) + position.y, position.z );
					/*checking if we can hit from 20 to 20*/
					/*ray_t ray;
					trace_t tr;
					ray.initialize ( position, location );
					trace_filter traceFilter;
					traceFilter.skip = local_player::m_data.pointer;
					interfaces::trace_ray->trace_ray ( ray, MASK_SHOT_BRUSHONLY, &traceFilter, &tr );
					if ( tr.did_hit ( ) )
						continue;

					float dmg = autowall::GetDamage ( local_player::m_data.pointer, location, target_head, awall);
					float dist = Distance ( position, tr.end );
					if ( dmg  > 10.f)  {
						if ( dist < closest ) {
							m_data.forward_position = vec3_t( location.x, location.y, m_data.origin.z);

							m_data.moving_position = m_data.forward_position;

							m_data.state = peek_state::going_forward;
							closest = dist;
						}

					}

				}
			}
		}
		void move ( c_usercmd * cmd ) {
			const auto rotate_movement = [ & ] ( float yaw ) {
				float rotation = DEG2RAD ( csgo::m_strafe_angles.y - yaw );

				float cos_rot = cos ( rotation );
				float sin_rot = sin ( rotation );

				float new_forwardmove = ( cos_rot * cmd->forwardmove ) - ( sin_rot * cmd->sidemove );
				float new_sidemove = ( sin_rot * cmd->forwardmove ) + ( cos_rot * cmd->sidemove );

				cmd->forwardmove = new_forwardmove;
				cmd->sidemove = new_sidemove;
			};

			vec3_t angle {}, final_vec = m_data.origin - m_data.moving_position;
			 math::vector_angles ( final_vec, angle );

			cmd->forwardmove = 450.f;

			/* rotate move value */
			/*	rotate_movement ( angle.y );
			}
			void moving_check ( ) {


				switch ( m_data.state ) {
				case peek_state::going_forward:

				{
					float distance = math::calc_distance ( m_data.origin, m_data.forward_position, false );
					interfaces::console->console_printf ( "distance to forward position %f \n", distance );
					if ( distance < 1.5f ) {
						m_data.moving_position = m_data.backward_position;
						m_data.state = peek_state::going_backward;
					}
				}
					break;
				case peek_state::going_backward:
				{
					float distance = math::calc_distance ( m_data.origin, m_data.backward_position, false );
					interfaces::console->console_printf ( "distance to backward position %i \n", distance );
					if ( distance < 3 ) {
						m_data.finished = true;
					}
				}

					break;
				default:
					break;
				}

			}
			void reset ( ) {
				m_data.moving_position = vec3_t ( );
				m_data.forward_position = vec3_t ( );
				m_data.backward_position = vec3_t ( );
				m_data.state = peek_state::standby;
				m_data.finished = false;
				m_data.pressed_last_tick = false;
				m_data.run = false;

				visuals::notifications::add ( "Reset peek." );
			}
		}*/
}