#include "../../features.hpp"
#include "../../../menu/render/menu_render.hpp"


namespace visuals::projectiles {

	std::vector<ImVec2> poly_test;
	void think ( ) {
		bool is_atleast_one = config.visuals_world_draw_grenades || config.visuals_world_draw_weapons;
		if ( !is_atleast_one )
			return;

		const auto is_grenade = [ ] ( const int id ) {
			return  id == 114 || id == 157 || id == 48 || id == 9 || id == 100 ||id == 156 ;
		};
	
		poly_test.clear ( );
		visuals::projectiles::grenades::grenades.clear ( );

		float radius = 100;
		float step = M_PI * 2.0 / 50;


	
		for ( int i = 1; i < interfaces::entity_list->get_highest_index ( ); i++ ) {

			entity_t * entity = reinterpret_cast< entity_t * >( interfaces::entity_list->get_client_entity ( i ) );
			if ( !entity )
				continue;


			const auto id = entity->client_class ( )->class_id;
		
		
	      if( is_grenade( id ) )
			visuals::projectiles::grenades::think ( entity );
		}
	}
	void draw ( ) {
	//	visuals::projectiles::weapons::draw ( );
		visuals::projectiles::grenades::draw ( );
	
	}
}