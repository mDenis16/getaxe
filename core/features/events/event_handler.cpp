#include "../features.hpp"

#pragma comment( lib, "Winmm.lib" )

c_event_listener* event_handler = new c_event_listener();

c_event_listener::c_event_listener ( ) { }

c_event_listener::~c_event_listener ( ) {
	if ( !m_eventlistener )
		return;

	//g_csgo.m_game_event->RemoveListener( this );
}

bool c_event_listener::setup ( ) {
	m_eventlistener = new c_event_listener ( );
	if ( !m_eventlistener )
		return false;

	interfaces::event_manager->add_listener ( this, "player_hurt", false );
	interfaces::event_manager->add_listener ( this, "item_purchase", false );
	interfaces::event_manager->add_listener ( this, "bullet_impact", false );
	interfaces::event_manager->add_listener ( this, "weapon_fire", false );
	interfaces::event_manager->add_listener ( this, "player_footstep", false );

	if ( !interfaces::event_manager->find_listener ( this, "player_hurt" ) || !interfaces::event_manager->find_listener ( this, "item_purchase" )
		|| !interfaces::event_manager->find_listener ( this, "bullet_impact" ) || !interfaces::event_manager->find_listener ( this, "weapon_fire" ) ) {
		_RPT0 ( _CRT_ERROR, "Failed to setup event listener(s). Ignoring this message will result in instability or features not working." );
		return false;
	}

	return true;
}

bool c_event_listener::remove ( ) {
	interfaces::event_manager->remove_listener ( this );

	return true;
}


