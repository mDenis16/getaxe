#include "../features.hpp"


namespace visuals {

	c_handler * handler = new c_handler ( );

	std::mutex mtx;

	void c_handler::on_entity_created ( void * ent ) {

		entity_t * entity = reinterpret_cast< entity_t * > ( ent );
		/*
		case CBaseCSGrenadeProjectile:
		{
			if ( const auto model = prjectile->model ( ); model && strstr ( model->name, "flashbang" ) )
				return "k";
			else
				return "l";
			break;
		}
		case CDecoyProjectile:
			return "o";
			break;
		case CMolotovProjectile:
			return "n";
			break;
		case CSmokeGrenadeProjectile:
			return "m";
			break;
		default:
			return "?";
			break;
		*/
		const auto is_grenade = [ ] ( const int id ) {
			return  id == CBaseCSGrenadeProjectile || id == CDecoyProjectile || id == CMolotovProjectile || id == CSmokeGrenadeProjectile;
		};

		auto client_class = entity->client_class ( );

		if ( !client_class )
			return;

		mtx.lock ( );

		std::cout << "on_entity_created " << std::endl;

		if ( client_class->class_id == CCSPlayer ) {

			visual_player * data = new visual_player ( );
			
			data->index = entity->index ( );
			data->class_id = entity->client_class ( )->class_id;
			data->entity = entity;
			data->player = reinterpret_cast< player_t * >( entity );
			entity_list.push_back ( data );

			//delete data;
		}
		else if ( client_class->class_id == CInferno ) {
			visual_grenade * data = new visual_grenade ( );
			data->index = entity->index ( );
			data->class_id = entity->client_class ( )->class_id;
			data->entity = entity;
			data->spawn_curtime = interfaces::globals->cur_time;
			entity_list.push_back ( data );
		}
		else if ( is_grenade ( client_class->class_id ) ) {

			visual_projectile * data = new visual_projectile ( );
			data->index = entity->index ( );
			data->class_id = entity->client_class ( )->class_id;
			data->entity = entity;
			data->spawn_curtime = interfaces::globals->cur_time;
			data->explode_time = interfaces::globals->cur_time + 1.6f;

			entity_list.push_back ( data );

		

		}
	
		mtx.unlock ( );
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

		if ( legitbot && legitbot->active_target && legitbot->active_target == ent )
			legitbot->reset_target ( );

		const auto it = std::find_if ( entity_list.begin ( ), entity_list.end ( ), [ & ] ( visual_data * data ) {
			return data->index == entity_index;
		} );

		mtx.lock ( );

		if ( it != entity_list.end ( ) ) {
			const int list_index = std::distance ( entity_list.begin ( ), it );

			const auto it = std::find_if ( handler->entity_list.begin ( ), handler->entity_list.end ( ), [ & ] ( visual_data * data ) {
				return data->index == entity_index;
			} );

			if ( it != handler->entity_list.end ( ) ) {

				const int list_index = std::distance ( handler->entity_list.begin ( ), it );
				
				delete entity_list.at ( list_index );
				entity_list.erase ( it );
			}
		}
		mtx.unlock ( );

	}

	void c_handler::init ( ) {
		interfaces::entity_list->add_listener_entity ( this );
	}

	void c_handler::remove ( ) {
		interfaces::entity_list->remove_listener_entity ( this );
	}

	void c_handler::on_update ( ) {

		mtx.lock ( );
		for ( auto & entity : entity_list ) {
			if ( entity->mark_deletetion )
				continue;

			entity->on_update ( );
		}
		mtx.unlock ( );


	}

	
	void c_handler::on_render ( ) {

		
		if ( visuals::render && legitbot )
			legitbot->draw_debug ( visuals::render );


		mtx.lock ( );
		for ( auto & entity : entity_list ) {
			
			if ( entity->mark_deletetion ) {

				const auto it = std::find_if ( entity_list.begin ( ), entity_list.end ( ), [ & ] ( visual_data * data ) {
					return data->index == entity->index;
				} );

				if ( it != entity_list.end ( ) ) {
					const int list_index = std::distance ( entity_list.begin ( ), it );

					delete entity_list.at ( list_index );
					entity_list.erase ( it );
					std::cout << "removed entity from list " << std::endl;
					continue;
				}
		
			}


			entity->on_render ( );
			
		}
		mtx.unlock ( );
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
