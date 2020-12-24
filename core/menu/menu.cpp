﻿#include "menu.hpp"
#include "../../dependencies/utilities/csgo.hpp"
/* include imgui */
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/impl/imgui_impl_dx9.h"
#include "ImGui/impl/imgui_impl_win32.h"
#include <string>
#include <functional>
#include "../menu/render/menu_render.hpp"

#include "../features/features.hpp"


IDirect3DTexture9* menu::buttonimage = nullptr;

std::string font_location = "C:\\Users\\topor\\Desktop\\ImGui-APP-master\\ImGui Application\Debug\\Montserrat-Regular.ttf";
menu::menu_data menu::m_menu_data;
menu::option_data menu::m_option_data [ 100 ];

void menu::start( IDirect3DDevice9* device ) {


}


void menu::option_slider ( const char * name, const char * description, float min, float max, float & value ) {


	ImGui::PushFont ( c_menu::get().bigf );
	ImGui::Text ( name );
	ImGui::PopFont ( );


	ImGui::SameLine ( );
	ImGui::PushItemWidth ( ImGui::GetWindowWidth ( ) / 3 );

	ImGui::SetCursorPosX ( ImGui::GetWindowWidth ( ) - ImGui::CalcItemWidth ( ) - 80 );
	ImGui::SliderFloat ( std::string ( "##slider" + std::string ( name ) ).data ( ), &value, min, max );
	ImGui::PopItemWidth ( );
	ImGui::PushFont ( c_menu::get ( ).smallf );

	ImGui::Text ( description );
	ImGui::PopFont ( );

}
void menu::option_slider_int ( const char * name, const char * description, int min, int max, int & value ) {


	ImGui::PushFont ( c_menu::get ( ).bigf );
	ImGui::Text ( name );
	ImGui::PopFont ( );

	ImGui::SameLine ( );
	ImGui::PushItemWidth ( ImGui::GetWindowWidth ( ) / 3 );

	ImGui::SetCursorPosX ( ImGui::GetWindowWidth ( ) - ImGui::CalcItemWidth ( ) - 80 );
	ImGui::SliderInt ( std::string ( "##slider" + std::string ( name ) ).data ( ), &value, min, max );
	ImGui::PopItemWidth ( );

	ImGui::PushFont ( c_menu::get ( ).smallf );

	ImGui::Text ( description );
	ImGui::PopFont ( );

}
void menu::option_multicombobox ( const char * name, const char * description, std::vector<combobox_select_item> multi_items ) {


	static float f = 0.f;
	ImGui::PushFont ( c_menu::get ( ).bigf );
	ImGui::Text ( name );
	ImGui::PopFont ( );

	ImGui::SameLine ( );
	ImGui::PushItemWidth ( ImGui::GetWindowWidth ( ) / 4 );

	ImGui::SetCursorPosX ( ImGui::GetWindowWidth ( ) - ImGui::CalcItemWidth ( ) - 320 );
	static const char * current_item = NULL;
	std::string preview = "";
	bool none_selected = false;
	for ( size_t i = 0; i < multi_items.size ( ); i++ ) {
		if ( *multi_items [ i ].value ) {
			if ( preview.length ( ) > 0 )
				preview += ", ";
			none_selected = true;
			preview += multi_items [ i ].name;
		}
	}
	if ( !none_selected )
		preview = "none";
	ImGui::SetCursorPosX ( ImGui::GetWindowWidth ( ) - ImGui::CalcItemWidth ( ) - 80 );
	if ( ImGui::BeginCombo ( std::string ( "##" + std::string ( name ) ).data ( ), preview.c_str ( ) ) ) {
		for ( int n = 0; n < multi_items.size ( ); n++ ) {
			//bool is_selected = ( current_item == multi_items [ n ].name );
			if ( ImGui::Selectable ( multi_items [ n ].name.c_str ( ), multi_items [ n ].value ) )
				current_item = multi_items [ n ].name.c_str ();
			if ( multi_items [ n ].value )
				ImGui::SetItemDefaultFocus ( );   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
		}
		ImGui::EndCombo ( );
	}
	ImGui::PopItemWidth ( );


	ImGui::PushFont ( c_menu::get ( ).smallf );

	ImGui::Text ( description );
	ImGui::PopFont ( );
}
void menu::option_combobox ( const char * name, const char * description, std::vector<std::string> items, int& value ) {

	static float f = 0.f;
	ImGui::PushFont ( c_menu::get().bigf );
	ImGui::Text ( name );
	ImGui::PopFont ( );

	ImGui::SameLine ( );
	ImGui::PushItemWidth ( ImGui::GetWindowWidth ( ) / 4 );

	ImGui::SetCursorPosX ( ImGui::GetWindowWidth ( ) - ImGui::CalcItemWidth ( ) - 320 );
	
	//static const char * current_item = NULL;
	ImGui::SetCursorPosX ( ImGui::GetWindowWidth ( ) - ImGui::CalcItemWidth ( ) - 80 );
	if ( ImGui::BeginCombo ( std::string ( "##combobox" + std::string ( name ) ).data ( ), items [ value ].c_str() )) {
		for ( int n = 0; n < items.size(); n++ ) {
			bool is_selected = ( value == n );
			if ( ImGui::Selectable ( items [ n ].data(), is_selected ) ) {
				//current_item = items [ n ].c_str ();
				value = n;
			}
			if ( is_selected )
				ImGui::SetItemDefaultFocus ( );
		}
		ImGui::EndCombo ( );
	}

	ImGui::PopItemWidth ( );


	ImGui::PushFont ( c_menu::get ( ).smallf );

	ImGui::Text ( description );
	ImGui::PopFont ( );

}
void menu::option_single ( const char * name, const char * description, bool & value, float color [ 4 ] ) {
	const ImGuiWindowFlags m_window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings;


	ImGui::PushFont ( c_menu::get ( ).bigf );
	ImGui::Text ( name );
	ImGui::PopFont ( );
	if ( color ) {
		ImGui::SameLine ( ImGui::GetWindowWidth ( ) - 90 );

		ImGui::ColorEdit4 ( std::string(std::string("color ") + std::string(name)).c_str(), color, ImGuiColorEditFlags_NoInputs );
	}
	ImGui::SameLine ( ImGui::GetWindowWidth ( ) - 60 );
	ImGui::Checkbox ( name, &value );
	ImGui::PushFont ( c_menu::get ( ).smallf );

	ImGui::Text ( description );
	ImGui::PopFont ( );




	
}

