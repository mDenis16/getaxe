
#pragma once
#include <iostream>
#include "../includes.h"

#include "../config.h"

namespace ui {

	config_item::config_item ( object * parent, std::string _name, std::string _hash ) {
		this->parrent = parent;
		this->renderer = this->parrent->renderer;
		this->type = config_item_element;
		this->name = _name;
		this->hash = _hash;
		



		this->parrent->add_children ( this );
		update ( );
	}


	void config_item::draw ( ) {

		this->handle ( );


		this->renderer->AddRectFilled ( this->mins, this->maxs, ImColor ( 27, 28, 31, 255 ), 5.5f );
		this->renderer->AddRect ( this->mins, this->maxs, ImColor ( 255, 255, 255, 15 ), 5.5f );

		ImVec2 middle = ImVec2 ( ( this->mins.x + this->maxs.x ) / 2.f, ( mins.y + maxs.y ) / 2.f );

		//middle.y -= ImGui::CalcTextSize ( this->name.c_str ( ), 13.f, ui::font_widgets ).y;

		this->renderer->AddText ( ui::font_widgets, 14.f, ImVec2 ( this->mins.x + 12.f, this->mins.y + 12.f ), ImColor ( 255, 255, 255, 255 ), name.data() );

		this->renderer->AddRectFilled ( this->button_mins, this->button_maxs, ImColor ( 46, 49, 52, ui::focused_item == this->_id ? 211 : 45 ), 4.5f );
		this->renderer->AddRect ( this->button_mins, this->button_maxs, ImColor ( 255, 255, 255, 15 ), 4.5f );

		std::string status = this->loaded ? "Save" : "Load";

		const ImVec2 text_size = ImGui::CalcTextSize ( "Load", 15.f, ui::font_widgets );
		ImVec2 text_pos = ImVec2 ( );
		text_pos.x = ( ( this->button_mins.x + this->button_maxs.x ) / 2.f ) - text_size.x / 2.f;
		text_pos.y = ( this->button_mins.y + this->button_maxs.y ) / 2.f - text_size.y / 2.f;


		this->renderer->AddText ( ui::font_widgets, 15.f, text_pos, ImColor ( 255, 255, 255, 255 ), status.c_str() );
		//this->renderer->AddRect ( this->delete_mins, this->delete_maxs, ImColor ( 255, 0, 0, 255 ) );
	    //this->renderer->AddRect ( this->copy_mins, this->copy_maxs, ImColor ( 255, 0, 0, 255 ) );

		this->renderer->AddText ( ui::font_icons, 13.f, this->delete_pos, ImColor ( 255, 255, 255, 200 ), ICON_BIN );
		this->renderer->AddText ( ui::font_icons, 13.f, this->copy_pos, ImColor ( 255, 255, 255, 200 ), ICON_COPY );
	}
	void config_item::handle_mouse_input ( ) {



		if ( in_animation )
			return;


		auto mouse_pos = ui::get_cursor ( );

		this->hovering_button = ( mouse_pos.x > this->button_mins.x && mouse_pos.y > this->button_mins.y && mouse_pos.x < this->button_maxs.x && mouse_pos.y < this->button_maxs.y );
	}

	void config_item::handle ( ) {


		handle_mouse_input ( );

		
		if ( this->hovering_button && key_released ( VK_LBUTTON ) ) {
			if ( this->loaded ) {
				/*to do save config ;)*/
				static_cast< config_box * >( this->parrent )->save_config ( this->name,false, this->hash );

			}
			else {
				for ( auto& child : this->parrent->children )
					static_cast< config_item * >( child )->loaded = false;

				static_cast< config_box * >( this->parrent )->load_config ( this->hash, false );

				this->loaded = true;
			}
		}


	}

	
	void config_item::update ( ) {

		auto parent = static_cast< ui::config_box * >( this->parrent );
		if ( this->index > 0 ) {
			auto & back = this->parrent->children.at ( this->index - 1 );
			this->mins.x = this->parrent->mins.x + 15;
			this->maxs.x = this->parrent->maxs.x - 15;

			this->mins.y = back->maxs.y + 10;
			this->maxs.y = this->mins.y + 60;
		}
		else {
			

			this->mins.x = this->parrent->mins.x + 15;
			this->maxs.x = this->parrent->maxs.x - 15;

		
			this->mins.y = this->parrent->mins.y - parent->scroll_progress;




			
			// (this->parrent->maxs.y - this->parrent->mins.y)
			this->maxs.y = this->mins.y + 60;
		}

		this->button_maxs.x = this->maxs.x - 10;
		this->button_mins.x = this->button_maxs.x - 85;

		this->button_maxs.y = this->maxs.y - 16;
		this->button_mins.y = this->mins.y + 16;


		const float spacing = 3.f;
		/*delete button*/
		this->delete_mins.y = this->button_mins.y + 3;
		this->delete_maxs.y = this->button_maxs.y - 3;

		this->delete_maxs.x = this->button_mins.x - 9.f;
		this->delete_mins.x = this->delete_maxs.x - (this->delete_maxs.y - this->delete_mins.y);

		
		/*copy button*/
		this->copy_mins.y = this->button_mins.y + 3;
		this->copy_maxs.y = this->button_maxs.y - 3;

		this->copy_maxs.x = this->delete_mins.x - spacing;
		this->copy_mins.x = this->copy_maxs.x - ( this->copy_maxs.y - this->copy_mins.y );




		
		 auto text_size = ImGui::CalcTextSize ( ICON_BIN, 13.f, ui::font_icons );
		 this->delete_pos = ImVec2 ( ( ( this->delete_mins.x + this->delete_maxs.x ) / 2.f - text_size.x / 2.f ), ( this->delete_mins.y + this->delete_maxs.y ) / 2.f - text_size.y / 2.f );

		 text_size = ImGui::CalcTextSize ( ICON_COPY, 13.f, ui::font_icons );
		 this->copy_pos = ImVec2 ( ( ( this->copy_mins.x + this->copy_maxs.x ) / 2.f - text_size.x / 2.f ), ( this->copy_mins.y + this->copy_maxs.y ) / 2.f - text_size.y / 2.f );



	}
}