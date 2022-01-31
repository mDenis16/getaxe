#include "../../helpers/helpers.h"
#include <renderlib/imgui/imgui.h>
#include <config.h>

#include "legitbot.h"
#include "../visuals/visuals.h"


#include <deque>

    c_legitbot * legitbot = new c_legitbot ( );
	double easeInOutExpo ( double t ) {
		if ( t < 0.5 ) {
			return ( pow ( 2, 16 * t ) - 1 ) / 510;
		}
		else {
			return 1 - 0.5 * pow ( 2, -16 * ( t - 0.5 ) );
		}
	}
	vec3_t CerpAngle ( vec3_t & From, vec3_t & To, float StepX, float StepY ) {
		vec3_t n;

		float CubicStepX = ( 1.f - std::cos ( StepX * M_PI ) ) / 2.f;
		float CubicStepY = ( 1.f - cos ( StepY * M_PI ) ) / 2.f;
		vec3_t Delta = ( To - From );
		Delta.angle_normalize ( );
		n.x = ( From.x + CubicStepX * Delta.x );
		n.y = ( From.y + CubicStepY * Delta.y );
		n.angle_normalize ( );

		return n;

	}
	double easeOutBounce ( double t ) {
		return 1 - pow ( 2, -6 * t ) * abs ( cos ( t * M_PI * 3.5 ) );
	}
	float InvLerpc ( float a, float b, float v ) {
		return ( v - a ) / ( b - a );
	}

	void SmoothAngle(vec3_t& From, vec3_t& To, float Percent)
	{
		vec3_t VecDelta = From - To;
		VecDelta.angle_normalize();
		VecDelta.x *= Percent;
		VecDelta.y *= Percent;
		To = From - VecDelta;
	}
	void c_legitbot::aim_at ( vec3_t & position ) {
		static vec3_t old_angles = vec3_t ( );

		
		aim_angles = math::calc_angle ( local_player::data().eye_position, position );
	

		aim_angles.angle_normalize ( );
		aim_angles.angle_clamp ( );
		vec3_t hitbox_angle = math::calc_angle ( local_player::data().eye_position, hitbox );
		hitbox_angle.angle_normalize ( );
		hitbox_angle.angle_clamp ( );

		//CerpAngle ( current_cmd->viewangles, hitbox_angle, dt_progress, dt_progress );

		control_rcs ( aim_angles );
	

		aim_angles.angle_normalize ( );

		vec3_t delta;
		delta.x = current_cmd->viewangles.x - aim_angles.x;
		delta.y = current_cmd->viewangles.y - aim_angles.y;
		delta.angle_normalize ( );



		//std::cout << " aim progress " << dt_progress << std::endl;


		if (!delta.is_zero()) {

			float delta_percent_X = delta.x / 100.f;
			float dleta_percent_Y = delta.y / 100.f;



			float speed = active_target->velocity().Length2D();
			speed = std::clamp(speed, 1.f, 255.f);
			auto aim_spd = 1.f;
			/*
						if ( !is_singleshot && dt_progress >= 1.f ) {
							float invlerp =  InvLerpc ( 1.f, 300.f, speed );
							float aim_speed = std::clamp ( ( settings->aim_speed * 1.4347826087f ) - std::lerp( invlerp, 1.f, settings->aim_speed), 1.f, 100.f );

						/*	current_cmd->viewangles.x = current_cmd->viewangles.x - delta_percent_X * aim_speed;
							current_cmd->viewangles.y = current_cmd->viewangles.y - dleta_percent_Y * aim_speed;
							std::cout << " linear smooth "  << " aim speed : " << aim_speed  << std::endl;*/


							//	current_cmd->viewangles.x = current_cmd->viewangles.x - delta_percent_X * std::clamp(settings->aim_speed - speed * .1f, 1.f, 100.f);
							//	current_cmd->viewangles.y = current_cmd->viewangles.y - dleta_percent_Y * std::clamp(settings->aim_speed - speed * .1f, 1.f, 100.f);

							//	current_cmd->viewangles.x = current_cmd->viewangles.x - delta_percent_X * std::clamp ( settings->aim_speed - speed * .1f, 1.f, 100.f );
								//current_cmd->viewangles.y = current_cmd->viewangles.y - dleta_percent_Y * std::clamp ( settings->aim_speed - speed * .1f, 1.f, 100.f );

			//if (is_singleshot) {
		
				aim_spd = std::clamp(settings->aim_speed - speed * .1f, 1.f, 100.f);


				if (local_player::data().last_mouse_x != 0 || local_player::data().last_mouse_y != 0) {

					aim_spd = 15;
					std::cout << "Low smoothing " << std::endl;

				}

				float invlerp = InvLerpc(1.f, 300.f, speed);
				float calc_speed = std::clamp((settings->aim_speed * 1.4347826087f) - std::lerp(invlerp, 1.f, settings->aim_speed), 1.f, 100.f);
				if (active_target->velocity().Length2D() > 150)
					aim_spd = calc_speed;


			//}else {
			//	if (dt_progress >= 1.f) { /*finished bezier*/


			//	}
			//	else {
			//		// don't apply any smoothing while doing bezier
			//		aim_spd = 100.f;

			//	}
			//}
		

			std::cout << "Mouse x " << local_player::data().last_mouse_x << "  Mouse y " << local_player::data().last_mouse_y << std::endl;
			//std::cout << "Aim speed " << aim_spd << std::endl;

			current_cmd->viewangles = CerpAngle(current_cmd->viewangles, aim_angles, aim_spd * 0.01f, aim_spd * 0.01f);

			current_cmd->viewangles.angle_normalize ( );

			interfaces::engine->set_view_angles ( current_cmd->viewangles );
		}

	}
	double easeInExpo ( double t ) {
		return ( pow ( 2, 8 * t ) - 1 ) / 255;
	}

	float easeOutElastic ( float x ) {
		const float c4 = ( 2.f * M_PI ) / 3.f;

		return x == 0
			? 0
			: x == 1
			? 1
			: std::pow ( 2, -10 * x ) * sin ( ( x * 10 - 0.75 ) * c4 ) + 1;
	}
	double easeInOutElastic ( double t ) {
		double t2;
		if ( t < 0.45 ) {
			t2 = t * t;
			return 8 * t2 * t2 * sin ( t * M_PI * 9 );
		}
		else if ( t < 0.55 ) {
			return 0.5 + 0.75 * sin ( t * M_PI * 4 );
		}
		else {
			t2 = ( t - 1 ) * ( t - 1 );
			return 1 - 8 * t2 * t2 * sin ( t * M_PI * 9 );
		}
	}
	double easeOutQuad ( double t ) {
		return t * ( 2 - t );
	}
	void c_legitbot::run ( c_usercmd * cmd ) {

		

		if ( !local_player::data().alive || !local_player::data().active_weapon || local_player::data().active_weapon->is_non_aim ( ) ) {
			return;
		}

		weapon_change ( );

		settings = &config.weapon_groups [ config.active_category ];
		current_cmd = cmd;

		if ( !settings->enabled )
			return;
		changed_target = false;

		is_singleshot = config.active_category == 0 || config.active_category == 1 || config.active_category == 4;
		first_aim = !did_attack_before && current_cmd->buttons & in_attack;

		if ( !is_singleshot ) {
			if (current_cmd->buttons & in_attack && !did_attack_before) {
				in_aiming = true;
			}
			else if (did_attack_before && !(current_cmd->buttons && in_attack)) {
				in_aiming = false;
				reset_target();
				target_change_tick = 0;
			}

		}
		else {
			if ( current_cmd->buttons & in_attack && !did_attack_before ) {
				in_aiming = true;
				aim_start_tick = interfaces::globals->tick_count;
			}
		}
		avoid_bezier = cmd->mousedx != 0 || cmd->mousedy != 0;


		
	


		if ( in_aiming ) {

			find_target ( );

			if ( valid_target && active_target && settings->aim_speed > 1.f  && active_target->is_alive()) {
				if ( first_aim ) {
					float angle_to_enemy = math::calc_angle ( local_player::ptr()->origin ( ), active_target->origin ( ) ).y;
					original_angle = math::normalize_yaw ( angle_to_enemy );
				}
				static auto predict_origin = [ ] ( vec3_t origin, vec3_t velocity, int ticks ) {
					return origin + ( velocity * ( interfaces::globals->interval_per_tick * static_cast< float >( ticks ) ) );
				};

				int predict_ticks = std::lerp(0, 4, dt_progress);
				
				if (active_target->velocity().Length2D() < 170)
					predict_ticks = 2;



				vec3_t target_hitbox = active_target->get_hitbox_position ( hitbox_head );

				if ( !target_hitbox.is_zero ( ) && local_player::ptr()->can_see_player_pos ( active_target, local_player::data().eye_position, target_hitbox ) ) {


					hitbox = predict_origin ( active_target->get_hitbox_position ( hitbox_head ), active_target->velocity ( ), predict_ticks );

				
					

					vec3_t pos = calculate_trail ( );

					if ( !pos.is_zero ( ) ) {

						aim_at ( pos );
					}
					else {
					//	std::cout << "pos is zero" << std::endl;
					}
				}
			}
			else {
				if (valid_target || active_target )
				reset_target ( );


				if (is_singleshot )
				   in_aiming = false;
			}
		}
		else {
			/*if ( active_target || valid_target ) {
				reset_target ( );
				
			}
			
		*/
		}
		//std::cout << "target index " << target_index << std::endl;

		did_attack_before = current_cmd->buttons & in_attack;
	
		/*if ( is_singleshot && in_aiming && cmd->buttons & in_attack && (interfaces::globals->tick_count - aim_start_tick) < 10 )
			current_cmd->buttons &= ~in_attack;*/

		if ( in_aiming ) {
			if ( is_singleshot && !( cmd->buttons & in_attack ) ) {
				ray_t ray;
				trace_t trace;
				trace_filter filter;
				vec3_t forward_dir = math::angle_vector ( cmd->viewangles );

				vec3_t forward = local_player::data().eye_position + ( forward_dir * 8192.f );
				filter.skip = local_player::ptr();
				ray.initialize ( local_player::data().eye_position, forward );

				interfaces::trace_ray->trace_ray ( ray, MASK_SHOT | CONTENTS_GRATE, &filter, &trace );
				//if ( trace.entity == active_target && local_player::ptr()->can_shoot_time(interfaces::globals->cur_time) )
					//cmd->buttons |= in_attack;
			}
			 
			if ( active_target && valid_target && settings->shoot_delay > 0.f && current_cmd->buttons & in_attack && !is_singleshot ) {
				if ( dt_progress <= ( settings->shoot_delay * 0.01f ) ) {
					current_cmd->buttons &= ~in_attack; //delay shot
					
				}
			}
			
		}
	
		changed_target = false;
		//std::cout << "shot this tick"  << (did_attack_before  ? "true" : "false") << std::endl;
	}
	vec3_t c_legitbot::calculate_trail ( ) {

	



		vec3_t view_angles = current_cmd->viewangles;

		
		vec3_t current_punch = local_player::ptr()->aim_punch_angle ( );
		view_angles.x += current_punch.x * 2.0;
		view_angles.y += current_punch.y * 2.0;

		vec3_t head_start = local_player::ptr()->eye_pos ( );

		float distance_to_hitbox = head_start.distance_to ( hitbox );

		vec3_t angle_to_hitbox = math::calc_angle(head_start, hitbox); 

		vec3_t forward_hitbox_dirrection = math::angle_vector (angle_to_hitbox);

		vec3_t forward_hitbox = head_start + ( forward_hitbox_dirrection * distance_to_hitbox );


		vec3_t forward_dirrection = math::angle_vector ( view_angles );

		vec3_t forward = head_start + ( forward_dirrection * distance_to_hitbox );

		float angle_to_enemy = math::calc_angle ( local_player::ptr()->origin ( ), active_target->origin ( ) ).y;
		angle_to_enemy = math::normalize_yaw ( angle_to_enemy );


		if ( first_aim || (!settings->lock_path && avoid_bezier) || changed_target) {
			start_position = forward;
			
			initial_fov = math::get_fov(view_angles, angle_to_hitbox, distance_to_hitbox);
			first_aim = false;
			original_angle = angle_to_enemy;
		}
		
	


		if ( !start_position.is_zero ( ) ) {

			 /*lock timer when user moving mouse*/
			if ( settings->aim_speed > 0.f) {

				if (active_target) {
					/*float speed = active_target->velocity().Length2D();
					speed = std::clamp(speed, 1.f, 255.f);


					float invlerp = InvLerpc(1.f, 300.f, speed);
					float aim_speed = std::clamp((settings->aim_speed * 1.4347826087f) - std::lerp(invlerp, 1.f, settings->aim_speed), 1.f, 100.f);*/

					//std::cout << "Initial fov " << std::endl;

					//dt_progress += interfaces::globals->interval_per_tick * (initial_fov / (100.f - settings->aim_speed));
					dt_progress += interfaces::globals->interval_per_tick * settings->aim_speed * 0.1f;

				}
			}

			if ( dt_progress >= 1.f ) dt_progress = 1.f;
		//	else
				//std::cout << "avoiding bezier " << std::endl;
			
			
			ImVec2 start2d, end2d;


			float& a1 = settings->bezier_curve [ 0 ];
			float& a2 = settings->bezier_curve [ 1 ];
			float& a3 = settings->bezier_curve [ 2 ];
			float& a4 = settings->bezier_curve [ 3 ];


			vec3_t a = start_position;
			vec3_t d = forward_hitbox;

			vec3_t b, c;

			b.x = std::lerp ( start_position.x, forward_hitbox.x, a2 );
			b.y = std::lerp ( start_position.y, forward_hitbox.y, 1.f - a1 );
			b.z = std::lerp ( forward_hitbox.z, start_position.z, a2 );



			c.x = std::lerp ( forward_hitbox.x, start_position.x, a2 );
			c.y = std::lerp ( forward_hitbox.y, start_position.y, a3 );
			c.z = std::lerp ( forward_hitbox.z, start_position.z, a4 );


			//std::cout << "dt_progress " << dt_progress << std::endl;
			

			vec3_t p = math::bezier (( dt_progress ) , a, b, c, d );

		 


			return p;
		}
		return vec3_t ( 0, 0, 0 );
	}

	void c_legitbot::delay_shoot ( ) {
		if ( valid_target && active_target  && !is_singleshot) {
			
		}
	}
	
	
	void c_legitbot::draw_debug ( ImDrawList * render ) {
		return;
		
			if ( local_player::ptr() && local_player::ptr()->is_alive ( ) ) {
				vec3_t view_angles, forward;
				vec3_t local = local_player::ptr()->eye_pos ( );

				interfaces::engine->get_view_angles ( view_angles );
				view_angles.angle_normalize ( ); view_angles.angle_clamp ( );

				math::angle_vectors ( view_angles, forward );

				auto hitbox_ang = math::calc_angle ( local, hitbox );
				hitbox_ang.angle_normalize ( ); hitbox_ang.angle_normalize ( );

				vec3_t hitbox_forward;
				math::angle_vectors ( hitbox_ang, hitbox_forward );

				float dist = hitbox.distance_to ( local );

				vec3_t start = local + ( forward * 8192.f );

				vec3_t end = local + ( hitbox_forward * 8192.f );


				ImVec2 from, to;

				if ( visuals::world_to_screen ( start, from ) && visuals::world_to_screen ( end, to ) ) {

					render->AddLine ( from, to, ImColor ( 255, 0, 0, 255 ), 3.f );

				}
			}

		if ( valid_target && !hitbox.is_zero ( ) && local_player::ptr() && local_player::ptr()->is_alive ( ) ) {

			vec3_t view_angles, forward;
			vec3_t local = local_player::ptr()->eye_pos ( );

			interfaces::engine->get_view_angles ( view_angles );

			math::angle_vectors ( view_angles, forward );

			vec3_t start = local + ( forward * ( local.distance_to ( hitbox ) ) );

			ImVec2 start2d, end2d;

			auto point_value = ( hitbox - start ) / static_cast< float >( 10 );

			std::vector<vec3_t> points;
			points.reserve ( 10 );
			std::vector<ImVec2> points2s;
			points2s.reserve ( 10 );

			ImVec2 pp;

			float a1 = settings->bezier_curve [ 0 ];
			float a2 = settings->bezier_curve [ 1 ];
			float a3 = settings->bezier_curve [ 2 ];
			float a4 = settings->bezier_curve [ 3 ];


			points.push_back ( start );
			points.push_back ( hitbox );

			vec3_t a = start;
			vec3_t d = hitbox;

			vec3_t b, c;

			b.x = std::lerp ( start.x, hitbox.x, a2 );
			b.y = std::lerp ( start.y, hitbox.y, 1.f - a1 );
			b.z = std::lerp ( hitbox.z, start.z, a2 );



			c.x = std::lerp ( hitbox.x, start.x, a2 );
			c.y = std::lerp ( hitbox.y, start.y, a3 );
			c.z = std::lerp ( hitbox.z, start.z, a4 );



			points.push_back ( c );
			points.push_back ( b );

			float ddt = 1.0f / 20;

			for ( float dt = 0; dt < 1; dt += ddt ) {
				vec3_t p = math::bezier ( dt, a, b, c, d );
				points.push_back ( p );
			}

			auto & tran_frame = active_target->coord_frame ( );

			for ( auto & p : points ) {

				if ( visuals::world_to_screen ( p, pp ) )
					points2s.push_back ( pp );
			}
			//	}
			int i = 1;

			for ( auto & p : points2s ) {
				i += 50;

				render->AddCircleFilled ( p, 3.f, ImColor ( i, i, 255, 255 ) );
			}

		}
	}