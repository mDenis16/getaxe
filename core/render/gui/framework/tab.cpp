#include "../includes.h"
#include <WinUser.h>

namespace ui {
	void tab::draw ( ) {
		if (!this->text) return;

		handle ( );
		


	//	this->renderer->AddRect ( this->mins, this->maxs, ImColor ( 255, 0, 255, 255 ) );
		auto text_size = ImGui::CalcTextSize ( this->text->data ( ), 14.f, ui::font_widgets );
		this->renderer->AddText ( ui::font_widgets, 14.f, ImVec2 ( (this->mins.x + this->maxs.x ) / 2.f - text_size.x / 2.f, (this->mins.y + this->maxs.y) / 2.f - text_size.y / 2.f), ImColor ( 255, 255, 255, this->selected ? 245 : 195 ), this->text->data ( ) );

		if ( !this->selected )
			return;

		for ( auto & child : this->children )
			child->draw ( );
	}
	void tab::handle ( ) {
		this->handle_mouse_input ( );
		if ( this->hovering && key_pressed ( VK_LBUTTON ) )
		{
			if ( this->selected )
				return;


			bool old_val = this->selected;
			if ( !this->parrent->children.empty ( ) ) {
				for ( object * child : this->parrent->children ) {
					static_cast< tab * >( child )->selected = false;
				}
			}
			this->selected = !old_val;
		}


	}
	void tab::handle_mouse_input ( ) {

		auto mouse_pos = ui::get_cursor ( );
		this->hovering = ( mouse_pos.x > this->mins.x && mouse_pos.y > this->mins.y && mouse_pos.x < this->maxs.x && mouse_pos.y < this->maxs.y );
	}
	void tab::update ( ) {
		/*
		this->mins = this->parrent->mins;

		if ( this->index > 0 ) {
			try {
				auto & back = this->parrent->children.at ( this->index - 1 );
				this->mins.x = back->maxs.x;
			}
			catch(int ex) {

			}
		}

		*/

		auto text_size = ImGui::CalcTextSize ( this->text->data ( ), 18.f, ui::font_widgets );
		if ( this->index > 0 ) {
			auto & old = this->parrent->children.at ( this->index - 1 );

			this->mins.x = old->maxs.x;
			this->mins.x += 15;

			this->mins.y = old->mins.y;

			this->maxs = ImVec2 ( this->mins.x + text_size.x, old->maxs.y );
		}
		else {

			this->mins = this->parrent->mins;
			this->mins.x += 15;

			this->maxs = ImVec2 ( this->mins.x + text_size.x, this->mins.y + 40 );
		}

		for ( auto & child : this->children )
			child->update ( );

		//if ( this->children.size ( ) ) {
			//this->maxs.y = this->children.back ( )->maxs.y + 5;
		//}
	
	}
}