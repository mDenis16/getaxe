#pragma once
#include <vector>
#include <string>
#include <map>

namespace modules_manager
{
    void init_modules();

  inline std::vector<std::string> modules = { "engine.dll", "vgui2.dll", "vphysics.dll" ,"materialsystem.dll", "client.dll", "vstdlib.dll",   "inputsystem.dll",
  	"matchmaking.dll", "serverbrowser.dll", "server.dll","datacache.dll", "vguimatsurface.dll", "filesystem_stdio.dll" };

  inline std::map<std::size_t, std::uint32_t> MODULES_ADDRESS;

}