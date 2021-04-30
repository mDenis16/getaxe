#pragma once
#include "../../dependencies/utilities/csgo.hpp"

#include "misc/events/events.hpp"
#include "../helpers/helpers.h"
#include "../../core/render/gui/config.h"
#include <optional>

#define DEBUG_LOG printf
#define local_pointer local_player::m_data.pointer
#define localdata local_player::m_data

#include "../features/setup/animations.h"
#include "../features/visuals/modulation/modulation.h"
#include "../features/visuals/visuals.h"
#include "../render/d3dfont.h"


namespace event_manager {
	void round_prestart ( i_game_event * );
	void round_end ( i_game_event * );
}

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
	void present ( IDirect3DDevice9 * device );
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

