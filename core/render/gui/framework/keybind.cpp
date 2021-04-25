#include "../includes.h"


namespace ui {




	key_bind_muie::key_bind_muie ( object * parent, config_manager::key_bind_item * key_bind ) {
		this->parrent = parent;
		this->renderer = this->parrent->renderer;
		this->type = keybind_element;
		this->key = &key_bind->key;
		this->key_mode = &key_bind->mode;
		
		if ( this->parrent->type != multibox_element )
		  key_bind->new_value = &this->new_value;

		this->padding = 15.f;

		update ( );



		new  key_selector ( "Key bind", this, key );
		auto combo_test = new combobox ( "Key Mode", this, std::vector<std::string>{"press", "toggle"}, *this->key_mode, 55.f );
		//new checkbox ( "New Value", this, this->new_value );

		switch ( this->parrent->type ) {
		case checkbox_element:
		{
			auto parent = static_cast< checkbox * >( this->parrent );

			new checkbox ( "New Value", this, *( bool * ) &this->new_value );
		}
		break;
		case slider_element:
		{
			auto parent = static_cast< slider * >( this->parrent );
			new slider ( "New Value", this, *( float * ) &this->new_value, *( float * ) &parent->value_mins, *( float * ) &parent->value_maxs, ui::slider_type::degrees );

		}
		break;
		case combobox_element:
		{
			auto parent = static_cast< combobox * >( this->parrent );
			this->open = &parent->key_bind_open;
			new combobox ( "New Value", this, parent->items, *( int * ) &this->new_value, 50.f );

		}
		break;
		case multibox_element:
		{
			auto parent = static_cast< multibox * >( this->parrent );
			this->open = &parent->key_bind_open;

			key_bind->new_value = new std::vector<int> ( parent->items.size ( ) );
			this->new_value = key_bind->new_value;

			new multibox ( "New Value", this, parent->items, this->new_value, 50.f );
			//std::string text, object * parent, std::vector<std::string> _items, bool * _value_array, float _bb_width = 0.f 
		}
		break;
		default:
			break;
		}

		this->parrent->add_children ( this );

		update ( );
	}

	void key_bind_muie::draw ( ) {

		if ( !this->open ) {
			if ( this->parrent->type == slider_element )
				this->open = &static_cast< slider * >( this->parrent )->key_bind_open;
			else if ( this->parrent->type == checkbox_element )
				this->open = &static_cast< checkbox * >( this->parrent )->key_bind_open;
			else if ( this->parrent->type == combobox_element )
				this->open = &static_cast< combobox * >( this->parrent )->key_bind_open;
			else if ( this->parrent->type == multibox_element )
				this->open = &static_cast< multibox * >( this->parrent )->key_bind_open;
		}

		if ( !*this->open )
			return;

		this->handle ( );


		this->renderer->AddRectFilled ( this->mins, this->maxs, ImColor ( 27, 28, 31, 255 ), 3.5f );
		this->renderer->AddRect ( this->mins, this->maxs, ImColor ( 255, 255, 255, 15 ), 4.5f );

		for ( size_t i = this->children.size ( ) - 1; i != ( size_t ) -1; i-- )
			this->children.at ( i )->draw ( );

	}

	void key_bind_muie::handle_mouse_input ( ) {


		auto mouse_pos = ui::get_cursor ( );

		this->hovering = ( mouse_pos.x > this->mins.x && mouse_pos.y > this->mins.y && mouse_pos.x < this->maxs.x && mouse_pos.y < this->maxs.y );
	}





	void key_bind_muie::handle ( ) {

		if ( *this->open ) {
			auto mouse_pos = ui::get_cursor ( );
			bool hovering_bb = ( mouse_pos.x > this->mins.x && mouse_pos.y > this->mins.y && mouse_pos.x < this->maxs.x && mouse_pos.y < this->maxs.y );

			if ( key_released ( VK_LBUTTON ) && !hovering_bb ) {
				*this->open = false;
				ui::focused_item = -1;
				return;
			}
		}





	}



	void key_bind_muie::update ( ) {


		this->mins.x = this->parrent->maxs.x - 3.f;
		this->maxs.x = this->mins.x + this->width;

		this->mins.y = this->parrent->maxs.y - 3.f;



		for ( auto & child : this->children )
			child->update ( );
		if ( !this->children.empty ( ) ) {
			this->maxs.y = this->children.back ( )->maxs.y + 10.f;
		}
	}
}