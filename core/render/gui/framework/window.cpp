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

		

	
		if ( !this->dragging && key_pressed ( VK_LBUTTON ) && hover_top ) {
			this->dragging = true;
			this->last_mouse_pos = mouse_pos;
			this->original_pos = this->mins;
		}
		else if ( this->dragging && key_released ( VK_LBUTTON ) ) {
			this->last_mouse_pos = mouse_pos;
			this->dragging = false;
			this->original_pos = this->mins;
		}
		
	


		if ( this->dragging ) {



			this->mins.x = (int)(mouse_pos.x - delta_x);
			this->mins.y = (int)(mouse_pos.y - delta_y);

			this->maxs = ImVec2 ( this->mins.x + this->width, this->mins.y + this->height );


			update ( );

		}
		else {
			this->delta_x = std::fabs(mouse_pos.x - this->mins.x);
			this->delta_y = std::fabs(mouse_pos.y - this->mins.y);
		}

		auto clr = ImColor ( 27, 27, 37, 225 );
		this->renderer->AddRectFilled ( this->mins, ImVec2 ( this->maxs.x, this->mins.y + 10 ), ImColor ( 27, 27, 37, 255 ), 12.f, ImDrawCornerFlags_::ImDrawCornerFlags_Top );
		this->renderer->AddRectFilledMultiColor ( ImVec2 ( this->mins.x, this->mins.y + 10 ), this->maxs, ImColor ( 27, 27, 37, 255 ), ImColor ( 27, 27, 37, 255 ), clr, clr );
//
		this->renderer->AddLine ( ImVec2( this->resize_min.x, this->resize_max.y), ImVec2 ( this->resize_max.x, this->resize_min.y ), ImColor ( 255, 0, 255, 125 ));

		for ( size_t i = this->children.size ( ) - 1; i != ( size_t ) -1; i-- )
			this->children.at ( i )->draw ( );
	
	}
	void window::handle ( ) {

	
		if ( this->is_main_window ) {
			for ( int i = 0; i < 256; i++ ) {
				prev_key_state [ i ] = key_state [ i ];
				key_state [ i ] = GetAsyncKeyState ( i );
			}

			if ( key_pressed ( VK_INSERT ) ) {
				*this->open = !*this->open;
				if ( *this->open ) {
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

		if ( this->flags & flags::window_resizable ) {
			auto mouse_pos = ui::get_cursor ( );

			this->hovering_resize = ( mouse_pos.x > this->resize_min.x && mouse_pos.y > this->resize_min.y && mouse_pos.x < this->resize_max.x && mouse_pos.y < this->resize_max.y );

			if ( hovering_resize && key_pressed ( VK_LBUTTON ) )
				this->resizing = true;
			else if ( this->resizing && key_released ( VK_LBUTTON ) )
				this->resizing = false;


			if ( this->resizing ) {
				this->maxs = mouse_pos;

				this->height = this->maxs.y - this->mins.y;
				this->width = this->maxs.x - this->mins.x;
				update ( );
			}
		}
	}

	void window::update ( ) {

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

		this->resize_max = ImVec2 ( this->maxs.x - 2, this->maxs.y - 2 );
		this->resize_min = ImVec2 ( this->resize_max.x - 15, this->resize_max.y - 15 );

		for ( auto & child : this->children )
			child->update ( );

	}
	void window::handle_mouse_input ( ) {

	}
}