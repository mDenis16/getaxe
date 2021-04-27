#include "../features.hpp"
#include "../../../dependencies/imgui/imgui.h"

#include "../../../dependencies/imgui/imgui_internal.h"
#include "../../render/gui/gui.h"


namespace visuals {

	void visual_player::render_name ( ) {

		std::string print ( player_info.fakeplayer ? std::string ( "bot " ).append ( player_info.name ).c_str ( ) : player_info.name );

		std::transform ( print.begin ( ), print.end ( ), print.begin ( ), ::tolower );

		//primitive_string ( true, fonts [ ESP ], box_data.top_center.x, box_data.top_center.y - 15.f, config.player_visual [ type ].name_color, font_center, print.data ( ) );

		auto text_size = ImGui::CalcTextSize ( print.c_str ( ), 13.f, visuals::esp_font );
		auto center_x = box_data.top_center.x - text_size.x / 2.f;
		auto center_y = box_data.top_center.y - 15.5f;

		float offset_x = 1.1f;
		float offset_y = 1.1f;



		render->AddText ( visuals::esp_font, 13.f, ImVec2 ( center_x + offset_x, center_y + offset_y ), ImColor(0,0, 0, 255), print.c_str ( ) );
		render->AddText ( visuals::esp_font, 13.f, ImVec2( center_x, center_y ), config.player_visual [ type ].name_color, print.c_str ( ) );
	}
	void visual_player::render_box ( ) {

		static float size = 0.1f;
		int old_flags = render->Flags;
		render->Flags = ImDrawListFlags_AntiAliasedLines | ImDrawListFlags_AntiAliasedLines;

		render->AddRect ( this->box_data.mins, this->box_data.maxs, ImColor ( 0, 0, 0, 255 ), 0.f, 15, 3.f );

		render->AddRect ( this->box_data.mins, this->box_data.maxs, config.player_visual [ type ].bound_box_color, 0.f, 15, 1.f);
		render->Flags = old_flags;




	}
	void visual_player::render_flags ( ) {

		auto _x = box_data.x + box_data.w + 4, _y = box_data.y + 3;

		 auto draw_flag = [ & ] ( const char * msg, ... ) -> void {
			 primitive_string ( true, fonts [ FLAGS ], _x, _y, config.player_visual [ type ].flags_color, ImColor ( 0, 0, 0, 255 ), visuals::font_left, msg );
			_y += 8;
		};

		auto & flags = config.player_visual [ type ].flags_input;


		if ( flags[FLAGS_MONEY] )
			draw_flag ( "$ %i", money );

		if ( flags [ FLAGS_ARMOR ] ) {

			std::string text;

			if ( helmet && kevlar )
				text = "HK";
			else if ( kevlar )
				text = "K";

			draw_flag ( text.c_str() );
		}

		if ( flags [ FLAGS_KIT ] && defuser ) {
			draw_flag ( "KIT" );
		}
		if ( flags [ FLAGS_SCOPED ] && scoped ) {
			draw_flag ( "SCOPED" );
		}
		if ( flags [ FLAGS_FAKEDUCKING ] && fake_duck ) {
			draw_flag ( "FD" );
		}
		if ( flags [ FLAGS_C4 ] && bomb_carrier ) {
			draw_flag ( "C4" );
		}

	}
	void visual_player::render_ammo ( ) {

	}
	void visual_player::render_health ( ) {
		static auto get_health_clr = [ & ] ( int health ) {
			return ImColor ( static_cast< int >( 255 - ( health * 2.55f ) ), static_cast< int >( health * 2.55f ), 0, 100 );
		};

		static auto get_health_clr_1 = [ ] ( int health ) {
			auto clr = get_health_clr ( health );
			clr.get_rgb ( );

			clr.rgb.x -= 75;
			clr.rgb.y -= 75;
			clr.rgb.z -= 75;

			if ( clr.rgb.x < 0 )
				clr.rgb.x = 0;

			if ( clr.rgb.y < 0 )
				clr.rgb.y = 0;

			if ( clr.rgb.z < 0 )
				clr.rgb.z = 0;

			if ( clr.rgb.w < 0 )
				clr.rgb.w = 0;

			return clr;
		};


		static auto filled_box_outlined = [ ] ( const float x, const float y, const float w, const float h, const ImColor color, const ImColor outline, const float thickness ) {
		    render->AddRectFilled ( ImVec2 ( x, y ), ImVec2 ( x + w, y + h ), color );
			render->AddRect ( ImVec2 ( x, y ), ImVec2 ( x + w, y + h ), outline, 0.f, 15, thickness );
		};
		static auto filled_box = [ ] ( const float x, const float y, const float w, const float h, const ImColor color ) {
			render->AddRectFilled ( ImVec2 ( x, y ), ImVec2 ( x + w, y + h ), color );
		};

		int elements = 0;

		health = std::clamp ( health, 0, 100 );

		auto multiplier = health / 100.f;

		multiplier = std::clamp ( multiplier, 0.f, 1.f );

		const auto height = ( box_data.h - 2 ) * multiplier;


		const int red = static_cast< int >( 255 - health * 2.55f );
		const int green = static_cast< int >( health * 2.55f );

	

		
		filled_box_outlined ( box_data.x - 6.7f, box_data.y - 1.f, 3, box_data.h + 1.5f, ImColor ( 0, 0, 0, static_cast< int >( 200 * 0.3f ) ), ImColor ( 0, 0, 0, static_cast< int >( 255 ) ), 1 );
		filled_box ( box_data.x - 5.7f, box_data.y + box_data.h - height - 1, 1.0f, height - 1.5f, ImColor ( red, green, 0, static_cast< int >( 255 ) ) );

		auto x_p = box_data.x - 6;
		auto w_p = 4;
		auto rend_pos = ImVec2 ( ( x_p + ( ( x_p + w_p ) - x_p ) / 2.0f ), ( box_data.y + box_data.h ) - height );

		//primitive_string ( true, fonts [ FLAGS ],  rend_pos.x, rend_pos.y, ImColor(0, 0, 0, 255), ImColor ( 255, 255, 255, 255 ), font_center, std::to_string ( health ).c_str ( ) );

	}
	void visual_player::render_weapon ( ) {
		//dsdsDrawText ( c_menu::get ( ).weapon_icons, _data.weapon_icon, ImVec2 ( ( _data.box_data.x + ( ( _data.box_data.x + _data.box_data.w ) - _data.box_data.x ) / 2.0f ), _data.box_data.y + _data.box_data.h + 5 ), 8.f, clr_h, 0.1f, true, true );
		 

		auto text_size = ImGui::CalcTextSize ( weapon_icon.c_str(), 11.f, weapon_font );
		

		render->AddText ( visuals::weapon_font, 11.f, ImVec2 ( box_data.bottom_center.x - text_size.x / 2.f, box_data.bottom_center.y + 5 ), config.player_visual [ type ].weapon_color, weapon_icon.c_str ( ) );

	}
	void visual_player::render_entity ( ) {
		
		auto & cfg = config.player_visual [ type ];

		if ( on_screen ) {
			if ( cfg.bounding_box )
				render_box ( );
			if ( cfg.name )
				render_name ( );
			if ( cfg.health )
				render_health ( );
			if ( cfg.ammo )
				render_ammo ( );
			if ( cfg.weapon )
				render_weapon ( );
			if ( cfg.flags )
			  render_flags ( );
		}
		 
			if ( cfg.out_of_pov )
				render_offscreen ( );
		
	}
	void visual_player::queue_entity ( void* entity ) {
		ent = reinterpret_cast <entity_t*>(entity);
		player = reinterpret_cast < player_t * >( entity );

		dormant = player->dormant ( );
		enemy = player->is_enemy ( );
		health = player->health ( );
		index = player->index ( );
		origin = player->abs_origin ( );
		interfaces::engine->get_player_info ( player->index ( ), &player_info );
		mins = player->mins ( );
		maxs = player->maxs ( );
		type = player->is_enemy ( ) ? 1 : 0;
		valid = true;
		on_screen = calculate_box ( );

		if ( player->active_weapon ( ) )
			weapon_icon = weapon_icons [ player->active_weapon ( )->item_definition_index ( ) ];


		handle_flags ( );
		
	}