void c_event_listener::fire_game_event ( i_game_event * m_event ) {
	if ( !m_event )
		return;

	if ( !strcmp ( m_event->get_name ( ), "bullet_impact" ) ) {
		int user_id = m_event->get_int ( "userid" );

		auto ent = interfaces::entity_list->get< player_t > ( interfaces::engine->get_player_for_user_id ( user_id ) );
		if ( !ent )
			return;

		player_visual_base * cfg = nullptr;

		int type = ent->is_teammate ( ) ? 0 : 1;

		if ( ent == local_pointer )
			type = 2;

		if ( type > 1 )
			cfg = &config.local_visual;
		else
			cfg = &config.player_visual [ type ];

		vec3_t position { m_event->get_float ( "x" ), m_event->get_float ( "y" ), m_event->get_float ( "z" ) };

		if ( cfg->bullet_impacts )
			interfaces::debug_overlay->add_box_overlay ( position, vec3_t ( -2, -2, -2 ), vec3_t ( 2, 2, 2 ), vec3_t ( 0, 0, 0 ), 0, 0, 255, 127, 1.f );

		if (cfg->bullet_tracers ) {
			cfg->bullet_tracers_color.get_rgb ( );

			BeamInfo_t beam_info;
			beam_info.m_nType = TE_BEAMPOINTS;
			beam_info.m_pszModelName = "sprites/white.vmt";
			beam_info.m_nHaloIndex = -1;
			beam_info.m_flHaloScale = 0.f;
			beam_info.m_flLife = 4.f;
			beam_info.m_flWidth = 0.6f;
			beam_info.m_flEndWidth = 0.6f;
			beam_info.m_flFadeLength = 1.f;
			beam_info.m_flAmplitude = 2.f;
			beam_info.m_flBrightness = cfg->bullet_tracers_color.rgb.w;
			beam_info.m_flSpeed = 0.2f;
			beam_info.m_nStartFrame = 0;
			beam_info.m_flFrameRate = 0.f;
			beam_info.m_flRed = cfg->bullet_tracers_color.rgb.x;
			beam_info.m_flGreen = cfg->bullet_tracers_color.rgb.y;
			beam_info.m_flBlue = cfg->bullet_tracers_color.rgb.z;
			beam_info.m_nSegments = 2;
			beam_info.m_bRenderable = true;
			beam_info.m_nFlags = FBEAM_ONLYNOISEONCE | FBEAM_NOTILE | FBEAM_HALOBEAM;

			// create beam backwards because it looks nicer.
			beam_info.m_vecStart = position;
			beam_info.m_vecEnd = ent->eye_pos ( );


			Beam_t * beam = interfaces::render_beams->create_beam_point ( beam_info );
			if ( beam )
			   interfaces::render_beams->draw_beam ( beam );
		}
	}


	/*if ( !strcmp ( m_event->get_name ( ), "player_hurt" ) ) {
		int attacker = g_csgo.m_engine->get_player_for_user_id ( m_event->get_int ( "attacker" ) );
		int target = g_csgo.m_engine->get_player_for_user_id ( m_event->get_int ( "userid" ) );
		if ( attacker == g_csgo.m_engine->get_local_player ( ) && target != g_csgo.m_engine->get_local_player ( ) ) {
			auto * ent = g_csgo.m_entity_list->get< c_csplayer > ( target );

			player_info_t info;
			if ( g_csgo.m_engine->get_player_info ( target, &info ) && ent ) {
				if ( g_vars.misc.hitmarker_sound ) {
					std::string sound_path = "hitsounds\\" + g_menu.m_selected_sound_text;

					// you can do this better but im lazy.
					PlaySoundA ( sound_path.c_str ( ), nullptr, SND_ASYNC );
				}

				std::string s = info.m_player_name;
				std::transform ( s.begin ( ), s.end ( ), s.begin ( ), tolower );

				if ( g_vars.visuals.hitmarker )
					g_cl.m_hitmarker_alpha = 1.f;

				if ( g_vars.misc.log_damage )
					g_notify.add ( true, OSHGui::Drawing::Color::FromARGB ( 220, 249, 44, 69 ), "hit %s in the %s for %i damage.", s.c_str ( ),
						hitgroup_to_name ( m_event->get_int ( "hitgroup" ) ), m_event->get_int ( "dmg_health" ) );
			}
		}
	}
	else if ( !strcmp ( m_event->get_name ( ), "item_purchase" ) ) {
		int user = g_csgo.m_engine->get_player_for_user_id ( m_event->get_int ( "userid" ) );

		auto local = c_csplayer::get_local ( );
		if ( !local || local->team ( ) == m_event->get_int ( "team" ) )
			return;

		player_info_t info;
		if ( g_csgo.m_engine->get_player_info ( user, &info ) ) {
			if ( g_vars.misc.log_purchases )
				g_notify.add ( true, OSHGui::Drawing::Color::FromARGB ( 220, 249, 44, 69 ), "%s bought %s", info.m_player_name, m_event->get_string ( "weapon" ) );
		}
	}*/

	if ( !strcmp ( m_event->get_name ( ), "player_footstep" ) ) {
		int user_id = m_event->get_int( "userid" );
		auto ent = interfaces::entity_list->get< player_t >( interfaces::engine->get_player_for_user_id( user_id ) );
		if( !ent )
			return;

		auto local = local_pointer;

		if( !local || ent == local )
			return;

		player_visual * cfg = nullptr;

		int type = ent->is_teammate ( ) ? 0 : 1;

		if ( ent == local_pointer )
			return;


		cfg = &config.player_visual [ type ];
		if ( cfg->foot_steps ) {
			cfg->foot_steps_color.get_rgb ( );

			/*BeamInfo_t beam_info;

			beam_info.m_nType = TE_BEAMRINGPOINT;
			beam_info.m_pszModelName = "sprites/purplelaser1.vmt";
			beam_info.m_nModelIndex = interfaces::model_info->get_model_index ( "sprites/purplelaser1.vmt" );
			beam_info.m_nHaloIndex = -1;
			beam_info.m_flHaloScale = 5;
			beam_info.m_flLife = 5.f;
			beam_info.m_flWidth = 5.f;
			beam_info.m_flFadeLength = 1.f;
			beam_info.m_flAmplitude = 0.f;
			beam_info.m_flRed = cfg->foot_steps_color.rgb.x;
			beam_info.m_flGreen = cfg->foot_steps_color.rgb.y;
			beam_info.m_flBlue = cfg->foot_steps_color.rgb.z;
			beam_info.m_flBrightness = cfg->foot_steps_color.rgb.w;
			beam_info.m_flSpeed = 0.f;
			beam_info.m_nStartFrame = 0;
			beam_info.m_flFrameRate = 1.f;
			beam_info.m_nSegments = -1;
			beam_info.m_nFlags = FBEAM_FADEOUT;
			beam_info.m_vecCenter = ent->abs_origin ( ) + vec3_t ( 0, 0, 5 );
			beam_info.m_flStartRadius = 0.f;
			beam_info.m_flEndRadius = 75.f;

			const auto beam = interfaces::render_beams->create_beam_ring_point ( beam_info );

			if ( beam )
				interfaces::render_beams->draw_beam ( beam );*/

			BeamInfo_t beam_info;
			beam_info.m_nType = TE_BEAMRINGPOINT;
			beam_info.m_pszModelName = "sprites/purplelaser1.vmt";
			beam_info.m_nModelIndex = interfaces::model_info->get_model_index ( "sprites/purplelaser1.vmt" );
			beam_info.m_nHaloIndex = -1;
			beam_info.m_flHaloScale = 5;
			beam_info.m_flLife = .50f;
			beam_info.m_flWidth = 10.f;
			beam_info.m_flFadeLength = 1.0f;
			beam_info.m_flAmplitude = 0.f;
			beam_info.m_flRed = cfg->foot_steps_color.rgb.x;
			beam_info.m_flGreen = cfg->foot_steps_color.rgb.y;
			beam_info.m_flBlue = cfg->foot_steps_color.rgb.z;
			beam_info.m_flBrightness = cfg->foot_steps_color.rgb.w;
			beam_info.m_flSpeed = 0.f;
			beam_info.m_nStartFrame = 0.f;
			beam_info.m_flFrameRate = 60.f;
			beam_info.m_nSegments = -1;
			beam_info.m_nFlags = FBEAM_FADEOUT;
			beam_info.m_vecCenter = ent->abs_origin ( ) + vec3_t ( 0, 0, 5 );
			beam_info.m_flStartRadius = 20.f;
			beam_info.m_flEndRadius = 640.f;

			auto beam = interfaces::render_beams->create_beam_ring_point ( beam_info );
			if ( beam )
				interfaces::render_beams->draw_beam ( beam );
		}
	}
}