#include "../../features/features.hpp"
#include "../hooks.hpp"

namespace hooks::callback {


	void __fastcall draw_model_exec ( void * ecx, void * edx, void * ctx, const draw_model_state_t & state, const model_render_info_t & info, matrix3x4_t * custom_bone_to_world ) {
	
			return draw_model_execute_original ( ecx, edx, ctx, state, info, custom_bone_to_world );

	}
}