	void visual_player::handle_flags ( ) {
		if ( !config.player_visual [ type ].flags )
			return;


		auto & flags = config.player_visual [ type ].flags_input;
		if ( flags.at(FLAGS_MONEY ) == 1)
		  money = player->money ( );
		if ( flags.at ( FLAGS_ARMOR ) == 1 ) {
			kevlar = player->armor ( ) > 0;
			helmet = player->has_helmet ( );
		}
		if ( flags.at ( FLAGS_C4 ) == 1 )
		defuser = player->has_defuser ( );
		if ( flags.at ( FLAGS_SCOPED ) == 1 )
		scoped = player->is_scoped ( );
		
		if ( flags.at ( FLAGS_FAKEDUCKING ) == 1 ) {
			auto animstate = player->get_anim_state ( );


			auto fakeducking = [ & ] ( ) -> bool {
				static auto stored_tick = 0;
				static int crouched_ticks [ 65 ];

				if ( animstate->m_duck_amount ) //-V550
				{
					if ( animstate->m_duck_amount < 0.9f && animstate->m_duck_amount > 0.5f ) //-V550
					{
						if ( stored_tick != interfaces::globals->tick_count ) {
							crouched_ticks [ player->index ( ) ]++;
							stored_tick = interfaces::globals->tick_count;
						}

						return crouched_ticks [ player->index ( ) ] > 16;
					}
					else
						crouched_ticks [ player->index ( ) ] = 0;
				}

				return false;
			};

			fake_duck = fakeducking ( ) && player->flags ( ) & fl_onground && !animstate->m_hit_ground;
		}

		

	}

