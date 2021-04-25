
namespace ui {

	class tab : public object {
		std::string text;
		object * panel;


		int padding = 0;
		ImColor bg_color = ImColor ( 0, 0, 0, 255 );
	public:
		bool selected = false;
		bool in_entering = false;
		bool in_leaving = false;
		float anim_frame = 0.f;

		tab ( std::string text, object * parent, float padding ) {
			
			this->parrent = parent;
			this->text = text;
			this->padding = padding;
			this->width = ((90.f / 100.f) * (float)parent->width) ;
			this->renderer = parent->renderer;
			this->panel = nullptr;

			this->type = tab_element;
	
	
			this->bg_color = ImColor ( 200, 0, 25, 255 );
			if ( !this->parrent->children_index )
				this->selected = true;
			update ( );
			
			parent->add_children ( this );
		}

		void draw ( ) override;
		void handle ( ) override;
		void handle_mouse_input ( ) override;
		void update ( ) override;
	};
}
