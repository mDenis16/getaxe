#include "helpers.h"
#include "../features/features.hpp"

namespace local_player {
	data m_data = {};

	void detect_game ( ) {
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
	}
	void begin_tick ( c_usercmd * cmd ) {
		csgo::cmd = cmd;
		detect_game ( );
		const auto get_random_seed = [ & ] ( ) {
			using o_fn = unsigned long ( __cdecl * )( std::uintptr_t );
			static auto offset = utilities::pattern_scan ( "client.dll", "55 8B EC 83 E4 F8 83 EC 70 6A 58" );
			static auto MD5_PseudoRandom = reinterpret_cast< o_fn >( offset );
			return MD5_PseudoRandom ( cmd->command_number ) & 0x7FFFFFFF;
		};

		cmd->randomseed = get_random_seed ( );


		m_data.orig_viewangle = cmd->viewangles;
		m_data.pointer = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( interfaces::engine->get_local_player ( ) ) );
		csgo::local_player = local_player::m_data.pointer;
		m_data.alive = m_data.pointer->health ( ) > 0;
		if ( m_data.alive ) {
			m_data.active_weapon = m_data.pointer->active_weapon ( );
			if ( m_data.active_weapon ) {
				m_data.weapon_data = m_data.active_weapon->get_weapon_data ( );
				m_data.have_weapon = true;
			}
			m_data.eye_position = engine_prediction::unpredicted_eye;
			m_data.velocity = engine_prediction::unpredicted_velocity;
		}
	}
	bool available ( ) {

		if ( !interfaces::engine->is_in_game ( ) )
			return false;

		if ( !m_data.pointer )
			return false;

		if ( !m_data.alive )
			return false;

		return true;
	}

	void end_tick ( c_usercmd * cmd ) {
		auto & correct = local_player::m_data.data.emplace_front ( );

		correct.command_number = cmd->command_number;
		correct.choked_commands = interfaces::clientstate->m_choked_commands + 1;
		correct.tickcount = interfaces::globals->tick_count;

		if ( csgo::send_packet )
			local_player::m_data.choked_number.clear ( );
		else
			local_player::m_data.choked_number.emplace_back ( correct.command_number );

		while ( static_cast< int >(local_player::m_data.data.size ( )) > static_cast<int>(( 2.0f / interfaces::globals->interval_per_tick ) ))
			local_player::m_data.data.pop_back ( );

		auto & out = local_player::m_data.commands.emplace_back ( );

		out.is_outgoing = csgo::send_packet;
		out.is_used = false;
		out.command_number = cmd->command_number;
		out.previous_command_number = 0;

		while ( static_cast< int >( local_player::m_data.commands.size ( )) > static_cast<int>( ( 1.0f / interfaces::globals->interval_per_tick )) )
			local_player::m_data.commands.pop_front ( );

		if ( !csgo::send_packet ) {
			auto net_channel = interfaces::clientstate->m_net_channel;

			if ( net_channel->choked_packets > 0 && !( net_channel->choked_packets % 4 ) ) {
				auto backup_choke = net_channel->choked_packets;
				net_channel->choked_packets = 0;

				net_channel->send_datagram ( );
				--net_channel->out_sequence_nr;

				net_channel->choked_packets = backup_choke;
			}
		}
	}
}