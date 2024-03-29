#include "../hooks.hpp"
#include <renderlib/imgui/imgui.h>
#include <renderlib/imgui/impl/imgui_impl_dx9.h>
#include <renderlib/imgui/impl/imgui_impl_win32.h>
#include "../../features/visuals/visuals.h"
#include "../../features/visuals/damage_indicator/damage_indicator.h"
#include "../../features/overlay.hpp"

namespace hooks::callback {
	long __stdcall  present_hook ( IDirect3DDevice9 * device ) {
		
		if ( hooks::unloading )
			return present_original ( device );

		static uintptr_t firstAddress = reinterpret_cast< uintptr_t >( _ReturnAddress ( ) );
		if ( firstAddress != reinterpret_cast< uintptr_t >(_ReturnAddress ( )) )
		return present_original ( device );

	
		overlay::initialize ( device );



		ImGui_ImplDX9_NewFrame ( );
		ImGui_ImplWin32_NewFrame ( );
		ImGui::NewFrame ( );

		//visuals::on_render ( );

		visuals::render = ImGui::GetBackgroundDrawList ( );
		visuals::handler->on_render ( );
		dmg_indicator->on_render ( );
		overlay::present ( device, visuals::render );
		/*if ( visuals::radar )
			visuals::radar->on_render ( );*/

		ImGui::Render ( );
		ImGui_ImplDX9_RenderDrawData ( ImGui::GetDrawData ( ) );

	
		return present_original ( device );
	}
}
