#include "../features.hpp"

misc::movement::recorder::global_data misc::movement::recorder::m_global_data;

namespace misc::movement::recorder {


	void run ( c_usercmd * cmd ) {
		handle_input ( );

		switch ( m_global_data.state ) {
		case recording_state::state_idle:
			
			break;
		case recording_state::state_playing:
			playing ( cmd );
			break;
		case recording_state::state_recording:
			recording ( cmd );
			break;
		default:
			break;
		}
	}
	void handle_input ( ) {
		static int last_press = 0;
		if ( interfaces::inputsystem->is_button_down ( button_code_t::KEY_V ) && (interfaces::globals->tick_count - last_press ) > 120) {
			if ( m_global_data.state == recording_state::state_recording ) {

				interfaces::console->console_printf ( "RECORDING WAS SAVED." );
				m_global_data.state = recording_state::state_idle;
			}else if ( m_global_data.state == recording_state::state_idle ) {

				interfaces::console->console_printf ( "RECORDING HAS STARTED." );
				m_global_data.state = recording_state::state_idle;
			}

		    last_press = interfaces::globals->tick_count;
		}
	}

	void recording ( c_usercmd * cmd ) {
		custom_cmd cmd_copy;
		read_cmd ( cmd, cmd_copy );  

		m_global_data.current_recording.cmds.push_back ( cmd_copy );
	}
	void save_record ( ) {

	}
	void playing ( c_usercmd * /*cmd*/ ) {
		
	}
	void start_recording ( ) {
		m_global_data.state = state_recording;

	}
	void apply_cmd ( c_usercmd * cmd, custom_cmd & cmd_copy ) {
		cmd->viewangles = cmd_copy.viewangles;
		cmd->forwardmove = cmd_copy.forwardmove;
		cmd->sidemove = cmd_copy.sidemove;
		cmd->upmove = cmd_copy.upmove;
		cmd->buttons = cmd_copy.buttons;
		interfaces::engine->set_view_angles ( cmd->viewangles );
	}

	void read_cmd ( c_usercmd * cmd, custom_cmd & cmd_copy ) {
		cmd_copy.viewangles = cmd->viewangles;
		cmd_copy.pos = local_player::m_data.pointer->origin();
		cmd_copy.forwardmove = cmd->forwardmove;
		cmd_copy.sidemove = cmd->sidemove;
		cmd_copy.upmove = cmd->upmove;
		cmd_copy.buttons = cmd->buttons;
	}

}