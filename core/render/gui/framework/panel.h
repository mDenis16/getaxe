
namespace ui {

	class panel : public object {

	public:
	

		panel (  object * parent ) {

			this->parrent = parent;
			this->width = parent->width;
			this->renderer = parent->renderer;
			this->type = panel_element;

			update ( );

			parent->add_children ( this );
		}

		void draw ( ) override;
		void handle ( ) override;
		void handle_mouse_input ( ) override;
		void update ( ) override;
	};
}
