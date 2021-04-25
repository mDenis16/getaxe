#include "../../features/features.hpp"
#include "../hooks.hpp"


namespace hooks::callback {
	void __stdcall frame_stage_notify ( client_frame_stage_t stage ) {


		reinterpret_cast< void ( __stdcall * )( client_frame_stage_t ) >( list.at ( hook_index::frame_stage_notify )->original )( stage );
	}
}