	void rotate_circle ( std::vector<ImVec2> & points, ImVec2 center, float rotation ) {


		for ( auto & point : points ) {
			point.x -= center.x;	point.y -= center.y;

			const auto tempX = point.x;
			const auto tempY = point.y;

			const auto theta = DEG2RAD ( rotation );
			const auto c = cos ( theta );
			const auto s = sin ( theta );

			point.x = tempX * c - tempY * s;
			point.y = tempX * s + tempY * c;

			point.x += center.x; point.y += center.y;
		}
	}

	void visual_player::render_offscreen ( ) {
		auto isOnScreen = [ ] ( vec3_t origin, ImVec2 & screen ) -> bool {
			if ( !world_to_screen ( origin, screen ) )
				return false;

			static int iScreenWidth, iScreenHeight;
			interfaces::engine->get_screen_size ( iScreenWidth, iScreenHeight );

			auto xOk = iScreenWidth > screen.x;
			auto yOk = iScreenHeight > screen.y;

			return xOk && yOk;
		};

		ImVec2 screenPos;

		if ( isOnScreen ( origin, screenPos ) )
			return;

		vec3_t viewAngles;
		interfaces::engine->get_view_angles ( viewAngles );

		static int width, height;
		interfaces::engine->get_screen_size ( width, height );

		auto screenCenter = ImVec2 ( width * 0.5f, height * 0.5f );
		auto angleYawRad = DEG2RAD ( viewAngles.y - math::calc_angle ( localdata.eye_position, origin ).y - 90.0f );

		const auto radius = config.player_visual[type].out_of_pov_radius;
		auto size = config.player_visual [ type ].out_of_pov_base_size;
		
		ImVec2 base_pos = ImVec2();
		base_pos.x = screenCenter.x + ( ( ( ( width - ( size * 3 ) ) * 0.5f ) * ( radius / 100.0f ) ) * cos ( angleYawRad ) ) + ( int ) ( 6.0f * ( ( ( float ) size - 4.0f ) / 16.0f ) );
		base_pos.y = screenCenter.y + ( ( ( ( height - ( size * 3 ) ) * 0.5f ) * ( radius / 100.0f ) ) * sin ( angleYawRad ) );

		if ( config.player_visual[type].foot_steps )
			base_pos = ImVec2 ( width * 0.7f, height * 0.7f );
	   
		if ( base_pos.x < 40 )
			base_pos.x = 40;
		if ( base_pos.y < 40 )
			base_pos.y = 40;

		if ( base_pos.x > width - 40 )
			base_pos.x = width - 40;
		if ( base_pos.y > height - 40 )
			base_pos.y = height - 40;


		int rounding_error = 16;
		float step = ( M_PI / rounding_error );
		double a = - step * 3.0;

		std::vector<ImVec2> points;


		while ( a <= M_PI + ( step * 4.0 ) ) {
			points.push_back ( ImVec2 ( base_pos.x + size * cos ( a ), base_pos.y + size * sin ( a ) ) );
			a += step;
		}

		points.push_back ( ImVec2 ( base_pos.x, base_pos.y - size * 2.f ) );

		auto rot = viewAngles.y - math::calc_angle ( localdata.eye_position, origin ).y;// -90.0f;

		rot = math::normalize_yaw ( rot );
		

		while ( rot < 0.f )
			rot += 360.f;
		while ( rot > 360.f )
			rot -= 180.f;

		
		rotate_circle ( points, base_pos, rot );


		render->AddConvexPolyFilled ( points.data ( ), points.size ( ), config.player_visual[type].out_of_pov_color );
		render->AddPolyline ( points.data ( ), points.size ( ), ImColor ( 255, 255, 255, 125 ), ImDrawListFlags_AntiAliasedLines, 1.f );


		auto text_size = ImGui::CalcTextSize ( "e", size, ui::font_icons );

		ImVec2 original_pos = base_pos;
		original_pos.x -= text_size.x / 2.f;
		original_pos.y -= text_size.y / 2.f;


		render->AddText ( ui::font_icons, size, original_pos, ImColor ( 255, 255, 255, 255 ), "e" );

	}
}