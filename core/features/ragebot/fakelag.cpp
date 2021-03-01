#include "../features.hpp"


void fake_lag::create_move( bool& send_packet ) {

	if ( !local_player::m_data.alive )
		return;



	uint32_t choke_target = static_cast< uint32_t>(config.antiaim_fakelag);
	int player_state = 0;
	if ( local_player::m_data.pointer->is_moving ( ) )
		player_state = 1;
	else
		player_state = 0;

	if ( local_player::m_data.pointer->is_in_air ( ) )
		player_state = 2;
	if ( shifting::_shift.allow )
		choke_target = 1;
	else {
		if ( localdata.fakeducking )
			choke_target = 14;
	}
	
	send_packet = interfaces::clientstate->m_choked_commands >= choke_target;
	if ( anti_aim::choke_next_tick  ) {
		send_packet = !config.
			antiaim_fakelag_flags [ 4 ];
		anti_aim::choke_next_tick = false;
	}
	//on_peek ( cmd, send_packet );
	
}
