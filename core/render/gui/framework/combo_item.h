#pragma once

namespace ui {

	class combo_item : public object {

	public:

		std::string title;

		float alpha_progress = 0.f;
		float target_alpha_progress = 0.f;
		std::vector<ImVec2> triangle;
		int* value;
			
		combo_item ( std::string text, object * parent, int& _value ) {


			this->title = text;
			this->value = &_value;
			this->parrent = parent;
			this->renderer = this->parrent->renderer;
			this->type = combo_element;

			

			this->parrent->add_children ( this );
		}

		combo_item ( std::string text, object * parent) {


			this->title = text;

			this->parrent = parent;
			this->renderer = this->parrent->renderer;
			this->type = combo_element;


			this->parrent->add_children ( this );
		}


		void draw ( ) override;
		void handle ( ) override;
		void update ( ) override;
		void handle_mouse_input ( ) override;

		void handle_hovering ( );

	};
}
