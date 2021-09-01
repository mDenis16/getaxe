#include <Windows.h>
#include <iostream>
#include <cmath>

#include "../includes.h"

namespace ui {

	void combo_item::draw ( ) {
		auto parent = static_cast< combobox * >( this->parrent );
		int alpha = 255;

		if ( this->parrent->type == multibox_element ) {
			auto multibox_parent = static_cast< multibox * >( this->parrent );
			if ( this->mins.y < multibox_parent->bb_min.y - 4.5f ) {
				float dist = std::fabs ( ( multibox_parent->bb_min.y - 18.f - this->mins.y ) / 18.f );

				alpha = std::clamp ( dist, 0.f, 1.f ) * 255.f;
			}
			else if ( this->maxs.y > multibox_parent->bb_max.y + 4.5f ) {
				float dist = std::fabs ( ( multibox_parent->bb_max.y - 18.f - this->mins.y ) / 18.f );

				alpha = std::clamp ( dist, 0.f, 1.f ) * 255.f;
			}
			//else
			//	alpha = 255;
		}
		else {
			auto multibox_parent = static_cast< combobox * >( this->parrent );
			if ( this->mins.y < multibox_parent->bb_min.y - 4.5f ) {
				float dist = std::fabs ( ( multibox_parent->bb_min.y - 18.f - this->mins.y ) / 18.f );

				alpha = std::clamp ( dist, 0.f, 1.f ) * 255.f;
			}
			else if ( this->maxs.y > multibox_parent->bb_max.y + 4.5f ) {
				float dist = std::fabs ( ( multibox_parent->bb_max.y - 18.f - this->mins.y ) / 18.f );

				alpha = std::clamp ( dist, 0.f, 1.f ) * 255.f;
			}
			//else
			//	alpha = 255;
		}

		ImVec2 middle = ImVec2 ( ( this->mins.x + this->maxs.x ) / 2.f, ( this->maxs.y + this->mins.y ) / 2.f );

		middle.y -= ImGui::CalcTextSize ( this->title.c_str ( ), 13.f, ui::font_widgets ).y / 2.f;
		
		
		if ( this->parrent->type == combobox_element ) {
			if ( *parent->value == this->index )
				this->renderer->AddRectFilled ( this->mins, this->maxs, ImColor ( 25, 125, 123, 15 ), 0.f );
		}
		else if ( this->parrent->type == multibox_element ) {
			if ( *this->value > 0 )
				this->renderer->AddRectFilled ( this->mins, this->maxs, ImColor ( 255, 125, 123, 15 ), 0.f );

		}
		

		this->renderer->AddText ( ui::font_widgets, 13.f, ImVec2 ( this->mins.x + 5.f, middle.y ), ImColor ( 255, 255, 255, alpha ), this->title.c_str ( ) );
		//this->renderer->AddRect ( this->mins, this->maxs, ImColor ( 25, 125, 123, 255 ), 0.f );
		
	//	this->renderer->AddRect ( this->mins, this->maxs, ImColor ( 255, 0, 123, 255 ), 0.f );
	}
	void combo_item::handle_mouse_input ( ) {




	}
	void combo_item::handle_hovering ( ) {
	
		if ( this->hovering && !this->was_hovering ) /*enter hover time*/ {
			this->hover_start_time = ImGui::GetTime ( );
			ui::debug_logs.push_back ( new ui::debug_log ( "A intrat " + std::to_string( ImGui::GetTime ( )) ) );
		}
		else if ( !this->hovering && this->was_hovering )
			this->hover_start_time = ImGui::GetTime ( );

		//if ( this->hover_start_time != -1.f ) {
			float progress = ImGui::GetTime ( ) - hover_start_time;  progress = std::clamp ( progress * 3.5f, 0.f, 1.f );



			if ( this->hovering )
			this->renderer->AddRectFilled ( this->mins, this->maxs, ImColor ( 25, 125, 123, ( int ) std::lerp ( 1.f, 25.f, progress ) ) );
			else
				this->renderer->AddRectFilled ( this->mins, this->maxs, ImColor ( 25, 125, 123, ( int ) std::lerp ( 1.f, 25.f, 1.f - progress ) ) );
		//}
		this->was_hovering = this->hovering;
	}
	void combo_item::handle ( ) {
		//if ( ui::focused_item != this->parrent->_id )
			//return;

		auto mouse_pos = ui::get_cursor ( );

		this->hovering = ( mouse_pos.x > this->mins.x + 10 && mouse_pos.y > this->mins.y && mouse_pos.x < this->maxs.x - 10 && mouse_pos.y < this->maxs.y );

		if ( this->parrent->type == combobox_element ) {
			auto parent = static_cast< combobox * >( this->parrent );


			handle_mouse_input ( );

			if ( this->hovering && key_pressed ( VK_LBUTTON ) ) {
				parent->set_value ( this->index );
				ui::debug_logs.push_back ( new debug_log ( "Set value" ) );
				
			}
		}
		else if ( this->parrent->type == multibox_element ) {
			auto parent = static_cast< multibox * >( this->parrent );


			handle_mouse_input ( );

			if ( this->hovering && key_released ( VK_LBUTTON ) ) {
				if ( *this->value > 0 ) {
					*this->value = 0;
				}
				else if ( *this->value <= 0 ) {
					*this->value = 1;
				}

				parent->on_update ( );
				parent->refresh_preview ( );

			}
		}

		handle_hovering ( );

		/*if ( ui::focused_item == this->_id && !this->hovering && key_released ( VK_LBUTTON ) ) {


		}*/
	}
	void combo_item::update ( ) {


		if ( this->index > 0 ) {
			auto back = this->parrent->children.at ( this->index - 1 );
			if ( this->parrent->type == combobox_element ) {
				this->mins = ImVec2 ( static_cast< ui::combobox * >( this->parrent )->bb_min.x, back->maxs.y );
				this->maxs = ImVec2 ( static_cast< ui::combobox * >( this->parrent )->bb_max.x, this->mins.y + 18 );
			}
			else {
				this->mins = ImVec2 ( static_cast< ui::multibox * >( this->parrent )->bb_min.x, back->maxs.y );
				this->maxs = ImVec2 ( static_cast< ui::multibox * >( this->parrent )->bb_max.x, this->mins.y + 18 );
			}
		}
		else {
			

			if ( this->parrent->type == combobox_element ) {
				this->mins = static_cast< ui::combobox * >( this->parrent )->bb_min;
				this->mins.y = this->parrent->mins.y -static_cast< ui::combobox * >( this->parrent )->scroll_progress;
				this->maxs = ImVec2 ( static_cast< ui::combobox * >( this->parrent )->bb_max.x, this->mins.y + 18 );
			}
			else if ( this->parrent->type == multibox_element ) {
				this->mins = static_cast< ui::multibox * >( this->parrent )->bb_min;
				this->mins.y = this->parrent->mins.y -static_cast< ui::multibox * >( this->parrent )->scroll_progress;
				this->maxs = ImVec2 ( static_cast< ui::multibox * >( this->parrent )->bb_max.x, this->mins.y + 18 );


			}

		}
	}
}