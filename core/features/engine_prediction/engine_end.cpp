#include "../features.hpp"

void c_engine_prediction::end ( ) {
	if (player) {
		interfaces::game_movement->finish_track_prediction_errors(player);

		interfaces::move_helper->set_host(0);

		interfaces::game_movement->Reset();

		// CPlayerMove::FinishCommand
		{
			*reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint32_t>(player) + 0x3348) = 0;

			*prediction_random_seed = -1;
			*prediction_player = 0;
		}


		interfaces::globals->cur_time = old_cur_time;
		interfaces::globals->frame_time = old_frame_time;
	}
}