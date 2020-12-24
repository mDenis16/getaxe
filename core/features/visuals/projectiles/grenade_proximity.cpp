#include <vector>
#include "../../features.hpp"
#include "../../../menu/render/menu_render.hpp"
#include "../../nade prediction/nade_prediction.hpp"


namespace visuals::projectiles::grenades::proximity {


	std::vector<proximity_data> proximity_grenades;
	void grenade_thrown ( i_game_event * event ) {
		if ( !event )
			return;

		if ( !interfaces::engine->is_connected ( ) && !interfaces::engine->is_in_game ( ) )
			return;



		auto entity = interfaces::engine->get_player_for_user_id ( event->get_int ( "userid" ) );
		if ( !entity )
			return;




	}

	void think ( ) {

		if ( !config.visuals_world_draw_grenades_proximity )
			return;
		static float last_shot_time [ 64 ];


		
		for ( int i = 1; i <= interfaces::globals->max_clients; i++ ) {
			player_t * entity = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( i ) );
		
			int detonations_count = 0;
			if ( !entity )
				continue;

			if ( entity->dormant ( ) )
				continue;
		
			if ( !entity->is_alive ( ) )
				continue;

			if ( !entity->client_class ( ) )
				continue;

			if ( !entity->active_weapon ( ) )
				continue;

	

			if ( !entity->active_weapon ( )->is_grenade() )
				continue;

			/*
			if ( !weapon->pin_pulled( ) || ( ucmd->buttons & in_attack ) || ( ucmd->buttons & in_attack2 ) )
		{
			float throwTime = weapon->throw_time( );
			*/
			interfaces::console->console_printf ( "simtime %f | shot time %f | pin_pulled %i | throw time %f \n", entity->simulation_time ( ), entity->active_weapon ( )->m_fLastShotTime ( ), entity->active_weapon ( )->pin_pulled(), entity->active_weapon ( )->throw_time() );
			bool shot_this_tick = false;
			if (entity->active_weapon()->m_fLastShotTime() != last_shot_time[i] )
				shot_this_tick = true;
			
			last_shot_time [ i ] = entity->active_weapon ( )->m_fLastShotTime ( );

			if ( !shot_this_tick )
				continue;
			
			//if ( entity->active_weapon ( )->throw_time ( ) == 0.0 )
		//		continue;


			constexpr float restitution = 0.45f;
			constexpr float power [ ] = { 1.0f, 1.0f, 0.5f, 0.0f };
			constexpr float velocity = 403.0f * 0.9f;

			float step, gravity, new_velocity, unk01;
			int index {}, grenade_act { 1 };
			vec3_t pos, thrown_direction, start, eye_origin;
			vec3_t angles, thrown;

			static auto sv_gravity = interfaces::console->get_convar ( "sv_gravity" );

			gravity = sv_gravity->get_float ( ) / 8.0f;
			step = interfaces::globals->interval_per_tick;

			eye_origin = entity->get_eye_pos ( );
			angles = entity->eye_angles ( );

			thrown = angles;

			if ( thrown.x < 0 )
				thrown.x = -10 + thrown.x * ( ( 90 - 10 ) / 90.0f );
			else
				thrown.x = -10 + thrown.x * ( ( 90 + 10 ) / 90.0f );


			auto primary_attack = false;
			auto secondary_attack = false;//user_cmd->buttons & in_attack2;

			if ( primary_attack && secondary_attack )
				grenade_act = ACT_LOB;
			else if ( secondary_attack )
				grenade_act = ACT_DROP;

			unk01 = power [ grenade_act ];

			unk01 = unk01 * 0.7f;
			unk01 = unk01 + 0.3f;

			new_velocity = velocity * unk01;

			math::angle_vectors ( thrown, thrown_direction );

			start = eye_origin + thrown_direction * 16.0f;
			thrown_direction = ( thrown_direction * new_velocity ) + entity->velocity ( );

			
			for ( auto time = 0.0f; index < 500; time += step ) {

				pos = start + thrown_direction * step;
				trace_t trace;
				ray_t ray;
				trace_filter_skip_one_entity filter ( entity );

				ray.initialize ( start, pos );
				interfaces::trace_ray->trace_ray ( ray, MASK_SOLID, &filter, &trace );

				if ( trace.flFraction != 1.0f ) {
					thrown_direction = trace.plane.normal * -2.0f * thrown_direction.dot ( trace.plane.normal ) + thrown_direction;

					thrown_direction *= restitution;

					pos = start + thrown_direction * trace.flFraction * step;

					time += ( step * ( 1.0f - trace.flFraction ) );
				}

				auto detonate = nade_pred.detonated ( entity->active_weapon ( ), time, trace );

				
				start = pos;

				thrown_direction.z -= gravity * trace.flFraction * step;

				if ( detonate ) {
					detonations_count++;
				}
				if ( detonations_count >= 3 ) 					{
					proximity_grenades.push_back ( { pos, interfaces::globals->cur_time } );
					detonations_count = 0;
					break;
				}
			}


		
			

		

		}

	}
	void think_paint ( ) {
		
		for ( int t = 0; t < proximity_grenades.size ( ); t++ ) {
			interfaces::debug_overlay->world_to_screen ( proximity_grenades.at ( t ).position, proximity_grenades.at ( t ).w2s );
			if ( std::fabs ( interfaces::globals->cur_time - proximity_grenades.at ( t ).curtime ) > 5.f )
				proximity_grenades.erase ( proximity_grenades.begin ( ) + t );
		}
	}
	void draw ( ) {
		for ( auto _data : proximity_grenades ) {


				c_menu::get ( ).draw->AddCircleFilled ( ImVec2 ( _data.w2s.x, _data.w2s.y ), 30, ImColor ( 0, 0, 0, 200 ) );
			
		}
	}

}