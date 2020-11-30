#include <vector>
#include "../../features.hpp"
#include "../../../menu/render/menu_render.hpp"


namespace visuals::projectiles::grenades {


	std::vector<grenade_data> grenades;

	void think ( entity_t * grenade ) {
		if ( !variables::visuals::world::draw_grenades )
			return;

		if ( grenade->dormant ( ) )
			return;


		//if ( !reinterpret_cast< weapon_t * >( grenade )->is_grenade ( ) )
		//	return;
//
		auto client_class = grenade->client_class ( );
		auto model = grenade->model ( );

		if ( !model )
			return;

		if ( model ) {
			vec3_t grenade_position, grenade_origin;

			auto model = interfaces::model_info->get_studio_model ( grenade->model ( ) );

			if ( !model || !strstr ( model->name_char_array, "thrown" ) && !strstr ( model->name_char_array, "dropped" ) )
				return;
			std::string name = model->name_char_array;

		
			grenade_data _data;
			_data.name = name;
			_data.on_screen = visuals::get_playerbox ( reinterpret_cast< entity_t * >( grenade ), _data.box_data );
			_data.weapon_icon = visuals::weapon_to_icon ( reinterpret_cast< weapon_t * >( grenade )->item_definition_index ( ) );
			grenades.push_back ( _data );
		}
	}
	void draw ( ) {
		for ( auto _data : grenades ) {
			if ( _data.on_screen ) {
				visuals::draw_text ( c_menu::get ( ).weapon_icons, _data.weapon_icon, ImVec2 ( ( _data.box_data.x + ( ( _data.box_data.x + _data.box_data.w ) - _data.box_data.x ) / 2.0f ), _data.box_data.y + _data.box_data.h + 4 ), 12, variables::visuals::world::draw_weapons_color, 0.1f, true, true );
			}
		}
	}

}