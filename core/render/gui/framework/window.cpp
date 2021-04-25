#include "../includes.h"

#include <Windows.h>
#include <queue>

namespace ui {
	void window::draw ( ) {
		if ( !this->open )
			return;

		this->handle_mouse_input ( );
		auto mouse_pos = ui::get_cursor ( );

		bool hover_top = ( mouse_pos.x > this->mins.x && mouse_pos.y > this->mins.y - 30 && mouse_pos.x < this->maxs.x && mouse_pos.y < this->mins.y + 20 );
		this->dragging = hover_top && ( GetAsyncKeyState ( VK_LBUTTON ) & 0x8000 );

	
		static float delta_x = 0.f, delta_y = 0.f;
		static float mins_y = 0.f;
		static float mins_x = 0.f;
		if ( this->dragging ) {



			this->mins.x = mouse_pos.x - delta_x;
			this->mins.y = mouse_pos.y - delta_y;
		
			this->maxs = ImVec2 ( this->mins.x + this->width, this->mins.y + this->height );

			for ( auto & child : this->children )
				child->update ( );

		}
		else {
			delta_x = mouse_pos.x - this->mins.x;
			delta_y = mouse_pos.y - this->mins.y;
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
			this->open = !this->open;
			if ( this->open ) {
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