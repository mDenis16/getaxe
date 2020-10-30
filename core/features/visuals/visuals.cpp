#include "../features.hpp"

std::array< float, 64 > alpha;
std::deque<visuals::loginfo>visuals::event_;

void visuals::think( ) {
	if ( !csgo::local_player || !interfaces::engine->is_connected( ) || !interfaces::engine->is_in_game( ) )
		return;
	static int height = 0; static int width = 0;
	if ( height == 0 || width == 0 )
		interfaces::engine->get_screen_size( width, height );
	for ( int i = 1; i <= interfaces::globals->max_clients; i++ ) {
		auto entity = reinterpret_cast< player_t* >( interfaces::entity_list->get_client_entity( i ) );

		if ( !entity || entity == csgo::local_player || entity->health( ) <= 0 )
			continue;

		if ( entity->team( ) == csgo::local_player->team( ) && !variables::visuals::teammates )
			continue;

		if ( variables::visuals::activation_type == 1 && !csgo::local_player->can_see_player_pos( entity, entity->get_eye_pos( ) ) && csgo::local_player->is_alive( ) )
			continue;



		const auto index = entity->index( );
		const float step = 255.f * ( 20.f / 15.f ) * interfaces::globals->frame_time;

		variables::visuals::dormant?entity->dormant( )
			?alpha.at( index ) -= step:alpha.at( index ) += step * 5.f:entity->dormant( )
			?alpha.at( index ) = 0.f:alpha.at( index ) = 220.f;

		if ( alpha.at( index ) > 220.f )
			alpha.at( index ) = 220.f;

		if ( alpha.at( index ) < 0.f )
			alpha.at( index ) = 0.f;

		player( entity );

	}
	vec3_t w2s_peek = vec3_t( ); vec3_t w2s_frompeek = vec3_t( );
	if ( autostop::m_autostop_data.state > 0 && math::world_to_screen( autostop::m_autostop_data.to, w2s_peek ) && math::world_to_screen(autostop::m_autostop_data.from, w2s_frompeek )) {


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
		draw_3d_circle( autostop::m_autostop_data.to, 30, 5, color::red() );
		draw_3d_circle( autostop::m_autostop_data.from, 30, 5, color::blue() );
	}
	for ( size_t i = 0; i < shot_processor::shots.size( ); i++ )
	{
		auto shot = shot_processor::shots.at( i );
		if ( shot.approved ) {
			vec3_t w2s_from = vec3_t( ); vec3_t w2s_to = vec3_t( ); vec3_t w2s_impact = vec3_t( );
			if ( math::world_to_screen( shot.shotpos, w2s_from ) && math::world_to_screen( shot.hitpos, w2s_to ) )
			{
				render::draw_line( w2s_from.x, w2s_from.y, w2s_to.x, w2s_to.y, color::red( ) );
				render::draw_circle( w2s_to.x, w2s_to.y, 3, 5, color::black( ) );
				render::draw_text_string( w2s_to.x + 3, w2s_to.y, render::fonts::menu_font, std::string( "dmg: " ) + std::to_string( shot.hit_info.damage ), true, color::black( ) );

			}
		}
	}

	vec3_t weapon_shoot_pos_w2s = vec3_t( );
	if ( math::world_to_screen( ragebot::weapon_shoot_pos, weapon_shoot_pos_w2s ) ) {
		render::draw_circle( weapon_shoot_pos_w2s.x, weapon_shoot_pos_w2s.y, 5, 5, color::green( ) );
	}
	for ( size_t i = 0; i < anti_aim::points.size( ); i++ ) {

		auto point = anti_aim::points.at( i );
		vec3_t end_w2s = vec3_t( );
		vec3_t start_w2s = vec3_t( );
		if ( math::world_to_screen( point.start, start_w2s ) && math::world_to_screen( point.end, end_w2s ) )
		{


			render::draw_text_string( end_w2s.x, end_w2s.y + 20.f, render::fonts::watermark_font, std::to_string( i ), false, color( 153, 102, 204 ) );
			if ( point.selected )
			{
				render::draw_text_string( end_w2s.x, end_w2s.y, render::fonts::watermark_font, std::to_string( point.tickness ), false, color( 255, 255, 255 ) );
				render::draw_text_string( end_w2s.x, end_w2s.y + 10.f, render::fonts::watermark_font, std::string( "angle " ) + std::to_string( point.angle ), false, color( 255, 255, 255 ) );
			}
			else if ( point.original )
			{
				render::draw_text_string( end_w2s.x, end_w2s.y, render::fonts::watermark_font, std::to_string( point.tickness ), false, color( 0, 255, 0 ) );
				render::draw_text_string( end_w2s.x, end_w2s.y + 10.f, render::fonts::watermark_font, std::string( "angle " ) + std::to_string( point.angle ), false, color( 0, 255, 0 ) );

			}
			else
			{
				render::draw_text_string( end_w2s.x, end_w2s.y, render::fonts::watermark_font, std::to_string( point.tickness ), false, color( 153, 102, 204 ) );
				render::draw_text_string( end_w2s.x, end_w2s.y + 10.f, render::fonts::watermark_font, std::string( "angle " ) + std::to_string( point.angle ), false, color( 153, 102, 204 ) );
			}

		}
	}
	for ( size_t i = 0; i < event_.size( ); i++ ) {
		float time = interfaces::globals->cur_time - event_ [ i ].time;
		if ( time < 6.f ) {
			render::draw_text_string( 5, 5 + i * 15, render::fonts::watermark_font, "[amtlib]", false, color( 153, 102, 204 ) );
			render::draw_text_string( 74, 5 + i * 15, render::fonts::watermark_font, event_ [ i ].text.c_str( ), false, event_ [ i ].color );
		}
		else {
			event_.erase( event_.begin( ) + i );
		}
	}
	for ( size_t i = 0; i < autostop::points_to_draw.size( ); i++ )
	{
		auto point = autostop::points_to_draw.at( i );
		vec3_t w2s = vec3_t( );
		if ( math::world_to_screen( point.pos, w2s ) ) {
			render::draw_filled_rect( w2s.x, w2s.y, 2, 2, color::green( ) );
			render::draw_text_string( w2s.x, w2s.y, render::fonts::watermark_font, point.name, true, color::red( ) );

		}
	}
	for ( size_t i = 0; i < ragebot::hitscan_points.size( ); i++ )
	{
		auto point = ragebot::hitscan_points.at( i );
		vec3_t w2s = vec3_t( );
		if ( math::world_to_screen( point.point, w2s ) ) {
			if ( point.is_safe )
				render::draw_filled_rect( w2s.x, w2s.y, 4, 4, color::green( ) );
			else
				render::draw_filled_rect( w2s.x, w2s.y, 4, 4, color::red( ) );
		}
	}
	if ( csgo::local_player && csgo::local_player->health() > 0 && csgo::local_player->is_scoped( ) && csgo::local_player->active_weapon() ) {


		vec3_t punchAngle = csgo::local_player->aim_punch_angle( );

		float center_x = width / 2;
		float center_y = height / 2;
		auto weapon = csgo::local_player->active_weapon( );
		if ( weapon ) {
			int size = weapon->inaccuracy( ) * 100.0f;


			if ( size < 5 )
			{
				render::draw_line( 0, center_y, width, center_y, color( 0, 0, 0, 220 ) );
				render::draw_line( center_x, 0, center_x, height, color( 0, 0, 0, 220 ) );
			}

			render::draw_filled_rect( center_x - 10000, center_y - size / 2, 10000, size, color( 0, 0, 0, 190 ) );
			render::draw_filled_rect( center_x, center_y - size / 2, 10000, size, color( 0, 0, 0, 190 ) );

			render::draw_filled_rect( center_x - size / 2, center_y - 10000, size, 10000, color( 0, 0, 0, 190 ) );
			render::draw_filled_rect( center_x - size / 2, center_y, size, 10000, color( 0, 0, 0, 190 ) );
		}
	}
	
	render::draw_filled_rect( height - 40, width - 10, ( ( 40 / csgo::m_goal_shift ) * tickbase_system::m_shift_data.m_needs_recharge ), 20, color::green( ) );
}


