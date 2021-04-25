#include "../includes.h"

#include <Windows.h>
#include <iostream>
#include <cmath>

namespace ui {

	const char * keys [ ] = { "NO KEY", "M1", "M2", "BRK", "M3", "M4", "M5",
	"UNK", "BSPC", "TAB", "UNK", "UNK", "UNK", "ENTER", "UNK", "UNK", "SHFT",
	"CTRL", "ALT","PAUSE","CAPS", "UNK", "UNK", "UNK", "UNK", "UNK", "UNK",
	"ESC", "UNK", "UNK", "UNK", "UNK", "SPACE","PGUP", "PGDOWN", "END", "HOME", "Left",
	"UP", "RIGHT", "DOWN", "UNK", "PRNT", "UNK", "PRTSCR", "INS","DEL", "UNK", "0", "1",
	"2", "3", "4", "5", "6", "7", "8", "9", "UNK", "UNK", "UNK", "UNK", "UNK", "UNK",
	"UNK", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U",
	"V", "W", "X","Y", "Z", "LFTWIN", "RGHTWIN", "UNK", "UNK", "UNK", "NUM0", "NUM1",
	"NUM2", "NUM3", "NUM4", "NUM5", "NUM6","NUM7", "NUM8", "NUM9", "*", "+", "UNK", "-", ".", "/", "F1", "F2", "F3",
	"F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12","F13", "F14", "F15", "F16", "F17", "F18", "F19", "F20", "F21",
	"F22", "F23", "F24", "UNK", "UNK", "UNK", "UNK", "UNK","UNK", "UNK", "UNK",
	"NUMLCK", "SCRLLCK", "UNK", "UNK", "UNK", "UNK", "UNK", "UNK", "UNK",
	"UNK", "UNK","UNK", "UNK", "UNK", "UNK", "UNK", "LSHFT", "RSHFT", "LCTRL",
	"RCTRL", "LMENU", "RMENU", "UNK","UNK", "UNK","UNK", "UNK", "UNK", "UNK",
	"UNK", "UNK", "UNK", "NTRK", "PTRK", "STOP", "PLAY", "UNK", "UNK",
	"UNK", "UNK", "UNK", "UNK", ";", "+", ",", "-", ".", "?", "~", "UNK", "UNK",
	"UNK", "UNK","UNK", "UNK", "UNK", "UNK", "UNK", "UNK", "UNK",
	"UNK", "UNK", "UNK", "UNK", "UNK", "UNK", "UNK", "UNK","UNK",
	"UNK", "UNK", "UNK", "UNK", "UNK", "UNK", "UNK", "UNK", "UNK", "UNK", "UNK",
	"UNK", "UNK", "UNK","UNK", "UNK", "UNK", "UNK", "UNK", "UNK",
	"UNK", "UNK", "UNK", "UNK", "UNK", "UNK", "UNK", "UNK", "UNK",
	"UNK", "UNK", "UNK", "UNK", "UNK", "UNK", "UNK", "UNK", "UNK",
	"UNK", "UNK" };


	key_selector::key_selector ( std::string text, object * parent, int * key_bind ) {



		this->parrent = parent;
		this->renderer = this->parrent->renderer;
		this->type = key_selector_element;
		this->text = text;
		this->bind_key = key_bind;
		
		this->parrent->add_children ( this );


	}
	
	void key_selector::draw ( ) {

		this->handle ( );
		ImVec2 middle = ImVec2 ( ( this->mins.x + this->maxs.x ) / 2.f, ( this->maxs.y + this->mins.y ) / 2.f );

		middle.y -= ImGui::CalcTextSize ( this->text.c_str ( ) ).y / 6.f;
		//this->renderer->AddRectFilled ( this->mins, this->maxs, ImColor ( 255, 0, 0, 255 ) );
		
		this->renderer->AddText ( ui::font_widgets, 13.f, ImVec2 ( this->mins.x + this->parrent->padding, middle.y ), ImColor ( 255, 255, 255, 225 ), "Key bind" );

		this->renderer->AddRectFilled ( this->bb_min, this->bb_max, ImColor ( 46, 49, 52, 45 ), 3.5f );
		this->renderer->AddRect ( this->bb_min, this->bb_max, ImColor ( 255, 255, 255, 15 ), 3.5f );

		auto text_size = ImGui::CalcTextSize ( keys [ *this->bind_key ], 13.f, ui::font_widgets );

		this->renderer->AddText ( ui::font_widgets, 13.f, ImVec2 ( ((this->bb_min.x + this->bb_max.x) / 2.f) - (text_size.x / 2.f), middle.y ), ImColor ( 255, 255, 255, (int)alpha ), keys[*this->bind_key]);
	
	}
	void key_selector::handle_mouse_input ( ) {
		if ( ui::focused_item != -1 )
			return;
		  
		auto mouse_pos = ui::get_cursor ( );

		this->hovering = ( mouse_pos.x > this->bb_min.x && mouse_pos.y > this->bb_min.y && mouse_pos.x < this->bb_max.x && mouse_pos.y < this->bb_max.y );
	}
	
	void key_selector::handle ( ) {

		handle_mouse_input ( );
		if ( this->listening ) {
			
				if ( pulse ) {
					alpha += ( ( 1000.0 / ( double ) ImGui::GetIO ( ).Framerate ) / 5.f );
					if ( alpha >= 100 )
						pulse = !pulse;
				}
				else {
					alpha -= ( ( 1000.0 / ( double ) ImGui::GetIO ( ).Framerate ) / 5.f );
					if ( alpha <= 0 )
						pulse = !pulse;
				}
			
			for ( auto i = 0; i < 255; ++i ) {
				// a key is updated
				if ( key_released ( i ) ) {
					// menu or escape key kills the listener
					if ( i == VK_ESCAPE ) {
						*this->bind_key = 0;
						this->listening = false;
						break;
					}

					// we have a valid key
					else {
						*this->bind_key = i;
						this->listening = false;
						break;
					}
				}
			}
		}
		else {
			this->alpha = 100;
		}
		if ( this->hovering && key_released ( VK_LBUTTON ) ) {
			this->listening = !this->listening;
		}
		
		
	}
	void key_selector::update ( ) {

		

		
		
			
		if ( this->index > 0 ) {
			auto & back = this->parrent->children.at ( this->index - 1 );
			this->mins = ImVec2 ( this->parrent->mins.x, back->maxs.y + 8 );
			this->maxs = ImVec2 ( this->parrent->maxs.x, this->mins.y + 20 );


			//this->bb_min = ImVec2 ( this->maxs.x - 119, this->mins.y );
			//this->bb_max = ImVec2 ( this->maxs.x, this->maxs.y );

	
		}
		else {
			this->mins = ImVec2 ( this->parrent->mins.x , this->parrent->mins.y + 10 );
			this->maxs = ImVec2 ( this->parrent->maxs.x , this->mins.y + 22.9f );


			 //this->bb_min = ImVec2 ( this->maxs.x - 119, this->mins.y + 3 );
		     //this->bb_max = ImVec2 ( this->maxs.x, this->maxs.y );

		
		}

		this->bb_min = ImVec2 ( this->maxs.x - this->parrent->padding - 70, this->mins.y + 2.f );
		this->bb_max = ImVec2 ( this->maxs.x - this->parrent->padding, this->maxs.y - 2.f );

	}
}