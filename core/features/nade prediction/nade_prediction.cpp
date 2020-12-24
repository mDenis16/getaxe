#include "nade_prediction.hpp"

#include "../../menu/render/menu_render.hpp"

#include "../../menu/ImGui/imgui.h"
#include "../../menu/ImGui/imgui_internal.h"
#include "../../../dependencies/interfaces/i_memalloc.h"
c_nade_prediction nade_pred;

void c_nade_prediction::predict ( c_usercmd * user_cmd ) {
	if ( !interfaces::engine->is_connected ( ) && !interfaces::engine->is_in_game ( ) )
		return;

	if ( !config.visuals_world_grenade_prediction )
		return;
	player_hurt = false;
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

	eye_origin = local_player::m_data.pointer->get_eye_pos ( );
	angles = user_cmd->viewangles;

	thrown = angles;

	if ( thrown.x < 0 )
		thrown.x = -10 + thrown.x * ( ( 90 - 10 ) / 90.0f );
	else
		thrown.x = -10 + thrown.x * ( ( 90 + 10 ) / 90.0f );


	auto primary_attack = user_cmd->buttons & in_attack;
	auto secondary_attack = user_cmd->buttons & in_attack2;

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
	thrown_direction = ( thrown_direction * new_velocity ) + local_player::m_data.pointer->velocity ( );

	for ( auto time = 0.0f; index < 500; time += step ) {

		pos = start + thrown_direction * step;
		trace_t trace;
		ray_t ray;
		trace_filter_skip_one_entity filter ( local_player::m_data.pointer );

		ray.initialize ( start, pos );
		interfaces::trace_ray->trace_ray ( ray, MASK_SOLID, &filter, &trace );

		if ( trace.flFraction != 1.0f ) {
			thrown_direction = trace.plane.normal * -2.0f * thrown_direction.dot ( trace.plane.normal ) + thrown_direction;

			thrown_direction *= restitution;

			pos = start + thrown_direction * trace.flFraction * step;

			time += ( step * ( 1.0f - trace.flFraction ) );
		}

		auto detonate = detonated ( local_player::m_data.pointer->active_weapon ( ), time, trace );

		_points.at ( index++ ) = c_nadepoint ( start, pos, trace.flFraction != 1.0f, true, trace.plane.normal, detonate );
		start = pos;

		thrown_direction.z -= gravity * trace.flFraction * step;

		if ( detonate ) {
			for ( int i { 1 }; i <= interfaces::globals->max_clients; ++i ) {

				player_t * player = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( i ) );
				if ( !player->is_valid_player ( ) )
					continue;

				// get center of mass for player.
				vec3_t center = player->world_space_center ( );

				// get delta between center of mass and final nade pos.
				vec3_t delta = center - pos;

				if ( local_player::m_data.active_weapon->item_definition_index ( ) == weapon_hegrenade ) {

					if ( delta.length ( ) > 350.f )
						continue;
					auto dmg_ray = ray_t ( ); dmg_ray.initialize ( pos, center );
					// check if our path was obstructed by anything using a trace.
					interfaces::trace_ray->trace_ray ( dmg_ray, MASK_SHOT, ( trace_filter * ) &filter, &trace );

					// something went wrong here.
					if ( !trace.entity || trace.entity != player )
						continue;

					// rather 'interesting' formula by valve to compute damage.
					float d = ( delta.length ( ) - 25.f ) / 140.f;
					float damage = 105.f * std::exp ( -d * d );

					// scale damage.
					autowall::ScaleDamage ( hitgroup_chest, player, 1.f, damage );

					if ( damage > 5 ) {
						player_hurt = true;
						break;

					}

				}
			}
			break;
		}
	}

	for ( auto n = index; n < 500; ++n )
		_points.at ( n ).m_valid = false;

	_predicted = true;
	should_draw = true;
}

