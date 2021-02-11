#include <vector>
#include "../../features.hpp"
#include "../../../menu/render/menu_render.hpp"
#include "../../nade prediction/nade_prediction.hpp"


namespace visuals::projectiles::grenades::proximity {


	std::vector<proximity_data*> proximity_grenades;
	bool proximity_data::detonated (  float time, trace_t & trace ) {
		if ( !this->grenade )
			return true;

		if ( !this->grenade->client_class ( ) )
			return false;

		const auto index = this->grenade->client_class ( )->class_id;

		switch ( index ) {
		case class_ids::base_cs_grenade_projectile:
			if ( time > 2.5f )
				return true;
			break;

		case class_ids::molotov_projectile:
			if ( trace.flFraction != 1.0f && trace.plane.normal.z > 0.7f || time > 3.5f )
				return true;
			break;

		case class_ids::decoy_projectile:
		case class_ids::smoke_grenade_projectile:
			if ( time > 5.0f )
				return true;
			break;
		}

		return false;
	}
	
	void proximity_data::predict ( ) {
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

		eye_origin = this->position;
		angles = math::calc_angle ( this->position, grenade->origin ( ) );

		thrown = angles;

		if ( thrown.x < 0 )
			thrown.x = -10 + thrown.x * ( ( 90 - 10 ) / 90.0f );
		else
			thrown.x = -10 + thrown.x * ( ( 90 + 10 ) / 90.0f );


		auto primary_attack = true;// user_cmd->buttons & in_attack;
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
		vec3_t vel = ( grenade->origin() - this->position ) / math::ticks_to_time(15);

		thrown_direction = ( thrown_direction * new_velocity ) + vel;

		for ( auto time = 0.0f; index < 500; time += step ) {

			pos = start + thrown_direction * step;
			trace_t trace;
			ray_t ray;
			trace_filter_skip_one_entity filter ( grenade );

			ray.initialize ( start, pos );
			interfaces::trace_ray->trace_ray ( ray, MASK_SOLID, &filter, &trace );

			if ( trace.flFraction != 1.0f ) {
				thrown_direction = trace.plane.normal * -2.0f * thrown_direction.dot ( trace.plane.normal ) + thrown_direction;

				thrown_direction *= restitution;

				pos = start + thrown_direction * trace.flFraction * step;

				time += ( step * ( 1.0f - trace.flFraction ) );
			}

			auto detonate = detonated ( time, trace );

			//_points.at ( index++ ) = c_nadepoint ( start, pos, trace.flFraction != 1.0f, true, trace.plane.normal, detonate );
			start = pos;

			thrown_direction.z -= gravity * trace.flFraction * step;

			if ( detonate ) {
				this->predicted = true;
				this->predicted_position = pos;
				break;
			}
		}
	}

	proximity_data * search_data ( entity_t * grenade ) {
		proximity_data * ptr = nullptr;
		for ( auto& grenade_data : proximity_grenades ) {
			if ( grenade_data->grenade == grenade ) {
				ptr = grenade_data;
				break;
			}
		}
		return ptr;
	}

	void think (  ) {
		return;
		if ( !config.visuals_world_draw_grenades )
			return;


		const auto is_grenade = [ ] ( const int id ) {
			return  id == class_ids::molotov_projectile || id == smoke_grenade_projectile || id == decoy_projectile || id == base_cs_grenade_projectile;
		};


		for ( int i = 1; i < interfaces::entity_list->get_highest_index ( ); i++ ) {

			entity_t * grenade = reinterpret_cast< entity_t * >( interfaces::entity_list->get_client_entity ( i ) );
			if ( !grenade )
				continue;

			if ( !grenade->client_class ( ) )
				continue;

			const auto id = grenade->client_class ( )->class_id;


			if ( is_grenade ( id ) ) {

				auto client_class = grenade->client_class ( );

				if ( !client_class )
					return;

				auto entity_index = grenade->index ( );

				proximity_data * instance = search_data ( grenade );

				if ( instance != nullptr ) {
				//	interfaces::console->console_printf ( "GRENADE SPAWN TICK %i, CURRENT TICK %i, PREDICTED %i", instance->spawn_tick, interfaces::globals->tick_count, instance->predicted );
					if (!instance->predicted &&  ( interfaces::globals->tick_count - instance->spawn_tick ) > 15 ) {
						instance->predict ( );
					}
				}
				else {
					auto data = new proximity_data ( grenade );
					proximity_grenades.push_back ( data );
				}
			}
		}
	}
	
	void draw ( ) {
		return;
		for ( auto _data : proximity_grenades ) {
			if ( _data->predicted )
				continue;

			ImVec2 screen_pos = ImVec2 ( );

			if (visuals::world_to_screen(_data->predicted_position, screen_pos) )
				c_menu::get ( ).draw->AddCircleFilled ( screen_pos, 30, ImColor ( 255, 25, 55, 255 ) );
			
		}
	}

}