#include "../../features/features.hpp"
#include "../hooks.hpp"
#include "../../../dependencies/imgui/imgui.h"
#include "../../../dependencies/imgui/impl/imgui_impl_dx9.h"
#include "../../../dependencies/imgui/impl/imgui_impl_win32.h"


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
		overlay::present ( device, visuals::render );

		ImGui::Render ( );
		ImGui_ImplDX9_RenderDrawData ( ImGui::GetDrawData ( ) );


		return present_original ( device );
	}
}
