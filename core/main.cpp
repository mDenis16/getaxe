#include "../dependencies/utilities/csgo.hpp"
#include "features/features.hpp"
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS 


unsigned long WINAPI initialize(void* instance) {
	
	while ( !GetModuleHandleA ( "serverbrowser.dll" ) )
		std::this_thread::sleep_for ( std::chrono::milliseconds ( 100 ) );


	csgo::window = FindWindowA ( "Valve001", nullptr );

	while ( ! csgo::window )
		std::this_thread::sleep_for ( std::chrono::milliseconds ( 100 ) );


	console::initialize("csgo-cheat console");

//#if RELEASE
	//connection::main ( );

	try {
		interfaces::initialize();
		modulation::create_materials ( );
		hooks::initialize();
		hooks::create_hooks ( );
	}

	catch (const std::runtime_error & error) {
		MessageBoxA(NULL, error.what(), "csgo-cheat error!", MB_OK | MB_ICONERROR);
		FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
	}

	while (!GetAsyncKeyState(VK_END))
		std::this_thread::sleep_for ( std::chrono::milliseconds ( 500 ) );

	hooks::unloading = true;

	modulation::release_mem ( );

	FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
}

unsigned long WINAPI release() {

	hooks::release();
	console::release();

	return TRUE;
}

std::int32_t WINAPI DllMain(const HMODULE instance [[maybe_unused]], const unsigned long reason, const void* reserved [[maybe_unused]] ) {
	DisableThreadLibraryCalls(instance);

	switch (reason) {
	case DLL_PROCESS_ATTACH: {
		if (auto handle = CreateThread(nullptr, NULL, initialize, instance, NULL, nullptr))
			CloseHandle(handle);

		break;
	}

	case DLL_PROCESS_DETACH: {
		release();
		break;
	}
	}

	return true;
}
