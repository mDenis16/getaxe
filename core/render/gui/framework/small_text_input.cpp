
#include "../includes.h"
#include <Windows.h>
#include <iostream>
#include <cmath>
#include <sstream>
#include <iomanip>


namespace ui {
	
	void small_text_input::draw ( ) {

		this->handle ( );


		//this->renderer->AddRectFilled ( this->mins, this->maxs, ImColor ( 255, 255, 255, 255 ), 0.f );
		//	this->renderer->AddText ( ui::font_widgets, 13.f, ImVec2 ( this->mins.x, middle.y ), ImColor ( 255, 0, 255, 225 ), this->title.c_str() );
		ImVec2 middle = ImVec2 ( ( this->mins.x + this->maxs.x ) / 2.f, ( this->maxs.y + this->mins.y ) / 2.f );

		middle.y -= ImGui::CalcTextSize ( this->text_val.c_str ( ) ).y / 6.f;
	
		this->renderer->AddRectFilled ( this->mins, this->maxs, ImColor ( 46, 49, 52, ui::focused_item == this->_id ? 211 : 45 ), 4.5f );
		this->renderer->AddRect ( this->mins, this->maxs, ImColor ( 255, 255, 255, 15 ), 4.5f );


		ImVec2 text_pos = ImVec2 ( );

		auto text_size = ImGui::CalcTextSize ( this->text_val.c_str ( ) );

		auto center = ( this->maxs.x + this->mins.x ) / 2.f;
		text_pos.x = center;
		if ( this->text_val.length ( ) == 2 )
			text_pos.x -= 4.9f;
		else if ( this->text_val.length ( ) == 3 )
			text_pos.x -= 7.8f;
		else if ( this->text_val.length ( ) == 4 )
			text_pos.x -= 12.6f;
		text_pos.y = middle.y;


		this->renderer->AddText ( ui::font_widgets, 11.5f, text_pos, ImColor ( 255, 255, 255, 225 ), this->text_val.c_str ( ) );
	}
	void small_text_input::handle_mouse_input ( ) {
		if ( ui::focused_item != -1 )
			return;

		auto mouse_pos = ui::get_cursor ( );

		this->hovering = ( mouse_pos.x > this->mins.x && mouse_pos.y > this->mins.y && mouse_pos.x < this->maxs.x && mouse_pos.y < this->maxs.y );
		this->hold_down = ( GetAsyncKeyState ( VK_LBUTTON ) & 0x8000 );
	}
	void small_text_input::handle ( ) {


		handle_mouse_input ( );
		
		if ( this->hovering && this->hold_down ) {
			ui::focused_item = this->_id;
		}
		

		if ( ui::focused_item == this->_id ) {

		
				if ( key_pressed ( VK_ESCAPE ) || key_pressed ( VK_RETURN ) || ( !this->hovering && key_pressed ( VK_LBUTTON ) ) ) {
					ui::focused_item = -1;
				}
				else if ( key_pressed ( VK_BACK ) && !this->text_val.empty ( ) ) {
					this->text_val.pop_back ( );
				}
				else if ( this->text_val.length ( ) < 12 ) {
					if ( key_pressed ( 0xBE ) ) {
						this->text_val += ".";
						return;
					}
					for ( int i = 0x30; i <= 0x39; i++ ) {
						
						if ( key_pressed ( i ) )
							this->text_val += static_cast< char >( i );
			
					}
				}
				if ( key_pressed ( VK_RETURN ) ) {
					auto parent = static_cast< slider * >( this->parrent );
					try {
						auto parsed = ::atof ( this->text_val.c_str ( ) );
						*( float * ) &parent->value = parsed;
					
						parent->in_animation = true;
						parent->target_fill = ( *( float * ) &parent->value * 100 ) / *( float * ) &parent->value_maxs;


					}
					catch ( int e ) {

					}

				}
		
		}


	}
	void small_text_input::update ( ) {

		if ( this->parrent->type == object_type::slider_element ) {
			this->mins.x = this->parrent->maxs.x - 26;
			this->mins.y = this->parrent->mins.y + 3;


			this->maxs.x = this->parrent->maxs.x + 4.5f;
			this->maxs.y = this->parrent->maxs.y - 3;


			auto s = (float) *( float * ) ( static_cast< slider * >( this->parrent )->value );
			
			std::stringstream stream;
			if (s < 10.0f )
			stream << std::fixed << std::setprecision ( 2 ) << s;
			else
				stream << std::fixed << std::setprecision ( 0 ) << s;
			this->text_val = stream.str ( );
		}
	
	}
}