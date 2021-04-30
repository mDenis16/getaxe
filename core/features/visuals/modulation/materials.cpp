#pragma once
#include "../../features.hpp"

namespace modulation {

	std::array<custom_material, 4> materials;

	void release_mem ( ) {
	
	}
	void create_materials ( ) {
	
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

		

		/* never trust win32 memory management inside game hooks */

	
		materials.at ( mat_type::regular ).mat_address = interfaces::material_system->find_material ( "material_textured", NULL );
		materials.at ( mat_type::flat ).mat_address = interfaces::material_system->find_material ( "material_flat", NULL );
		materials.at ( mat_type::reflective ).mat_address = interfaces::material_system->find_material ( "material_reflective", NULL );
		materials.at ( mat_type::glow ).mat_address = interfaces::material_system->find_material ( "overlayglow", NULL );

		for ( auto & mat : materials ) 
			mat.envmaptint = mat.mat_address->find_var ( "$envmaptint", &mat.found_env_tint );
		
		DEBUG_LOG ( "CHAMS INITIALIZED!" );
	}
	i_material * create_material ( shader_type_t shade, bool ignorez, bool wireframe ) {
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

	
}