#include "../features.hpp"



	Movement g_movement { };;

	void Movement::JumpRelated ( ) {
		if ( !config.misc.movement.bhop )
			return;

		if ( !csgo::cmd )
			return;

		if ( !localdata.alive )
			return;

		static const auto sv_autobunnyhopping = interfaces::console->get_convar ( crypt_str ( "sv_autobunnyhopping" ) );
	
		if ( !sv_autobunnyhopping )
			return;

		if ( sv_autobunnyhopping->get_int ( ) == 1 )
			return;

		// No need to bhop there
		const auto move_type = local_pointer->move_type ( );
		if ( move_type == movetype_ladder ||
			move_type == movetype_noclip ||
			move_type == movetype_observer )
			return;

		// Is player underwater?
		//if (g_cl.m_local->m_nWaterLevel() >= 2)
		//	return;

		static auto LastJumped = false;
		static auto ShouldFake = false;

		const auto flags = local_pointer->flags ( );

		if ( !LastJumped && ShouldFake ) {
			ShouldFake = false;
			 csgo::cmd->buttons |= in_jump;
		}
		else if ( csgo::cmd->buttons & in_jump ) {

			if ( config.misc.movement.air_duck ) {
				csgo::cmd->buttons |= in_duck;
			}


			if ( flags & fl_onground ) {
				LastJumped = true;
				ShouldFake = true;
			}
			else {
				csgo::cmd->buttons &= ~in_jump;
				LastJumped = false;
			}
		}
		else {
			LastJumped = false;
			ShouldFake = false;
		}
	}

	void Movement::Strafe ( ) {
		vec3_t velocity;
		float  abs_delta, velocity_delta, correct;
		float yaw_add = 0.f;

		// don't strafe while we prolly want to jump scout..
		if ( g_movement.m_slow_motion )
			return;

		// don't strafe while noclipping or on ladders..
		if ( local_pointer->move_type ( ) == movetype_noclip || local_pointer->move_type ( ) == movetype_ladder )
			return;

		// get networked velocity ( maybe absvelocity better here? ).
		// meh, should be predicted anyway? ill see.
		velocity = local_pointer->get_abs_velocity ( );

		// get the velocity len2d ( speed ).
		m_speed = velocity.Length2D ( );

		// compute the ideal strafe angle for our velocity.
		m_ideal = ( m_speed > 0.f ) ? RAD2DEG ( std::asin ( 15.f / m_speed ) ) : 90.f;
		m_ideal2 = ( m_speed > 0.f ) ? RAD2DEG ( std::asin ( 30.f / m_speed ) ) : 90.f;

		// some additional sanity.
		math::clamp ( m_ideal, 0.f, 90.f );
		math::clamp ( m_ideal2, 0.f, 90.f );

		// save entity bounds ( used much in circle-strafer ).
		m_mins = local_pointer->mins ( );
		m_maxs = local_pointer->maxs ( );

		// save our origin
		m_origin = local_pointer->origin ( );

		// disable strafing while pressing shift.
		if ( ( csgo::cmd->buttons & in_speed ) || ( local_pointer->flags() & fl_onground ) )
			return;

		// for changing direction.
		// we want to change strafe direction every call.
		m_switch_value *= -1.f;

		// for allign strafer.
		++m_strafe_index;

		if ( localdata.pressing_move && config.misc.movement.wasd_strafe ) {
			// took this idea from stacker, thank u !!!!
			enum EDirections {
				FORWARDS = 0,
				BACKWARDS = 180,
				LEFT = 90,
				RIGHT = -90,
				BACK_LEFT = 135,
				BACK_RIGHT = -135
			};

			float wish_dir { };

			// get our key presses.
			bool holding_w = csgo::cmd->buttons & in_forward;
			bool holding_a = csgo::cmd->buttons & in_moveleft;
			bool holding_s = csgo::cmd->buttons & in_back;
			bool holding_d = csgo::cmd->buttons & in_moveright;

			// move in the appropriate direction.
			if ( holding_w ) {
				//	forward left
				if ( holding_a ) {
					wish_dir += ( EDirections::LEFT / 2 );
				}
				//	forward right
				else if ( holding_d ) {
					wish_dir += ( EDirections::RIGHT / 2 );
				}
				//	forward
				else {
					wish_dir += EDirections::FORWARDS;
				}
			}
			else if ( holding_s ) {
				//	back left
				if ( holding_a ) {
					wish_dir += EDirections::BACK_LEFT;
				}
				//	back right
				else if ( holding_d ) {
					wish_dir += EDirections::BACK_RIGHT;
				}
				//	back
				else {
					wish_dir += EDirections::BACKWARDS;
				}

			 csgo::cmd->forwardmove = 0;
			}
			else if ( holding_a ) {
				//	left
				wish_dir += EDirections::LEFT;
			}
			else if ( holding_d ) {
				//	right
				wish_dir += EDirections::RIGHT;
			}

			yaw_add += math::normalize_yaw ( wish_dir );
		}

		// cancel out any forwardmove values.
		csgo::cmd->forwardmove = 0.f;

		// do allign strafer.
		if ( config.misc.movement.a_strafe  ) {
			float angle = std::max ( m_ideal2, 4.f );

			if ( angle > m_ideal2 && !( m_strafe_index % 5 ) )
				angle = m_ideal2;

			// add the computed step to the steps of the previous circle iterations.
			m_circle_yaw = math::normalize_yaw ( m_circle_yaw + angle );

			// apply data to usercmd.
			yaw_add = m_circle_yaw;
			csgo::cmd->sidemove = -450.f;

			return;
		}

		// do ciclestrafer
		else if ( config.misc.movement.circle_strafe ) {
			// if no duck jump.
			if ( !config.misc.movement.air_duck ) {
				// crouch to fit into narrow areas.
				csgo::cmd->buttons |= in_duck;
			}

			DoPrespeed ( );
			return;
		}

		else if ( config.misc.movement.z_strafe ) {
			float freq = ( config.misc.movement.z_strafe_frequency * 0.2f ) * interfaces::globals->realtime;

			// range [ 1, 100 ], aka grenerates a factor.
			float factor = config.misc.movement.z_strafer_distance * 0.5f;

			yaw_add += ( factor * std::sin ( freq ) );
		}

		if ( !config.misc.movement.auto_strafe )
			return;

	
		m_circle_yaw = m_old_yaw = localdata.strafe_angles.y;



		static convar * cl_sidespeed = interfaces::console->get_convar ( "cl_sidespeed" );

		localdata.strafe_angles.y += yaw_add;
		csgo::cmd->forwardmove = 0.f;
		csgo::cmd->sidemove = 0.f;

		const auto delta = math::normalize_yaw ( localdata.strafe_angles.y - RAD2DEG ( atan2 ( local_player::m_data.pointer->velocity ( ).y, local_player::m_data.pointer->velocity ( ).x ) ) );

		csgo::cmd->sidemove = delta > 0.f ? -cl_sidespeed->get_float ( ) : cl_sidespeed->get_float ( );

		localdata.strafe_angles.y = math::normalize_yaw ( localdata.strafe_angles.y - delta );
		
		// we can accelerate more, because we strafed less then needed
		// or we got of track and need to be retracked.
	/*	if ( abs_delta <= m_ideal || abs_delta >= 30.f ) {
			// compute angle of the direction we are traveling in.
			vec3_t velocity_angle;
			math::vector_angles ( velocity, velocity_angle );

			// get the delta between our direction and where we are looking at.
			velocity_delta = math::normalize_yaw ( localdata.strafe_angles.y - velocity_angle.y );

			// correct our strafe amongst the path of a circle.
			correct = m_ideal;

			if ( velocity_delta <= correct || m_speed <= 15.f ) {
				// not moving mouse, switch strafe every tick.
				if ( -correct <= velocity_delta || m_speed <= 15.f ) {
					localdata.strafe_angles.y += ( m_ideal * m_switch_value );
					csgo::cmd->sidemove = 450.f * m_switch_value;
				}

				else {
					localdata.strafe_angles.y = velocity_angle.y - correct;
					csgo::cmd->sidemove = 450.f;
				}
			}

			else {
				localdata.strafe_angles.y = velocity_angle.y + correct;
				csgo::cmd->sidemove = -450.f;
			}
		}*/
	}

	void Movement::DoPrespeed ( ) {
		float   mod, min, max, step, strafe, time, angle;
		vec3_t  plane;

		// min and max values are based on 128 ticks.
		mod = interfaces::globals->interval_per_tick * 128.f;

		// scale min and max based on tickrate.
		min = 2.25f * mod;
		max = 5.f * mod;

		// compute ideal strafe angle for moving in a circle.
		strafe = m_ideal * 2.f;

		// clamp ideal strafe circle value to min and max step.
		math::clamp ( strafe, min, max );

		// calculate time.
		time = 320.f / m_speed;

		// clamp time.
		math::clamp ( time, 0.35f, 1.f );

		// init step.
		step = strafe;

		while ( true ) {
			// if we will not collide with an object or we wont accelerate from such a big step anymore then stop.
			if ( !WillCollide ( time, step ) || max <= step )
				break;

			// if we will collide with an object with the current strafe step then increment step to prevent a collision.
			step += 0.2f;
		}

		if ( step > max ) {
			// reset step.
			step = strafe;

			while ( true ) {
				// if we will not collide with an object or we wont accelerate from such a big step anymore then stop.
				if ( !WillCollide ( time, step ) || step <= -min )
					break;

				// if we will collide with an object with the current strafe step decrement step to prevent a collision.
				step -= 0.2f;
			}

			if ( step < -min ) {
				if ( GetClosestPlane ( plane ) ) {
					// grab the closest object normal
					// compute the angle of the normal
					// and push us away from the object.
					angle = RAD2DEG ( std::atan2 ( plane.y, plane.x ) );
					step = -math::normalize_yaw ( m_circle_yaw - angle ) * 0.1f;
				}
			}

			else
				step -= 0.2f;
		}

		else
			step += 0.2f;

		// add the computed step to the steps of the previous circle iterations.
		m_circle_yaw = math::normalize_yaw ( m_circle_yaw + step );

		// apply data to usercmd.
		localdata.strafe_angles.y = m_circle_yaw;
		csgo::cmd->sidemove = ( step >= 0.f ) ? -450.f : 450.f;
	}

	bool Movement::GetClosestPlane ( vec3_t & plane ) {
		trace_t            trace;
		trace_world_only filter;
		vec3_t                start { m_origin };
		float                 smallest { 1.f };
		const float		      dist { 75.f };

		// trace around us in a circle
		for ( float step { }; step <= M_PI; step += ( math::pi / 10.f ) ) {
			// extend endpoint x units.
			vec3_t end = start;
			end.x += std::cos ( step ) * dist;
			end.y += std::sin ( step ) * dist;
		
		
			interfaces::trace_ray->trace_ray ( ray_t ( start, end, m_mins, m_maxs ), CONTENTS_SOLID, &filter, &trace );

			// we found an object closer, then the previouly found object.
			if ( trace.flFraction < smallest ) {
				// save the normal of the object.
				plane = trace.plane.normal;
				smallest = trace.flFraction;
			}
		}

		// did we find any valid object?
		return smallest != 1.f && plane.z < 0.1f;
	}

	bool Movement::WillCollide ( float time, float change ) {

		static convar * sv_jump_impulse = interfaces::console->get_convar ( "sv_jump_impulse" );
		static convar * sv_gravity = interfaces::console->get_convar ( "sv_gravity" );

		struct PredictionData_t {
			vec3_t start;
			vec3_t end;
			vec3_t velocity;
			float  direction;
			bool   ground;
			float  predicted;
		};

		PredictionData_t      data;
		trace_t            trace;
		trace_world_only filter;

		// set base data.
		data.ground = local_pointer->flags() & fl_onground;
		data.start = m_origin;
		data.end = m_origin;
		data.velocity = local_pointer->velocity ( );
		data.direction = RAD2DEG ( std::atan2 ( data.velocity.y, data.velocity.x ) );

		for ( data.predicted = 0.f; data.predicted < time; data.predicted += interfaces::globals->interval_per_tick ) {
			// predict movement direction by adding the direction change.
			// make sure to normalize it, in case we go over the -180/180 turning point.
			data.direction = math::normalize_yaw ( data.direction + change );

			// pythagoras.
			float hyp = data.velocity.Length2D ( );

			// adjust velocity for new direction.
			data.velocity.x = std::cos ( DEG2RAD ( data.direction ) ) * hyp;
			data.velocity.y = std::sin ( DEG2RAD ( data.direction ) ) * hyp;

			// assume we bhop, set upwards impulse.
			if ( data.ground )
				data.velocity.z = sv_jump_impulse->get_float ( );

			else
				data.velocity.z -= sv_gravity->get_float ( ) * interfaces::globals->interval_per_tick;

			// we adjusted the velocity for our new direction.
			// see if we can move in this direction, predict our new origin if we were to travel at this velocity.
			data.end += ( data.velocity * interfaces::globals->interval_per_tick );

			// trace
			interfaces::trace_ray->trace_ray ( ray_t ( data.start, data.end, m_mins, m_maxs ), MASK_PLAYERSOLID, &filter, &trace );

			// check if we hit any objects.
			if ( trace.flFraction != 1.f && trace.plane.normal.z <= 0.9f )
				return true;

			if ( trace.startSolid || trace.allsolid )
				return true;

			// adjust start and end point.
			data.start = data.end = trace.end;

			// move endpoint 2 units down, and re-trace.
			// do this to check if we are on th floor.
			interfaces::trace_ray->trace_ray ( ray_t ( data.start, data.end - vec3_t { 0.f, 0.f, 2.f }, m_mins, m_maxs ), MASK_PLAYERSOLID, &filter, &trace );

			// see if we moved the player into the ground for the next iteration.
			data.ground = trace.did_hit ( ) && trace.plane.normal.z > 0.7f;
		}

		// the entire loop has ran
		// we did not hit shit.
		return false;
	}

	void Movement::FixMove ( c_usercmd * cmd, vec3_t wish_angles ) {
		vec3_t view_fwd, view_right, view_up, cmd_fwd, cmd_right, cmd_up;
		math::angle_vectors ( wish_angles, &view_fwd, &view_right, &view_up );
		math::angle_vectors ( cmd->viewangles, &cmd_fwd, &cmd_right, &cmd_up );

		const auto v8 = sqrtf ( ( view_fwd.x * view_fwd.x ) + ( view_fwd.y * view_fwd.y ) );
		const auto v10 = sqrtf ( ( view_right.x * view_right.x ) + ( view_right.y * view_right.y ) );
		const auto v12 = sqrtf ( view_up.z * view_up.z );

		const vec3_t norm_view_fwd ( ( 1.f / v8 ) * view_fwd.x, ( 1.f / v8 ) * view_fwd.y, 0.f );
		const vec3_t norm_view_right ( ( 1.f / v10 ) * view_right.x, ( 1.f / v10 ) * view_right.y, 0.f );
		const vec3_t norm_view_up ( 0.f, 0.f, ( 1.f / v12 ) * view_up.z );

		const auto v14 = sqrtf ( ( cmd_fwd.x * cmd_fwd.x ) + ( cmd_fwd.y * cmd_fwd.y ) );
		const auto v16 = sqrtf ( ( cmd_right.x * cmd_right.x ) + ( cmd_right.y * cmd_right.y ) );
		const auto v18 = sqrtf ( cmd_up.z * cmd_up.z );

		const vec3_t norm_cmd_fwd ( ( 1.f / v14 ) * cmd_fwd.x, ( 1.f / v14 ) * cmd_fwd.y, 0.f );
		const vec3_t norm_cmd_right ( ( 1.f / v16 ) * cmd_right.x, ( 1.f / v16 ) * cmd_right.y, 0.f );
		const vec3_t norm_cmd_up ( 0.f, 0.f, ( 1.f / v18 ) * cmd_up.z );

		const auto v22 = norm_view_fwd.x * cmd->forwardmove;
		const auto v26 = norm_view_fwd.y * cmd->forwardmove;
		const auto v28 = norm_view_fwd.z * cmd->forwardmove;
		const auto v24 = norm_view_right.x * cmd->sidemove;
		const auto v23 = norm_view_right.y * cmd->sidemove;
		const auto v25 = norm_view_right.z * cmd->sidemove;
		const auto v30 = norm_view_up.x * cmd->upmove;
		const auto v27 = norm_view_up.z * cmd->upmove;
		const auto v29 = norm_view_up.y * cmd->upmove;

		cmd->forwardmove = ( ( ( ( norm_cmd_fwd.x * v24 ) + ( norm_cmd_fwd.y * v23 ) ) + ( norm_cmd_fwd.z * v25 ) )
			+ ( ( ( norm_cmd_fwd.x * v22 ) + ( norm_cmd_fwd.y * v26 ) ) + ( norm_cmd_fwd.z * v28 ) ) )
			+ ( ( ( norm_cmd_fwd.y * v30 ) + ( norm_cmd_fwd.x * v29 ) ) + ( norm_cmd_fwd.z * v27 ) );
		cmd->sidemove = ( ( ( ( norm_cmd_right.x * v24 ) + ( norm_cmd_right.y * v23 ) ) + ( norm_cmd_right.z * v25 ) )
			+ ( ( ( norm_cmd_right.x * v22 ) + ( norm_cmd_right.y * v26 ) ) + ( norm_cmd_right.z * v28 ) ) )
			+ ( ( ( norm_cmd_right.x * v29 ) + ( norm_cmd_right.y * v30 ) ) + ( norm_cmd_right.z * v27 ) );
		cmd->upmove = ( ( ( ( norm_cmd_up.x * v23 ) + ( norm_cmd_up.y * v24 ) ) + ( norm_cmd_up.z * v25 ) )
			+ ( ( ( norm_cmd_up.x * v26 ) + ( norm_cmd_up.y * v22 ) ) + ( norm_cmd_up.z * v28 ) ) )
			+ ( ( ( norm_cmd_up.x * v30 ) + ( norm_cmd_up.y * v29 ) ) + ( norm_cmd_up.z * v27 ) );

		wish_angles = cmd->viewangles;

		if ( local_pointer->move_type ( ) != movetype_ladder )
			cmd->buttons &= ~( in_forward | in_back | in_moveright | in_moveleft );
	}

	/*void Movement::AutoPeek ( ) {
		// set to invert if we press the button.
		if ( g_input.GetKeyState ( g_cfg [ XOR ( "misc_auto_peek" ) ].get<int> ( ) ) && !m_slow_motion ) {
			if ( g_cl.m_old_shot )
				m_invert = true;

			vec3_t move { g_cl.m_cmd->m_forward_move, g_cl.m_cmd->m_side_move, 0.f };

			if ( m_invert ) {
				move *= -1.f;
				g_cl.m_cmd->m_forward_move = move.x;
				g_cl.m_cmd->m_side_move = move.y;
			}
		}

		else m_invert = false;

	}*/

	void Movement::AutoStop ( ) {
		/*if ( g_cl.m_ground && g_cfg [ XOR ( "rage_aimbot_auto_stop" ) ].get<bool> ( ) && !m_slow_motion ) {
			//if (g_cfg[XOR("rage_aimbot_auto_stop_type_full")].get<bool>()) {
			if ( g_aimbot.m_stop ) {
				if ( g_cl.m_speed > 15.f ) {
					// convert velocity to angular momentum.
					ang_t angle;
					math::VectorAngles ( g_cl.m_local->m_vecVelocity ( ), angle );

					// get our current speed of travel.
					float speed = g_cl.m_local->m_vecVelocity ( ).length ( );

					// fix direction by factoring in where we are looking.
					angle.y = g_cl.m_view_angles.y - angle.y;

					// convert corrected angle back to a direction.
					vec3_t direction;
					math::AngleVectors ( angle, &direction );

					vec3_t stop = direction * -speed;

					g_cl.m_cmd->m_forward_move = stop.x;
					g_cl.m_cmd->m_side_move = stop.y;
				}
				else {
					g_cl.m_cmd->m_forward_move = 0;
					g_cl.m_cmd->m_side_move = 0;
				}

				g_aimbot.m_stop = false;
			}
		}*/
	}

	/*thanks onetap.com*/
	void Movement::ClampMovementSpeed ( const float& speed ) {

		if ( !csgo::cmd || !localdata.alive )
			return;

		csgo::cmd->buttons |= in_speed;

		csgo::cmd->buttons |= in_speed;

		float squirt = std::sqrtf ( ( csgo::cmd->forwardmove * csgo::cmd->forwardmove ) + ( csgo::cmd->sidemove * csgo::cmd->sidemove ) );

		if ( squirt > speed ) {
			float squirt2 = std::sqrtf ( ( csgo::cmd->forwardmove * csgo::cmd->forwardmove ) + ( csgo::cmd->sidemove * csgo::cmd->sidemove ) );

			float cock1 = csgo::cmd->forwardmove / squirt2;
			float cock2 = csgo::cmd->sidemove / squirt2;

			auto Velocity = local_player::m_data.pointer->velocity ( ).Length2D ( );

			if ( speed + 1.0 <= Velocity ) {
				csgo::cmd->forwardmove = 0;
				csgo::cmd->sidemove = 0;
			}
			else {
				csgo::cmd->forwardmove = cock1 * speed;
				csgo::cmd->sidemove = cock2 * speed;
			}
		}
	}

	void Movement::slow_walk ( ) {
		vec3_t velocity { local_pointer->velocity() };
		int    ticks { }, max { 7 };

		if ( !config.misc.movement.slow_walk )
			return;


		if ( localdata.alive && localdata.active_weapon ) {
			// get the max possible speed whilest we are still accurate.
			float flMaxSpeed = local_pointer->is_scoped ( ) > 0 ? localdata.weapon_data->flMaxPlayerSpeedAlt : localdata.weapon_data->flMaxPlayerSpeed;
			float flDesiredSpeed = ( flMaxSpeed * 0.33000001 );

			ClampMovementSpeed ( flDesiredSpeed );
		}
	}
	
