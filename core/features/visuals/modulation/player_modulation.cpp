#pragma once
#include "../../helpers/helpers.h"
#include <renderlib/imgui/imgui.h>


#include "../visuals.h"
#include <config.h>

#include "modulation.h"

#define player_modulation_registers void * ecx, void * edx, void * ctx, const draw_model_state_t & state, const model_render_info_t & info, matrix3x4_t * custom_bone_to_world, bool & skip_return, player_t* player, chams_visual& chams_data

namespace modulation {

	void run_player ( void * ecx, void * edx, void * ctx, const draw_model_state_t & state, const model_render_info_t & info, matrix3x4_t * custom_bone_to_world, bool & skip_return, player_t* player ) {
		
		if ( !player )
			return;


		int type = player->is_teammate ( ) ? 0 : 1;
		if ( player == local_player::ptr() )
			type = 2;

		/*
		VISIBLE,
	HIDDEN,
	BACKTRACK,
	HIT,
	ATTACHMENTS,
	CHAMS_MODEL_MAX
		*/

		//float old_modulation [ 4 ] = { 0,0,0,0 }; interfaces::render_view->get_color_modulation ( old_modulation );
		//float old_blend = interfaces::render_view->get_blend ( );
		if ( type <= 1 ) {
			auto & visible_data = config.player_visual [ type ].chams [ VISIBLE ];
			auto & hidden_data = config.player_visual [ type ].chams [ HIDDEN ]; hidden_data.force_z = true;


			run_player_chams ( ecx, edx, ctx, state, info, custom_bone_to_world, skip_return, player, hidden_data );
			run_player_chams ( ecx, edx, ctx, state, info, custom_bone_to_world, skip_return, player, visible_data );
		}
		else {
			auto & local_data = config.local_visual.chams [local_chams_model::LOCAL ];
			run_player_chams ( ecx, edx, ctx, state, info, custom_bone_to_world, skip_return, player, local_data );
		}

	//	interfaces::render_view->modulate_color ( old_modulation ); 
		interfaces::model_render->override_material ( NULL );
		//interfaces::render_view->set_blend ( old_blend );
	}


	void run_player_chams ( void * ecx, void * edx, void * ctx, const draw_model_state_t & state, const model_render_info_t & info, matrix3x4_t * custom_bone_to_world, bool & skip_return, player_t * player, chams_visual & chams_data ) {
		if ( !chams_data.enable )
			return;

		

		if ( chams_data.material == glow && materials.at ( chams_data.material ).envmaptint && materials.at ( chams_data.material ).found_env_tint )
			materials.at ( chams_data.material ).envmaptint->SetVector ( ( float * ) &chams_data.color.Value );
		else
			 interfaces::render_view->modulate_color ( ( float * ) &chams_data.color.Value );
		
		if ( chams_data.color.Value.w < 0.90 )
		   interfaces::render_view->set_blend ( chams_data.color.Value.w );

		if (chams_data.force_z  )
		   materials.at ( chams_data.material ).mat_address->set_material_var_flag ( material_var_flags_t::material_var_ignorez, true );
		
		interfaces::model_render->override_material ( materials.at ( chams_data.material ).mat_address );
		draw_model_execute_original ( ecx, edx, ctx, state, info, custom_bone_to_world );
		interfaces::model_render->override_material ( nullptr );
		
		if ( chams_data.force_z )
		   materials.at ( chams_data.material ).mat_address->set_material_var_flag ( material_var_flags_t::material_var_ignorez, false );

		skip_return = true;
	}

	void run_player_history ( void * ecx, void * edx, void * ctx, const draw_model_state_t & state, const model_render_info_t & info, matrix3x4_t * custom_bone_to_world, bool & skip_return, player_t * player, chams_visual & chams_data ) {

		skip_return = true;
	}

	void run_player_hit ( void * ecx, void * edx, void * ctx, const draw_model_state_t & state, const model_render_info_t & info, matrix3x4_t * custom_bone_to_world, bool & skip_return, player_t * player, chams_visual & chams_data ) {

		skip_return = true;
	}

	void run_player_attachments ( void * ecx, void * edx, void * ctx, const draw_model_state_t & state, const model_render_info_t & info, matrix3x4_t * custom_bone_to_world, bool & skip_return, player_t * player ) {

		//if ( !player )
		//	return;

		int type = 0;// player->is_teammate ( ) ? 0 : 1;

		float old_modulation [ 4 ] = { 0,0,0,0 }; interfaces::render_view->get_color_modulation ( old_modulation );
		float old_blend = interfaces::render_view->get_blend ( );


		auto & attachments_data = config.player_visual [ type ].chams [ ATTACHMENTS ];
		run_player_chams ( ecx, edx, ctx, state, info, custom_bone_to_world, skip_return, player, attachments_data );


		interfaces::render_view->modulate_color ( old_modulation );
		interfaces::model_render->override_material ( NULL );
		interfaces::render_view->set_blend ( old_blend );

	}
	
}