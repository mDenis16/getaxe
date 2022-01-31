#pragma once
#include "../../dependencies/utilities/csgo.hpp"

#include "../helpers/helpers.h"
#include "../../core/render/gui/config.h"
#include <optional>

//#define DEBUG_LOG printf
//#define local_player::ptr() local_player::data().pointer
//#define local_player::data() local_player::data()
//
//#include "../features/engine_prediction/engine_prediction.h"
//#include "../features/autowall/autowall.h"
//
//
//#include "../features/inventory_changer/CSharedObject.h"
//#include "../features/inventory_changer/CPlayerInventory.h"
//#include "../features/inventory_changer/CSInventoryManager.h"
//#include "../features/events/event_handler.h"
//
//#include "../features/visuals/modulation/modulation.h"
//#include "../features/visuals/visuals.h"
//#include "../features/misc/misc.h"
//#include "../features/visuals/damage_indicator/damage_indicator.h"
//#include "../render/d3dfont.h"
//#include "../features/lagcomp/lag_compensation.h"
//
//#include "../features/ragebot/ragebot.h"
//#include "../features/legitbot/legitbot.h"
//#include "../features/misc/misc.h"
//
//#include "../features/antiaim/fakelag.h"


class c_font;

namespace overlay {
	namespace fonts_ns
	{
		extern std::shared_ptr<c_font> esp;
		extern std::shared_ptr<c_font> esp_small;
		extern std::shared_ptr<c_font> lby;

		extern std::shared_ptr<c_font> controlfont;
		extern std::shared_ptr<c_font> tabfont;
		extern std::shared_ptr<c_font> tabfontthicc;
		extern std::shared_ptr<c_font> subtabfont;
		extern std::shared_ptr<c_font> smallfont;
		extern std::shared_ptr<c_font> keybindfont;
	}


	void initialize ( IDirect3DDevice9 * device );
	void __stdcall end_present ( IDirect3DDevice9 * /*device*/ );
	void __stdcall pre_render ( IDirect3DDevice9 * device );
	void present ( IDirect3DDevice9 * device, ImDrawList * render );
	void text ( ImVec2 pos, std::wstring string, ImColor color, std::shared_ptr<c_font> & font, uint8_t flags );
	void invalidate_device_objects ( );
	void init_device_ojects ( IDirect3DDevice9 * dev );

	inline IDirect3DDevice9 * g_pDevice;

	extern std::shared_ptr<c_font> esp;
	extern std::shared_ptr<c_font> esp_small;

	static constexpr auto points = 64;

	extern IDirect3DDevice9 * dev;
	extern D3DVIEWPORT9 port;

	extern std::vector<std::shared_ptr<c_font>> fonts;


}

