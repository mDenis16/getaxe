

namespace ui {

	class sub_tab : public object {
		std::string text;
	
		int padding = 0;
		object * panel = nullptr;
		ImColor bg_color = ImColor ( 25, 25, 25, 255 );
	public:
		ImFont * icon_font = nullptr;
		float icon_size = 5.f;
		float offset_y = 0.f;
		std::string icon_text;

		object * child_assign = nullptr;
		bool selected = false;
		bool last_selected = false;
		sub_tab ( std::string text, std::string icon, ImFont* _icon_font, float _offset_y, float _icon_size, object * parent ) {
			this->parrent = parent;
			this->text = text;
			this->icon_size = _icon_size;
			this->padding = padding;
			this->icon_font = _icon_font;
			this->renderer = parent->renderer;
			this->width = parent->width;
			this->height = 30;
			this->offset_y = _offset_y;
			this->icon_text = icon;
			this->type = sub_tab_element;

			if ( !this->parrent->children_index )
				this->selected = true;

			update ( );
			parent->add_children ( this );
		}
		void set_panel ( object * obj ) {
			this->panel = panel;
		}
		void draw ( ) override;
		void handle ( ) override;
		void update ( ) override;
		void handle_mouse_input ( ) override;
	};
}
