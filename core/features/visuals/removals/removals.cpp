#include "../../helpers/helpers.h"
#include <renderlib/imgui/imgui.h>

#include <renderlib/imgui/imgui_internal.h>
#include "../../../render/gui/gui.h"
#include <config.h>

#include "../visuals.h"


namespace visuals::removals {
	void remove_smoke ( ) {
		if ( !config.local_visual.removals_input [ REMOVALS_SMOKE ] )
			return;

		static auto smoke_count = *reinterpret_cast< uint32_t ** >( utilities::pattern_scan ( "client.dll", "A3 ? ? ? ? 57 8B CB" ) + 1 );
		static bool removed = false;



		static std::vector<const char *> smoke_materials = {
			"particle/vistasmokev1/vistasmokev1_fire",
			"particle/vistasmokev1/vistasmokev1_smokegrenade",
			"particle/vistasmokev1/vistasmokev1_emods",
			"particle/vistasmokev1/vistasmokev1_emods_impactdust"
		};
		static std::vector< i_material *> materials;

		if ( materials.empty ( ) ) {
			for ( auto material_name : smoke_materials ) {
				i_material * smoke = interfaces::material_system->find_material ( material_name, TEXTURE_GROUP_OTHER );
		
				materials.push_back ( smoke );

			}
		}
		
		*( int * ) smoke_count = 0;

		for ( auto& mat : materials ) {
			mat->set_material_var_flag ( material_var_wireframe, true );
		}
	}

	void remove_flash ( ) {
		if ( !config.local_visual.removals_input [ REMOVALS_FLASH ] )
			return;

		if ( local_player::ptr() && local_player::ptr()->is_alive ( ) ) {
			if ( local_player::ptr()->flash_duration ( ) > 0.0f )
				local_player::ptr()->flash_duration ( ) = 0.0f;
		}
	}
}