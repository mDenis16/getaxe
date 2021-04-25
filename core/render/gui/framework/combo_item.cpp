#include <Windows.h>
#include <iostream>
#include <cmath>

#include "../includes.h"

namespace ui {

	void combo_item::draw ( ) {
		auto parent = static_cast< combobox * >( this->parrent );
	
		
		ImVec2 middle = ImVec2 ( ( this->mins.x + this->maxs.x ) / 2.f, ( this->maxs.y + this->mins.y ) / 2.f );

		middle.y -= ImGui::CalcTextSize ( this->title.c_str ( ), 13.f, ui::font_widgets ).y / 2.f;

	
		if ( this->parrent->type == combobox_element ) {
			if ( *parent->value == this->index  )
				this->renderer->AddRectFilled ( this->mins, this->maxs, ImColor ( 25, 125, 123, 15 ), 0.f );
		}
		else if ( this->parrent->type == multibox_element ) {
			if (*this->value > 0)
			    this->renderer->AddRectFilled ( this->mins, this->maxs, ImColor ( 255, 125, 123, 15 ), 0.f );
		
		}
		if ( this->hovering )
			this->renderer->AddRectFilled ( this->mins, this->maxs, ImColor ( 25, 125, 123, 5 ), 0.f );


		//this->renderer->AddRect ( this->mins, this->maxs, ImColor ( 25, 125, 123, 255 ), 0.f );
		this->renderer->AddText ( ui::font_widgets, 13.f, ImVec2 ( this->mins.x + 5.f, middle.y ), ImColor ( 255, 255, 255, 215 ), this->title.c_str ( ) );
		
	}
	void combo_item::handle_mouse_input ( ) {


	

	}
	void combo_item::handle ( ) {
		//if ( ui::focused_item != this->parrent->_id )
			//return;

		auto mouse_pos = ui::get_cursor ( );

		this->hovering = ( mouse_pos.x > this->mins.x + 10 && mouse_pos.y > this->mins.y && mouse_pos.x < this->maxs.x - 10 && mouse_pos.y < this->maxs.y );

		if ( this->parrent->type == combobox_element ) {
			auto parent = static_cast< combobox * >( this->parrent );
		
	
			handle_mouse_input ( );

			if ( this->hovering && key_released ( VK_LBUTTON ) ) {
				parent->set_value ( this->index );
			}
		}
		else if ( this->parrent->type == multibox_element ) {
			auto parent = static_cast< multibox * >( this->parrent );


			handle_mouse_input ( );

			if ( this->hovering && key_released ( VK_LBUTTON ) ) {
				if ( *this->value > 0 ) {
					*this->value = 0;
				}
				else if ( *this->value <= 0 ) {
					*this->value = 1;
				}
				
				parent->on_update ( );
				parent->refresh_preview ( );

			}
		}

		/*if ( ui::focused_item == this->_id && !this->hovering && key_released ( VK_LBUTTON ) ) {

			
		}*/
	}
	void combo_item::update ( ) {
		
		auto parent = static_cast< combobox * >( this->parrent );
		
		if ( this->index > 0 ) {
			auto back = parent->children.at ( this->index - 1 );
			this->mins = ImVec2 ( parent->bb_min.x, back->maxs.y  );
			this->maxs = ImVec2 ( parent->bb_max.x, this->mins.y + 18 );
		}
		else {

			if (this->parrent->type == combobox_element )
			  this->mins = ImVec2 ( parent->bb_min.x, parent->bb_min.y - parent->scroll_progress );
			else if (this->parrent->type == multibox_element )
				this->mins = ImVec2 ( parent->bb_min.x, parent->bb_min.y - parent->scroll_progress );

			this->maxs = ImVec2 ( parent->bb_max.x, this->mins.y + 18 );
		}
 
		
	}
}