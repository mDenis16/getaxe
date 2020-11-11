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


	//	switch ( variables::visuals::chams::material ) {
		//case 1:
			mat = interfaces::material_system->find_material ( "debug/debugambientcube", nullptr );
			zmat = interfaces::material_system->find_material ( "debug/debugambientcube", nullptr );
			zmat->set_material_var_flag ( material_var_flags_t::material_var_ignorez, true );
			//break;
	/*	case 2:
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
		*/

		auto model_name = interfaces::model_info->get_model_name( ( model_t* ) info.model );
		auto entity = reinterpret_cast< player_t* >( interfaces::entity_list->get_client_entity( info.entity_index ) );
		if ( !model_name || !entity )
			return;

		if ( model_name && strstr( model_name, "models/player" ) ) {
			bool should_draw_visible = entity->is_enemy ( ) ? variables::visuals::modulation::enemy::visible : variables::visuals::modulation::team::visible;
			bool should_draw_xyz = entity->is_enemy ( ) ? variables::visuals::modulation::enemy::visible : variables::visuals::modulation::team::visible;

			auto color_visible = entity->is_enemy ( ) ? variables::visuals::modulation::enemy::visible_color : variables::visuals::modulation::team::visible_color;
			auto color_xyz = entity->is_enemy ( ) ? variables::visuals::modulation::enemy::xyz_color : variables::visuals::modulation::team::xyz_color;
			

				if ( should_draw_visible ) {
				

					interfaces::render_view->modulate_color ( color_visible );
					interfaces::render_view->set_blend ( color_visible [ 3 ] );
					interfaces::model_render->override_material ( mat );

					draw_model_execute_original ( interfaces::model_render, ctx, state, info, player_manager::best_tick_global [ entity->index() ].bone );
				}
				if ( should_draw_xyz ) {
				
					interfaces::render_view->modulate_color ( color_xyz );
					interfaces::render_view->set_blend ( color_xyz [ 3 ] );
					interfaces::model_render->override_material ( zmat );

					draw_model_execute_original ( interfaces::model_render, ctx, state, info, bone_to_world );
				}
			}
			
		
			if ( !entity->is_alive( ) || !entity->is_alive( ) || entity->client_class( )->class_id == ccsragdoll ) {
				int x = 0.5;
				float clr_chams_dead [ 4 ] { x,x,x,x };

				interfaces::render_view->modulate_color( clr_chams_dead );
				interfaces::render_view->set_blend( clr_chams_dead [ 3 ] );
				interfaces::model_render->override_material( mat );

				draw_model_execute_original( interfaces::model_render, ctx, state, info, bone_to_world );
			}

}

