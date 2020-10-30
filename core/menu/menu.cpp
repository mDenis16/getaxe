#include "menu.hpp"

void menu::render( ) {
	zgui::poll_input( "Counter-Strike: Global Offensive" );

	static int tab = 0;

	if ( zgui::begin_window( "amtlib for counter strike: global offensive", { 500, 350 }, render::fonts::menu_font, zgui::zgui_window_flags_none ) ) {
		zgui::push_cursor_pos( { 5, 24 } );
		zgui::begin_groupbox( "", { 490, 15 } );
		{
			zgui::push_cursor_pos( { 158, 24 } );

			if ( zgui::clickable_text( "rage" ) )
				tab = 0;

			zgui::same_line( );

			if ( zgui::clickable_text( "esp" ) )
				tab = 1;

			zgui::same_line( );

			if ( zgui::clickable_text( "misc" ) )
				tab = 2;

			zgui::same_line( );

			if ( zgui::clickable_text( "skins" ) )
				tab = 3;

			zgui::same_line( );

			if ( zgui::clickable_text( "config" ) )
				tab = 4;
		}
		zgui::end_groupbox( );

		switch ( tab ) {
		case 0:
			tabs::rage( );
			break;
		case 1:
			tabs::visuals( );
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		}

		zgui::end_groupbox( );
		zgui::end_window( );
	}

}

void menu::tabs::rage( ) {
	zgui::push_cursor_pos( { 5, 48 } );
	zgui::begin_groupbox( "group 1", { 241, 297 }, zgui::zgui_groupbox_flags_title_centered );
	{
		zgui::checkbox( "enable", variables::ragebot::enabled );
		zgui::checkbox( "auto fire", variables::ragebot::auto_fire );
		zgui::checkbox( "silent aim", variables::ragebot::silent_aim );

		zgui::slider_float( "min dmg", 0.f, 110.f, variables::ragebot::min_dmg );
		zgui::slider_float( "head pointscale", 0.f, 100.f, variables::ragebot::head_scale );
		zgui::slider_float( "body pointscale", 0.f, 100.f, variables::ragebot::point_scale );
		zgui::slider_float( "hitchance", 0.f, 100.f, variables::ragebot::hitchance );
		zgui::combobox( "target selection", { "fov", "distance" }, variables::ragebot::target_selection );

		zgui::combobox( "prioritize record", { "last", "first", "velocity", "dmg" }, variables::ragebot::prioritize_record );
		zgui::combobox( "hitbox priority", { "head", "neck", "upper chest", "chest", "pelvis" }, variables::ragebot::prioritize_hitbox );
		zgui::slider_int( "max targets", 0, 20, variables::ragebot::max_targets );
		

	} zgui::end_groupbox( );

	zgui::same_line( );
	zgui::next_column( 241, 25 );

	zgui::begin_groupbox( "antiaim", { 241, 297 }, zgui::zgui_groupbox_flags_title_centered );
	{
		zgui::begin_groupbox( "real", { 118, 230 }, zgui::zgui_groupbox_flags_title_centered );
		{
			zgui::combobox( "pitch", { "off", "up", "zero", "down" }, variables::antiaim::pitch );
			zgui::combobox( "yaw", { "off", "auto", "backwards", "test" }, variables::antiaim::yaw );
			zgui::slider_int( "fakelag", 0, 64, variables::antiaim::fakelag );
			zgui::checkbox( "on peek", variables::antiaim::on_peek );
			zgui::checkbox( "auto slow down", variables::ragebot::auto_slow_down );
			zgui::checkbox( "only safe point", variables::ragebot::safe_point );
			zgui::checkbox( "resolver", variables::ragebot::resolver );
			zgui::checkbox( "autostop", variables::ragebot::autostop );
			zgui::slider_int( "db tap ticks", 0, 64, variables::antiaim::db_tap_ticks );
			zgui::checkbox( "doubletap", variables::ragebot::double_tap );
			zgui::checkbox( "choke after shot", variables::antiaim::chokeAfterShoot );
			zgui::checkbox( "antiaim after shot", variables::antiaim::antiaimAfterShoot );
		
			
		}zgui::end_groupbox( );



	} zgui::end_groupbox( );
}


void menu::tabs::visuals( ) {
	zgui::push_cursor_pos( { 5, 48 } );
	zgui::begin_groupbox( "group 1", { 241, 297 }, zgui::zgui_groupbox_flags_title_centered );
	{
		zgui::combobox( "activation type", { "always", "visible only", "on key" }, variables::visuals::activation_type );
		if ( variables::visuals::activation_type == 2 )
			zgui::key_bind( "##key", variables::visuals::esp_key );
		zgui::checkbox( "teammates", variables::visuals::teammates );
		zgui::checkbox( "dormant", variables::visuals::dormant );
		zgui::checkbox( "bounding box", variables::visuals::box );
		zgui::checkbox( "name", variables::visuals::name );
		zgui::checkbox( "health bar", variables::visuals::health );
		zgui::checkbox( "weapon", variables::visuals::weapon );
		zgui::checkbox( "flags", variables::visuals::flags );
		zgui::checkbox( "money", variables::visuals::money );
		zgui::checkbox( "distance", variables::visuals::distance );
		zgui::checkbox( "glow", variables::visuals::glow );
		zgui::checkbox( "hit marker", variables::visuals::hitmarker );
		zgui::checkbox( "hit marker sound", variables::visuals::hitmarker_sound );
	} zgui::end_groupbox( );

	zgui::same_line( );
	zgui::next_column( 241, 25 );

	zgui::begin_groupbox( "group 2", { 241, 120 }, zgui::zgui_groupbox_flags_title_centered );
	{
		zgui::checkbox( "enemy", variables::visuals::chams::enemy_chams );
		zgui::colorpicker(  "enemy color", variables::visuals::chams::enemy_color );
		zgui::colorpicker( "enemy visible color", variables::visuals::chams::enemy_visible_color );
		zgui::checkbox( "team", variables::visuals::chams::teammate_chams );
		zgui::checkbox( "player behind the wall", variables::visuals::chams::xyz);
		
		if ( variables::visuals::chams::material )
			zgui::combobox( "##chams", { "default", "solid" }, variables::visuals::chams::material );

		zgui::checkbox( "backtrack chams", variables::visuals::chams::view_backtrack );

		zgui::slider_float( "fov", 0, 64, variables::visuals::fov );
		zgui::slider_float( "rotate circle", 0, 360, variables::visuals::circle_rotate );
		zgui::checkbox( "no smoke", variables::misc::removals::smoke );
		zgui::checkbox( "no flash", variables::misc::removals::smoke );
		zgui::checkbox( "nade prediction", variables::visuals::nade_prediction );
		//zgui::checkbox( "disable model occlusion", re );

	} zgui::end_groupbox( );

	zgui::begin_groupbox( "group 3", { 241, 167 }, zgui::zgui_groupbox_flags_title_centered );
	{

	} zgui::end_groupbox( );
}
