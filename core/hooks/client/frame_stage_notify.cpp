#include "../../features/features.hpp"
#include "../hooks.hpp"


namespace hooks::callback {
	void __stdcall frame_stage_notify ( client_frame_stage_t stage ) {

		if ( stage == FRAME_START )
			ui::handle_image_data ( );

		if ( !interfaces::engine->is_in_game ( ) )
			return reinterpret_cast< void ( __stdcall * )( client_frame_stage_t ) >( list.at ( hook_index::frame_stage_notify )->original )( stage );

		if ( !interfaces::engine->is_connected ( ) )
			return reinterpret_cast< void ( __stdcall * )( client_frame_stage_t ) >( list.at ( hook_index::frame_stage_notify )->original )( stage );

		local_player::m_data.pointer = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( interfaces::engine->get_local_player ( ) ) );

		if ( stage == FRAME_START ) {
			visuals::removals::remove_flash ( );
			visuals::removals::remove_smoke ( );
			visuals::handler->intialization ( );
		}



		csgo::local_player = local_player::m_data.pointer;


		if ( localdata.alive && local_pointer && local_pointer->is_alive ( ) ) {
			animations::update_local_update_start ( stage );
		}




		reinterpret_cast< void ( __stdcall * )( client_frame_stage_t ) >( list.at ( hook_index::frame_stage_notify )->original )( stage ); /*original function call*/

		if ( local_player::available ( ) ) { /*update animations here to get interpolated origin by game. ps: it uses uninterpolated data*/
			animations::update_animations_update_end ( stage );


			//animations::update_fake_animation ( );
		}

		if ( stage == client_frame_stage_t::FRAME_RENDER_START ) {
			visuals::handler->on_update ( );

		}

		if ( stage == client_frame_stage_t::FRAME_NET_UPDATE_END && local_pointer && localdata.in_game && local_pointer->is_alive() ) {
			return;

			UINT * hWeapons = local_pointer->get_weapons ( );

			if ( !hWeapons )
				return;

			auto LocalPlayerInfo = local_pointer->get_info ( );
			static auto g_CSInventoryManager = *reinterpret_cast< CSInventoryManager ** >( utilities::pattern_scan ( "client.dll", "B9 ?? ?? ?? ?? 8D 44 24 10 89 54 24 14" ) + 0x1 );

			static auto local_inventory_offset = *reinterpret_cast< uintptr_t * >( utilities::pattern_scan ( "client.dll", "8B 8F ? ? ? ? 0F B7 C0 50" ) + 0x2 );

			//auto LocalInventory = g_CSInventoryManager->GetLocalPlayerInventory ( );

			static int itm_id = 500;
			itm_id++;



			auto LocalInventory = *reinterpret_cast< CPlayerInventory ** >( g_CSInventoryManager + local_inventory_offset );


			//for ( int nIndex = 0; hWeapons [ nIndex ]; nIndex++ ) {
				weapon_t * pWeapon = local_pointer->active_weapon ( );//( weapon_t * ) interfaces::entity_list->get_client_entity_handle ( hWeapons [ nIndex ] );

				if ( !pWeapon )
					return;


				//if ( LocalPlayerInfo.low.xuidlow != pWeapon->m_OriginalOwnerXuidLow ( ) )
					//return;

				//if ( LocalPlayerInfo.low.xuidhigh != pWeapon->m_OriginalOwnerXuidHigh ( ) )
					//return;

				auto econItemView = pWeapon->GetEconItemView ( );
				if ( !econItemView )
					return;
			
			//		std::cout << "	econItemView->m_iItemDefinitionIndex()" << econItemView->m_iItemDefinitionIndex ( ) << std::endl;
				auto weaponDefinition = econItemView->GetStaticData ( );
				if ( !weaponDefinition )
					return;

			
				auto econSkinData = LocalInventory->GetItemInLoadout ( local_pointer->team ( ), weaponDefinition->GetEquippedPosition ( ) );
				if ( !econSkinData )
					return;
				
				
				auto econData = econSkinData->GetSOCData ( );

				if ( !econData )
					return;

			

				//econItemView->m_iAccountID ( ) = LocalPlayerInfo.low.xuidlow;
				//econItemView->m_iItemIDLow ( ) = *econData->GetItemID ( );
			//}
		}
	}
}