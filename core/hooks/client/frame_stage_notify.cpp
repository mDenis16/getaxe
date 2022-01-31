#include "../../helpers/helpers.h"
#include "../hooks.hpp"


#include "../../features/animations/animations_manager.h"
#include "../../features/visuals/visuals.h"
#include <UIFramework/framework_internal.h>

namespace hooks::callback
{
	static void clear_ref_counted_vector(c_utl_vector<IRefCounted*>& vec)
	{
		for (auto& e : vec)
		{
			if (e)
			{
				e->unreference();
				e = nullptr;
			}
		}
		vec.removeAll();
	}

	static void force_item_update(weapon_t* item)
	{
		auto& view = item->m_Item();
		item->m_bCustomMaterialInitialized() = (item->m_nFallbackPaintKit() <= 0);
		item->m_CustomMaterials().removeAll();
		view.m_CustomMaterials().removeAll();
		clear_ref_counted_vector(view.m_VisualsDataProcessors());
		item->post_data_update(0);
		item->on_data_changed(0);
	}
	player_t* util_player_by_index(int index)
	{
		typedef player_t* (__fastcall* player_index)(int);
		static player_index c_index = (player_index)utilities::pattern_scan("server.dll", "85 C9 7E 32 A1 ? ? ? ?");

		if (!c_index)
			return nullptr;

		return c_index(index);
	}
	void __stdcall frame_stage_notify(client_frame_stage_t stage)
	{
		if (hooks::unloading)
			return reinterpret_cast<void(__stdcall*)(client_frame_stage_t)>(list.at(hook_index::frame_stage_notify)->
				original)(stage);

		static float m_update_time = 0.f;

		/*if (stage == FRAME_START)
			ui::handle_image_data();*/

		if (!interfaces::engine->is_in_game())
			return reinterpret_cast<void(__stdcall*)(client_frame_stage_t)>(list.at(hook_index::frame_stage_notify)->
				original)(stage);

		if (!interfaces::engine->is_connected())
			return reinterpret_cast<void(__stdcall*)(client_frame_stage_t)>(list.at(hook_index::frame_stage_notify)->
				original)(stage);

		//local_player::set_ptr(reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player())));

		if (stage == FRAME_START)
		{
			visuals::removals::remove_flash();
			visuals::removals::remove_smoke();
			visuals::handler->intialization();
		}


		//csgo::local_player = local_player::ptr();


		//if (local_player::data().alive && local_player::ptr() && local_player::ptr()->is_alive())
		//{
		//	//	animations::update_local_update_start ( stage );
		//}



		static float next_update = interfaces::globals->cur_time;
		if (stage == FRAME_RENDER_START)
		{
			visuals::handler->on_update();
			
			
		}

		//bool run = false;

		//if (run && stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START && local_player::ptr() && local_player::data().in_game && local_player::ptr()
		//	->is_alive())
		//{
		//	static auto g_CSInventoryManager = *reinterpret_cast<CSInventoryManager**>(utilities::pattern_scan(
		//		"client.dll", "B9 ?? ?? ?? ?? 8D 44 24 10 89 54 24 14") + 0x1);

		//	static auto local_inventory_offset = *reinterpret_cast<uintptr_t*>(utilities::pattern_scan(
		//		"client.dll", "8B 8F ? ? ? ? 0F B7 C0 50") + 0x2);


		//	if (interfaces::inputsystem->is_button_down(KEY_C) && (next_update <= interfaces::globals->cur_time))
		//	{
		//		if (local_player::data().active_weapon)
		//		{
		//			auto econItemView = local_player::data().active_weapon->GetEconItemView();
		//			if (econItemView)
		//			{
		//				static auto UpdateItem = reinterpret_cast<void* (__thiscall*)(void*, void*)>(
		//					utilities::pattern_scan_rel_function("client.dll",
		//					                                     "E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 8B CE E8 ? ? ? ? EB 3B",
		//					                                     1));


		//				UpdateItem(econItemView, econItemView);
		//			}
		//		}

		//		next_update = interfaces::globals->cur_time + 2.f;
		//	}

		//	auto LocalInventory = *reinterpret_cast<CPlayerInventory**>(g_CSInventoryManager + local_inventory_offset);


		//	auto LocalPlayerInfo = local_player::data().pointer->get_info();


		//	UINT* hWeapons = local_player::ptr()->get_weapons();

		//	if (!hWeapons)
		//		return reinterpret_cast<void(__stdcall*)(client_frame_stage_t)>(list.at(hook_index::frame_stage_notify)
		//			->original)(stage); /*original function call*/


		//	for (int nIndex = 0; hWeapons[nIndex]; nIndex++)
		//	{
		//		auto pWeapon = static_cast<weapon_t*>(interfaces::entity_list->get_client_entity_handle(
		//			hWeapons[nIndex]));

		//		if (!pWeapon)
		//			continue;

		//		if (LocalPlayerInfo.low.xuidlow != pWeapon->m_OriginalOwnerXuidLow())
		//			continue;

		//		if (LocalPlayerInfo.low.xuidhigh != pWeapon->m_OriginalOwnerXuidHigh())
		//			continue;

		//		auto econItemView = pWeapon->GetEconItemView();
		//		if (!econItemView)
		//			continue;

		//		auto weaponDefinition = econItemView->GetStaticData();
		//		if (!weaponDefinition)
		//			continue;

		//		auto equipt_position = weaponDefinition->GetEquippedPosition();

		//		std::cout << " weapon with equipt_position " << equipt_position << std::endl;

		//		auto econSkinData = LocalInventory->GetItemInLoadout(
		//			local_player::data().pointer->team(), weaponDefinition->GetEquippedPosition());
		//		if (!econSkinData)
		//			continue;

		//		auto econData = econSkinData->GetSOCData();

		//		if (!econData)
		//			continue;

		//		

		//		pWeapon->m_iAccountID() = LocalInventory->GetSteamID();
		//		pWeapon->itemIDHigh() = static_cast<std::uint32_t>(*econData->GetItemID() >> 32);
		//		pWeapon->itemIDLow() = static_cast<std::uint32_t>(*econData->GetItemID() & 0xFFFFFFFF);
		//	}
		//}

	

		reinterpret_cast<void(__stdcall*)(client_frame_stage_t)>(list.at(hook_index::frame_stage_notify)->
		                                                              original)(stage); /*original function call*/
		


		
		if (stage == FRAME_RENDER_START)
		{
			for (int i = 1; i <= interfaces::globals->max_clients; i++) {
				player_t* player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));

				if (!player)
					continue;

				if (player->dormant())
					continue;

				if (!player->is_alive())
					continue;


				float fDuration = -1.f;
				static uintptr_t pCall = (uintptr_t)utilities::pattern_scan("server.dll", "55 8B EC 81 EC ? ? ? ? 53 56 8B 35 ? ? ? ? 8B D9 57 8B CE");
				PVOID pEntity = util_player_by_index(i);

				if (pEntity)
				{
					__asm
					{
						pushad
						movss xmm1, fDuration
						push 0 //bool monoColor
						mov ecx, pEntity
						call pCall
						popad
					}
				}

			}
		}
 	}
}
