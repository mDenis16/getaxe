#include "../features.hpp"

c_engine_prediction * engine_prediction = new c_engine_prediction ( );

c_engine_prediction::c_engine_prediction ( ) {

	if ( !move_data )
		move_data = ::operator new( 182 );
	
}

 c_engine_prediction::~c_engine_prediction ( ) {
	if ( move_data )
		::operator delete( move_data );
}


void c_engine_prediction::run_think ( ) {

	
	int thinktick = player->m_nNextThinkTick ( );


	if ( thinktick <= 0 || thinktick > player->get_tick_base ( ) )
		return;

	static auto unknown_fn = reinterpret_cast< void ( __thiscall * )( int ) >( utilities::pattern_scan ( "client.dll", "55 8B EC 56 57 8B F9 8B B7 ? ? ? ? 8B" ) );
	unknown_fn ( -1 );

	player->think ( );

}
void c_engine_prediction::sig_scan ( ) {

	if ( !prediction_random_seed || !prediction_player ) {
		prediction_random_seed = *reinterpret_cast< int ** >( utilities::pattern_scan ( "client.dll", "A3 ? ? ? ? 66 0F 6E 86" ) + 0x1 );
		prediction_player = *reinterpret_cast< int ** >( utilities::pattern_scan ( "client.dll", "89 35 ? ? ? ? F3 0F 10 48" ) + 0x2 );
	}


}
void c_engine_prediction::start ( player_t * _player, c_usercmd * cmd ) {
	if ( !interfaces::model_cache )
		return;

	if ( !local_player::available ( ) )
		return;

	player = _player;

	interfaces::globals->cur_time = player->get_tick_base ( ) * interfaces::globals->interval_per_tick;
	interfaces::globals->frame_time = interfaces::prediction->EnginePaused ? 0.0f : interfaces::globals->interval_per_tick;

	sig_scan ( );

	// CPlayerMove::StartCommand
	{
		*reinterpret_cast< std::uint32_t * >( reinterpret_cast< std::uint32_t >( player ) + 0x3348) = reinterpret_cast < std::uint32_t >( cmd );

		*prediction_random_seed = cmd ? cmd->randomseed : -1;
		*prediction_player = reinterpret_cast< int >( player );
	}


	interfaces::move_helper->set_host ( player );

	interfaces::game_movement->start_track_prediction_errors ( player ); // @BigMoneyBoss - (*(void (__thiscall **)(int *, _DWORD *))(game_movement + 12))(&game_movement, player);

	if ( cmd->weaponselect != 0 ) {
		weapon_t * weapon = reinterpret_cast< weapon_t * >( interfaces::entity_list->get_client_entity ( cmd->weaponselect ) );

		if ( weapon ) {
			weapon_info_t * weapon_data = weapon->get_weapon_data ( );

			if ( weapon_data )
				player->select_item ( weapon_data->szWeaponName, cmd->weaponsubtype );
		}
	}



	if ( cmd->impulse )
		*reinterpret_cast< std::uint32_t * >( reinterpret_cast< std::uint32_t >( player ) + 0x31FC ) = cmd->impulse;

	// CBasePlayer::UpdateButtonState
	{
		cmd->buttons |= *reinterpret_cast< std::uint32_t * >( reinterpret_cast< std::uint32_t >( player ) + 0x3330 );

		int m_nButtons = cmd->buttons;
		int * m_afButtonLast = reinterpret_cast< int * >( reinterpret_cast< std::uint32_t >( player ) + 0x3208);
		int buttonsChanged = m_nButtons ^ *m_afButtonLast;
		*reinterpret_cast< std::uint32_t * >( reinterpret_cast< std::uint32_t >( player ) + 0x31FC) = *m_afButtonLast;
		*m_afButtonLast = m_nButtons;
		*reinterpret_cast< std::uint32_t * >( reinterpret_cast< std::uint32_t >( player ) + 0x3200) = m_nButtons & buttonsChanged;
		*reinterpret_cast< std::uint32_t * >( reinterpret_cast< std::uint32_t >( player ) + 0x3204) = buttonsChanged & ~m_nButtons;
	}

	interfaces::prediction->check_moving_ground ( player, interfaces::globals->frame_time );

	player->set_local_viewangles ( cmd->viewangles );

	// CPlayerMove::RunPreThink
	{
		if ( player->physics_run_think ( 0 ) )
			player->pre_think ( );
	}

	// CPlayerMove::RunThink
	run_think ( );

	interfaces::prediction->setup_move ( player, cmd, interfaces::move_helper, reinterpret_cast< player_move_data * >( move_data ) );


	interfaces::game_movement->process_movement ( player, reinterpret_cast< player_move_data * >( move_data ) );

	interfaces::prediction->finish_move ( player, cmd, reinterpret_cast< player_move_data * >( move_data ) );

	interfaces::move_helper->process_impacts ( );

	// CPlayerMove::RunPostThink
	{
		auto post_think = [ ] ( player_t * player ) -> int {
			utilities::get_virtual_function< void ( __thiscall * )( void * ) > ( interfaces::model_cache, 33 )( interfaces::model_cache );

			if ( player->is_alive ( ) /* index 155 for vfunc */ || *reinterpret_cast< std::uint32_t * >( reinterpret_cast< std::uint32_t >( player ) + 0x3A81 ) ) {
				utilities::get_virtual_function< void ( __thiscall * )( void * ) > ( player, 339 )( player );

				if ( player->flags ( ) & 1 ) // FL_ONGROUND
					*reinterpret_cast< std::uintptr_t * >( std::uintptr_t ( player ) + 0x3014 ) = 0;

				if ( *reinterpret_cast< std::uint32_t * >( reinterpret_cast< std::uint32_t >( player ) + 0x28BC ) == -1 )
					utilities::get_virtual_function< void ( __thiscall * )( void *, int ) > ( player, 218 )( player, 0 );

				utilities::get_virtual_function< void ( __thiscall * )( void * ) > ( player, 219 )( player );

				static auto post_think_v_physics = reinterpret_cast< bool ( __thiscall * )( player_t * ) >( utilities::pattern_scan ( "client.dll", "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 8B D9 56 57 83 BB" ) );
				post_think_v_physics ( player );
			}

			static auto simulate_player_simulated_entities = reinterpret_cast< bool ( __thiscall * )( player_t * ) >( utilities::pattern_scan ( "client.dll", "56 8B F1 57 8B BE ? ? ? ? 83 EF 01 78 72" ) );
			simulate_player_simulated_entities ( player );

			return utilities::get_virtual_function< int ( __thiscall * )( void * ) > ( interfaces::model_cache, 34 )( interfaces::model_cache );
		};

		post_think ( player );

		//-----------------------------------------------------------------------------
// Purpose: Called after any movement processing
// Input  : *player - 
//-----------------------------------------------------------------------------

		*prediction_random_seed = NULL;
		*prediction_player = NULL;

		interfaces::game_movement->Reset ( );
	}
}