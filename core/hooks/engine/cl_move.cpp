#include "../../features/features.hpp"
#include "../hooks.hpp"


namespace hooks::callback {
	void __cdecl cl_move ( float accumulated_extra_samples, bool final_tick ) {

	//	if ( !local_pointer )
			return cl_move_original ( accumulated_extra_samples, final_tick );

		if ( shifting::_shift.allow && shifting::_shift.recharge_ticks && ( local_pointer->get_tick_base ( ) >= shifting::_shift.wait_for_recharge || !shifting::_shift.wait_for_recharge ) ) {
			shifting::_shift.recharge_ticks--;
			//	++local_pointer->get_tick_base ( );
			if ( shifting::_shift.recharge_ticks <= 0 ) {
				shifting::_shift.can_shift = true;
				shifting::_shift.can_recharge = false;

			}

			return;
		}


		if ( shifting::_shift.allow ) {

			while ( shifting::_shift.shift_ticks ) {



				cl_move_original ( accumulated_extra_samples, true );

				shifting::_shift.shift_ticks--;

				if ( shifting::_shift.shift_ticks <= 0 ) {
					shifting::_shift.recharge_ticks = 14;
					shifting::_shift.wait_for_recharge = local_pointer->get_tick_base ( ) + 128;
					shifting::_shift.next_tickbase_shift = 0;
				}


			}

		}
		else {
			shifting::_shift.shift_ticks = 0;
			shifting::_shift.recharge_ticks = 14;
			shifting::_shift.can_shift = false;
		}

		cl_move_original ( accumulated_extra_samples, true );
	}
}