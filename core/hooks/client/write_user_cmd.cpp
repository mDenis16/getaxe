#include "../hooks.hpp"
namespace hooks::callback {
	bool __fastcall write_user_cmd ( void * ecx, void *, int m_nSlot, bf_write * m_pBuffer, int m_nFrom, int m_nTo, bool m_bNewCmd ) {

		return write_user_cmd_original ( ecx, m_nSlot, m_pBuffer, m_nFrom, m_nTo, m_bNewCmd );


		/*
		int shift_target = 14;
		if ( !shift_target )
			return write_user_cmd_original ( ecx, slot, buf, from, to, is_new_command );

		if ( from != -1 )
			return true;

		auto final_from = -1;

		uintptr_t frame_ptr;
		__asm mov frame_ptr, ebp;

		auto backup_commands = reinterpret_cast < int * > ( frame_ptr + 0xFD8 );
		auto new_commands = reinterpret_cast < int * > ( frame_ptr + 0xFDC );

		auto newcmds = *new_commands;
		auto shift = shift_target;


		*backup_commands = 0;

		auto choked_modifier = newcmds + shift;

		if ( choked_modifier > 62 )
			choked_modifier = 62;

		*new_commands = choked_modifier;

		auto next_cmdnr = interfaces::clientstate->m_choked_commands + interfaces::clientstate->m_last_outgoing_command + 1;
		auto final_to = next_cmdnr - newcmds + 1;

		if ( final_to <= next_cmdnr ) {
			while ( write_user_cmd_original ( ecx, slot, buf, final_from, final_to, true ) ) {
				final_from = final_to++;

				if ( final_to > next_cmdnr )
					goto next_cmd;
			}

			return false;
		}
	next_cmd:

		auto user_cmd = interfaces::input->get_user_cmd ( final_from );

		if ( !user_cmd )
			return true;

		c_usercmd to_cmd;
		c_usercmd from_cmd;

		from_cmd = *user_cmd;
		to_cmd = from_cmd;

		to_cmd.command_number++;
		to_cmd.tick_count += 200;

		if ( newcmds > choked_modifier )
			return true;

		for ( auto i = choked_modifier - newcmds + 1; i > 0; --i ) {
		//	WriteUserCmd ( buf, &to_cmd, &from_cmd );

			from_cmd = to_cmd;
			to_cmd.command_number++;
			to_cmd.tick_count++;
		}

		shifting::_shift.current_shift = shift;
		return true;*/
	}
}