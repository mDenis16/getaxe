#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <Windows.h>

namespace ui {

	class window : public object {
		std::string title;
		float rounding = 0.f;
		bool dragging = false;
		bool failed_dragging = false;
		bool pressed_drag = false;
		bool* open;
	public:
		
		window ( std::string title_font, int width, int height,  ImDrawList* dr, bool & window_open, float rounding = 0.f ) {
			this->open = &window_open;
			this->title = title_font;
			this->width = width;
			this->height = height;
			this->width = width;
			this->renderer = dr;
			this->rounding = rounding;
			this->type = window_element;
			this->flex = line;
			this->rounding = rounding;
			this->mins = ImVec2 ( 30, 30 );
			this->maxs = ImVec2(this->mins.y + width, this->mins.y + height);
			this->_id = ui::last_element_id;
			ui::last_element_id++;

		}



		void draw ( ) override;
		void handle ( ) override;
		void update ( ) override;
		void handle_mouse_input ( ) override;

	};
}