bool c_nade_prediction::detonated ( weapon_t * weapon, float time, trace_t & trace ) {
	if ( !weapon )
		return true;

	const auto index = weapon->item_definition_index ( );

	switch ( index ) {
	case weapon_hegrenade:
	case weapon_flashbang:
		if ( time > 2.5f )
			return true;
		break;

	case weapon_molotov:
	case weapon_incgrenade:
		if ( trace.flFraction != 1.0f && trace.plane.normal.z > 0.7f || time > 3.5f )
			return true;
		break;

	case weapon_decoy:
	case weapon_smokegrenade:
		if ( time > 5.0f )
			return true;
		break;
	}

	return false;
}
/*
case class_ids::molotov_projectile:
		name = "incendiary";
		icon = "p";
		break;
	case class_ids::smoke_grenade_projectile:
		name = "smoke";
		icon = "m";
		break;
	case class_ids::base_cs_grenade_projectile:
	{
		const model_t * model = ent->model ( );

		if ( !model )
			return std::tuple< std::string, std::string> ( name, icon );

		const auto hdr = interfaces::model_info->get_studio_model ( model );
		if ( hdr->name_char_array [ 16 ] == 's' ) {
			name = "flashbang";
			icon = "k";
		}
		else {
			name = "he grenade";
			icon = "l";
		}
	}

		break;
	case class_ids::decoy_projectile:
		name = "decoy";
		icon = "o";
		break;
	default:
		break;
	}
*/
bool c_nade_prediction::detonated ( class_ids id, float time, trace_t & trace ) {



	switch ( id ) {
	case base_cs_grenade_projectile:
		if ( time > 2.5f )
			return true;
		break;

	case molotov_projectile:
		if ( trace.flFraction != 1.0f && trace.plane.normal.z > 0.7f || time > 3.5f )
			return true;
		break;
	case smoke_grenade_projectile:
		if ( time > 5.0f )
			return true;
		break;
	}

	return false;
}
void c_nade_prediction::trace ( c_usercmd * user_cmd ) {
	if ( !interfaces::engine->is_connected ( ) && !interfaces::engine->is_in_game ( ) )
		return;
	static bool predicted_last_Tick = false;
	if ( !( user_cmd->buttons & in_attack ) && !( user_cmd->buttons & in_attack2 ) ) {
		_predicted = false;
		_w2s = false;
		should_draw = false;
		if ( predicted_last_Tick ) {

			should_empty = true;
			predicted_last_Tick = false;
		}
		return;
	}

	const static std::vector< int > nades {
		weapon_flashbang,
		weapon_smokegrenade,
		weapon_hegrenade,
		weapon_molotov,
		weapon_decoy,
		weapon_incgrenade
	};

	auto weapon = local_player::m_data.pointer->active_weapon ( );
	if ( !weapon )
		return;

	predicted_last_Tick = true;
	if ( std::find ( nades.begin ( ), nades.end ( ), weapon->item_definition_index ( ) ) != nades.end ( ) )
		return predict ( user_cmd );

	_predicted = false;
}

void c_nade_prediction::paint_traverse ( ) {


	points_detonate.clear ( );
	points.clear ( );
	if ( _predicted ) {

		for ( auto & p : _points ) {
			if ( !p.m_valid )
				continue;


			if ( interfaces::debug_overlay->world_to_screen ( p.m_start, p.m_start_2 ) && interfaces::debug_overlay->world_to_screen ( p.m_end, p.m_end_2 ) ) {
				points.push_back ( ImVec2 ( p.m_start.x, p.m_start.y ) );
			}
			if ( p.m_detonate ) {
				float step = M_PI * 2.0 / 50;


				for ( float a = 0; a < ( M_PI * 2.0 ); a += step ) {
					vec3_t location ( 144 * cos ( a ) + p.m_end.x, 144 * sin ( a ) + p.m_end.y, p.m_end.z );
					vec3_t w2s_pos = vec3_t ( );
					if ( interfaces::debug_overlay->world_to_screen ( location, w2s_pos ) ) {
						points_detonate.push_back ( ImVec2 ( w2s_pos.x, w2s_pos.y ) );
					}
				}
			}
		}
		_updated = true;
	}


}
void c_nade_prediction::draw ( ) {


	if ( points_detonate.size ( ) > 0 )
		c_menu::get ( ).draw->AddConvexPolyFilled ( points_detonate.data ( ), points_detonate.size ( ), player_hurt ? ImColor ( 255, 0, 0, 150 ) : ImColor ( config.visuals_world_grenade_prediction_color [ 0 ], config.visuals_world_grenade_prediction_color [ 1 ], config.visuals_world_grenade_prediction_color [ 2 ], 0.4f ) );


	if ( points.size ( ) > 0 )
		c_menu::get ( ).draw->AddPolyline ( points.data ( ), points.size ( ), player_hurt ? ImColor ( 255, 0, 0, 255 ) : ImColor ( config.visuals_world_grenade_prediction_color [ 0 ], config.visuals_world_grenade_prediction_color [ 1 ], config.visuals_world_grenade_prediction_color [ 2 ], config.visuals_world_grenade_prediction_color [ 3 ] ), false, 0.8f );


	/*
	if ( _updated ) {
		copy_points = points;
		copy_points_detonate = points_detonate;
		points.clear ( );
		points_detonate.clear ( );
		_updated = false;
	}
	if ( should_empty ) {
		if ( copy_points.size ( ) ) copy_points.clear ( );
		if ( copy_points_detonate.size ( ) ) copy_points_detonate.clear ( );
		if ( points.size ( ) ) points.clear ( );
		if ( points_detonate.size ( ) ) points_detonate.clear ( );
		should_empty = false;
	}*/
	//	if ( p.m_detonate || p.m_plane )
		//	c_menu::get ( ).draw->AddCircleFilled ( ImVec2 ( p.m_start_2.x, p.m_start_2.y ), 2.f, p.m_detonate ? ImColor ( 255, 0, 0 ) : ImColor ( 255, 255, 255 ) );

	//	if ( p.m_detonate )
		//	draw_3d_circle ( p.m_end, 30, 170, color::red ( ) );


	//}

}