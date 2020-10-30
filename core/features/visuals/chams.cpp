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

void visuals::chams::run( i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* bone_to_world ) {

	static auto draw_model_execute_original = reinterpret_cast< hooks::draw_model_execute::fn >( hooks::get_virtual( interfaces::model_render, 21 ) );

	if ( !interfaces::engine->is_connected( ) && !interfaces::engine->is_in_game( ) )
		return;

	if ( !csgo::local_player )
		return;

	static i_material* mat = nullptr;
	static i_material* zmat = nullptr;

	static i_material* norm = create_material( shader_type_t::VertexLitGeneric, false, false );
	static i_material* znorm = create_material( shader_type_t::VertexLitGeneric, true, false );

	static i_material* flat = create_material( shader_type_t::UnlitGeneric, false, false );
	static i_material* zflat = create_material( shader_type_t::UnlitGeneric, true, false );

	static i_material* wire = create_material( shader_type_t::UnlitGeneric, false, true );
	static i_material* zwire = create_material( shader_type_t::UnlitGeneric, true, true );


	switch ( variables::visuals::chams::material ) {
	case 1:
		mat = norm;
		zmat = znorm;
		break;
	case 2:
		mat = flat;
		zmat = zflat;
		break;
	case 3:
		mat = wire;
		zmat = zwire;
		break;
	default:
		mat = norm;
		zmat = znorm;
		break;
	}

	auto model_name = interfaces::model_info->get_model_name( ( model_t* ) info.model );
	auto entity = reinterpret_cast< player_t* >( interfaces::entity_list->get_client_entity( info.entity_index ) );
	if ( !model_name || !entity )
		return;

	if ( model_name && strstr( model_name, "models/player" ) ) {

		if ( variables::visuals::chams::view_backtrack ) {

			if ( !csgo::local_player->is_alive( ) )
				return;
			float backtrack_color [ 4 ] { variables::visuals::chams::backtrack_colro.r, variables::visuals::chams::backtrack_colro.g,variables::visuals::chams::backtrack_colro.b, variables::visuals::chams::backtrack_colro.a };

			if ( entity && entity->is_alive( ) && !entity->dormant( ) ) {
				int i = entity->index( );

				if ( csgo::local_player&& csgo::local_player->is_alive( ) && entity->team( ) != csgo::local_player->team( ) ) {

					auto record = player_manager::best_tick_global [ info.entity_index ];


					if ( record.simtime > 0.f && record.bone  ) {
						interfaces::render_view->modulate_color( backtrack_color );
						interfaces::render_view->set_blend( variables::visuals::chams::backtrack_colro.a);
						interfaces::model_render->override_material( mat );

						draw_model_execute_original( interfaces::model_render, ctx, state, info, record.bone );
					}
				}
			}
		}
		if ( variables::visuals::chams::enemy_chams ) {

			if ( entity->is_enemy( ) ) {
			
				if ( variables::visuals::chams::xyz ) {
					float enemy_color [ 4 ] { variables::visuals::chams::enemy_color.r, variables::visuals::chams::enemy_color.g,variables::visuals::chams::enemy_color.b, variables::visuals::chams::enemy_color.a };
					interfaces::render_view->modulate_color( enemy_color );
					interfaces::render_view->set_blend( enemy_color [ 3 ] );
					interfaces::model_render->override_material( zmat );

					draw_model_execute_original( interfaces::model_render, ctx, state, info, bone_to_world );
				}
				if ( entity->get_anim_state( ) ) {

					auto goal_feet_yaw = entity->get_anim_state( )->m_flGoalFeetYaw;
					auto left = goal_feet_yaw + 58.f;
					auto right = goal_feet_yaw - 58.f;
					left = math::normalize_yaw( left );
					right = math::normalize_yaw( right );
					matrix_t left_matrix [ MAXSTUDIOBONES ];
					ragebot::get_rotated_matrix( entity, 24.f, left_matrix );
					float enemy_left_matrix [ 4 ] { 255, 0, 0, 150 };
					interfaces::render_view->modulate_color( enemy_left_matrix );
					interfaces::render_view->set_blend( enemy_left_matrix [ 3 ] );
					interfaces::model_render->override_material( mat );

					draw_model_execute_original( interfaces::model_render, ctx, state, info, csgo::left_player_bones [ entity->index( ) ] );


					matrix_t right_matrix [ MAXSTUDIOBONES ];
					ragebot::get_rotated_matrix( entity, -24.f, right_matrix );
					float enemy_right_matrix [ 4 ] { 255, 0, 255, 150 };
					interfaces::render_view->modulate_color( enemy_right_matrix );
					interfaces::render_view->set_blend( enemy_right_matrix [ 3 ] );
					interfaces::model_render->override_material( mat );

					draw_model_execute_original( interfaces::model_render, ctx, state, info, csgo::right_player_bones [ entity->index( ) ] );
				}
			}
		}
		if ( variables::visuals::chams::teammate_chams ) {

			if ( !entity->is_enemy( ) ) {
				float team_color [ 4 ] { variables::visuals::chams::team_color.r, variables::visuals::chams::team_color.g,variables::visuals::chams::team_color.b, variables::visuals::chams::team_color.a };
				float team_color_visible [ 4 ] { variables::visuals::chams::team_color_visible.r, variables::visuals::chams::team_color_visible.g,variables::visuals::chams::team_color_visible.b, variables::visuals::chams::team_color_visible.a };
				if ( variables::visuals::chams::xyz ) {
					interfaces::render_view->modulate_color( team_color );
					interfaces::render_view->set_blend( team_color [ 3 ] );
					interfaces::model_render->override_material( zmat );

					draw_model_execute_original( interfaces::model_render, ctx, state, info, bone_to_world );
				}

				interfaces::render_view->modulate_color( team_color_visible );
				interfaces::render_view->set_blend( team_color_visible [ 3 ] );
				interfaces::model_render->override_material( mat );

				draw_model_execute_original( interfaces::model_render, ctx, state, info, bone_to_world );

			}
		}
		
		/*if ( !entity->is_alive( ) || !entity->is_alive( ) || entity->client_class( )->class_id == ccsragdoll ) {
			int x = 0.5;
			float clr_chams_dead [ 4 ] { x,x,x,x };

			interfaces::render_view->modulate_color( clr_chams_dead );
			interfaces::render_view->set_blend( clr_chams_dead [ 3 ] );
			interfaces::model_render->override_material( mat );

			draw_model_execute_original( interfaces::model_render, ctx, state, info, bone_to_world );
		}*/
	}
}