void visuals::player( player_t* entity ) {
	if ( ( entity->dormant( ) && alpha [ entity->index( ) ] == 0 ) && !variables::visuals::dormant )
		return;

	auto weapon = entity->active_weapon( );
	if ( !weapon )
		return;

	bbox_t box;
	if ( !calculate_box( entity, box ) )
		return;

	if ( variables::visuals::box ) {
		render::draw_outline( box.x - 1, box.y - 1, box.w + 2, box.h + 2, color( 0, 0, 0, alpha [ entity->index( ) ] ) );
		render::draw_rect( box.x, box.y, box.w, box.h, color( 255, 255, 255, alpha [ entity->index( ) ] ) );
		render::draw_outline( box.x + 1, box.y + 1, box.w - 2, box.h - 2, color( 0, 0, 0, alpha [ entity->index( ) ] ) );

	}

	if ( variables::visuals::health ) {
		int hp = std::clamp( entity->health( ), 0, 100 );
		auto health_color = color( 255 - hp * 2.55, hp * 2.55, 0, alpha [ entity->index( ) ] );

		bbox_t box( box.x - 5, box.y, 1, box.h );

		render::draw_filled_rect( box.x - 1, box.y - 1, box.w + 2, box.h + 2, color( 0, 0, 0, alpha [ entity->index( ) ] ) );
		render::draw_filled_rect( box.x, box.y, box.w, box.h, color( health_color ) );
	}

	if ( variables::visuals::name ) {
		std::string name = entity->get_info( ).name;

		if ( name.length( ) > 17 )
			name = name.substr( 0, 17 ) + "...";

		std::transform( name.begin( ), name.end( ), name.begin( ), tolower );
		render::draw_text_string( box.x + box.w * 0.5, box.y - 13, render::fonts::menu_font, entity->get_info( ).fakeplayer?std::string( "(bot) " ).append( name ):name, true, color( 255, 255, 255, alpha [ entity->index( ) ] ) );

		int c = 40;

		int b = 16;
		auto anim_overlay = entity->get_anim_overlays( ) [ 3 ];
		/*render::draw_text_string( box.x + 35, box.y - b, render::fonts::menu_font, std::string( "sequence act " ) + std::string( " is " ) + std::to_string( entity->get_sequence_act( anim_overlay.sequence ) ), true, color( 255, 255, 255, alpha [ entity->index( ) ] ) ); b += 16;

		render::draw_text_string( box.x - c, box.y - b, render::fonts::menu_font, std::string( "order is " ) + std::to_string( anim_overlay.order ), true, color( 255, 255, 255, alpha [ entity->index( ) ] ) ); b += 16;
		render::draw_text_string( box.x - c, box.y - b, render::fonts::menu_font, std::string( "sequence is " ) + std::to_string( anim_overlay.sequence ), true, color( 255, 255, 255, alpha [ entity->index( ) ] ) ); b += 16;
		render::draw_text_string( box.x - c, box.y - b, render::fonts::menu_font, std::string( "previous_cycle is " ) + std::to_string( anim_overlay.previous_cycle ), true, color( 255, 255, 255, alpha [ entity->index( ) ] ) ); b += 16;
		render::draw_text_string( box.x - c, box.y - b, render::fonts::menu_font, std::string( "weight is " ) + std::to_string( anim_overlay.weight ), true, color( 255, 255, 255, alpha [ entity->index( ) ] ) ); b += 16;
		render::draw_text_string( box.x - c, box.y - b, render::fonts::menu_font, std::string( "weight_delta_rate is " ) + std::to_string( anim_overlay.weight_delta_rate ), true, color( 255, 255, 255, alpha [ entity->index( ) ] ) ); b += 16;
		render::draw_text_string( box.x - c, box.y - b, render::fonts::menu_font, std::string( "playback_rate is " ) + std::to_string( anim_overlay.playback_rate ), true, color( 255, 255, 255, alpha [ entity->index( ) ] ) ); b += 16;
		render::draw_text_string( box.x - c, box.y - b, render::fonts::menu_font, std::string( "cycle is " ) + std::to_string( anim_overlay.cycle ), true, color( 255, 255, 255, alpha [ entity->index( ) ] ) ); b += 16;
		if (entity->get_anim_state() )
		render::draw_text_string( box.x + 35, box.y - b, render::fonts::menu_font, std::string( "feet yaw" ) + std::string( " is " ) + std::to_string( entity->get_anim_state( )->m_flGoalFeetYaw), true, color( 255, 255, 255, alpha [ entity->index( ) ] ) ); b += 16;*/
		if ( resolver::resolver_data [ entity->index( ) ].resolved ) {
			render::draw_text_string( box.x, box.y + 6, render::fonts::menu_font, resolver::resolver_data [ entity->index( ) ].resolve_type, true, color( 255, 255, 255, alpha [ entity->index( ) ] ) );
		}else if ( resolver::resolver_data [ entity->index( ) ].is_using_desync && !resolver::resolver_data [ entity->index( ) ].resolved )
			render::draw_text_string( box.x, box.y + 6, render::fonts::menu_font, std::string( resolver::resolver_data [ entity->index( ) ].extended_desync ? "extended" : "partial" ), true, color( 255, 255, 255, alpha [ entity->index( ) ] ) );

		if ( resolver::resolver_data [ entity->index( ) ].will_peek )
			render::draw_text_string( box.x, box.y + 12, render::fonts::menu_font, std::string( "peeking" ), true, color( 255, 255, 255, alpha [ entity->index( ) ] ) );
		//	render::draw_text_string( box.x - 6, box.y + 6, render::fonts::menu_font, std::string( "missed shots " )  + std::to_string( resolver::resolver_data [ entity->index() ].missed_shots), true, color( 255, 255, 255, alpha [ entity->index( ) ] ) );

	}

	if ( variables::visuals::name && player_manager::extrapolate::extrapolate_records [ entity->index( ) ].bone ) {
		auto p_studio_hdr = interfaces::model_info->get_studio_model( entity->model( ) );
		if ( !p_studio_hdr )
			return;

		vec3_t v_parent, v_child, s_parent, s_child;

		for ( int i = 0; i < p_studio_hdr->bones_count; i++ ) {
			studio_bone_t* bone = p_studio_hdr->bone( i );
			if ( !bone )
				return;

			if ( bone && ( bone->flags & BONE_USED_BY_HITBOX ) && ( bone->parent != -1 ) ) {
				v_child = entity->get_bone_position( i, player_manager::extrapolate::extrapolate_records [ entity->index( ) ].bone );
				v_parent = entity->get_bone_position( bone->parent, player_manager::extrapolate::extrapolate_records[ entity->index()].bone );

				if ( math::world_to_screen( v_parent, s_parent ) && math::world_to_screen( v_child, s_child ) )
					render::draw_line( s_parent [ 0 ], s_parent [ 1 ], s_child [ 0 ], s_child [ 1 ], color( 255, 255, 255, alpha [ entity->index( ) ] ) );
			}
		}
	}


	if ( variables::visuals::weapon ) {
		auto info = weapon->get_weapon_data( );
		if ( !info )
			return;

		const char* name = info->szHudName;
		std::wstring localised_name = interfaces::localize->find( name );

		std::string print = utilities::unicode_to_ascii( localised_name );
		std::transform( print.begin( ), print.end( ), print.begin( ), tolower );

		render::draw_text_string( box.x + box.w * 0.5f, box.y + box.h - 1, render::fonts::menu_font, print, true, color( 255, 255, 255, alpha [ entity->index( ) ] ) );
	}

	if ( variables::visuals::flags ) {
		std::vector<std::pair<std::string, color>> flags;
		std::string armor_flag;

		vec3_t enemy_vec_origin = entity->origin( );
		vec3_t local_vec_origin = csgo::local_player->origin( );

		char distance_to [ 32 ];
		sprintf_s( distance_to, "%.0f ft", utilities::get_distance( local_vec_origin, enemy_vec_origin ) );

		if ( variables::visuals::money )
			flags.push_back( std::pair<std::string, color>( "$" + std::to_string( entity->money( ) ), color( 130, 160, 7, alpha [ entity->index( ) ] ) ) );

		if ( entity->has_helmet( ) )
			armor_flag += "h";

		if ( entity->armor( ) > 0 )
			armor_flag += "k";

		if ( !armor_flag.empty( ) )
			flags.push_back( std::pair<std::string, color>( armor_flag, color( 255, 255, 255, alpha [ entity->index( ) ] ) ) );

		if ( entity->is_flashed( ) )
			flags.push_back( std::pair<std::string, color>( std::string( "flash" ), color( 255, 255, 255, alpha [ entity->index( ) ] ) ) );

		if ( entity->is_scoped( ) )
			flags.push_back( std::pair<std::string, color>( std::string( "scope" ), color( 255, 255, 255, alpha [ entity->index( ) ] ) ) );

		if ( variables::visuals::distance )
			flags.push_back( std::pair<std::string, color>( std::string( distance_to ), color( 255, 255, 255, alpha [ entity->index( ) ] ) ) );

		auto position = 0;
		for ( auto text : flags ) {
			render::draw_text_string( box.x + box.w + 3, box.y + position - 2, render::fonts::menu_font, text.first, false, text.second );
			position += 10;
		}
	}
}

