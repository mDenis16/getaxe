#pragma once

#include "../hooks.hpp"
#include <config.h>
#include "../../features/visuals/modulation/modulation.h"

namespace hooks::callback {

	static bool created_materials = false;
	void __fastcall draw_model_exec ( void * ecx, void * edx, void * ctx, const draw_model_state_t & state, const model_render_info_t & info, matrix3x4_t * custom_bone_to_world ) {

		if ( !created_materials ) {
			modulation::create_materials ( );
			created_materials = true;
		}

		auto name = interfaces::model_info->get_model_name ( info.pModel );

		auto is_weapon = strstr ( name, "weapons/v_" ) && !strstr ( name, "arms" );
		
		if ( interfaces::model_render->is_forced_material_override ( ) && !is_weapon )
			return draw_model_execute_original ( ecx, edx, ctx, state, info, custom_bone_to_world );

		bool skip_return = false;
		
		modulation::run ( ecx, edx, ctx, state, info, custom_bone_to_world, skip_return );
		

		if ( !skip_return )
		 draw_model_execute_original ( ecx, edx, ctx, state, info, custom_bone_to_world );


	
	}
}