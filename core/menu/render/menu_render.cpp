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
	//gpu_task::execute_gpu_trace ( );
	visuals::player::present( );
	visuals::notifications::present ( );
	visuals::local_esp ( );
	visuals::projectiles::draw ( );

	ImGui::End( );


//	visuals::circle_filled ( 500, 300, 50.f, 10.f, 100, D3DCOLOR_ARGB ( 205, 000, 000, 200 ) );
}

