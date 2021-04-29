#include "../../features/features.hpp"
#include "../hooks.hpp"
#include "../../../dependencies/imgui/impl/imgui_impl_dx9.h"


namespace hooks::callback {
	long __stdcall  reset_hook ( IDirect3DDevice9 * thisptr, void * params ) {
		
		ImGui_ImplDX9_InvalidateDeviceObjects ( );
		ImGui_ImplDX9_CreateDeviceObjects ( );

		overlay::invalidate_device_objects ( );

		long hr = reset_original ( thisptr, params );

		return hr;
	}
}