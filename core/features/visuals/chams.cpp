#include "../features.hpp"

#define find_mat interfaces::material_system->find_material 
#define override_mat interfaces::model_render->override_material


std::array<i_material *, 4> visuals::chams::materials;
std::array<std::vector<visuals::player::hit_chams>, 64> visuals::player::chams_log;

i_material * visuals::chams::create_material ( shader_type_t shade, bool ignorez, bool wireframe ) {
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
	material_data += "\t\"$wireframe\" \"" + std::to_string ( wireframe ) + "\"\n";
	material_data += "\t\"$ignorez\" \"" + std::to_string ( ignorez ) + "\"\n";

	material_data += "\t\"$mod2x\" \"1\"\n";
	material_data += "\t\"$nocull\" \"1\"\n"; // for Modulate shader	

	material_data += "\"Proxies\"\n{\n";
	// add proxies here.
	material_data += "}\n"; // end of proxies.

	material_data += "}\n"; // end of shader type.

	auto kv = static_cast< c_key_values * >( malloc ( 36 ) );
	kv->init ( shade_type.c_str ( ) );
	kv->load_from_buffer ( material_name.c_str ( ), material_data.c_str ( ) );

	return interfaces::material_system->create_material ( material_name.c_str ( ), kv );
}
void visuals::chams::init ( ) {
	std::ofstream ( "csgo\\materials\\material_textured.vmt" ) << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";

	std::ofstream ( "csgo\\materials\\material_flat.vmt" ) << R"#("UnlitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";

	std::ofstream ( "csgo\\materials\\material_reflective.vmt" ) << R"#("VertexLitGeneric" {
      "$basetexture" "vgui/white_additive"
      "$ignorez" "0"
      "$envmap" "env_cubemap"
      "$normalmapalphaenvmapmask" "1"
      "$envmapcontrast"  "1"
      "$nofog" "1"
      "$model" "1"
      "$nocull" "0"
      "$selfillum" "1"
      "$halflambert" "1"
      "$znearer" "0"
      "$flat" "1" 
}
)#";

	std::ofstream ( "csgo/materials/overlayglow.vmt" ) << R"#("VertexLitGeneric" {
 
	"$additive" "1"
	"$envmap" "models/effects/cube_white"
	"$envmaptint" "[1 1 1]"
	"$envmapfresnel" "1"
	"$envmapfresnelminmaxexp" "[0 1 2]"
	"$alpha" "0.8"
})#";


	materials.at ( mat_type::regular ) = find_mat ( "material_textured", NULL );
	materials.at ( mat_type::flat ) = find_mat ( "material_flat", NULL );
	materials.at ( mat_type::reflective ) = find_mat ( "material_reflective", NULL );
	materials.at ( mat_type::glow ) = find_mat ( "overlayglow", NULL );


	DEBUG_LOG ( "CHAMS INITIALIZED!" );
}
visuals::chams::hook::fn visuals::chams::hook::draw_model_execute_original = nullptr;
namespace visuals::chams {