void menu::option ( const char * name, const char * description, std::function< void ( ) > func, int max_height, bool & value, float color [ 4 ] ) {
	const ImGuiWindowFlags m_window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings;
	m_menu_data.option_index++;
	const ImGuiIO & io = ImGui::GetIO ( );
	//printf( "element at index " + m_menu_data.option_index );
	auto & data = menu::m_option_data [ m_menu_data.option_index ];
	data.max_height = max_height;
	bool hovered = false;
	ImVec2 rect_left;
	ImVec2  rect_right;

	ImGui::BeginChild ( name, ImVec2 ( 0, data.cur_height ), true, m_window_flags );
	{
		ImGui::PushFont ( c_menu::get().bigf );
		ImGui::Text ( name );
		 rect_left = ImGui::GetItemRectMin ( );
		ImGui::PopFont ( );
		if ( color ) {
			ImGui::SameLine ( ImGui::GetWindowWidth ( ) - 90 );
		
			ImGui::ColorEdit4 ( std::string ( std::string ( name ) + std::string ( " color" ) ).data ( ), color, ImGuiColorEditFlags_AlphaBar );
		}
		ImGui::SameLine ( ImGui::GetWindowWidth ( ) - 60 );
		ImGui::Checkbox ( name, &value );
		 rect_right = ImGui::GetItemRectMax ( );
		ImGui::PushFont ( c_menu::get ( ).smallf );

		ImGui::Text ( description );
		rect_right.y = ImGui::GetItemRectMax ( ).y;
		ImGui::PopFont ( );
		
		hovered = ImGui::IsMouseHoveringRect ( rect_left, rect_right );
		ImGui::BeginChild ( name, ImVec2 ( 0, 0 ), true, m_window_flags );
		func ( );
		ImGui::EndChild ( );

	}

	ImGui::EndChild ( );
	if ( ImGui::IsItemHovered ( ) && hovered && io.MouseDown [ 0 ] && !data.in_transition ) {
		data.in_transition = true;
		data.enable = !data.enable;
	}
	if ( data.in_transition && max_height > 0 ) {
		if ( data.enable ) {
			if ( data.cur_height <= data.max_height ) {
				data.cur_height += 25;
			   //ImGui::GetWindowDrawList()->AddRectFilled(left, right )
			}
			else {
				data.in_transition = false;
				data.cur_height = data.max_height;
			}
		}
		else {
			if ( data.cur_height >= 25 ) {
				data.cur_height -= 25;
				if ( data.cur_height <= data.height )
					data.in_transition = false;
				//data.cur_height = 0;
			}
		}

	}



}

	 static int tab_index = 0;
