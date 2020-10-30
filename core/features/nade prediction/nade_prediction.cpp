#include "nade_prediction.hpp"

c_nade_prediction nade_pred;

void c_nade_prediction::predict( c_usercmd* user_cmd ) {
	if ( !interfaces::engine->is_connected( ) && !interfaces::engine->is_in_game( ) )
		return;

	constexpr float restitution = 0.45f;
	constexpr float power[ ] = { 1.0f, 1.0f, 0.5f, 0.0f };
	constexpr float velocity = 403.0f * 0.9f;

	float step, gravity, new_velocity, unk01;
	int index{}, grenade_act{ 1 };
	vec3_t pos, thrown_direction, start, eye_origin;
	vec3_t angles, thrown;

	static auto sv_gravity = interfaces::console->get_convar( "sv_gravity" );

	gravity = sv_gravity->get_float( ) / 8.0f;
	step = interfaces::globals->interval_per_tick;

	eye_origin = csgo::local_player->get_eye_pos( );
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

	unk01 = power[ grenade_act ];

	unk01 = unk01 * 0.7f;
	unk01 = unk01 + 0.3f;

	new_velocity = velocity * unk01;

	math::angle_vectors( thrown, thrown_direction );

	start = eye_origin + thrown_direction * 16.0f;
	thrown_direction = ( thrown_direction * new_velocity ) + csgo::local_player->velocity( );

	for ( auto time = 0.0f; index < 500; time += step ) {

		pos = start + thrown_direction * step;
		trace_t trace;
		ray_t ray;
		trace_filter_skip_one_entity filter( csgo::local_player );

		ray.initialize( start, pos );
		interfaces::trace_ray->trace_ray( ray, MASK_SOLID, &filter, &trace );

		if ( trace.flFraction != 1.0f ) {
			thrown_direction = trace.plane.normal * -2.0f * thrown_direction.dot( trace.plane.normal ) + thrown_direction;

			thrown_direction *= restitution;

			pos = start + thrown_direction * trace.flFraction * step;

			time += ( step * ( 1.0f - trace.flFraction ) );
		}

		auto detonate = detonated( csgo::local_player->active_weapon( ), time, trace );

		_points.at( index++ ) = c_nadepoint( start, pos, trace.flFraction != 1.0f, true, trace.plane.normal, detonate );
		start = pos;

		thrown_direction.z -= gravity * trace.flFraction * step;

		if ( detonate )
			break;
	}

	for ( auto n = index; n < 500; ++n )
		_points.at( n ).m_valid = false;

	_predicted = true;
}

bool c_nade_prediction::detonated( weapon_t* weapon, float time, trace_t& trace ) {
	if ( !weapon )
		return true;

	const auto index = weapon->item_definition_index( );

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

void c_nade_prediction::trace( c_usercmd* user_cmd ) {
	if ( !interfaces::engine->is_connected( ) && !interfaces::engine->is_in_game( ) )
		return;

	if ( !( user_cmd->buttons & in_attack ) && !( user_cmd->buttons & in_attack2 ) ) {
		_predicted = false;
		return;
	}

	const static std::vector< int > nades{
		weapon_flashbang,
		weapon_smokegrenade,
		weapon_hegrenade,
		weapon_molotov,
		weapon_decoy,
		weapon_incgrenade
	};

	auto weapon = csgo::local_player->active_weapon( );
	if ( !weapon )
		return;

	if ( std::find( nades.begin( ), nades.end( ), weapon->item_definition_index( ) ) != nades.end( ) )
		return predict( user_cmd );

	_predicted = false;
}

void c_nade_prediction::draw( ) {
	if ( !interfaces::engine->is_connected( ) && !interfaces::engine->is_in_game( ) )
		return;

	if ( !csgo::local_player || !csgo::local_player->is_alive( ) )
		return;
	auto draw_3d_circle = [ ] ( vec3_t position, const int points, float radius, color color ) {
		static int texture = interfaces::surface->create_new_texture_id( true );

		unsigned char color_buffer [ 4 ] = { 255, 0, 0, 255 };

		interfaces::surface->set_texture_rgba( texture, color_buffer, 1, 1 );
		interfaces::surface->set_texture( texture );

		float step = 3.141592654f * 2.0f / points;
		vec3_t prev_scr_pos = vec3_t( );
		int step_count = 0;
		for ( float a = 0; a < 3.141592654f * 2.0f; a += step ) {
			vec3_t start( radius * cosf( a ) + position.x, radius * sinf( a ) + position.y, position.z );
			vec3_t start2d = vec3_t( );
			vec3_t center_2d = vec3_t( );
			if ( interfaces::debug_overlay->world_to_screen( position, center_2d ) && interfaces::debug_overlay->world_to_screen( start, start2d ) ) {
				if ( !prev_scr_pos.is_zero( ) ) {
					render::draw_line( prev_scr_pos.x, prev_scr_pos.y, start2d.x, start2d.y, color );

				}
				prev_scr_pos = start2d;

			}
		}
	};
	
	vec3_t start, end;
	if ( _predicted ) {
		for ( auto& p : _points ) {
			if ( !p.m_valid )
				break;

			auto nade_prediction_color = color::green( );
			if ( math::world_to_screen( p.m_start, start ) && math::world_to_screen( p.m_end, end ) ) {
				render::draw_line( start.x, start.y, end.x, end.y, nade_prediction_color );

				if ( p.m_detonate || p.m_plane )
					render::draw_rect( start.x - 2, start.y - 2, 5, 5, p.m_detonate ? color( 255, 0, 0 ) :  color( 255, 255, 255 ) );

				if ( p.m_detonate )
					draw_3d_circle( p.m_end, 30, 15, color::red( ) );
			
			}
		}
	}
}