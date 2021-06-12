#pragma once
#include "../../features.hpp"

namespace modulation {

	void run ( void * ecx, void * edx, void * ctx, const draw_model_state_t & state, const model_render_info_t & info, matrix3x4_t * custom_bone_to_world, bool & skip_return ) {
		auto model_name = interfaces::model_info->get_model_name ( ( model_t * ) info.pModel );
		std::string mdl { info.pModel->name };

		if ( mdl.find ( ( "player" ) ) != std::string::npos ) {
			auto ent = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( info.entity_index ) );
			if ( ent ) {
				bool is_ragdoll = ent->client_class ( )->class_id != CCSPlayer;
				auto ent_corrected = is_ragdoll ? ( player_t * ) interfaces::entity_list->get_client_entity_handle ( ( uintptr_t ) ent->m_hPlayer ( ) ) : ent;
				
				if ( ent_corrected != nullptr ) {
					if ( is_ragdoll && !config.player_visual [ ent_corrected->is_teammate ( ) ? 0 : 1 ].ragdoll_chams )
						return;/*crash here*/

					run_player ( ecx, edx, ctx, state, info, custom_bone_to_world, skip_return, ent_corrected );
				}
			}
		}
		else if ( local_pointer && local_pointer->is_alive ( ) && !interfaces::input->m_camera_in_third_person ) {
			if ( mdl.find ( "arms" ) != std::string::npos ) {
				run_player_chams ( ecx, edx, ctx, state, info, custom_bone_to_world, skip_return, local_pointer, config.local_visual.chams [ ARMS ] );

			}
			else if ( mdl.find ( "arms" ) == std::string::npos || mdl.find ( "sleeve" ) == std::string::npos )
				run_player_chams ( ecx, edx, ctx, state, info, custom_bone_to_world, skip_return, local_pointer, config.local_visual.chams [ WEAPON ] );
		}
	}

	void run_glow ( ) {
		for ( int i = 0; i < interfaces::glow_manager->size; i++ ) {
			if ( interfaces::glow_manager->objects [ i ].unused ( ) || !interfaces::glow_manager->objects [ i ].entity )
				continue;
			//continue;
			auto & glow_object = interfaces::glow_manager->objects [ i ];

			auto entity = reinterpret_cast< entity_t * >( glow_object.entity );
			if ( !entity || entity->dormant ( ) )
				continue;

			const auto client_class = entity->client_class ( );

			if ( !client_class )
				continue;

			switch ( client_class->class_id ) {
			case CCSPlayer:
			{
				
				auto type = reinterpret_cast<player_t*>(entity)->is_teammate ( ) ? 0 : 1;

				if ( entity == local_pointer )
					type = 2;

				glow_visual * cfg;
				if ( type > 1 )
					cfg = &config.local_visual.glow;
				else
					cfg = &config.player_visual [ type ].glow;

				if ( cfg->enable ) {
				

					glow_object.set ( cfg->color.Value.x, cfg->color.Value.y, cfg->color.Value.z, cfg->color.Value.w, cfg->bloom_amount, true, false);
				}

				break;
			}
			default:
			{
				if ( client_class->class_id >= CWeaponAug && client_class->class_id <= CWeaponXM1014 || client_class->class_id == CAK47 || client_class->class_id == CDEagle ) {
					glow_visual * cfg = &config.weapons_visual.glow;

					if ( cfg->enable ) 
						glow_object.set ( cfg->color.Value.x, cfg->color.Value.y, cfg->color.Value.z, cfg->color.Value.w, cfg->bloom_amount, true, false );
					
				}
			}
			break;
			}
		}
	}
}