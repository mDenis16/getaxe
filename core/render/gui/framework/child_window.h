

namespace ui {

	enum child_type {
		normal,
		tab_container,
		main_container
	};
	class child_window : public object {



		ImColor bg_color = ImColor ( 255, 255, 255, 255 );
	public:
		object * tab_assign;
		std::string title;
		bool float_bottom = false;
		child_type type_child = normal;
		float rounding = 0.f;
		child_window ( std::string title_font, int width, int height, ImColor color, object * parent ) {
		
			this->title = title_font;
			this->width = width;
			this->height = height;
			this->renderer = parent->renderer;
			this->parrent = parent;
			this->type = child_window_element;
			this->bg_color = color;
			parent->add_children ( this );
		}
		child_window ( std::string title_font, float percent_width, float percent_height, ImColor color, object * parent, child_type _type ) {
		


			this->title = title_font;


			this->width = ( percent_width / 100.f ) * parent->width;
			this->height = ( percent_height / 100.f ) * parent->height;
			this->renderer = parent->renderer;
			this->parrent = parent;
			this->type_child = _type;
			this->type = child_window_element;
			this->bg_color = color;
			this->mins = this->parrent->mins;


			if ( !parent->children.empty ( ) ) {
				this->mins.x = parent->children.back ( )->mins.x;

			}
			this->maxs = ImVec2 ( this->mins.x + this->width, this->mins.y + this->height );

			update ( );
			parent->add_children ( this );
			update ( );
		}
		child_window ( std::string title_font, float percent_width, float percent_height, ImColor color, object * parent, child_type _type, int _flags ) {
		
			this->flags = _flags;

			this->title = title_font;


			this->width = ( percent_width / 100.f ) * parent->width;
			this->height = ( percent_height / 100.f ) * parent->height;
			this->renderer = parent->renderer;
			this->parrent = parent;
			this->type_child = _type;
			this->type = child_window_element;
			this->bg_color = color;
			this->mins = this->parrent->mins;


			if ( !parent->children.empty ( ) ) {
				this->mins.x = parent->children.back ( )->maxs.x;

			}
			this->maxs = ImVec2 ( this->mins.x + this->width, this->mins.y + this->height );

			update ( );
			parent->add_children ( this );
			update ( );
		}
		child_window ( std::string title_font, float percent_width, float percent_height, ImColor color, object * parent, float_side __float, float _rounding = 0.f, float _padding = 0.f ) {
		
			this->title = title_font;
			this->padding = _padding;
			this->rounding = _rounding;

			this->width = ( percent_width / 100.f ) * parent->width;
			this->height = ( percent_height / 100.f ) * parent->height;
			this->renderer = parent->renderer;
			this->parrent = parent;
			this->type = child_window_element;
			this->_float = __float;
			this->bg_color = color;
			this->float_bottom = true;

			this->maxs = ImVec2 ( this->mins.x + this->width, this->mins.y + this->height );
			update ( );
			parent->add_children ( this );
		}

		void draw ( ) override;
		void handle ( ) override;
		void update ( ) override;
		void handle_mouse_input ( ) override;
		void handle_tab_children ( );
	};
}
