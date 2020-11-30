#include "../../features.hpp"


namespace visuals::projectiles {

	
	void think ( ) {
		bool is_atleast_one = variables::visuals::world::draw_grenades || variables::visuals::world::draw_weapons;
		if ( !is_atleast_one )
			return;

		visuals::projectiles::weapons::weapons.clear ( );
		visuals::projectiles::grenades::grenades.clear ( );
		for ( size_t i = 1; i < interfaces::entity_list->get_highest_index ( ); i++ ) {

			entity_t * entity = reinterpret_cast< entity_t * >( interfaces::entity_list->get_client_entity ( i ) );
			if ( !entity )
				continue;

			if ( entity->is_player ( ) )
				continue;
		
		
		//	visuals::projectiles::weapons::think ( reinterpret_cast< weapon_t * > ( entity ) );
		//	visuals::projectiles::grenades::think ( entity );
		   
		}
	}
	void draw ( ) {
	//	visuals::projectiles::weapons::draw ( );
		//visuals::projectiles::grenades::draw ( );
	}
}