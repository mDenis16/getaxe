#include "../../features/features.hpp"
#include "../hooks.hpp"
#include "../../../dependencies/imgui/imgui.h"
#include "../../../dependencies/imgui/impl/imgui_impl_dx9.h"
#include "../../../dependencies/imgui/impl/imgui_impl_win32.h"


namespace hooks::callback {
	long __stdcall  present_hook ( IDirect3DDevice9 * device ) {
		
		overlay::initialize ( device );
		ImGui_ImplDX9_NewFrame ( );
		ImGui_ImplWin32_NewFrame ( );


		overlay::present ( device );

		ImGui::EndFrame ( );
		ImGui::Render ( );


		ImGui_ImplDX9_RenderDrawData ( ImGui::GetDrawData ( ) );

		auto ret =  present_original ( device );
	
		return ret;
	}
}
