#include <string>
#include <vector>
#include <memory>
namespace ui {

	
	class small_text_input : public object {

	public:


		ImVec2 bb_min;
		ImVec2 bb_max;
		void * value;
		std::string text_val = "0.0f";
		void * old_value;
		
		void * value_mins = nullptr;
		void * value_maxs = nullptr;
		text_type input_type;
		small_text_input ( object * parent, text_type _type ) {

			this->parrent = parent;
			this->renderer = this->parrent->renderer;
			this->type = object_type::small_text_input_element;
			this->input_type = _type;
	

			update ( );
			this->parrent->add_children ( this );
		}
		small_text_input ( object * parent, float & _value, float mins, float maxs, text_type _type ) {
			
			this->parrent = parent;
			this->renderer = this->parrent->renderer;
			this->type = object_type::small_text_input_element;
			this->input_type = _type;
		

			update ( );
			this->parrent->add_children ( this );
		}


		void draw ( ) override;
		void handle ( ) override;
		void update ( ) override;
		void handle_mouse_input ( ) override;

	};
}
