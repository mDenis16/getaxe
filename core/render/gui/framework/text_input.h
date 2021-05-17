

namespace ui {

	enum text_type {
		string,
		floats,
		hash,
		password
	};
	class text_input : public object {

	public:


		ImVec2 bb_min;
		ImVec2 bb_max;
		void * value;
		std::string * text_val;
		std::string place_holder = "none";
		void * old_value;
		float offset_y = 0.f;



		void * value_mins = nullptr;
		void * value_maxs = nullptr;
		int input_underline_alpha = 100;
		bool pulse = false;
		double last_key_press = 0.0;
		text_type input_type;

		text_input ( object * parent, std::string place_holder, std::string & value, text_type _type, float percent_width, float percent_height ) {
		

			this->width = ( percent_width / 100.f ) * parent->width;
			this->height = ( percent_height / 100.f ) * parent->height;
			this->parrent = parent;
			this->renderer = this->parrent->renderer;
			this->type = object_type::text_input_element;
			this->input_type = _type;
			this->text_val = &value;
			this->place_holder = place_holder;
		
			
			update ( );
			this->parrent->add_children ( this );
			update ( );
		}
		text_input ( object * parent, text_type _type ) {

			this->parrent = parent;
			this->renderer = this->parrent->renderer;
			this->type = object_type::text_input_element;
			this->input_type = _type;
			width = 200;
			height = 30;

			update ( );
			this->parrent->add_children ( this );
		}
		text_input ( object * parent, float & _value, float mins, float maxs, text_type _type ) {

			this->parrent = parent;
			this->renderer = this->parrent->renderer;
			this->type = object_type::text_input_element;
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
