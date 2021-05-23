
#include <Windows.h>
#include <iostream>
#include <cmath>
#include <sstream>
#include <iomanip>
#include "../includes.h"

namespace ui {
	const char * text_keys [ 254 ] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x",
"y", "z", nullptr, nullptr, nullptr, nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6",
"7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, ";", "+", ",", "-", ".", "/?", "~", nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "[", "\\", "]", "'", nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

	double round_up ( double value, int decimal_places ) {
		const double multiplier = std::pow ( 10.0, decimal_places );
		return std::ceil ( value * multiplier ) / multiplier;
	}
	void text_input::draw ( ) {

		this->handle ( );
		if ( this->flags & ui::flags::text_input_as_element ) {
			this->renderer->AddRectFilled ( this->bb_min, this->bb_max, ImColor ( 46, 49, 52, ui::focused_item == this->_id ? 211 : 45 ), 4.5f );
			this->renderer->AddRect ( this->bb_min, this->bb_max, ImColor ( 255, 255, 255, 15 ), 4.5f );


			const ImVec2 text_size = ImGui::CalcTextSize ( this->text_val->empty ( ) ? this->place_holder.c_str ( ) : this->text_val->c_str ( ), 12.f, ui::font_widgets );
			ImVec2 text_pos = ImVec2 ( );
			text_pos.x = this->bb_min.x + 10;
			text_pos.y = ( this->bb_min.y + this->bb_max.y ) / 2.f - text_size.y / 2.f;

			if ( this->input_type == text_type::type_int ) {
				this->renderer->AddText ( ui::font_widgets, 12.f, text_pos, ImColor ( 255, 255, 255, this->text_val->empty ( ) ? 125 : 255 ),  std::to_string( *( int * ) this->value).c_str() );
			}
			else {
				if ( this->input_type != text_type::password ) {
					this->renderer->AddText ( ui::font_widgets, 12.f, text_pos, ImColor ( 255, 255, 255, this->text_val->empty ( ) ? 125 : 255 ), this->text_val->empty ( ) ? this->place_holder.c_str ( ) : this->text_val->c_str ( ) );
				}
				else {
					std::string t = "";
					for ( size_t i = 0; i < this->text_val->size ( ); i++ ) {
						t += "*";
					}
					this->renderer->AddText ( ui::font_widgets, 12.f, text_pos, ImColor ( 255, 255, 255, this->text_val->empty ( ) ? 125 : 255 ), this->text_val->empty ( ) ? this->place_holder.c_str ( ) : t.c_str ( ) );
				}
			}


			if ( !this->text_val->empty ( ) ) {
				const ImVec2 last_character_size = ImGui::CalcTextSize ( &this->text_val->back ( ), 12.f, ui::font_widgets );



				const auto from = ImVec2 ( this->bb_min.x + text_size.x + 12, text_pos.y + text_size.y + 1 );
				if ( ui::focused_item == this->_id )
					this->renderer->AddRectFilled ( from, ImVec2 ( from.x + 10, from.y + 1 ), ImColor ( 255, 255, 255, input_underline_alpha ) );
			}


			ImVec2 middle = ImVec2 ( ( this->mins.x + this->maxs.x ) / 2.f, ( this->maxs.y + this->mins.y ) / 2.f );

			middle.y -= ImGui::CalcTextSize ( this->title.c_str ( ), 12.f, ui::font_widgets ).y / 2.f;




			this->renderer->AddText ( ui::font_widgets, 13.f, ImVec2 ( this->mins.x, middle.y ), ImColor ( 255, 255, 255, 225 ), this->title.c_str ( ) );

		}
		else {
			if ( this->input_type == text_type::string || this->input_type == text_type::password ) {
				this->renderer->AddRectFilled ( this->mins, this->maxs, ImColor ( 46, 49, 52, ui::focused_item == this->_id ? 211 : 45 ), 4.5f );
				this->renderer->AddRect ( this->mins, this->maxs, ImColor ( 255, 255, 255, 15 ), 4.5f );
				const ImVec2 text_size = ImGui::CalcTextSize ( this->text_val->empty ( ) ? this->place_holder.c_str ( ) : this->text_val->c_str ( ), 15.f, ui::font_widgets );
				ImVec2 text_pos = ImVec2 ( );
				text_pos.x = this->mins.x + 10;
				text_pos.y = ( this->mins.y + this->maxs.y ) / 2.f - text_size.y / 2.f;

				if ( this->input_type != text_type::password ) {
					this->renderer->AddText ( ui::font_widgets, 15.f, text_pos, ImColor ( 255, 255, 255, this->text_val->empty ( ) ? 125 : 255 ), this->text_val->empty ( ) ? this->place_holder.c_str ( ) : this->text_val->c_str ( ) );
				}
				else {
					std::string t = "";
					for ( size_t i = 0; i < this->text_val->size ( ); i++ ) {
						t += "*";
					}
					this->renderer->AddText ( ui::font_widgets, 15.f, text_pos, ImColor ( 255, 255, 255, this->text_val->empty ( ) ? 125 : 255 ), this->text_val->empty ( ) ? this->place_holder.c_str ( ) : t.c_str ( ) );
				}
				if ( !this->text_val->empty ( ) ) {
					const ImVec2 last_character_size = ImGui::CalcTextSize ( &this->text_val->back ( ), 15.f, ui::font_widgets );



					const auto from = ImVec2 ( this->mins.x + text_size.x + 12, text_pos.y + text_size.y + 1 );
					if ( ui::focused_item == this->_id )
						this->renderer->AddRectFilled ( from, ImVec2 ( from.x + 10, from.y + 1 ), ImColor ( 255, 255, 255, input_underline_alpha ) );
				}
			}
			else {
				ImVec2 middle = ImVec2 ( ( this->mins.x + this->maxs.x ) / 2.f, ( this->maxs.y + this->mins.y ) / 2.f );

				middle.y -= ImGui::CalcTextSize ( this->text_val->c_str ( ) ).y / 6.f;

				this->renderer->AddRectFilled ( this->mins, this->maxs, ImColor ( 46, 49, 52, ui::focused_item == this->_id ? 211 : 45 ), 4.5f );
				this->renderer->AddRect ( this->mins, this->maxs, ImColor ( 255, 255, 255, 15 ), 4.5f );


				ImVec2 text_pos = ImVec2 ( );

				auto text_size = ImGui::CalcTextSize ( this->text_val->c_str ( ) );

				auto center = ( this->maxs.x + this->mins.x ) / 2.f;
				text_pos.x = center;
				if ( this->text_val->length ( ) == 2 )
					text_pos.x -= 4.9f;
				else if ( this->text_val->length ( ) == 3 )
					text_pos.x -= 7.8f;
				else if ( this->text_val->length ( ) == 4 )
					text_pos.x -= 12.6f;
				text_pos.y = middle.y;

				if ( this->hovering )
					this->renderer->AddText ( ui::font_widgets, 11.5f, text_pos, ImColor ( 255, 255, 255, 225 ), this->text_val->c_str ( ) );
			}
		}
	}
	void text_input::handle_mouse_input ( ) {


		const auto & mouse_pos = ImGui::GetMousePos ( );

	
		if (this->flags & ui::flags::text_input_as_element )
			this->hovering = ( mouse_pos.x > this->bb_min.x && mouse_pos.y > this->bb_min.y && mouse_pos.x < this->bb_max.x && mouse_pos.y < this->bb_max.y );
		else
	    	this->hovering = ( mouse_pos.x > this->mins.x && mouse_pos.y > this->mins.y && mouse_pos.x < this->maxs.x && mouse_pos.y < this->maxs.y );

	}
	bool keydown ( int key ) {
		return ( GetAsyncKeyState ( key ) & 0x8000 ) != 0;
	}
	void text_input::handle ( ) {


		handle_mouse_input ( );

		if ( key_pressed ( VK_LBUTTON ) ) {
			if ( this->hovering ) {
				ui::focused_item = this->_id;
			}
			else if ( ui::focused_item == this->_id )
				ui::focused_item = -1;
		}



		if ( ui::focused_item == this->_id ) {

			if ( this->input_type == text_type::string || this->input_type == text_type::password ) {
				if ( std::labs ( this->last_key_press - ImGui::GetTime ( ) ) < 150 ) {
					if ( pulse ) {
						input_underline_alpha += ( ( 1000.0 / ( double ) ImGui::GetIO ( ).Framerate ) / 5.f );
						if ( input_underline_alpha >= 100 )
							pulse = !pulse;
					}
					else {
						input_underline_alpha -= ( ( 1000.0 / ( double ) ImGui::GetIO ( ).Framerate ) / 5.f );
						if ( input_underline_alpha <= 0 )
							pulse = !pulse;
					}
				}
				else {
					input_underline_alpha = 100;
				}


				for ( size_t i = 0; i < 256; i++ ) {
					if ( key_pressed ( i ) ) {
						if ( i == VK_ESCAPE || i == VK_RETURN ) {
							ui::focused_item = -1;

							break;
						}

						if ( i == VK_SPACE ) {
							*this->text_val += " ";

						}
						else if ( i == VK_BACK && !this->text_val->empty ( ) ) {
							this->text_val->pop_back ( );
						}
						else if ( text_keys [ i ] != nullptr ) {
							if ( ( keydown ( VK_LSHIFT ) || keydown ( VK_RSHIFT ) ) )
								*this->text_val += static_cast< char >( i );
							else
								*this->text_val += text_keys [ i ];

							this->last_key_press = ImGui::GetTime ( );
						}

					}
				}
			}
			else if ( this->input_type == text_type::type_int ) {
				//*( int * ) this->value
				if ( key_pressed ( VK_ESCAPE ) || key_pressed ( VK_RETURN ) || ( !this->hovering && key_pressed ( VK_LBUTTON ) ) ) {
					ui::focused_item = -1;
				}
				else if ( key_pressed ( VK_BACK ) && !this->text_val->empty ( ) ) {
					this->text_val->pop_back ( );
				}
				else if ( this->text_val->length ( ) < 12 ) {
					if ( key_pressed ( 0xBE ) ) {
						return;
					}
					for ( int i = 0x30; i <= 0x39; i++ ) {

						if ( key_pressed ( i ) )
							*this->text_val += static_cast< char >( i );

						try {


							*( int * ) this->value = std::atoi ( this->text_val->c_str() );
						}
						catch ( int e ) {

						}
						
					}
				}
			}
			else {
				if ( key_pressed ( VK_ESCAPE ) || key_pressed ( VK_RETURN ) || ( !this->hovering && key_pressed ( VK_LBUTTON ) ) ) {
					ui::focused_item = -1;
				}
				else if ( key_pressed ( VK_BACK ) && !this->text_val->empty ( ) ) {
					this->text_val->pop_back ( );
				}
				else if ( this->text_val->length ( ) < 12 ) {
					if ( key_pressed ( 0xBE ) ) {
						*this->text_val += ".";
						return;
					}
					for ( int i = 0x30; i <= 0x39; i++ ) {

						if ( key_pressed ( i ) )
							*this->text_val += static_cast< char >( i );

					}
				}
				if ( key_pressed ( VK_RETURN ) ) {
					auto parent = static_cast< slider * >( this->parrent );
					try {
						auto parsed = ::atof ( this->text_val->c_str ( ) );
						*( float * ) &parent->value = parsed;

						parent->in_animation = true;
						parent->target_fill = ( *( float * ) &parent->value * 100 ) / *( float * ) &parent->value_maxs;


					}
					catch ( int e ) {

					}

				}
			}
		}


	}
	void text_input::update ( ) {

		if ( this->parrent->type == object_type::slider_element ) {
			this->mins.x = this->parrent->maxs.x - 26;
			this->mins.y = this->parrent->mins.y + 3;


			this->maxs.x = this->parrent->maxs.x + 4.5f;
			this->maxs.y = this->parrent->maxs.y - 3;


			auto s = *( float * ) ( &static_cast< slider * >( this->parrent )->value );

			std::stringstream stream;
			if ( s < 10.0f )
				stream << std::fixed << std::setprecision ( 2 ) << s;
			else
				stream << std::fixed << std::setprecision ( 0 ) << s;
			*this->text_val = stream.str ( );
		}
		else if ( this->parrent->type == object_type::child_window_element ) {
			if ( this->flags & flags::text_input_as_element ) {
				if ( this->index > 0 ) {
					
						auto & back = this->parrent->children.at ( this->index - 1 );
						this->mins = ImVec2 ( this->parrent->mins.x + this->parrent->padding, back->maxs.y + 8 );
						this->maxs = ImVec2 ( this->parrent->maxs.x - this->parrent->padding, this->mins.y + 20 );


						this->bb_min = ImVec2 ( ( this->maxs.x + this->mins.x ) / 2.f - 10.f, this->mins.y + 3 );
						this->bb_max = ImVec2 ( this->maxs.x, this->maxs.y );

				}
				else {


					this->mins = ImVec2 ( this->parrent->mins.x + this->parrent->padding, this->parrent->mins.y + 40 );
					this->maxs = ImVec2 ( this->parrent->maxs.x - this->parrent->padding, this->mins.y + 22.9f );


					this->bb_min = ImVec2 ( this->maxs.x - 120.f, this->mins.y + 3 );
					this->bb_max = ImVec2 ( this->maxs.x, this->maxs.y );

				
				}
			}
			else {
				if ( static_cast< child_window * >( this->parrent )->flags & flags::vertical_align_center ) {

					if ( this->children.size ( ) < 1 ) {
						this->mins.y = this->parrent->mins.y + ( this->parrent->maxs.y - ( this->parrent->mins.y + this->height ) ) / 2.f;
					}
					else {
						this->mins.y = this->parrent->mins.y + ( this->parrent->maxs.y - ( this->children.back ( )->maxs.y ) ) / 2.f;
					}
				}

				if ( static_cast< child_window * >( this->parrent )->flags & flags::align_left ) {
					this->mins.x = this->parrent->mins.x;
					this->maxs.x = this->mins.x + this->width;
					if ( this->index > 0 ) {
						const auto & back = this->parrent->children.at ( this->index - 1 );
						this->mins.y = back->maxs.y + 10 + this->offset_y;
						this->maxs.y = this->mins.y + 30;
					}
					else {


						this->maxs.y = this->mins.y + this->height;
					}

				}
			}


		}
	}
}