	void modulate ( float color [ 4 ], mat_type material ) {
		if ( materials.at ( material ) ) {
			materials.at ( material )->alpha_modulate ( color [ 3 ] );
			materials.at ( material )->color_modulate ( color [ 0 ], color [ 1 ], color [ 2 ] );

			bool found_tint;
			const auto tint = materials.at ( material )->find_var ( "$envmaptint", &found_tint );

			if ( found_tint && tint )
				tint->SetVector ( vec3_t ( color [ 0 ], color [ 1 ], color [ 2 ] ) );


			/*				if ( material == mat_type::glow_out_in ) {
								bool found_tint2;
								const auto transform = materials.at ( material )->find_var ( "$envmapfresnelminmaxexp", &found_tint2 );
								const float width = 6.f;
								if ( found_tint2 )
									transform->SetVectorComponent ( width * color [ 3 ], 1 );
							}
							*/
		}

	}


}
vec3_t matrix_get_origin ( const matrix3x4_t & src ) {
	return vec3_t ( src [ 0 ][ 3 ], src [ 1 ][ 3 ], src [ 2 ][ 3 ] );
}
void  matrix_set_origin ( vec3_t pos, matrix3x4_t & matrix ) {
	for ( size_t i {}; i < 3; ++i ) {
		matrix [ i ][ 3 ] = pos [ i ];
	}
}
void __fastcall hooks::draw_model_exec::hook ( void * ecx, void * edx, void * ctx, const draw_model_state_t & state, const model_render_info_t & info, matrix3x4_t * custom_bone_to_world ) {
	if ( !localdata.in_game || interfaces::model_render->is_forced_material_override ( ) )
		return o_draw_model_exec ( ecx, edx, ctx, state, info, custom_bone_to_world );


	static bool init = false;

	if ( !init ) {
		visuals::chams::init ( );
		init = true;
	}
	
	auto model_name = interfaces::model_info->get_model_name ( ( model_t * ) info.pModel );

	if ( config.visuals_world_removals [ 5 ] && strstr ( model_name, ( "shadow" ) ) != nullptr )
		return;

	if ( config.visuals_world_removals[ 3 ] && strstr ( model_name, ( "fire1" ) ) != nullptr )
		return;

	//bool is_atleast_one = config.visuals_modulation_enemy_enabled || config.visuals_modulation_team_enabled;




	bool is_player = strstr ( model_name, "models/player" ) != nullptr;
	bool is_arm = strstr ( model_name, "arms" ) != nullptr;
	//bool is_sleeve = strstr ( model_name, "sleeve" ) != nullptr;
	bool is_weapon = strstr ( model_name, "weapons/v_" ) != nullptr;

	for ( auto * material : visuals::chams::materials )
		material->increment_reference_count ( );

	/*if ( info.entity_index  > 0 && info.entity_index < 64) {
		const float log_color [ 4 ] = { 154, 53, 166, 200};
			for ( size_t i = 0; i < visuals::player::chams_log.at ( info.entity_index ).size ( ); i++ ) {
				auto &log = visuals::player::chams_log.at ( info.entity_index ).at ( i );
	

				interfaces::render_view->modulate_color ( log_color );
				visuals::chams::materials.at ( visuals::chams::mat_type::glow )->set_material_var_flag ( material_var_flags_t::material_var_ignorez, true );

				
					bool found_tint;
					const auto tint = visuals::chams::materials.at ( visuals::chams::mat_type::glow )->find_var ( "$envmaptint", &found_tint );

					if ( found_tint && tint )
						tint->SetVector ( vec3_t ( log_color [ 0 ], log_color [ 1 ], log_color [ 2 ] ) );
				
	

				override_mat ( visuals::chams::materials.at ( visuals::chams::mat_type::glow ) );
				o_draw_model_exec ( ecx, edx, ctx, state, info, log.bones );
	
				interfaces::model_render->override_material ( NULL );



				if ( ( interfaces::globals->cur_time - log.curtime ) > 10.f )
					visuals::player::chams_log.at ( info.entity_index ).erase ( visuals::player::chams_log.at ( info.entity_index ).begin ( ) + i );
			}
		
	}*/
	if ( is_player && local_player::m_data.pointer ) {
	
		auto entity = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( info.entity_index ) );

		if ( entity ) {
		
			if ( entity != local_player::m_data.pointer  ) {
				bool is_ragdoll = entity->client_class ( )->class_id != cs_player;
				auto ent_corrected = is_ragdoll ? ( player_t * ) interfaces::entity_list->get_client_entity_handle ( ( uintptr_t ) entity->m_hPlayer ( ) ) : entity;
				//interfaces::console->console_printf ( "ent corrected classid %i \n", entity->client_class ( )->class_id );
				if ( ent_corrected ) {
					auto index_ent = ent_corrected->index ( );
					//interfaces::console->console_printf ( "ent corrected index %i \n", index_ent );


					float log_color [ 4 ] = { 51, 62, 189, 255 };
					float old_modulation [ 4 ] = { 0,0,0,0 }; interfaces::render_view->get_color_modulation ( old_modulation );

					interfaces::render_view->modulate_color ( config.visuals_modulation_enemy_visible_color );
					visuals::chams::materials.at ( visuals::chams::mat_type::glow )->set_material_var_flag ( material_var_flags_t::material_var_ignorez, false );

					if constexpr ( visuals::chams::mat_type::glow == visuals::chams::mat_type::glow ) {
						bool found_tint;
						const auto tint = visuals::chams::materials.at ( visuals::chams::mat_type::glow )->find_var ( "$envmaptint", &found_tint );

						if ( found_tint && tint )
							tint->SetVector ( vec3_t ( config.visuals_modulation_enemy_visible_color [ 0 ], config.visuals_modulation_enemy_visible_color [ 1 ], config.visuals_modulation_enemy_visible_color [ 2 ] ) );
					}


			
					for ( size_t i = 0; i < visuals::player::chams_log.at ( index_ent ).size ( ); i++ ) {
						auto & log = visuals::player::chams_log.at( index_ent ).at ( i );

						override_mat ( visuals::chams::materials.at ( visuals::chams::mat_type::glow ) );
						o_draw_model_exec ( ecx, edx, ctx, state, info, log.bones );

						if ( ( interfaces::globals->cur_time - log.curtime ) > 6.5f )
							visuals::player::chams_log.at ( index_ent ).erase ( visuals::player::chams_log.at ( index_ent ).begin ( ) + i );
					}
				
					interfaces::render_view->modulate_color ( old_modulation );
					interfaces::model_render->override_material ( NULL );

				}

			}
			if ( entity == local_player::m_data.pointer ) {
				float old_modulation [ 4 ] = { 0,0,0,0 }; interfaces::render_view->get_color_modulation ( old_modulation );
			
					interfaces::render_view->modulate_color ( config.visuals_modulation_enemy_visible_color );
				    visuals::chams::materials.at ( config.visuals_modulation_enemy_material )->set_material_var_flag ( material_var_flags_t::material_var_ignorez, false );

				if ( config.visuals_modulation_enemy_material == visuals::chams::mat_type::glow ) {
					bool found_tint;
					const auto tint = visuals::chams::materials.at ( visuals::chams::mat_type::glow )->find_var ( "$envmaptint", &found_tint );

					if ( found_tint && tint )
						tint->SetVector ( vec3_t ( config.visuals_modulation_enemy_visible_color [ 0 ], config.visuals_modulation_enemy_visible_color [ 1 ], config.visuals_modulation_enemy_visible_color [ 2 ] ) );
				}

				override_mat ( visuals::chams::materials.at ( config.visuals_modulation_enemy_material ) );
				
				/*float delta = interfaces::globals->cur_time - ( local_player::m_data.pointer->simulation_time ( ) - local_player::m_data.pointer->get_old_simulation_time ( ) );
				float mul = 1.0 / 0.2f;

				vec3_t lerp = math::lerp ( local_player::m_data.pointer->origin(), csgo::real_origin, std::clamp ( delta * mul, 0.f, 1.f ) );

				matrix3x4_t ret [ 128 ];
				memcpy ( ret, csgo::fake_matrix, sizeof ( matrix3x4_t [ 128 ] ) );

			

				float flTimeDelta =  local_player::m_data.pointer->simulation_time ( ) - local_player::m_data.pointer->get_old_simulation_time ( );
				
			

				for ( size_t i {}; i < 128; ++i ) {
					const vec3_t curent_origin = matrix_get_origin ( custom_bone_to_world [ i ] );
					const vec3_t old_origin = matrix_get_origin ( csgo::fake_matrix [ i ] );
					auto interpolated_origin = math::lerp ( curent_origin, old_origin, flTimeDelta );

					matrix_set_origin ( interpolated_origin, ret [ i ] );
				}*/
				matrix3x4_t ret [ 128 ];
				
				for ( auto & i : csgo::fake_matrix ) {
					i [ 0 ][ 3 ] += info.origin.x;
					i [ 1 ][ 3 ] += info.origin.y;
					i [ 2 ][ 3 ] += info.origin.z;
				}
				o_draw_model_exec ( ecx, edx, ctx, state, info, csgo::fake_matrix );
				/*int angle_color = 0;
				for ( int angle = -180; angle <= 180; angle += 45 ) {
					matrix3x4_t newmat [ 128 ];
					const float color_rotate [ 4 ] = { 255 / angle_color,   255 / angle_color,  255 / angle_color, 255 };
					interfaces::render_view->modulate_color ( config.visuals_modulation_enemy_visible_color );
					player_manager::get_rotated_matrix ( local_pointer->abs_origin ( ), custom_bone_to_world, angle, newmat );
					override_mat ( visuals::chams::materials.at ( config.visuals_modulation_enemy_material ) );
					o_draw_model_exec ( ecx, edx, ctx, state, info, newmat );
					angle_color++;
				}*/
				interfaces::render_view->modulate_color ( old_modulation );
				interfaces::model_render->override_material ( NULL );


			}
			else if (entity->is_alive()) {

				bool is_teammate = entity->team ( ) == local_player::m_data.pointer->team ( );
				bool is_enemy = !is_teammate;
			
				//interfaces::entity_list->get_client_entity_handle( )
			//	auto index = entity->index ( );
				//interfaces::console->console_printf ( "drawing index %i in dme \n", index );
				if ( is_enemy && config.visuals_modulation_enemy_enabled ) {
				

					if ( config.visuals_modulation_enemy_xyz ) {
						/* 
						Exception thrown at 0x79C1C55C (materialsystem.dll) in csgo.exe: 0xC0000005: Access violation reading location 0x00000000.
						*/
						
			
						interfaces::render_view->modulate_color ( config.visuals_modulation_enemy_xyz_color );
						visuals::chams::materials.at ( config.visuals_modulation_enemy_material )->set_material_var_flag ( material_var_flags_t::material_var_ignorez, true );
						/*crash */
						if ( config.visuals_modulation_enemy_material == visuals::chams::mat_type::glow ) {
							bool found_tint;
							const auto tint = visuals::chams::materials.at ( visuals::chams::mat_type::glow )->find_var ( "$envmaptint", &found_tint );

							if ( found_tint && tint )
								tint->SetVector ( vec3_t ( config.visuals_modulation_enemy_xyz_color [ 0 ], config.visuals_modulation_enemy_xyz_color [ 1 ], config.visuals_modulation_enemy_xyz_color [ 2 ] ) );
						}
						const float red[4] = { 255,0,0, 255 };
						const float green [ 4 ] = { 0,255,0, 255 };
						const float blue [ 4 ] = { 0,0,255, 255 };

						override_mat ( visuals::chams::materials.at ( config.visuals_modulation_enemy_material ) );
						o_draw_model_exec ( ecx, edx, ctx, state, info, custom_bone_to_world );
						if ( !player_manager::records [ entity->index ( ) ].empty ( ) ) {

							auto oldest = player_manager::records [ entity->index ( ) ].front ( );
							
							interfaces::render_view->modulate_color ( red );
							o_draw_model_exec ( ecx, edx, ctx, state, info, oldest.bone_left );

							interfaces::render_view->modulate_color ( blue );

							o_draw_model_exec ( ecx, edx, ctx, state, info, oldest.bone_right );


						}
						/*if ( player_manager::records [ entity->index ( ) ].size ( ) ) {
							for ( auto record : player_manager::records [ entity->index ( ) ] ) {
								if (record.predicted )
									interfaces::render_view->modulate_color ( red );
								else
									interfaces::render_view->modulate_color ( blue );

								o_draw_model_exec ( ecx, edx, ctx, state, info, record.bone );
							}
				
						}*/
						interfaces::model_render->override_material ( NULL );

					}
					if ( config.visuals_modulation_enemy_visible ) {




						interfaces::render_view->modulate_color ( config.visuals_modulation_enemy_visible_color );
						visuals::chams::materials.at ( config.visuals_modulation_enemy_material )->set_material_var_flag ( material_var_flags_t::material_var_ignorez, false );

						if ( config.visuals_modulation_enemy_material == visuals::chams::mat_type::glow ) {
							bool found_tint;
							const auto tint = visuals::chams::materials.at ( visuals::chams::mat_type::glow )->find_var ( "$envmaptint", &found_tint );

							if ( found_tint && tint )
								tint->SetVector ( vec3_t ( config.visuals_modulation_enemy_visible_color [ 0 ], config.visuals_modulation_enemy_visible_color [ 1 ], config.visuals_modulation_enemy_visible_color [ 2 ] ) );
						}

						override_mat ( visuals::chams::materials.at ( config.visuals_modulation_enemy_material ) );

						o_draw_model_exec ( ecx, edx, ctx, state, info, custom_bone_to_world );
						interfaces::model_render->override_material ( NULL );
					}


					return;
				}
				else if ( is_teammate && config.visuals_modulation_team_enabled ) {
					if ( config.visuals_modulation_team_visible ) {




						interfaces::render_view->modulate_color ( config.visuals_modulation_team_visible_color );
						visuals::chams::materials.at ( config.visuals_modulation_team_material )->set_material_var_flag ( material_var_flags_t::material_var_ignorez, false );


						override_mat ( visuals::chams::materials.at ( config.visuals_modulation_team_material ) );

						o_draw_model_exec ( ecx, edx, ctx, state, info, custom_bone_to_world );
						interfaces::model_render->override_material ( NULL );
					}

					if ( config.visuals_modulation_team_xyz ) {



						interfaces::render_view->modulate_color ( config.visuals_modulation_team_xyz_color );
						visuals::chams::materials.at ( config.visuals_modulation_team_material )->set_material_var_flag ( material_var_flags_t::material_var_ignorez, true );


						override_mat ( visuals::chams::materials.at ( config.visuals_modulation_team_material ) );

						o_draw_model_exec ( ecx, edx, ctx, state, info, custom_bone_to_world );
						interfaces::model_render->override_material ( NULL );

					}

				}
			}
		}
	}
	else if ( config.visuals_modulation_local_enabled && is_arm && config.visuals_modulation_local_arms ) {/*float visuals_modulation_local_weapon_color [ 4 ] = { 114, 64, 133, 255 };
	   float visuals_modulation_local_arms_color [ 4 ] = { 114, 64, 133, 255 };*/
		interfaces::render_view->modulate_color ( config.visuals_modulation_local_arms_color );


		if ( config.visuals_modulation_local_arms_material == visuals::chams::mat_type::glow ) {
			bool found_tint;
			const auto tint = visuals::chams::materials.at ( visuals::chams::mat_type::glow )->find_var ( "$envmaptint", &found_tint );

			if ( found_tint && tint )
				tint->SetVector ( vec3_t ( config.visuals_modulation_local_arms_color [ 0 ], config.visuals_modulation_local_arms_color [ 1 ], config.visuals_modulation_local_arms_color [ 2 ] ) );
		}

		override_mat ( visuals::chams::materials.at ( config.visuals_modulation_local_arms_material ) );

		o_draw_model_exec ( ecx, edx, ctx, state, info, custom_bone_to_world );
		interfaces::model_render->override_material ( NULL );
		return;
	}
	 else if (config.visuals_modulation_local_enabled  && is_weapon && config.visuals_modulation_local_weapon ) {
		interfaces::render_view->modulate_color ( config.visuals_modulation_local_weapon_color );


		if ( config.visuals_modulation_local_weapon_material == visuals::chams::mat_type::glow ) {
			bool found_tint;
			const auto tint = visuals::chams::materials.at ( visuals::chams::mat_type::glow )->find_var ( "$envmaptint", &found_tint );

			if ( found_tint && tint )
				tint->SetVector ( vec3_t ( config.visuals_modulation_local_weapon_color [ 0 ], config.visuals_modulation_local_weapon_color [ 1 ], config.visuals_modulation_local_weapon_color [ 2 ] ) );
		}

		override_mat ( visuals::chams::materials.at ( config.visuals_modulation_local_weapon_material ) );

		o_draw_model_exec ( ecx, edx, ctx, state, info, custom_bone_to_world );
		interfaces::model_render->override_material ( NULL );
		return;
	}





	o_draw_model_exec ( ecx, edx, ctx, state, info, custom_bone_to_world );
	interfaces::model_render->override_material ( NULL );
}