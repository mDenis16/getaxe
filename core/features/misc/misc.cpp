#include "../features.hpp"
#include "../../helpers/helpers.h"
/*
void misc::movement::fake_duck ( c_usercmd * cmd ) {
	cmd->buttons |= in_bullrush;

	if ( !interfaces::clientstate )
		return;

	local_player::m_data.fakeducking = interfaces::inputsystem->is_button_down ( button_code_t::KEY_C );
	if ( !local_player::m_data.fakeducking )
		return;

	

	bool should_crouch = interfaces::clientstate->m_choked_commands >= 14 / 2;
	
	//interfaces::console->console_printf ( "fakeducking choke ticks %i should_crouch %i \n", interfaces::clientstate->m_choked_commands, should_crouch );
	if ( should_crouch )
		cmd->buttons |= ( 1 << 2 ) ;
	else
		cmd->buttons &= ~( 1 << 2 ) ;
}
void misc::movement::slide ( c_usercmd * cmd ) {
	if ( cmd->buttons & in_forward && cmd->buttons & in_back ) {
		cmd->buttons &= ~in_forward;
		cmd->buttons &= ~in_back;
	}

	if ( cmd->buttons & in_moveleft && cmd->buttons & in_moveright ) {
		cmd->buttons &= ~in_moveleft;
		cmd->buttons &= ~in_moveright;
	}

}
void misc::movement::bunny_hop(c_usercmd* cmd) {
	

	static bool bLastJumped = false;
	static bool bShouldFake = false;

	if ( !bLastJumped && bShouldFake ) {
		bShouldFake = false;
		cmd->buttons |= in_jump;
	}
	else if ( cmd->buttons & in_jump ) {
		if ( local_player::m_data.pointer->flags ( ) & fl_onground ) {
			bShouldFake = bLastJumped = true;
		}
		else {
			cmd->buttons &= ~in_jump;
			bLastJumped = false;
		}
	}
	else {
		bShouldFake = bLastJumped = false;
	}
	if ( local_player::m_data.pointer->move_type() == movetype_noclip || local_player::m_data.pointer->move_type ( )  == movetype_ladder )
		return;

	if ( cmd->buttons & in_speed && local_player::m_data.pointer->velocity ( ).Length2D ( ) < 1.f )
		return; // doesn't allow strafe when you hold shift and you're not moving

	static float yaw_add = 0.f;
	static const auto cl_sidespeed = interfaces::console->get_convar ( "cl_sidespeed" );
	if ( !( local_player::m_data.pointer->flags ( ) & fl_onground ) ) {
		bool back = cmd->buttons & in_back;
		bool forward = cmd->buttons & in_forward;
		bool right = cmd->buttons & in_moveleft;
		bool left = cmd->buttons & in_moveright;

		if ( back ) {
			yaw_add = -180.f;
			if ( right )
				yaw_add -= 45.f;
			else if ( left )
				yaw_add += 45.f;
		}
		else if ( right ) {
			yaw_add = 90.f;
			if ( back )
				yaw_add += 45.f;
			else if ( forward )
				yaw_add -= 45.f;
		}
		else if ( left ) {
			yaw_add = -90.f;
			if ( back )
				yaw_add -= 45.f;
			else if ( forward )
				yaw_add += 45.f;
		}
		else {
			yaw_add = 0.f;
		}
	 
		local_player::m_data.orig_viewangle.y += yaw_add;
		cmd->forwardmove = 0.f;
		cmd->sidemove = 0.f;

		const auto delta = math::normalize_yaw ( local_player::m_data.orig_viewangle.y - RAD2DEG ( atan2 ( local_player::m_data.pointer->velocity().y, local_player::m_data.pointer->velocity ( ).x ) ) );

		cmd->sidemove = delta > 0.f ? -cl_sidespeed->get_float ( ) : cl_sidespeed->get_float ( );

		local_player::m_data.orig_viewangle.y = math::normalize_yaw ( local_player::m_data.orig_viewangle.y - delta );
	}
};

void misc::thirdperson::think( ) {
	if ( !local_player::m_data.pointer )
		return;
	if ( !interfaces::engine->is_in_game( ) )
		return;
	if ( !interfaces::engine->is_connected( ) )
		return;




	static auto current_fraction = 0.0f;
	static vec3_t vecAngles;
	interfaces::engine->get_view_angles( vecAngles );
	float ideal_distance = config.local.thirdperson_distance;

	interfaces::input->m_camera_in_third_person = config.local.thirdperson;

	if ( interfaces::input->m_camera_in_third_person && local_player::m_data.pointer->health( ) > 0 ) {

			    vec3_t inverse_angles = vecAngles;

				inverse_angles.x *= -1.f, inverse_angles.y += 180.f;

				vec3_t direction = math::angle_vector ( inverse_angles );

				trace_world_only filter;
				trace_t trace;
				ray_t ray;
			

				ray.initialize ( local_player::m_data.pointer->origin ( ) + local_player::m_data.pointer->view_offset ( ), ( local_player::m_data.pointer->origin ( ) + local_player::m_data.pointer->view_offset ( ) ) + ( direction * ( ideal_distance + 5.f ) ) );
				interfaces::trace_ray->trace_ray ( ray, MASK_SHOT_HULL, &filter, &trace );


			
		
			 ideal_distance *= trace.flFraction;
	
			interfaces::input->m_camera_offset = interfaces::input->m_camera_offset = vec3_t ( vecAngles.x, vecAngles.y, ideal_distance );
		
	}
	else {
		interfaces::input->m_camera_offset = vec3_t( vecAngles.x, vecAngles.y, 0 );
	}

}

*/