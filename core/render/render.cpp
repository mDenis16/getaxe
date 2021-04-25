#include "../features/features.hpp"
#include "../../dependencies/imgui/imgui.h"
#include "../../dependencies/imgui/impl/imgui_impl_dx9.h"
#include <d3dx9.h>
#include "gui/gui.h"
#include "../../dependencies/imgui/impl/imgui_impl_win32.h"

namespace overlay {

	static bool initialized = false;
	static IDirect3DStateBlock9 * state_block;
	HWND game_hwnd = NULL;

	// The original WndProc used by the game window.
	WNDPROC game_wndproc = NULL;

	void __stdcall end_present ( IDirect3DDevice9 * /*device*/ ) {


		ImGui::Render ( );

		state_block->Apply ( );
		state_block->Release ( );
	}


	void __stdcall pre_render ( IDirect3DDevice9 * device ) {

		D3DVIEWPORT9 d3d_viewport;
		device->GetViewport ( &d3d_viewport );

		device->CreateStateBlock ( D3DSBT_ALL, &state_block );
		state_block->Capture ( );

		device->SetVertexShader ( nullptr );
		device->SetPixelShader ( nullptr );
		device->SetFVF ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );

		device->SetRenderState ( D3DRS_LIGHTING, FALSE );
		device->SetRenderState ( D3DRS_FOGENABLE, FALSE );
		device->SetRenderState ( D3DRS_CULLMODE, D3DCULL_NONE );
		device->SetRenderState ( D3DRS_FILLMODE, D3DFILL_SOLID );

		device->SetRenderState ( D3DRS_ZENABLE, D3DZB_FALSE );
		device->SetRenderState ( D3DRS_SCISSORTESTENABLE, TRUE );
		device->SetRenderState ( D3DRS_ZWRITEENABLE, FALSE );
		device->SetRenderState ( D3DRS_STENCILENABLE, FALSE );

		device->SetRenderState ( D3DRS_MULTISAMPLEANTIALIAS, TRUE );
		device->SetRenderState ( D3DRS_ANTIALIASEDLINEENABLE, TRUE );

		device->SetRenderState ( D3DRS_ALPHABLENDENABLE, TRUE );
		device->SetRenderState ( D3DRS_ALPHATESTENABLE, FALSE );
		device->SetRenderState ( D3DRS_SEPARATEALPHABLENDENABLE, TRUE );
		device->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		device->SetRenderState ( D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA );
		device->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		device->SetRenderState ( D3DRS_DESTBLENDALPHA, D3DBLEND_ONE );

		device->SetRenderState ( D3DRS_SRGBWRITEENABLE, FALSE );
		device->SetRenderState ( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA );
	}
	BOOL CALLBACK find_game_hwnd ( HWND hwnd, LPARAM game_pid ) {
		// Skip windows not belonging to the game process.
		DWORD hwnd_pid = NULL;

		GetWindowThreadProcessId ( hwnd, &hwnd_pid );

		if ( hwnd_pid != game_pid )
			return TRUE;

		// Set the target window handle and stop the callback.
		game_hwnd = hwnd;

		return FALSE;
	}

	void initialize ( IDirect3DDevice9 * device ) {
		if ( !initialized ) {

			ImGui::CreateContext ( );
			ImGuiIO & io = ImGui::GetIO ( );
			

			ui::font_title = io.Fonts->AddFontFromFileTTF ( "C:\\Windows\\Fonts\\MaisonNeue-Medium.ttf", 30.f );
			ui::font_widgets = io.Fonts->AddFontFromFileTTF ( "C:\\Windows\\Fonts\\MaisonNeue-Medium.ttf", 24.f );

			static const ImWchar icons_ranges [ ] = { 'a', 'd', 0 };
			ImFontConfig icons_config; icons_config.PixelSnapH = true;
			ui::font_icons = io.Fonts->AddFontFromFileTTF ( "C:\\Windows\\Fonts\\icomoon.ttf", 20.f, &icons_config );
			ui::font_menu_icons = io.Fonts->AddFontFromFileTTF ( "C:\\Windows\\Fonts\\undefeated.ttf", 50.0f );
			ui::menuicons = io.Fonts->AddFontFromFileTTF ( "C:\\Windows\\Fonts\\menuicons.ttf", 50.0f );

			ImGui_ImplDX9_Init ( FindWindow ( "Valve001", NULL ), device );
			ImGui_ImplWin32_Init ( FindWindow ( "Valve001", NULL ) );

			initialized = true;
		}

	}

	void present ( IDirect3DDevice9 * device ) {
		if ( initialized ) {

	
			ui::render ( );

	
			
			
		}
	}

}