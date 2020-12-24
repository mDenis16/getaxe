#include "../features.hpp"
tickbase_system::shift_data tickbase_system::m_shift_data;
tickbase_system::prediction tickbase_system::m_prediction;
void tickbase_system::write_user_cmd ( bf_write * buf, c_usercmd * incmd, c_usercmd * outcmd ) {
	// Generic WriteUserCmd code as seen in every hack ever
	using WriteUsercmd_t = void ( __fastcall * )( void *, c_usercmd *, c_usercmd * );
	static WriteUsercmd_t fnWriteUserCmd = reinterpret_cast< WriteUsercmd_t >( utilities::pattern_scan ( "client.dll", ( "55 8B EC 83 E4 F8 51 53 56 8B D9 8B 0D" ) ) );

	__asm {
		mov     ecx, buf
		mov     edx, incmd
		push    outcmd
		call    fnWriteUserCmd
		add     esp, 4
	}
}

void tickbase_system::pre_movement ( ) {
	//	return;

		// Invalidate next shift amount and the ticks to shift prior to shifting
	tickbase_system::m_shift_data.m_next_shift_amount = tickbase_system::m_shift_data.m_ticks_to_shift = 0;
}

void tickbase_system::post_movement ( ) {
	//return;
	// Perform sanity checks to make sure we're able to shift
	if ( !local_player::m_data.pointer->is_alive ( ) || !config.ragebot_double_tap ) {
		return;
	}
	auto weapon = local_player::m_data.pointer->active_weapon ( );
	if ( !csgo::cmd || !weapon ) {
		return;
	}
	auto index = weapon->item_definition_index ( );
	if ( !index )
		return;

	if ( index == weapon_revolver ||
		index == weapon_c4 ||
		weapon->get_weapon_data ( )->iWeaponType == WEAPONTYPE_KNIFE ||
		weapon->is_grenade ( ) ) {
		tickbase_system::m_shift_data.m_did_shift_before = false;
		tickbase_system::m_shift_data.m_should_be_ready = false;
		tickbase_system::m_shift_data.m_should_disable = true;
		return;
	}

	// Don't attempt to shift if we're supposed to
	if ( !tickbase_system::m_shift_data.m_should_attempt_shift ) {
		tickbase_system::m_shift_data.m_did_shift_before = false;
		tickbase_system::m_shift_data.m_should_be_ready = false;
		return;
	}

	tickbase_system::m_shift_data.m_should_disable = false;

	// Setup variables we will later use in order to dermine if we're able to shift tickbase or not

	// This could be made into a checkbox 
	bool bFastRecovery = config.ragebot_double_tap_filters [ 0];

	float flNonShiftedTime = math::ticks_to_time ( local_player::m_data.pointer->get_tick_base ( ) - csgo::m_goal_shift );

	// Determine if we are able to shoot right now (at the time of the shift)
	bool bCanShootNormally = local_player::m_data.pointer->can_shoot_time ( math::ticks_to_time ( local_player::m_data.pointer->get_tick_base ( ) ) );

	// Determine if we are able to shoot in the previous iShiftAmount ticks (iShiftAmount ticks before we shifted)
	bool bCanShootIn12Ticks = local_player::m_data.pointer->can_shoot_time ( flNonShiftedTime );

	bool bIsShooting = local_player::m_data.pointer->is_firing ( csgo::cmd, math::ticks_to_time ( local_player::m_data.pointer->get_tick_base ( ) ) );

	// Determine if we are able to shift the tickbase respective to previously setup variables (rofl)
	tickbase_system::m_shift_data.m_can_shift_tickbase = bCanShootIn12Ticks || ( !bCanShootNormally || bFastRecovery ) && ( tickbase_system::m_shift_data.m_did_shift_before );

	// If we can shift tickbase, shift enough ticks in order to double-tap
	// Always prioritise fake-duck if we wish to
	if ( tickbase_system::m_shift_data.m_can_shift_tickbase ) {
		// Tell the cheat to shift tick-base and disable fakelag
		tickbase_system::m_shift_data.m_next_shift_amount = csgo::m_goal_shift;
	}
	else {
		tickbase_system::m_shift_data.m_next_shift_amount = 0;
		tickbase_system::m_shift_data.m_should_be_ready = false;
	}


	// Are we even supposed to shift tickbase?
	if ( tickbase_system::m_shift_data.m_next_shift_amount > 0 ) {
		// Prevent m_iTicksAllowedForProcessing from being incremented.
		// g_cl.m_cmd->m_tick = INT_MAX;
		// Determine if we're able to double-tap  
		if ( bCanShootIn12Ticks ) {
			if ( tickbase_system::m_shift_data.m_prepare_recharge && !bIsShooting ) {
				tickbase_system::m_shift_data.m_needs_recharge = csgo::m_goal_shift;
				tickbase_system::m_shift_data.m_prepare_recharge = false;
			}
			else {
				if ( bIsShooting ) {
					// Store shifted command
					tickbase_system::m_prediction.m_shifted_command = csgo::cmd->command_number;

					// Store shifted ticks
					tickbase_system::m_prediction.m_shifted_ticks = abs ( tickbase_system::m_shift_data.m_current_shift );

					// Store original tickbase
					tickbase_system::m_prediction.m_original_tickbase = local_player::m_data.pointer->get_tick_base ( );

					// Update our wish ticks to shift, and later shift tickbase
					tickbase_system::m_shift_data.m_ticks_to_shift = tickbase_system::m_shift_data.m_next_shift_amount;
				}
			}
		}
		else {
			tickbase_system::m_shift_data.m_prepare_recharge = true;
			tickbase_system::m_shift_data.m_should_be_ready = false;
		}
	}
	else {
		tickbase_system::m_shift_data.m_prepare_recharge = true;
		tickbase_system::m_shift_data.m_should_be_ready = false;
	}

	// Note DidShiftBefore state 
	tickbase_system::m_shift_data.m_did_shift_before = tickbase_system::m_shift_data.m_next_shift_amount > 0;
}