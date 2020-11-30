#include "../features.hpp"
i_material* visuals::chams::create_material( shader_type_t shade, bool ignorez, bool wireframe ) {
	static const std::string material_name = "game_material.vmt";
	std::string shade_type;

	switch ( shade ) {
	case shader_type_t::VertexLitGeneric: shade_type = "VertexLitGeneric"; break;
	case shader_type_t::UnlitGeneric: shade_type = "UnlitGeneric"; break;
	case shader_type_t::Modulate: shade_type = "Modulate"; break;
	default: break;
	}

	std::string material_data;

	material_data += "\"" + shade_type + "\"\n{\n";

	material_data += "\t\"$model\" \"1\"\n";
	material_data += "\t\"$flat\" \"1\"\n";
	material_data += "\t\"$selfillum\" \"1\"\n";
	material_data += "\t\"$selfillumtint\" \"[8 8 8]\"\n";
	material_data += "\t\"$halflambert\" \"1\"\n";
	material_data += "\t\"$wireframe\" \"" + std::to_string( wireframe ) + "\"\n";
	material_data += "\t\"$ignorez\" \"" + std::to_string( ignorez ) + "\"\n";

	material_data += "\t\"$mod2x\" \"1\"\n";
	material_data += "\t\"$nocull\" \"1\"\n"; // for Modulate shader	

	material_data += "\"Proxies\"\n{\n";
	// add proxies here.
	material_data += "}\n"; // end of proxies.

	material_data += "}\n"; // end of shader type.

	auto kv = static_cast< c_key_values* >( malloc( 36 ) );
	kv->init( shade_type.c_str( ) );
	kv->load_from_buffer( material_name.c_str( ), material_data.c_str( ) );

	return interfaces::material_system->create_material( material_name.c_str( ), kv );
}
void visuals::chams::think ( ) {
	if ( !csgo::local_player || !interfaces::engine->is_connected ( ) && !interfaces::engine->is_in_game ( ) )
		return;

	/*if ( csgo::local_player->is_alive ( ) ) {

	
		static auto solid = interfaces::material_system->find_material ( "debug/debugdrawflat", TEXTURE_GROUP_MODEL );
		float color [ 3 ] = { variables::visuals::modulation::enemy::xyz_color [ 0 ], variables::visuals::modulation::enemy::xyz_color [ 1 ], variables::visuals::modulation::enemy::xyz_color [ 2 ] };
		interfaces::render_view->modulate_color ( color );
		interfaces::render_view->set_blend ( 1.0f );
		solid->set_material_var_flag ( material_var_ignorez, false );
		interfaces::model_render->override_material ( solid );
		vec3_t backup_abs_angles = csgo::local_player->abs_angles ( );
		csgo::local_player->set_abs_angles ( animations::m_data.fake_rotation );
		csgo::local_player->draw_model ( 1, 255 );
		csgo::local_player->set_abs_angles ( backup_abs_angles );

	}*/
	for ( int i = 1; i <= interfaces::globals->max_clients; i++ ) {
		auto entity = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( i ) );

		if ( !entity || entity == csgo::local_player || entity->health ( ) <= 0 )
			continue;

		bool is_teammate = entity->team ( ) == csgo::local_player->team ( );
		bool is_enemy = entity->team ( ) != csgo::local_player->team ( );

		static i_material * material = nullptr;
		static auto default_ = interfaces::material_system->find_material ( "reflective", TEXTURE_GROUP_MODEL );
		static auto solid = interfaces::material_system->find_material ( "debug/debugdrawflat", TEXTURE_GROUP_MODEL );

	//	default_->increment_reference_count ( );
		//solid->increment_reference_count ( );

		material = solid;

		if ( is_enemy ) {


			if ( variables::visuals::modulation::enemy::visible ) {
				float color [ 3 ] = { variables::visuals::modulation::enemy::xyz_color [ 0 ], variables::visuals::modulation::enemy::xyz_color [ 1 ], variables::visuals::modulation::enemy::xyz_color [ 2 ] };
				interfaces::render_view->modulate_color ( color );
				interfaces::render_view->set_blend ( 1.0f );
				material->set_material_var_flag ( material_var_ignorez, false );
				interfaces::model_render->override_material ( solid );
				entity->draw_model ( 1, 255 );
			}

			if ( variables::visuals::modulation::enemy::xyz ) {
				auto record = player_manager::best_tick_global [ entity->index ( ) ];
				if ( record.simtime > 0.f ) {
					float color [ 3 ] = { variables::visuals::modulation::enemy::xyz_color [ 0 ], variables::visuals::modulation::enemy::xyz_color [ 1 ], variables::visuals::modulation::enemy::xyz_color [ 2 ] };
					interfaces::render_view->modulate_color ( color );
					interfaces::render_view->set_blend ( 1.0f );
					vec3_t abs_origin = entity->abs_origin ( );
					material->set_material_var_flag ( material_var_ignorez, true );
					interfaces::model_render->override_material ( solid );
					entity->set_abs_origin ( record.origin );
						entity->draw_model ( 1, 255 );
						entity->set_abs_origin ( abs_origin );
				}
			}
		}

		if ( is_teammate ) {
			if ( variables::visuals::modulation::team::visible ) {
				float color [ 3 ] = { variables::visuals::modulation::team::visible_color [ 0 ], variables::visuals::modulation::team::visible_color [ 1 ], variables::visuals::modulation::team::visible_color [ 2 ] };
				
				interfaces::render_view->modulate_color ( color );
				interfaces::render_view->set_blend ( variables::visuals::modulation::team::visible_color [ 3 ] );
				material->set_material_var_flag ( material_var_ignorez, false );
				interfaces::model_render->override_material ( solid );
				entity->draw_model ( 1, 255 );
			}


			if ( variables::visuals::modulation::team::xyz ) {
				float color [ 3 ] = { variables::visuals::modulation::team::xyz_color [ 0 ], variables::visuals::modulation::team::xyz_color [ 1 ], variables::visuals::modulation::team::xyz_color [ 2 ] };
				interfaces::render_view->modulate_color ( color );
				interfaces::render_view->set_blend ( 1.0 );
				material->set_material_var_flag ( material_var_ignorez, true );
				interfaces::model_render->override_material ( solid );
				entity->draw_model ( 1, 255 );
			}
		}

		interfaces::model_render->override_material ( nullptr );

	}
}
