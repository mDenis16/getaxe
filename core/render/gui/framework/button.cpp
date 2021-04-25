#include "../includes.h"

#include <Windows.h>
#include <iostream>
#include <cmath>


namespace ui {

	button::button ( std::string text, object * parent, std::function<void ( )> func ) {



		this->text = text;

		this->call_back = func;

		this->parrent = parent;
		this->renderer = this->parrent->renderer;
		this->type = button_element;

		height = 30;
		width = ( 85.f / 100.f ) * this->parrent->width;


		this->parrent->add_children ( this );
		update ( );


	}
	void button::draw ( ) {

		this->handle ( );

		this->renderer->PushClipRect ( this->mins, this->maxs, false );


		this->renderer->AddCircleFilled ( this->animation_position, this->animation_step, ImColor ( 25, 125, 123, this->animated_alpha ) );
		this->renderer->PopClipRect ( );


		this->renderer->AddRectFilled ( this->mins, this->maxs, ImColor ( 46, 49, 52, ui::focused_item == this->_id ? 211 : 45 ), 4.5f );
		this->renderer->AddRect ( this->mins, this->maxs, ImColor ( 255, 255, 255, 15 ), 4.5f );

		const ImVec2 text_size = ImGui::CalcTextSize ( this->text.c_str ( ), 15.f, ui::font_widgets );
		ImVec2 text_pos = ImVec2 ( );
		text_pos.x = ( ( this->mins.x + this->maxs.x ) / 2.f ) - text_size.x / 2.f;
		text_pos.y = ( this->mins.y + this->maxs.y ) / 2.f - text_size.y / 2.f;


		this->renderer->AddText ( ui::font_widgets, 15.f, text_pos, ImColor ( 255, 255, 255, 255 ), this->text.c_str ( ) );



	}
	void button::handle_mouse_input ( ) {
		if ( ui::focused_item != -1 )
			return;

		auto mouse_pos = ui::get_cursor ( );

		this->hovering = ( mouse_pos.x > this->mins.x && mouse_pos.y > this->mins.y && mouse_pos.x < this->maxs.x && mouse_pos.y < this->maxs.y );
	}

	void button::handle ( ) {

		handle_mouse_input ( );

		if ( this->in_animation ) {
			this->hovering = false;
		}

		if ( this->hovering && key_released ( VK_LBUTTON ) ) {
			
			this->call_back ( );

			this->in_animation = true;
			this->animation_position = ImGui::GetMousePos ( );
		}

		if ( this->in_animation ) {



			if ( this->animation_step > ( this->mins.x + this->maxs.x ) / 2.f ) {
				if ( this->animated_alpha > 50 )
					this->animated_alpha -= ( ( 1000.0 / ( double ) ImGui::GetIO ( ).Framerate ) / 5.f );
				else
					this->animated_alpha -= ( ( 1000.0f / ImGui::GetIO ( ).Framerate ) / 3.f );
			}

			if ( this->animation_step < ( this->mins.x + this->maxs.x ) ) {
				this->animation_step += ( ( 1000.0 / ( double ) ImGui::GetIO ( ).Framerate ) / 3 );

				if ( this->animated_alpha <= 120 )
					this->animated_alpha += ( ( 1000.0 / ( double ) ImGui::GetIO ( ).Framerate ) / 8.f );

			}
			if ( this->animated_alpha <= 0 ) {
				this->in_animation = false;
				this->animation_step = 0;
				this->animated_alpha = 0;
			}
		}
	}
	void button::update ( ) {
		


		if ( this->parrent->type == object_type::child_window_element ) {
			if ( static_cast< child_window * >( this->parrent )->flags & flags::align_center ) {
				this->mins.x = this->parrent->mins.x + 40;
				this->maxs.x = this->parrent->maxs.x - 40;
				if ( this->index > 0 ) {
					const auto & back = this->parrent->children.at ( this->index - 1 );
					this->mins.y = back->maxs.y + 10 ;
					this->maxs.y = this->mins.y + 30;
				}
				else {

					this->mins.y = this->parrent->mins.y;
					this->maxs.y = this->mins.y + 30;
				}
			}
			else if ( static_cast< child_window * >( this->parrent )->flags & flags::align_left ) {
				this->mins.x = this->parrent->mins.x;
				this->maxs.x = this->mins.x + width;

				if ( this->index > 0 ) {
					const auto & back = this->parrent->children.at ( this->index - 1 );
					this->mins.y = back->maxs.y + 10;
					this->maxs.y = this->mins.y + height;
				}
				else {

					this->mins.y = this->parrent->mins.y;
					this->maxs.y = this->mins.y + height;
				}
			}
			else if ( static_cast< child_window * >( this->parrent )->flags & flags::align_right ) {
				this->maxs.x = this->parrent->maxs.x;
				this->mins.x = this->maxs.x - width;
			

				if ( this->index > 0 ) {
					const auto & back = this->parrent->children.at ( this->index - 1 );
					this->mins.y = back->maxs.y + 10;
					this->maxs.y = this->mins.y + height;
				}
				else {

					this->mins.y = this->parrent->mins.y;
					this->maxs.y = this->mins.y + height;
				}
			}
		}
	}
}