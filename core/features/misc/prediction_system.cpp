#include "../features.hpp"

void engineprediction::setup ( ) {
	if ( !local_player::available ( ) )
		return;

	
	if ( prediction_data.prediction_stage != SETUP )
		return;

	backup_data.flags = local_pointer->flags ( ); //-V807
	backup_data.velocity = local_pointer->velocity ( );

	prediction_data.old_curtime = interfaces::globals->cur_time; //-V807
	prediction_data.old_frametime = interfaces::globals->frame_time;

	interfaces::globals->cur_time = math::ticks_to_time ( local_pointer->get_tick_base ( ) );
	interfaces::globals->frame_time = interfaces::prediction->EnginePaused ? 0.0f : interfaces::globals->interval_per_tick;

	prediction_data.prediction_stage = PREDICT;
}

void engineprediction::predict ( c_usercmd * m_pcmd ) {
	if ( !local_player::available ( ) )
		return;


	if ( prediction_data.prediction_stage != PREDICT )
		return;

	if ( interfaces::clientstate->m_delta_tick > 0 )  //-V807
		interfaces::prediction->update ( interfaces::clientstate->m_delta_tick, true, interfaces::clientstate->m_last_command_ack, interfaces::clientstate->m_last_outgoing_command + interfaces::clientstate->m_choked_commands );

	if ( !prediction_data.prediction_random_seed )
		prediction_data.prediction_random_seed = *reinterpret_cast < int ** > ( utilities::pattern_scan (  ( "client.dll" ),  ( "A3 ? ? ? ? 66 0F 6E 86" ) ) + 0x1 );

	*prediction_data.prediction_random_seed = MD5_PseudoRandom ( m_pcmd->command_number ) & INT_MAX;

	if ( !prediction_data.prediction_player )
		prediction_data.prediction_player = *reinterpret_cast < int ** > ( utilities::pattern_scan (  ( "client.dll" ),  ( "89 35 ? ? ? ? F3 0F 10 48 20" ) ) + 0x2 );

	*prediction_data.prediction_player = reinterpret_cast < int > ( local_pointer );

	interfaces::game_movement->start_track_prediction_errors ( local_pointer ); //-V807
	interfaces::move_helper->set_host ( local_pointer );

	player_move_data move_data;
	memset ( &move_data, 0, sizeof ( player_move_data ) );

	interfaces::prediction->setup_move ( local_pointer, m_pcmd, interfaces::move_helper, &move_data );
	interfaces::game_movement->process_movement ( local_pointer, &move_data );
	interfaces::prediction->finish_move ( local_pointer, m_pcmd, &move_data );

	interfaces::game_movement->finish_track_prediction_errors ( local_pointer );
	interfaces::move_helper->set_host ( nullptr );

	/*auto viewmodel = local_pointer->view_model ( );

	if ( viewmodel ) {
		viewmodel_data.weapon = viewmodel->m_hWeapon ( ).Get ( );

		viewmodel_data.viewmodel_index = viewmodel->m_nViewModelIndex ( );
		viewmodel_data.sequence = viewmodel->m_nSequence ( );
		viewmodel_data.animation_parity = viewmodel->m_nAnimationParity ( );

		viewmodel_data.cycle = viewmodel->m_flCycle ( );
		viewmodel_data.animation_time = viewmodel->m_flAnimTime ( );
	}*/

	prediction_data.prediction_stage = FINISH;
}

void engineprediction::finish ( ) {
	if ( !local_player::available ( ) )
		return;

	if ( prediction_data.prediction_stage != FINISH )
		return;

	*prediction_data.prediction_random_seed = -1;
	*prediction_data.prediction_player = 0;

	interfaces::globals->cur_time = prediction_data.old_curtime;
	interfaces::globals->frame_time = prediction_data.old_frametime;
}