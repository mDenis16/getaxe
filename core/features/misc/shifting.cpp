#include "../features.hpp"

namespace shifting {
	bool initialized = false;
	void create_move ( c_usercmd * cmd ) {
		if ( !initialized ) {
			auto ChokeLimit = utilities::pattern_scan ( "engine.dll", "B8 ? ? ? ? 3B F0 0F 4F F0 89 5D FC" ) + 1;

			DWORD old;

			VirtualProtect ( ( void * ) ChokeLimit, 4, PAGE_EXECUTE_READWRITE, &old );
			*( uintptr_t * ) ChokeLimit = 62;
			VirtualProtect ( ( void * ) ChokeLimit, 4, old, &old );
			initialized = true;
		}


		if ( _shift.can_shift && localdata.active_weapon->can_double_tap ( ) ) {
			if ( cmd->buttons & cmd_buttons::in_attack ) {

				_shift.shift_ticks = _shift.next_tickbase_shift = config.ragebot_double_tap_ticks;
				shifting::_shift.original_tickbase = local_pointer->get_tick_base ( );
			
			
				_shift.can_shift = false;

				_shift.shifted_command = cmd->command_number;
				_shift.original_tickbase = local_pointer->get_tick_base ( );

			}
		}
	}
}