#include  "modules.hpp"
#include  "../fnv.hpp"
#include <Windows.h>

/// <summary>
/// This module should in loader only!
/// To be moved when server will be available!
/// </summary>

namespace modules_manager
{
	/// <summary>
	/// MODULES_ADDRESS to be sent to server for future address computing.
	/// </summary>
	void init_modules()
	{
		for (auto& _module : modules)
		{
			MODULES_ADDRESS[fnv::hash(_module.c_str())] = reinterpret_cast<std::uint32_t>(GetModuleHandleA(_module.c_str()));

		}
	}
}