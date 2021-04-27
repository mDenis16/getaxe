#include "../includes.h"

#include <Windows.h>
#include <queue>

namespace ui {
	void window::draw ( ) {
		if ( !*this->open )
			return;
		auto& imgdata = ImGui::GetIO ( );

		this->handle_mouse_input ( );
		auto mouse_pos = ui::get_cursor ( );

		bool hover_top = ( mouse_pos.x > this->mins.x && mouse_pos.y > this->mins.y - 50 && mouse_pos.x < this->maxs.x && mouse_pos.y < this->mins.y + 25 );
		this->dragging = hover_top && ( GetAsyncKeyState ( VK_LBUTTON ) & 0x8000 );

		static bool m_dragging = false;
		bool m_click = false;

		static decltype( this->mins ) dragging_pos { 0, 0 };


		static float delta_x = 0.f, delta_y = 0.f;

		if ( this->dragging ) {
			this->mins.x = mouse_pos.x - delta_x;
			this->mins.y = mouse_pos.y - delta_y;

			this->maxs = ImVec2 ( this->mins.x + this->width, this->mins.y + this->height );


			if ( this->mins.x < 0 )
				this->mins.x = 0;

			if ( this->mins.y < 0 )
				this->mins.y = 0;

			if ( this->maxs.x > ImGui::GetIO().DisplaySize.x )
				this->mins.x = ImGui::GetIO ( ).DisplaySize.x - this->width;

			if ( this->maxs.y > ImGui::GetIO ( ).DisplaySize.y )
				this->mins.y = ImGui::GetIO ( ).DisplaySize.y - this->maxs.y;

			this->maxs = ImVec2 ( this->mins.x + this->width, this->mins.y + this->height );


			for ( auto & child : this->children )
				child->update ( );

		}
		else {
			delta_x = std::fabs(mouse_pos.x - this->mins.x);
			delta_y = std::fabs(mouse_pos.y - this->mins.y);
		}

		this->renderer->AddRectFilled ( ImVec2 ( this->mins.x, this->mins.y + 30 ), ImVec2 ( this->maxs.x, this->maxs.y ), PRIMARY_COLOR, 12.f );

		for ( size_t i = this->children.size ( ) - 1; i != ( size_t ) -1; i-- )
			this->children.at ( i )->draw ( );
	
	}
	void window::handle ( ) {

		for ( int i = 0; i < 256; i++ ) {
			prev_key_state [ i ] = key_state [ i ];
			key_state [ i ] = GetAsyncKeyState ( i );
		}

		if ( key_pressed ( VK_INSERT ) ) {
			*this->open = !*this->open;
			if (*this->open ) {
				for ( object * child : this->children ) {

					std::queue<ui::object *> q;
					q.push ( child );

					while ( !q.empty ( ) ) {
						int n = q.size ( );


						while ( n > 0 ) {
							ui::object * p = q.front ( );
							p->should_reset = true;
							p->should_reanimate = true;

							q.pop ( );

							for ( int i = 0; i < p->children.size ( ); i++ )
								q.push ( p->children [ i ] );
							n--;
						}
					}
				}
			}
		}
	}

	void window::update ( ) {
		

	}
	void window::handle_mouse_input ( ) {

	}
}