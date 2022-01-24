#include "../features.hpp"

c_engine_prediction * engine_prediction = new c_engine_prediction ( );

c_engine_prediction::c_engine_prediction ( ) {

	
}

 c_engine_prediction::~c_engine_prediction ( ) {

}


void c_engine_prediction::run_think ( ) {

	
	auto GetNextThinkTick = reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint32_t>(player) + 63);
	static auto SetNextThink = reinterpret_cast<void(__thiscall*)(std::uint32_t*, int a2)>(utilities::pattern_scan("client.dll", "55 8B EC 56 57 8B F9 8B B7 ? ? ? ? 8B"));

	if ( *GetNextThinkTick != -1 && *GetNextThinkTick > 0 && *GetNextThinkTick <= player->get_tick_base ( ) )
	{
		*GetNextThinkTick = -1;
		SetNextThink((std::uint32_t*)local_pointer, 0);
	}

	


	player->think ( );

}
void c_engine_prediction::sig_scan ( ) {

	if ( !prediction_random_seed || !prediction_player ) {
		prediction_random_seed = *reinterpret_cast< int ** >( utilities::pattern_scan ( "client.dll", "A3 ? ? ? ? 66 0F 6E 86" ) + 0x1 );
		prediction_player = *reinterpret_cast< int ** >( utilities::pattern_scan ( "client.dll", "89 35 ? ? ? ? F3 0F 10 48" ) + 0x2 );
	}


}


void c_engine_prediction::start(player_t* _player, c_usercmd* cmd) {
	if (!interfaces::modelcache)
		return;

	if (!local_player::available())
		return;
	if (!interfaces::game_movement)
		return;


	player = _player;
	if (!_player) return;

	interfaces::globals->cur_time = player->get_tick_base() * interfaces::globals->interval_per_tick;
	interfaces::globals->frame_time = interfaces::prediction->EnginePaused ? 0.0f : interfaces::globals->interval_per_tick;
	//E8 ? ? ? ? 85 FF 75 0C
	static auto CopyUserCmd = reinterpret_cast<void(__thiscall*)(std::uint32_t*, std::uint32_t*)>(utilities::pattern_scan("client.dll", "E8 ? ? ? ? 85 FF 75 0C"));



	sig_scan();

	player_move_data move_data;
	memset(&move_data, 0, sizeof(player_move_data));
	// CPlayerMove::StartCommand
	
		
	*prediction_random_seed = cmd ? cmd->randomseed : -1;
		

	*reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint32_t>(player) + 0x3348) = reinterpret_cast <std::uint32_t>(cmd);
	//CopyUserCmd((std::uint32_t*)(player + 0x3298), (std::uint32_t*)cmd);

	*prediction_player = reinterpret_cast<std::uint32_t>(player);

	int backupButtons = cmd->buttons;

	cmd->buttons |= *(std::uint32_t*)(cmd + 0x3344);
	cmd->buttons &= ~*(std::uint32_t*)(cmd + 0x3340);

	

	interfaces::move_helper->set_host(player);
	interfaces::game_movement->start_track_prediction_errors(player); // @BigMoneyBoss - (*(void (__thiscall **)(int *, std::uint32_t *))(game_movement + 12))(&game_movement, player);


	//	(*(void(__thiscall**)(std::uint32_t*, std::uint32_t))(interfaces::game_movement + 12))((std::uint32_t*)interfaces::game_movement, (std::uint32_t)player);
	if ( cmd->weaponselect != 0 ) {
		weapon_t * weapon = reinterpret_cast< weapon_t * >( interfaces::entity_list->get_client_entity ( cmd->weaponselect ) );

		if ( weapon ) {
			weapon_info_t * weapon_data = weapon->get_weapon_data ( );

			if ( weapon_data )
				player->select_item ( weapon_data->szWeaponName, cmd->weaponsubtype );
		}
	}



	if (cmd->impulse)
		*reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint32_t>(player) + 0x320C) = cmd->impulse;

	int CmdButtons = cmd->buttons;
	int ButtonsChanged = CmdButtons ^ *(std::uint32_t*)(player + 0x3208);

	//	CBasePlayer::UpdateButtonState
	// player->m_afButtonLast = player->m_nButtons;
	*(std::uint32_t*)(player + 0x31FC) = *(std::uint32_t*)(player + 0x3208);

	// player->m_nButtons = CmdButtons;
	*(std::uint32_t*)(player + 0x3208) = CmdButtons;

	// player->m_afButtonPressed = CmdButtons & buttonsChanged;
	*(std::uint32_t*)(player + 0x3200) = CmdButtons & ButtonsChanged;

	// player->m_afButtonReleased = buttonsChanged & (~CmdButtons);
	*(std::uint32_t*)(player + 0x3204) = ButtonsChanged & ~CmdButtons;


	interfaces::prediction->check_moving_ground(player, interfaces::globals->frame_time);

	player->set_local_viewangles(cmd->viewangles);

	// CPlayerMove::RunPreThink
	{
		if (player->physics_run_think(0))
			player->pre_think();
	}

	auto GetNextThinkTick = reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint32_t>(player) + 0xFC);

	static auto SetNextThink = reinterpret_cast<void(__thiscall*)(std::uint32_t*, int a2)>(utilities::pattern_scan("client.dll", "55 8B EC 56 57 8B F9 8B B7 ? ? ? ? 8B"));

	if (*GetNextThinkTick != -1 && *GetNextThinkTick > 0 && *GetNextThinkTick <= player->get_tick_base())
	{
		*GetNextThinkTick = -1;
		SetNextThink((std::uint32_t*)local_pointer, 0);
		player->think();
	}

	interfaces::prediction->setup_move(player, cmd, interfaces::move_helper, &move_data);
	interfaces::game_movement->process_movement(player, &move_data);
	interfaces::prediction->finish_move(player, cmd, &move_data);

	interfaces::move_helper->process_impacts();


		auto post_think = [](player_t* player) -> int {
			utilities::get_virtual_function< void(__thiscall*)(void*) >(interfaces::modelcache, 33)(interfaces::modelcache);

			if (player->is_alive() /* index 156 for vfunc */ || *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint32_t>(player) + 39649)) {
				utilities::get_virtual_function< void(__thiscall*)(void*) >(player, 340)(player); // Update collision Bounds


				if (player->flags() & fl_onground) // FL_ONGROUND
					*reinterpret_cast<std::uintptr_t*>(std::uintptr_t(player) + 3081) = 0; // m_Local.m_flFallVelocity 


				if (*reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint32_t>(player) + 2608) == -1) // GetSequence
					utilities::get_virtual_function< void(__thiscall*)(void*, int) >(player, 219)(player, 0); // SetSequence

				utilities::get_virtual_function< void(__thiscall*)(void*) >(player, 220)(player); // StudioFrameAdvance

				static auto post_think_v_physics = reinterpret_cast<bool(__thiscall*)(player_t*)>(utilities::pattern_scan("client.dll", "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 8B D9 56 57 83 BB"));
				post_think_v_physics(player); // PostThinkVPhysics
			}

			static auto simulate_player_simulated_entities = reinterpret_cast<bool(__thiscall*)(player_t*)>(utilities::pattern_scan("client.dll", "56 8B F1 57 8B BE ? ? ? ? 83 EF 01 78 74"));
			simulate_player_simulated_entities(player); // SimulatePlayerSimulatedEntities

			return utilities::get_virtual_function< int(__thiscall*)(void*) >(interfaces::modelcache, 34)(interfaces::modelcache);
		};

		post_think(player);

		

		

		cmd->buttons = backupButtons;
}
