#pragma once
#include "../../features.hpp"

namespace modulation {

	void run ( void * ecx, void * edx, void * ctx, const draw_model_state_t & state, const model_render_info_t & info, matrix3x4_t * custom_bone_to_world, bool & skip_return ) {
		auto model_name = interfaces::model_info->get_model_name ( ( model_t * ) info.pModel );


		std::string mdl { info.pModel->name };

				//std::cout << "mdl name " << model_name << "entity index " <<info.entity_index << std::endl;

		if (  mdl.find ( ( "player" ) ) != std::string::npos  ) {
			auto ent = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( info.entity_index ) );
			if ( ent ) {
				bool is_ragdoll = ent->client_class ( )->class_id != cs_player;
				auto ent_corrected = is_ragdoll ? ( player_t * ) interfaces::entity_list->get_client_entity_handle ( ( uintptr_t ) ent->m_hPlayer ( ) ) : ent;

				if ( is_ragdoll && !config.player_visual [ ent_corrected->is_teammate ( ) ? 0 : 1 ].ragdoll_chams )
					return;

				  run_player ( ecx, edx, ctx, state, info, custom_bone_to_world, skip_return, ent_corrected );
			}
		}
		
	}
}