void visuals::glow::think( ) {
	if ( !variables::visuals::glow )
		return;

	for ( int i = 0; i < interfaces::glow_manager->size; i++ ) {
		if ( interfaces::glow_manager->objects [ i ].unused( ) || !interfaces::glow_manager->objects [ i ].entity )
			continue;

		auto& glow_object = interfaces::glow_manager->objects [ i ];

		auto entity = reinterpret_cast< player_t* >( glow_object.entity );
		if ( !entity )
			continue;

		const auto client_class = entity->client_class( );
		if ( !client_class )
			continue;

		switch ( client_class->class_id ) {
		case ccsplayer:
		{
			if ( entity->team( ) == csgo::local_player->team( ) && !variables::visuals::teammates )
				continue;

			if ( variables::visuals::activation_type == 2 )
				continue;

			glow_object.set( 0.804f, 0.22f, 0.427f, 1.00f );
			break;
		}
		default:
			break;
		}
	}
}


float dot_product( const vec3_t& a, const vec3_t& b ) {
	return ( a.x * b.x
		+ a.y * b.y
		+ a.z * b.z );
}
bool visuals::calculate_box( entity_t* ent, bbox_t& box ) {
	vec3_t origin, min, max, flb, brt, blb, frt, frb, brb, blt, flt;
	float left, top, right, bottom;

	origin = ent->origin( );
	min = ent->collideable( )->mins( ) + origin;
	max = ent->collideable( )->maxs( ) + origin;

	vec3_t points [ ] = {
		vec3_t( min.x, min.y, min.z ),
		vec3_t( min.x, max.y, min.z ),
		vec3_t( max.x, max.y, min.z ),
		vec3_t( max.x, min.y, min.z ),
		vec3_t( max.x, max.y, max.z ),
		vec3_t( min.x, max.y, max.z ),
		vec3_t( min.x, min.y, max.z ),
		vec3_t( max.x, min.y, max.z )
	};

	if ( !interfaces::debug_overlay->world_to_screen( points [ 3 ], flb ) || !interfaces::debug_overlay->world_to_screen( points [ 5 ], brt )
		|| !interfaces::debug_overlay->world_to_screen( points [ 0 ], blb ) || !interfaces::debug_overlay->world_to_screen( points [ 4 ], frt )
		|| !interfaces::debug_overlay->world_to_screen( points [ 2 ], frb ) || !interfaces::debug_overlay->world_to_screen( points [ 1 ], brb )
		|| !interfaces::debug_overlay->world_to_screen( points [ 6 ], blt ) || !interfaces::debug_overlay->world_to_screen( points [ 7 ], flt ) )
		return false;

	vec3_t arr [ ] = { flb, brt, blb, frt, frb, brb, blt, flt };

	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	for ( int i = 1; i < 8; i++ ) {
		if ( left > arr [ i ].x )
			left = arr [ i ].x;
		if ( bottom < arr [ i ].y )
			bottom = arr [ i ].y;
		if ( right < arr [ i ].x )
			right = arr [ i ].x;
		if ( top > arr [ i ].y )
			top = arr [ i ].y;
	}

	box.x = ( int ) left;
	box.y = ( int ) top;
	box.w = int( right - left );
	box.h = int( bottom - top );

	return true;
}
void visuals::capsule_overlay( player_t* entity, float duration, matrix_t pBoneToWorldOut [ 128 ] )
{
	if ( !entity )
		return;

	studio_hdr_t* pStudioModel = interfaces::model_info->get_studio_model( ( model_t* ) entity->model( ) );
	if ( !pStudioModel )
		return;



	auto pHitboxSet = pStudioModel->hitbox_set( 0 );
	if ( !pHitboxSet )
		return;

	auto VectorTransform2 = [ ] ( const vec3_t in1, matrix_t in2, vec3_t& out )
	{

		out [ 0 ] = dot_product( in1, vec3_t( in2 [ 0 ][ 0 ], in2 [ 0 ][ 1 ], in2 [ 0 ][ 2 ] ) ) + in2 [ 0 ][ 3 ];
		out [ 1 ] = dot_product( in1, vec3_t( in2 [ 1 ][ 0 ], in2 [ 1 ][ 1 ], in2 [ 1 ][ 2 ] ) ) + in2 [ 1 ][ 3 ];
		out [ 2 ] = dot_product( in1, vec3_t( in2 [ 2 ][ 0 ], in2 [ 2 ][ 1 ], in2 [ 2 ][ 2 ] ) ) + in2 [ 2 ][ 3 ];
	};

	for ( int i = 0; i < pHitboxSet->hitbox_count; i++ )
	{
		auto pHitbox = pHitboxSet->hitbox( i );
		if ( !pHitbox )
			continue;

		vec3_t vMin, vMax;
		VectorTransform2( pHitbox->mins, pBoneToWorldOut [ pHitbox->bone ], vMin ); //nullptr???
		VectorTransform2( pHitbox->maxs, pBoneToWorldOut [ pHitbox->bone ], vMax );

		if ( pHitbox->radius > -1 )
		{
			interfaces::debug_overlay->capsule_overlay( vMin, vMax, pHitbox->radius, 255, 0, 0, 255, duration );
		}
	}
}
