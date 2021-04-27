
namespace ui {

	class panel_container : public object {

	public:

		int * selected_index;
		int old_index;

		panel_container ( object * parent, int& selector ) {
			this->selected_index = &selector;
			this->parrent = parent;
			this->width = parent->width;
			this->renderer = parent->renderer;
			this->type = panel_cotainer_element;

			update ( );

			parent->add_children ( this );
		}

		void draw ( ) override;
		void handle ( ) override;
		void handle_mouse_input ( ) override;
		void update ( ) override;
	};
}
