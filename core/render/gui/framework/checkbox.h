#include <string>
#include <vector>
#include <memory>
namespace ui {

	class checkbox : public object {
		
	public:

		config_manager::key_bind_item * key_bind_controller = nullptr;

		std::string title;
		bool key_bind_open = false;
		ImVec2 bb_min;
		ImVec2 bb_max;

		ImVec2 key_bind_mins;
		ImVec2 key_bind_maxs;

		float key_bind_height = 70;
		float key_bind_width = 120;

		bool  *value;
		bool last_value;
		bool old_value;
		bool updated_last_time = false;
		int key_bind;
		int key_bind_mode = 0;

		

		long last_change_value = 0;
		checkbox ( std::string text, object * parent, bool & _value, ImColor & color, config_manager::key_bind_item & key_bind_item );
		checkbox ( std::string text, object * parent, bool & _value, config_manager::key_bind_item * key_bind_item );
		checkbox ( std::string text, object * parent, bool & _value, ImColor& color );

	
		checkbox ( std::string text, object* parent, bool& _value ) {


			this->title = text;
			this->value = &_value;
	
			
			this->parrent = parent;
			this->renderer = this->parrent->renderer;
			this->type = checkbox_element;

		

			update ( );
			this->parrent->add_children ( this );
		}

		void draw_keybind ( );
		void draw ( ) override;
		void handle ( ) override;
		void update ( ) override;
		void handle_mouse_input ( ) override;

	};
}
