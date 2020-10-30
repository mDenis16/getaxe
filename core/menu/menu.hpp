#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "zgui/zgui.hpp"
#include "variables.hpp"

namespace menu {
	inline static int current_tab = 0;

	void render();

	namespace tabs {
		void rage( );
		void visuals( );
	}
};
