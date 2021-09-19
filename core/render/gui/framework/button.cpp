#include "../includes.h"

#include <Windows.h>
#include <iostream>
#include <cmath>


namespace ui {

	button::button ( SecureString& text, object * parent, std::function<void ( )> func ) {



		this->text = &text;

		this->call_back = func;

		this->parrent = parent;
		this->renderer = this->parrent->renderer;
		this->type = button_element;

		height = 30;
		width = ( 85.f / 100.f ) * this->parrent->width;

		update ( );
		this->parrent->update ( );
		this->parrent->add_children ( this );
		this->parrent->update ( );
		update ( );

	}
	button::button ( SecureString& text, object * parent, float percent_width, float percent_height, std::function<void ( )> func ) {
	



		this->text = &text;

		this->call_back = func;

		this->parrent = parent;
		this->renderer = this->parrent->renderer;
		this->type = button_element;

		this->width = ( percent_width / 100.f ) * parent->width;
		this->height = ( percent_height / 100.f ) * parent->height;

		update ( );
		this->parrent->update ( );
		this->parrent->add_children ( this );
		this->parrent->update ( );
		update ( );
	}
	void button::draw ( ) {

		this->handle ( );

		float progress_alpha = ImGui::GetTime ( ) - hover_start_time;  progress_alpha = std::clamp ( progress_alpha * 3.5f, 0.f, 1.f );


		int alpha;

		if ( this->hovering ) {
			alpha = ( int ) std::lerp ( 45.f, 211.f, progress_alpha );

		}
		else {
			alpha = ( int ) std::lerp ( 45.f, 211.f, 1.f - progress_alpha );

		}
		
		this->renderer->PushClipRect ( this->mins, this->maxs, true );
		this->renderer->AddCircleFilled ( this->animation_position, this->animation_step, ImColor ( 25, 125, 123, this->animated_alpha ) );
		this->renderer->PopClipRect ( );


		this->renderer->AddRectFilled ( this->mins, this->maxs, ImColor ( 46, 49, 52, alpha ), 4.5f );
		this->renderer->AddRect ( this->mins, this->maxs, ImColor ( 255, 255, 255, 15 ), 4.5f );

		const ImVec2 text_size = ImGui::CalcTextSize ( this->text->data ( ), 15.f, ui::font_widgets );
		ImVec2 text_pos = ImVec2 ( );
		text_pos.x = ( ( this->mins.x + this->maxs.x ) / 2.f ) - text_size.x / 2.f;
		text_pos.y = ( this->mins.y + this->maxs.y ) / 2.f - text_size.y / 2.f;


		this->renderer->AddText ( ui::font_widgets, 15.f, text_pos, ImColor ( 255, 255, 255, 255 ), this->text->data ( ) );



	}
	void button::handle_mouse_input ( ) {
		

		auto mouse_pos = ui::get_cursor ( );

		this->hovering = ( mouse_pos.x > this->mins.x && mouse_pos.y > this->mins.y && mouse_pos.x < this->maxs.x && mouse_pos.y < this->maxs.y );
	}

	void button::handle ( ) {


		if ( !this->can_focus ( ) )
			return;

		handle_mouse_input ( );

		if ( this->was_hovering != this->hovering )
			this->hover_start_time = ImGui::GetTime ( );

	

		if ( this->hovering && key_pressed ( VK_LBUTTON ) ) {
			
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
		this->was_hovering = this->hovering;
	}
	void button::update ( ) {
		


		if ( this->parrent->type == object_type::child_window_element ) {
			if ( static_cast< child_window * >( this->parrent )->flags & flags::vertical_align_center ) {
				if ( this->parrent->children.size ( ) <= 1 ) {
					this->mins.y = this->parrent->mins.y + ( this->parrent->maxs.y - ( this->parrent->mins.y + this->height ) ) / 2.f;
				}
				else {
					this->mins.y = this->parrent->mins.y + ( this->parrent->maxs.y - ( this->parrent->children.back ( )->maxs.y ) ) / 2.f;
				}
			}

			if ( static_cast< child_window * >( this->parrent )->flags & flags::align_center ) {
				this->mins.x = this->parrent->mins.x + 40;
				this->maxs.x = this->parrent->maxs.x - 40;
				if ( this->index > 0 ) {
					const auto & back = this->parrent->children.at ( this->index - 1 );
					this->mins.y = back->maxs.y + 10 ;
					this->maxs.y = this->mins.y + 30;
				}
				else {

				//	this->mins.y = this->parrent->mins.y;
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

					//this->mins.y = this->parrent->mins.y;
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

					//this->mins.y = this->parrent->mins.y;
					this->maxs.y = this->mins.y + height;
				}
			}
		}
	}
}