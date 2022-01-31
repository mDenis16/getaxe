#include "../hooks.hpp"
#include <d3dx9.h>

#include <renderlib/imgui/imgui.h>
#include <renderlib/imgui/impl/imgui_impl_win32.h>

//extern LRESULT ImGui_ImplDX9_WndProcHandler ( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
extern LRESULT ImGui_ImplWin32_WndProcHandler ( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

namespace hooks::callback {
	
	LRESULT __stdcall wnd_proc ( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam ) {
	
		if ( ImGui_ImplWin32_WndProcHandler ( hwnd, message, wparam, lparam ) )
			return true;
	
	
		return CallWindowProcA ( wndproc_original, hwnd, message, wparam, lparam );
	}
}