void menu::render_menu( )
{
	/* TIP: We can't let ImGui move our window */

	if ( !c_menu::get( ).opened )
		return;

	const ImGuiWindowFlags m_window_flags =  ImGuiWindowFlags_NoCollapse |ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar;;

	//ImGui::SetNextWindowPos( ImVec2( 0, 0 ), ImGuiCond_Appearing );
	  ImGui::SetNextWindowSize( ImVec2( 800, 600 ), ImGuiCond_Always );

	
		 int element_index = 0;
	

		ImGui::PushFont( c_menu::get( ).normalf );
		ImGui::Begin( "ImGui Application" , &c_menu::get().opened , m_window_flags ); {
			ImVec4 color { ImColor( 25, 24, 25 ) };
			ImGui::PushStyleColor( ImGuiCol_ChildBg, color );
			ImGui::BeginChild( "ChildR", ImVec2( m_menu_data.width_transition, 0 ), false, m_window_flags ); {
				static int tabs_spacing = 10;
				if ( m_menu_data.show_tabs_buttons ) {
					ImGui::Dummy( ImVec2( 0, 280 ) );

					ImGui::Dummy ( ImVec2 ( 10, 0 ) );
					ImGui::SameLine ( );
					if ( ImGui::Button ( "rage", ImVec2 ( 212, 51 ) ) ) {
						tab_index = 0;
					
					}
					ImGui::Dummy( ImVec2( 10, 0 ) );
					ImGui::SameLine( );
					if ( ImGui::Button ( "legit", ImVec2 ( 212, 51 ) ) )
						tab_index = 1;

					ImGui::Dummy ( ImVec2 ( 10, 0 ) );
					ImGui::SameLine ( );
					if ( ImGui::Button ( "visuals", ImVec2 ( 212, 51 ) ) )
						tab_index = 2;


					if ( ImGui::Button ( "misc", ImVec2 ( 212, 51 ) ) )
						tab_index = 3;
				}
				else {
	
					ImGui::SetCursorPos( ImVec2( -2, ImGui::GetWindowHeight ( ) / 2 - 27.5 )  );
					ImGui::Image( menu::buttonimage, ImVec2( 55, 55 ) );
					
				}
			}
			ImGui::EndChild( );

			ImGui::PopStyleColor(  );
			if ( ImGui::IsItemHovered ( ImGuiHoveredFlags_RectOnly ) )
			{
				if ( !( m_menu_data.width_transition >= m_menu_data.width ) ) {
				
					m_menu_data.width_transition += 25;//( m_menu_data.width - m_menu_data.width_transition ) > 180 ?1:25;
					m_menu_data .show_tabs_buttons = true;
				}
				
			}
			else {
				if ( !( m_menu_data.width_transition <= m_menu_data.width_tabs ) ) {
					

					m_menu_data.width_transition -= 25;// m_menu_data.width_transition < 125?1:25;

					if ( m_menu_data.width_transition < 100 ) {
						m_menu_data.show_tabs_buttons = false;
				
					}
				}
			}

		
			ImGui::SameLine( );
			
			ImGui::BeginChild( "ChildL", ImVec2( 0, 0 ), true, m_window_flags );

			static bool asd3 = false;

			static float asddd [ 3 ] = { 255, 0,0 };//c_menu::get( ).opened 
			switch ( tab_index ) {
			case 0:
				menu::ragebot ( );
				break;

			case 1:
				menu::legitbot ( );
				break;

			case 2:
				menu::visuals ( );
				break;
			case 3:
#undef config
				ImGui::BeginChild ( "config", ImVec2 ( 409, 268 ), true );
				{
					static char entered_config_name [ 64 ] = { 0 };
					ImGui::SetCursorPosX ( ImGui::GetCursorPosX ( ) + 18 );
					ImGui::PushItemWidth ( 175 );
					ImGui::InputText ( ( "##CFG" ), entered_config_name, 64 );
					static int selected;
					std::string config;
					std::vector<std::string> configs = c_config::get ( ).get_configs ( );
					if ( configs.size ( ) > 0 ) {
						option_combobox ( "Config", "Select your config", configs, selected );
						config = configs [ c_config::get ( ).counts ];
					}
					c_config::get ( ).counts = selected;

					if ( configs.size ( ) > 0 ) {
						ImGui::SetCursorPosX ( ImGui::GetCursorPosX ( ) + 18 );
						if ( ImGui::Button ( ( "load" ), ImVec2 ( 175, 20 ) ) ) {
							c_config::get ( ).load_config ( config );

						}
					}
					if ( configs.size ( ) >= 1 ) {
						ImGui::SetCursorPosX ( ImGui::GetCursorPosX ( ) + 18 );
						if ( ImGui::Button ( ( "save" ), ImVec2 ( 175, 20 ) ) ) {
							c_config::get ( ).save_config ( config );
					
						}
					}
					ImGui::SetCursorPosX ( ImGui::GetCursorPosX ( ) + 18 );
					if ( ImGui::Button ( ( "create" ), ImVec2 ( 175, 20 ) ) ) {
						std::string config_file_name = entered_config_name;
						if ( config_file_name.size ( ) < 1 ) {
							config_file_name = ( "settings" );
						}
						c_config::get ( ).create_config ( config_file_name );

					}
					if ( config.size ( ) >= 1 ) {
						ImGui::SetCursorPosX ( ImGui::GetCursorPosX ( ) + 18 );
						if ( ImGui::Button ( ( "delete" ), ImVec2 ( 175, 20 ) ) ) {
							c_config::get ( ).remove_config ( config );

						}
					}

				}
				ImGui::EndChild (  );
				break;
			}
		

			m_menu_data.option_index = 0;
			ImGui::EndChild( );

		} 
		ImGui::PopFont( );
		ImGui::End( );

	}

void menu::render_visuals( ) {

}
	
