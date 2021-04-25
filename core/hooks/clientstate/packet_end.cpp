#include "../../features/features.hpp"


#include "../hooks.hpp"


namespace hooks::callback {
	void __fastcall packet_end ( void * ecx, void * )
	{
		/*local_player::m_data.pointer = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( interfaces::engine->get_local_player ( ) ) );

		if ( !local_player::m_data.pointer->is_alive ( ) )  //-V807
		{
			local_player::m_data.data.clear ( );
			return packet_end_original ( ecx );
		}

		if ( *( int * ) ( ( uintptr_t ) ecx + 0x164 ) == *( int * ) ( ( uintptr_t ) ecx + 0x16C ) ) {
			auto ack_cmd = *( int * ) ( ( uintptr_t ) ecx + 0x4D2C );
			auto correct = std::find_if ( local_player::m_data.data.begin ( ), local_player::m_data.data.end ( ),
				[ &ack_cmd ] ( const local_player::correction_data & other_data ) {
				return other_data.command_number == ack_cmd;
			}
			);

			auto netchannel = interfaces::engine->get_net_channel_info ( );

			if ( netchannel && correct != local_player::m_data.data.end ( ) ) {
				if ( local_player::m_data.last_velocity_modifier > local_player::m_data.pointer->m_flVelocityModifier ( ) + 0.1f ) {
					auto weapon = local_player::m_data.pointer->active_weapon ( );

					if ( !weapon || weapon->item_definition_index ( ) != weapon_revolver && !weapon->is_grenade ( ) ) //-V648
					{
						for ( auto & number : local_player::m_data.choked_number ) {
							auto cmd = &interfaces::input->m_commands [ number % 150 ];
							auto verified = &interfaces::input->m_verified [ number % 150 ];

							if ( cmd->buttons & ( cmd_buttons::in_attack | cmd_buttons::in_attack2 ) ) {
								cmd->buttons &= ~cmd_buttons::in_attack;

								verified->m_cmd = *cmd;
								verified->m_crc = cmd->get_checksum ( );
							}
						}
					}
				}

				local_player::m_data.last_velocity_modifier = local_player::m_data.pointer->m_flVelocityModifier ( );
			}
		}*/

		return  packet_end_original ( ecx );
	}

}