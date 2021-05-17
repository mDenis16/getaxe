#include "../includes.h"

#include <Windows.h>
#include <iostream>
#include <cmath>
#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream
#include <d3dx9.h>
#include "../../../../dependencies/utilities/crypt_str.h"




namespace ui {


	inventory_item::inventory_item ( object * parent, weapon* weapon, void* _arch ) {

		this->weap = weapon;
		this->archive = _arch;
		this->parrent = parent;
		this->renderer = this->parrent->renderer;


		this->parrent->add_children ( this );
		generate_preview ( );

		update ( );

	}

	void inventory_item::generate_preview ( ) {
		//esource\flash\econ\weapons\base_weapons\weapon_cz75a.png
		
		
		std::string preview = "resource/flash/econ/weapons/base_weapons/" + this->weap->weapon_name + ".png";

		mutex_image_data.lock ( );
		{
			ui::images_data.push_back ( new ui::image_queue ( &this->image_buffer, this->archive, preview ) );
			mutex_image_data.unlock ( );
		}

	}

	void inventory_item::draw ( ) {
		handle ( );



		this->handle ( );

		

		this->renderer->AddRectFilled ( this->mins, this->maxs, ImColor ( 27, 28, 31, 255 ), 3.5f );
		this->renderer->AddRect ( this->mins, this->maxs, ImColor ( 255, 255, 255, 15 ), 4.5f );

		if ( this->weap ) {
			this->renderer->AddText ( ui::font_widgets, 14.f, ImVec2 ( this->mins.x, this->maxs.y + 6 ), ImColor ( 255, 255, 255, 255 ), this->weap->display_name.c_str());
		}
		
		if ( this->image_buffer )
		  this->renderer->AddImage ( this->image_buffer, ImVec2( this->mins.x + 15, this->mins.y + 15), ImVec2 ( this->maxs.x - 15, this->maxs.y - 15 ) );

		//this->renderer->AddShadowRect ( this->mins, this->maxs, ImColor ( 255, 255, 255, 255 ), 15.f, ImVec2 ( 0, 0 ), ImDrawShadowFlags_CutOutShapeBackground );

		//for ( auto & child : children ) {
	//		child->draw ( );
	//	}



	}
	void inventory_item::handle_mouse_input ( ) {
		if ( ui::focused_item != -1 )
			return;

		auto mouse_pos = ui::get_cursor ( );

		this->hovering = ( mouse_pos.x > this->mins.x && mouse_pos.y > this->mins.y && mouse_pos.x < this->maxs.x && mouse_pos.y < this->maxs.y );
	}

	void inventory_item::handle ( ) {

		handle_mouse_input ( );
		auto mouse_pos = ui::get_cursor ( );




	}


	void inventory_item::update ( ) {



		this->width = 120;
		this->height = 96;
		this->padding = 12;
	
		int max_elements_on_line = ( this->parrent->maxs.x - this->parrent->mins.x ) / this->width;

		max_elements_on_line = (( (max_elements_on_line - 1 ) * this->padding ) + this->width * max_elements_on_line ) / this->width;


		if ( this->index == 0 ) {
			float end_point = this->parrent->mins.x + (( max_elements_on_line - 1 ) * this->padding ) + this->width * max_elements_on_line;
			float start_point = ( this->parrent->maxs.x - end_point ) / 2.f;

			this->mins.x = this->parrent->mins.x + start_point;
			this->mins.y = this->parrent->mins.y - static_cast< ui::child_window * >( this->parrent )->scroll_progress + padding;

			this->maxs.x = this->mins.x + this->width;
			this->maxs.y = this->mins.y + this->height;

		}
		else {
			auto & last = this->parrent->children.at ( this->index - 1 );

		
		
			this->mins.x = last->maxs.x + this->padding;
			this->maxs.x = this->mins.x + this->width;

			this->mins.y = last->mins.y;
			this->maxs.y = this->mins.y + this->height;

			if ( this->maxs.x > this->parrent->maxs.x  ) {

				auto & first_in_row = this->parrent->children.at ( this->index - max_elements_on_line  );
				this->mins.y = first_in_row->maxs.y + 55;
				this->maxs.y = this->mins.y + this->height;
				this->mins.x = first_in_row->mins.x;
				this->maxs.x = this->mins.x + this->width;
			}

		}

		for ( auto & child : children ) {
			child->update ( );
		}

	}

}