#include "../features.hpp"
#include "../../helpers/helpers.h"
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

	static bool enabled = false;
	static bool was_in_thirdperson = false;
	static auto thirdperson = false;

	thirdperson = true;

	static auto pressed = false;

	static auto key = false;

	if ( interfaces::inputsystem->is_button_down( button_code_t::MOUSE_MIDDLE ) && !pressed )
		pressed = true;
	else if ( !interfaces::inputsystem->is_button_down( button_code_t::MOUSE_MIDDLE ) && pressed ) {
		pressed = false;
		key = !key;
	}

	if ( key )
		thirdperson = false;

	static auto current_fraction = 0.0f;
	static vec3_t vecAngles;
	interfaces::engine->get_view_angles( vecAngles );

	if ( thirdperson && local_player::m_data.pointer->health( ) > 0 ) {
		if ( !interfaces::input->m_camera_in_third_person ) {


			float ideal_distance = 100.f;
			vec3_t inverse_angles = vecAngles;

				inverse_angles.x *= -1.f, inverse_angles.y += 180.f;

				vec3_t direction = math::angle_vector ( inverse_angles );

				trace_world_only filter;
				trace_t trace;
				ray_t ray;
			

				ray.initialize ( local_player::m_data.pointer->origin ( ) + local_player::m_data.pointer->view_offset ( ), ( local_player::m_data.pointer->origin ( ) + local_player::m_data.pointer->view_offset ( ) ) + ( direction * ( ideal_distance + 5.f ) ) );
				interfaces::trace_ray->trace_ray ( ray, MASK_SHOT_HULL, &filter, &trace );


			
		
			 ideal_distance *= trace.flFraction;
			

			interfaces::input->m_camera_in_third_person = true;
			interfaces::input->m_camera_offset = interfaces::input->m_camera_offset = vec3_t ( vecAngles.x, vecAngles.y, ideal_distance );
		}
	}
	else {
		interfaces::input->m_camera_in_third_person = false;
		interfaces::input->m_camera_offset = vec3_t( vecAngles.x, vecAngles.y, 0 );
	}

}

void misc::removals::remove_smoke( ) {
	if ( !config.visuals_world_removals[2] )
		return;

	if ( !interfaces::engine->is_connected( ) && !interfaces::engine->is_in_game( ) )
		return;

	static auto smoke_count = *reinterpret_cast< int** >( utilities::pattern_scan( "client.dll", "A3 ? ? ? ? 57 8B CB" ) + 1 );

	static std::vector<const char*> smoke_materials = {
		"particle/vistasmokev1/vistasmokev1_fire",
		"particle/vistasmokev1/vistasmokev1_smokegrenade",
		"particle/vistasmokev1/vistasmokev1_emods",
		"particle/vistasmokev1/vistasmokev1_emods_impactdust"
	};

	for ( auto material_name : smoke_materials ) {
		i_material* smoke = interfaces::material_system->find_material( material_name, TEXTURE_GROUP_OTHER );
		smoke->increment_reference_count( );
		smoke->set_material_var_flag( material_var_wireframe, true );

		*( int* ) smoke_count = 0;
	}
}
void copy_command ( c_usercmd * cmd, int tickbase_shift ) {
	static auto cl_forwardspeed = interfaces::console->get_convar (  ( "cl_forwardspeed" ) );
	static auto cl_sidespeed = interfaces::console->get_convar (  ( "cl_sidespeed" ) );
	bool slow_teleport = false; /*in menuu*/
	if ( slow_teleport ) {
		cmd->forwardmove = 0.0f;
		cmd->sidemove = 0.0f;
	}
	else {
		if ( local_player::m_data.original_forwardmove >= 5.0f )
			cmd->forwardmove = cl_forwardspeed->get_float ( );
		else if ( local_player::m_data.original_forwardmove <= -5.0f )
			cmd->forwardmove = -cl_forwardspeed->get_float ( );

		if ( local_player::m_data.original_sidemove >= 5.0f )
			cmd->sidemove = cl_sidespeed->get_float ( );
		else if ( local_player::m_data.original_sidemove <= -5.0f )
			cmd->sidemove = -cl_sidespeed->get_float ( );
	}

	auto commands_to_add = 0;

	do {
		auto sequence_number = commands_to_add + cmd->command_number;

		auto command = interfaces::input->get_user_cmd ( sequence_number );
		auto verified_command = interfaces::input->get_verified_user_cmd ( sequence_number );

		memcpy ( command, cmd, sizeof ( c_usercmd ) ); //-V598

		if ( command->tick_count != INT_MAX && interfaces::clientstate->m_delta_tick > 0 )
			interfaces::prediction->update ( interfaces::clientstate->m_delta_tick, true,interfaces::clientstate->m_last_command_ack, interfaces::clientstate->m_last_outgoing_command + interfaces::clientstate->m_choked_commands );

		command->command_number = sequence_number;
		command->hasbeenpredicted = command->tick_count != INT_MAX;

		++interfaces::clientstate->m_choked_commands; //-V807

		if ( interfaces::clientstate->m_net_channel ) {
			++interfaces::clientstate->m_net_channel->choked_packets;
			++interfaces::clientstate->m_net_channel->out_sequence_nr;
		}
		command->viewangles.angle_normalize ( );


		memcpy ( &verified_command->m_cmd, command, sizeof ( c_usercmd ) ); //-V598
		verified_command->m_crc = command->get_checksum ( );

		++commands_to_add;
	} while ( commands_to_add != tickbase_shift );

	*( bool * ) ( ( uintptr_t ) interfaces::prediction + 0x24 ) = true;
	*( int * ) ( ( uintptr_t ) interfaces::prediction + 0x1C ) = 0;
}

void misc::removals::remove_flash( ) {
	if ( !config.visuals_world_removals [ 1 ] )
		return;
	if ( !local_player::m_data.alive )
		return;

	if ( local_player::m_data.pointer->flash_duration( ) > 0.0f )
		local_player::m_data.pointer->flash_duration( ) = 0.0f;
}
bool misc::can_fire ( weapon_t* weapon, bool check_revolver ) {
	if ( shifting::_shift.shift_ticks == config.ragebot_double_tap_ticks )
		return false;

	if ( !weapon ) //-V704
		return false;

	if ( !weapon->is_non_aim ( ) && weapon->is_empty ( ) )
		return false;

	const auto nci = interfaces::engine->get_net_channel_info ( );

	if ( !nci )
		return false;

	auto owner = ( player_t * ) interfaces::entity_list->get_client_entity_handle ( weapon->owner_handle ( ) );

	if ( !owner )
		return false;

	auto server_time = math::ticks_to_time ( localdata.fixed_tickbase );

	if ( server_time < weapon->next_primary_attack ( ) )
		return false;

	if ( server_time < owner->next_attack ( ) )
		return false;


	return true;
}
