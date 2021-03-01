#include "../../features.hpp"
#include "../../../helpers/helpers.h"
namespace aimbot {
	std::vector<target> targets;
	
	void populate_list ( ) {
		targets.clear ( );

		for ( int i = 1; i <= interfaces::globals->max_clients; i++ ) {
			player_t * entity = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( i ) );

			if ( !entity )
				continue;

			if ( entity == local_player::m_data.pointer )
				continue;

			if ( !entity->is_alive ( ) )
				continue;

			//if ( entity->dormant ( ) )
				//continue;

			if ( entity->is_teammate ( ) )
				continue;
			if ( !player_manager::records [ i ].size ( ) )
				continue;

			if ( ( entity->origin ( ) - engine_prediction::unpredicted_eye ).length ( ) > local_player::m_data.weapon_data->flRange )
				continue;

			target t = {};
			t.player = entity;
			t.index = entity->index ( );
			t.health = entity->health ( );
			targets.push_back ( t );
		}
	}
	void sort_list ( ) {
		struct {
			bool operator()( struct target a, struct target b ) const {
				auto angle_a = math::calc_angle ( local_player::m_data.eye_position, a.player->get_eye_pos ( ) ); angle_a.angle_normalize ( );
				auto angle_b = math::calc_angle ( local_player::m_data.eye_position, b.player->get_eye_pos ( ) ); angle_b.angle_normalize ( );
				auto fov_a = math::get_fov ( local_player::m_data.orig_viewangle, angle_a );
				auto fov_b = math::get_fov ( local_player::m_data.orig_viewangle, angle_b );
				return  fov_a < fov_b;

			}
		} target_sort;

		std::sort ( targets.begin ( ), targets.end ( ), target_sort );
		if ( targets.size ( ) > 3 )
			targets.erase ( targets.begin ( ) + 3, targets.end ( ) );
	}
	void update_targets ( ) {
		
		for ( auto &target : targets ) {
			target.index = target.player->index ( );
			int trycount = 0;
			while ( !target.hitbox_set && trycount <= 3 ) {
				target.hitbox_set = interfaces::model_info->get_studio_model ( target.player->model ( ) )->hitbox_set ( 0 );
				trycount++;
			}
			target.health = target.player->health ( );
		}
	}
	void select_targets ( ) {
		populate_list ( );
		sort_list ( );
		update_targets ( );
	}
}