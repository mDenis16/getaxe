#include "../includes.h"

#include <Windows.h>
#include <queue>

namespace ui {
	void window::draw ( ) {

	//	renderer = ImGui::GetBackgroundDrawList ( );

		if ( !*this->open )
			return;

		auto& imgdata = ImGui::GetIO ( );

		this->handle_mouse_input ( );
		auto mouse_pos = ui::get_cursor ( );

		bool hover_top = mouse_pos.x > 0 && mouse_pos.y > 0 && mouse_pos.y < ImGui::GetIO ( ).DisplaySize.y && mouse_pos.y < ImGui::GetIO ( ).DisplaySize.y && ( mouse_pos.x > this->mins.x && mouse_pos.y > this->mins.y - 50 && mouse_pos.x < this->maxs.x && mouse_pos.y < this->mins.y + 25 );

		static ImVec2 last_mouse_pos;
		static ImVec2 original_pos;

	
		if ( !this->dragging && key_pressed ( VK_LBUTTON ) && hover_top ) {
			this->dragging = true;
			last_mouse_pos = mouse_pos;
			original_pos = this->mins;
		}
		else if ( this->dragging && key_released ( VK_LBUTTON ) ) {
			last_mouse_pos = mouse_pos;
			this->dragging = false;
			original_pos = this->mins;
		}
		
	

		static bool m_dragging = false;
		bool m_click = false;

		static decltype( this->mins ) dragging_pos { 0, 0 };


		static float delta_x = 0.f, delta_y = 0.f;

		if ( this->dragging ) {

			//if ( last_mouse_pos.x == 0 || last_mouse_pos.y == 0 )
			//	last_mouse_pos = mouse_pos;

		//	mouse_pos.x = std::lerp ( last_mouse_pos.x, mouse_pos.x, ImGui::GetIO().DeltaTime * 15.f );
		//	mouse_pos.y = std::lerp ( last_mouse_pos.y, mouse_pos.y, ImGui::GetIO ( ).DeltaTime * 15.f );

		//	last_mouse_pos = mouse_pos;

			this->mins.x = mouse_pos.x - delta_x;
			this->mins.y = mouse_pos.y - delta_y;

			this->maxs = ImVec2 ( this->mins.x + this->width, this->mins.y + this->height );

			if ( std::isinf ( this->mins.x ) || std::isinf ( this->mins.y ) ) {
				this->mins = original_pos;
			}
			else {

				if ( this->mins.x < 0 )
					this->mins.x = 0;

				if ( this->mins.y < 0 )
					this->mins.y = 0;

				if ( this->maxs.x > ImGui::GetIO ( ).DisplaySize.x )
					this->mins.x = ImGui::GetIO ( ).DisplaySize.x - this->width;

				if ( this->maxs.y > ImGui::GetIO ( ).DisplaySize.y )
					this->mins.y = ImGui::GetIO ( ).DisplaySize.y - this->height;
			}

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