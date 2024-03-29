#include "helpers.h"


local_player* local_player::instance;

	/*void fix_movement ( c_usercmd * cmd, vec3_t& wish_angles ) {
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

	}*/


	/*void detect_game ( ) {
		static int last_tick_count = 0;
		static bool waiting = false;
		bool real_in_game = interfaces::engine->is_in_game ( );

		if ( !m_data.in_game && real_in_game && !waiting ) {
			DEBUG_LOG ( "DETCTED IN GAME \n" );
			last_tick_count = interfaces::globals->tick_count;


			waiting = true;
		}
		else if ( !real_in_game ) {
			m_data.in_game = false;
			DEBUG_LOG ( "NO LONGER IN GAME \n" );
		
			waiting = false;
		}
		if ( real_in_game && !m_data.in_game && std::abs ( interfaces::globals->tick_count - last_tick_count ) > 50 ) {
			m_data.in_game = true;
			DEBUG_LOG ( "ARTIFICIAL IN GAME \n" );
		}
	}*/

    
	void local_player::begin_tick ( c_usercmd * cmd ) {
		
		auto m_data = i_local_data;

		
		const auto get_random_seed = [ & ] ( ) {
			using o_fn = unsigned long ( __cdecl * )( std::uintptr_t );
			static auto offset = utilities::pattern_scan ( "client.dll", "55 8B EC 83 E4 F8 83 EC 70 6A 58" );
			static auto MD5_PseudoRandom = reinterpret_cast< o_fn >( offset );
			return MD5_PseudoRandom ( cmd->command_number ) & 0x7FFFFFFF;
		};


	
		m_data.orig_viewangle = cmd->viewangles;
		m_data.pointer = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( interfaces::engine->get_local_player ( ) ) );


		//csgo::local_player = local_player::ptr();

		m_data.alive = m_data.pointer->health ( ) > 0;


		/*if (local_player::ptr() && m_data.alive && m_data.in_game ) {
			auto original_tickbase = local_player::ptr()->get_tick_base ( );
			cmd->randomseed = get_random_seed ( );

			m_data.active_weapon = m_data.pointer->active_weapon ( );
			if ( m_data.active_weapon ) {
				m_data.weapon_data = m_data.active_weapon->get_weapon_data ( );
				m_data.have_weapon = true;
			}
	

			local_player::data().eye_position = local_player::ptr()->get_eye_pos ( );


			engine_prediction->start(local_player::ptr(), cmd);
		}

		m_data.backup_tickbase = local_player::ptr()->get_tick_base ( );*/

		
	}
	bool local_player::available ( ) {

		if ( !interfaces::engine->is_in_game ( ) )
			return false;

		if ( !data().pointer )
			return false;

		if ( !data().alive )
			return false;

		return true;
	}

	void local_player::end_tick ( c_usercmd * cmd ) {

	/*	if ( local_player::data().alive ) {
			local_player::data().last_punch = local_player::ptr()->aim_punch_angle ( );
		}


		engine_prediction->end();*/

		//fix_movement ( cmd, local_player::data().orig_viewangle);


		/*auto & correct = local_player::data().data.emplace_front ( );

		correct.command_number = cmd->command_number;
		correct.choked_commands = interfaces::clientstate->m_choked_commands + 1;
		correct.tickcount = interfaces::globals->tick_count;

		if ( csgo::send_packet )
			local_player::data().choked_number.clear ( );
		else
			local_player::data().choked_number.emplace_back ( correct.command_number );

		while ( static_cast< int >(local_player::data().data.size ( )) > static_cast<int>(( 2.0f / interfaces::globals->interval_per_tick ) ))
			local_player::data().data.pop_back ( );

		auto & out = local_player::data().commands.emplace_back ( );

		out.is_outgoing = csgo::send_packet;
		out.is_used = false;
		out.command_number = cmd->command_number;
		out.previous_command_number = 0;

		while ( static_cast< int >( local_player::data().commands.size ( )) > static_cast<int>( ( 1.0f / interfaces::globals->interval_per_tick )) )
			local_player::data().commands.pop_front ( );

		if ( !csgo::send_packet ) {
			auto net_channel = interfaces::clientstate->m_net_channel;

			if ( net_channel->choked_packets > 0 && !( net_channel->choked_packets % 4 ) ) {
				auto backup_choke = net_channel->choked_packets;
				net_channel->choked_packets = 0;

				net_channel->send_datagram ( );
				--net_channel->out_sequence_nr;

				net_channel->choked_packets = backup_choke;
			}
		}*/



	}
	