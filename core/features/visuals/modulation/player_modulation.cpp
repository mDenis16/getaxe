#pragma once
#include "../../features.hpp"
#define player_modulation_registers void * ecx, void * edx, void * ctx, const draw_model_state_t & state, const model_render_info_t & info, matrix3x4_t * custom_bone_to_world, bool & skip_return, player_t* player, chams_visual& chams_data

namespace modulation {

	void run_player ( void * ecx, void * edx, void * ctx, const draw_model_state_t & state, const model_render_info_t & info, matrix3x4_t * custom_bone_to_world, bool & skip_return, player_t* player ) {
		
		if ( !player )
			return;


		int type = player->is_teammate ( ) ? 0 : 1;
		/*
		VISIBLE,
	HIDDEN,
	BACKTRACK,
	HIT,
	ATTACHMENTS,
	CHAMS_MODEL_MAX
		*/
		auto & visible_data = config.player_visual [ type ].chams[VISIBLE];
		auto & hidden_data = config.player_visual [ type ].chams [ HIDDEN ]; hidden_data.force_z = true;

		float old_modulation [ 4 ] = { 0,0,0,0 }; interfaces::render_view->get_color_modulation ( old_modulation );
		float old_blend = interfaces::render_view->get_blend ( );

		run_player_chams ( ecx, edx, ctx, state, info, custom_bone_to_world, skip_return, player, hidden_data );
		run_player_chams ( ecx, edx, ctx, state, info, custom_bone_to_world, skip_return, player, visible_data );
	

		interfaces::render_view->modulate_color ( old_modulation ); 
		interfaces::model_render->override_material ( NULL );
		interfaces::render_view->set_blend ( old_blend );
	}


	void run_player_chams ( void * ecx, void * edx, void * ctx, const draw_model_state_t & state, const model_render_info_t & info, matrix3x4_t * custom_bone_to_world, bool & skip_return, player_t * player, chams_visual & chams_data ) {
		if ( !chams_data.enable )
			return;

		interfaces::model_render->override_material ( materials.at ( chams_data.material ).mat_address );

		if ( chams_data.material == glow && materials.at ( chams_data.material ).envmaptint && materials.at ( chams_data.material ).found_env_tint )
			materials.at ( chams_data.material ).envmaptint->SetVector ( ( float * ) &chams_data.color.Value );
		else
		    interfaces::render_view->modulate_color ( ( float * ) &chams_data.color.Value );

		if ( chams_data.color.Value.w < 0.90 )
		interfaces::render_view->set_blend ( chams_data.color.Value.w );

		if (chams_data.force_z  )
		materials.at ( chams_data.material ).mat_address->set_material_var_flag ( material_var_flags_t::material_var_ignorez, true );
		draw_model_execute_original ( ecx, edx, ctx, state, info, custom_bone_to_world );
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