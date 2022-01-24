#pragma once

#include <windows.h>
#include <iostream>
#include <cstdint>
#include <memory>
#include <vector>
#include <thread>
#include <chrono>
#include <array>
#include <fstream>
#include <istream>
#include <unordered_map>
#include <intrin.h>

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
//#include "../core/menu/ImGui/imgui.h"
//#include "../core/menu/ImGui/imgui_internal.h"
//#include "../core/menu/ImGui/impl/imgui_impl_dx9.h"
#include "utilities/singleton.hpp"
#include "utilities/fnv.hpp"
#include "utilities/utilities.hpp"

#include "interfaces/interfaces.hpp"
#include "../source-sdk/sdk.hpp"
#include "../core/hooks/hooks.hpp"
#include "../dependencies/utilities/singleton.hpp"
#include "../dependencies/math/math.hpp"