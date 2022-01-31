

#pragma once


#include <deque>
#include <string>

namespace ui {

	extern void * window_pointer_cheat;

	void initialize ( );
	void render ( ImDrawList* render );

	



	void to_rgb ( ImColor & original );

}

