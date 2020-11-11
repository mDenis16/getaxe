#include "menu_render.hpp"
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_internal.h"
#include "../../menu/menu.hpp"
#include "../../features/features.hpp"
#include <d3dx9.h>
#include <vector>
#include <pplwin.h>


#define UNLEN 256
IDirect3DStateBlock9* state_block;
bool reverse = false;
int offset = 0;
bool show_popup = false;
static bool save_config = false;
static bool load_config = false;

namespace ImGui {
	static auto vector_getter = [ ] ( void* vec, int idx, const char** out_text ) {
		auto& vector = *static_cast< std::vector<std::string>* >( vec );
		if ( idx < 0 || idx >= static_cast< int >( vector.size( ) ) ) { return false; }
		*out_text = vector.at( idx ).c_str( );
		return true;
	};

	bool combo_array( const char* label, int* current_index, std::vector<std::string>& values ) {
		if ( values.empty( ) ) { return false; }
		return Combo( label, current_index, vector_getter,
			static_cast< void* >( &values ), values.size( ) );
	}

	long get_mils( ) {
		auto duration = std::chrono::system_clock::now( ).time_since_epoch( );
		return std::chrono::duration_cast< std::chrono::milliseconds >( duration ).count( );
	}

	void begin_popup( const char* text, int onScreenMils, bool* done ) {
		if ( !done )
			show_popup = true;

		ImGuiIO& io = ImGui::GetIO( );
		ImGuiStyle& style = ImGui::GetStyle( );
		int width = io.DisplaySize.x;
		static long old_time = -1;
		ImGui::SetNextWindowPos( ImVec2( width - offset, 100 ) );
		style.WindowMinSize = ImVec2( 100.f, 20.f );
		ImGui::Begin( "##PopUpWindow", &show_popup, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize );
		ImVec2 p = ImGui::GetCursorScreenPos( );

		ImGui::GetWindowDrawList( )->AddRectFilledMultiColor( ImVec2( p.x - 15, p.y - 13 ), ImVec2( p.x + ImGui::GetWindowWidth( ), p.y - 16 ), ImColor( 167, 24, 71, 255 ), ImColor( 58, 31, 87, 255 ), ImColor( 58, 31, 87, 255 ), ImColor( 167, 24, 71, 255 ) );

		long current_time_ms = get_mils( );

		ImVec2 text_size = ImGui::CalcTextSize( text );
		ImGui::SetCursorPosY( ImGui::GetWindowHeight( ) / 2 - text_size.y / 2 );
		ImGui::Text( text );

		if ( !reverse ) {
			if ( offset < ImGui::GetWindowWidth( ) )
				offset += ( ImGui::GetWindowWidth( ) + 5 ) * ( ( 1000.0f / ImGui::GetIO( ).Framerate ) / 100 );

			if ( offset >= ImGui::GetWindowWidth( ) && old_time == -1 ) {
				old_time = current_time_ms;
			}
		}

		if ( current_time_ms - old_time >= onScreenMils && old_time != -1 )
			reverse = true;

		if ( reverse ) {
			if ( offset > 0 )
				offset -= ( ImGui::GetWindowWidth( ) + 5 ) * ( ( 1000.0f / ImGui::GetIO( ).Framerate ) / 100 );
			if ( offset <= 0 ) {
				offset = 0;
				reverse = false;
				*done = true;
				old_time = -1;
				show_popup = false;
			}
		}

		ImGui::End( );
	}
}

void c_menu::run( ) {
	ImGui::GetIO( ).MouseDrawCursor = opened;
	static int page = 0;

	menu::render_menu( );
	
}

void c_menu::run_visuals( ) {
	ImGui::SetNextWindowPos( ImVec2( 0,0 ) );
	ImGui::SetNextWindowSize( ImVec2( 1920, 1080 ) );
	ImGui::Begin( "##espp", &show_popup, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground );
	draw =  ImGui::GetWindowDrawList( );

	visuals::player::present( );
	visuals::notifications::present ( );
	visuals::draw_debug_points ( );
	ImGui::End( );
}

