#include "../dependencies/utilities/csgo.hpp"
#include "features/features.hpp"
#include <Windows.h>

#include "../dependencies/utilities/aye_imports.hpp"
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
		modules_manager::init_modules();
		interfaces_manager::init_prop_table();

		interfaces::initialize();
		visuals::handler->init ( );
		event_handler->setup ( );


		hooks::initialize();
		hooks::create_hooks ( );
		std::this_thread::sleep_for ( std::chrono::milliseconds ( 500 ) );
	
	}

	catch (const std::runtime_error & error) {
		MessageBoxA(NULL, error.what(), "csgo-cheat error!", MB_OK | MB_ICONERROR);
		FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
	}

	





	while (!GetAsyncKeyState(VK_END) && !hooks::unloading )
		std::this_thread::sleep_for ( std::chrono::milliseconds ( 500 ) );

	std::this_thread::sleep_for ( std::chrono::milliseconds ( 500 ) );

	hooks::unloading = true;
	std::this_thread::sleep_for ( std::chrono::milliseconds ( 500 ) );

	FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);

	ui::done = true;
	return true;
}

unsigned long WINAPI release() {

	hooks::release();
	visuals::handler->remove ( );
	event_handler->remove ( );

	delete visuals::handler;
	delete event_handler;
	delete legitbot;
	delete engine_prediction;
	delete visuals::radar;


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
