

namespace ui {

	enum child_type {
		normal,
		tab_container,
		main_container
	};

	class child_window : public object {



		ImColor bg_color = ImColor ( 255, 255, 255, 255 );
	public:
		object * tab_assign  = nullptr;
		SecureString* title = nullptr;
		bool float_bottom = false;

		std::string search_child = "";

		int last_search_size = 0;

		float maximumContentSize = 0.f;
		float percent_width = 0.f;
		float percent_height = 0.f;

		float thumb_length = 0.f;
		float thumb_progress = 0.f;
		float max_thumb_progress = 0.f;
		ImVec2 thumb_path_mins = ImVec2(0,0), thumb_path_maxs = ImVec2 ( 0, 0 );

		ImVec2 thumb_mins = ImVec2 ( 0, 0 ), thumb_maxs = ImVec2 ( 0, 0 );
		
		bool modifying_thumb = false;

		float scroll_progress = 0.f;
		float target_scroll_progress = 0.f;


		child_type type_child = normal;
		float rounding = 0.f;
		child_window ( SecureString& title_font, int width, int height, ImColor color, object * parent ) {
		
			this->title = &title_font;
			this->width = width;
			this->height = height;
			this->renderer = parent->renderer;
			this->parrent = parent;
			this->type = child_window_element;
			this->bg_color = color;
			parent->add_children ( this );
		}
		child_window ( SecureString& title_font, float _percent_width, float _percent_height, ImColor color, object * parent, child_type _type ) {
		


			this->title = &title_font;


			this->percent_width = _percent_width;
			this->percent_height = _percent_height;
		
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
		child_window ( SecureString& title_font, float _percent_width, float _percent_height, ImColor color, object * parent, child_type _type, int _flags ) {
		
			this->flags = _flags;

			this->title = &title_font;

			this->percent_width = _percent_width;
			this->percent_height = _percent_height;

			this->renderer = parent->renderer;
			this->parrent = parent;
			this->type_child = _type;
			this->type = child_window_element;
			this->bg_color = color;
			this->mins = this->parrent->mins;


			if ( !parent->children.empty ( ) ) {
				this->mins.x = parent->children.back ( )->maxs.x;

			}
		
			update ( );
			parent->add_children ( this );
			update ( );
		}
		child_window ( SecureString& title_font, float _percent_width, float _percent_height, ImColor color, object * parent, float_side __float, float _rounding = 0.f, float _padding = 0.f ) {
		
			this->title = &title_font;
			this->padding = _padding;
			this->rounding = _rounding;
			this->percent_width = _percent_width;
			this->percent_height = _percent_height;


			this->renderer = parent->renderer;
			this->parrent = parent;
			this->type = child_window_element;
			this->_float = __float;
			this->bg_color = color;
			this->float_bottom = true;


			update ( );
			parent->add_children ( this );
		
		}

		void draw_scrollbar ( );

		void handle_scrollbar ( );

		void draw ( ) override;
		void handle ( ) override;
		void update ( ) override;
		void handle_mouse_input ( ) override;
		void handle_tab_children ( );
	};
}
