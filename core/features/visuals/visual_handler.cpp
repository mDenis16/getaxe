#include "../features.hpp"


namespace visuals {

	c_handler * handler = new c_handler ( );

	void c_handler::on_entity_created ( void * ent ) {

		entity_t * entity = reinterpret_cast< entity_t * > ( ent );

		const auto is_grenade = [ ] ( const int id ) {
			return  id == 114 || id == 157 || id == 48 || id == 9 || id == 100 || id == 156;
		};

		auto client_class = entity->client_class ( );

		if ( !client_class )
			return;


		std::cout << "on_entity_created " << std::endl;

		if ( client_class->class_id == CCSPlayer ) {

			visual_player * data = new visual_player ( );
			data->index = entity->index ( );
			data->entity = entity;
			data->player = reinterpret_cast< player_t * >( entity );
			entity_list.push_back ( data );

			//delete data;
		}
		else if ( is_grenade ( client_class->class_id ) ) {

			visual_projectile * data = new visual_projectile ( );
			data->index = entity->index ( );
			data->entity = entity;
			entity_list.push_back ( data );

			//delete data;

		}
	}
	struct isValue {
		int entity_index;

		isValue ( int value ) : entity_index ( value ) { }

		bool operator()( const visual_data * cls ) const {
			return ( cls->index == entity_index );
		}
	};
	void c_handler::on_entity_deleted ( void * ent ) {
		if ( !ent )
			return;

		entity_t * entity = reinterpret_cast< entity_t * > ( ent );

		std::cout << "on_entity_deleted " << std::endl;

		int entity_index = entity->index ( );

		if ( entity_index < 0 )
			return;

		const auto it = std::find_if ( entity_list.begin ( ), entity_list.end ( ), [ & ] ( visual_data * data ) {
			return data->index == entity_index;
		} );

		if ( it != entity_list.end ( ) ) {
			const int list_index = std::distance ( entity_list.begin ( ), it );

			delete entity_list.at ( list_index );
			entity_list.erase ( it );
			std::cout << "removed entity from list " << std::endl;
		}

	}

	void c_handler::init ( ) {
		interfaces::entity_list->add_listener_entity ( this );
	}

	void c_handler::remove ( ) {
		interfaces::entity_list->remove_listener_entity ( this );
	}

	void c_handler::on_update ( ) {
		for ( auto & entity : entity_list )
			entity->on_update ( );

	}

	void c_handler::on_render ( ) {
		for ( auto & entity : entity_list )
			entity->on_render ( );
	}

	void c_handler::local_player ( ) {

		/*	if ( local_player::m_data.pointer != local_player::m_data.old_pointer ) {

				visual_player * data = new visual_player ( );
				data->index = local_player::m_data.pointer->index ( );
				data->entity = local_player::m_data.pointer;
				data->player = reinterpret_cast< player_t * >( local_player::m_data.pointer );


				/*local player should be at 0 in list*/

				/*	if ( entity_list.empty ( ) ) {
						entity_list.push_back ( data );
					}
					else {
						entity_list.at ( 0 ) = data;
					}

					local_player::m_data.old_pointer = local_player::m_data.pointer;
				}*/

	}

	void c_handler::intialization ( ) { 

		if ( entity_list.empty ( ) ) {

			for ( size_t i = 1; i < interfaces::entity_list->get_highest_entity_index ( ); i++ ) {

				auto ent = interfaces::entity_list->get_client_entity ( i );

				if ( !ent )
					continue;

				on_entity_created ( ent );

			}
		}
	}
}
