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
#include "../features/visuals/visuals.h"


namespace event_manager {
	void round_prestart ( i_game_event * );
	void round_end ( i_game_event * );
}

namespace overlay {
	void initialize ( IDirect3DDevice9 * device );
	void __stdcall end_present ( IDirect3DDevice9 * /*device*/ );
	void __stdcall pre_render ( IDirect3DDevice9 * device );
	void present ( IDirect3DDevice9 * device );
	inline IDirect3DDevice9 * g_pDevice;

}

