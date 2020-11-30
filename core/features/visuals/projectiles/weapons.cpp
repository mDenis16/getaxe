#include <vector>
#include "../../features.hpp"
#include "../../../menu/render/menu_render.hpp"


namespace visuals::projectiles::weapons {


		std::vector<weapon_data> weapons;

		void think (weapon_t* weapon ) {
			if ( !variables::visuals::world::draw_weapons )
				return;
	
			if ( weapon->dormant ( ) )
				return;

			if ( !weapon->is_weapon ( ) )
				return;

			void * owner = interfaces::entity_list->get_client_entity_handle ( weapon->owner_handle ( ) );
			if ( owner )
				return;
		

		    weapon_data _data;
	 
			_data.name = _data.get_name ( );
			_data.owner_name = std::to_string ( weapon->owner_handle ( ) );
			_data.on_screen = visuals::get_playerbox ( reinterpret_cast< entity_t * >(weapon), _data.box_data );
			_data.weapon_icon = visuals::weapon_to_icon ( weapon->item_definition_index ( ) );
			weapons.push_back ( _data );
		}
		void draw ( ) {
			for ( auto _data : weapons ) {
				if ( _data.on_screen ) {
					visuals::draw_text ( c_menu::get ( ).weapon_icons, _data.weapon_icon, ImVec2 ( ( _data.box_data.x + ( ( _data.box_data.x + _data.box_data.w ) - _data.box_data.x ) / 2.0f ), _data.box_data.y + _data.box_data.h + 4 ), 12, variables::visuals::world::draw_weapons_color, 0.1f, true, true );

					
				}
			}
		}
	
}