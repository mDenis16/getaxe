
#include <Windows.h>
#include <iostream>
#include <cmath>
#include "../includes.h"

namespace ui {
	slider::slider ( std::string text, object * parent, float & _value, float mins, float maxs, slider_type _type ) {


		this->title = text;
		this->value = &_value;
		*( float * ) &this->value_mins = mins;
		*( float * ) &this->value_maxs = maxs;
		this->slide_type = _type;
		this->parrent = parent;
		this->renderer = this->parrent->renderer;
		this->type = slider_element;
		this->slide_type = _type;
		
		this->width_limit = 60;
		
		update ( );
		new small_text_input ( this, *( float * ) &this->value, mins, maxs, ui::text_type::floats );

		this->parrent->add_children ( this );
	}
	slider::slider ( std::string text, object * parent, float & _value, float mins, float maxs,  slider_type _type, config_manager::key_bind_item * key_bind_item ) {


		this->title = text;
		 this->value = &_value;
		*( float * ) &this->value_mins = mins;
		*( float * ) &this->value_maxs = maxs;
		this->slide_type = _type;
		this->key_bind_controller = key_bind_item;
		this->parrent = parent;
		this->renderer = this->parrent->renderer;
		this->type = slider_element;
	

		update ( );

		//new key_bind_muie ( this, key_bind );
		// object * parent, float & _value, float mins, float maxs, text_type _type 
		new small_text_input ( this, (float&)*( float * )this->value, mins, maxs, ui::text_type::floats );
		new key_bind_muie ( this, key_bind_item );
		this->parrent->add_children ( this );
	}
	


	void slider::draw ( ) {

		this->handle ( );

		if ( this->hovering && key_released ( VK_RBUTTON ) ) {
			this->key_bind_open = !this->key_bind_open;

		}
		//this->renderer->AddRectFilled ( this->mins, this->maxs, ImColor ( 255, 255, 255, 255 ), 0.f );

		
		

		this->renderer->AddRectFilled ( this->bb_min, this->bb_max, ImColor ( 46, 49, 52, 170 ), 12.f );
	
		auto filled = this->fill_percent * ( ( this->bb_max.x - this->bb_min.x ) ) / 100.f;
		this->renderer->AddRectFilled (this->bb_min, ImVec2( this->bb_min.x + filled, this->bb_max.y), ImColor(25, 125, 123, 255), 12.f );

		ImVec2 middle = ImVec2 ( ( this->mins.x + this->maxs.x ) / 2.f, ( this->maxs.y + this->mins.y ) / 2.f );

		middle.y -= ImGui::CalcTextSize ( this->title.c_str ( ), 13.f, ui::font_widgets ).y / 2.f;


		this->renderer->AddCircleFilled ( ImVec2 ( this->bb_min.x + filled, this->bb_min.y + ( this->bb_max.y - this->bb_min.y ) / 2.f ), 5.f, ImColor ( 255, 255, 255, 255 ) );
		
		this->renderer->AddText ( ui::font_widgets, 13.f, ImVec2 ( this->mins.x, middle.y ), ImColor ( 255, 255, 255, 225 ),  this->title.c_str() );

		for ( auto & child : this->children )
			child->draw ( );

	}
	void slider::handle_mouse_input ( ) {
		
		if ( !can_focus ( ) )
			return;


		auto mouse_pos = ui::get_cursor ( );

		this->hovering = ( mouse_pos.x > this->bb_min.x && mouse_pos.y > this->bb_min.y && mouse_pos.x < this->bb_max.x && mouse_pos.y < this->bb_max.y );
	}
	void slider::handle ( ) {

		if ( !this->old_value ) {
			this->old_value = std::malloc ( sizeof ( this->value ) );
			std::memcpy ( this->old_value, this->value, sizeof ( this->value ) );
		}



		if ( this->in_animation ) {

			if ( this->fill_percent > this->target_fill )
				this->fill_percent -= ( 1000.0 / ( double ) ImGui::GetIO ( ).Framerate ) / 8.f;
			else if ( this->fill_percent < this->target_fill ) {
				this->fill_percent += ( 1000.0 / ( double ) ImGui::GetIO ( ).Framerate ) / 8.f;

			}
			if ( std::fabs( this->fill_percent - this->target_fill) <= 1.f ) {
				this->fill_percent = this->target_fill;
				this->in_animation = false;
				if ( this->updated_last_time )
					this->updated_last_time = false;
			}
			return;

		}


			handle_mouse_input ( );
			if ( this->in_animation ) {
				this->hovering = false;
			}
			this->hold_down = ( GetAsyncKeyState ( VK_LBUTTON ) & 0x8000 );
			if ( !this->was_hovering && this->hovering && this->hold_down ) {
				this->was_hovering = true;

				if ( !this->hover_start )
					this->hover_start = ImGui::GetTime ( );
			}
			if ( !this->hold_down ) {
				this->was_hovering = false;
			}
			if ( this->was_hovering && this->hold_down ) {
				ui::focused_item = this->_id;
				auto mouse_pos = ui::get_cursor ( );
				float max_value = *( float * ) &this->value_maxs;
				float min_value = *( float * ) &this->value_mins;
				float old_fill = this->fill_percent;

				float temp_fill_percent = ( mouse_pos.x - this->bb_min.x );
				temp_fill_percent /= ( this->bb_max.x - this->bb_min.x );
				temp_fill_percent = ( temp_fill_percent * 100.f / 100 ) * 100;


				if ( temp_fill_percent > 100.f ) {
					temp_fill_percent = 100.f;
				}


				if ( temp_fill_percent < 0.f ) {
					temp_fill_percent = 0.f;
				}

				if ( std::fabs ( temp_fill_percent - old_fill ) > 30 ) {
					this->target_fill = temp_fill_percent;
					this->in_animation = true;
				}
				else {
					this->fill_percent = temp_fill_percent;
					this->target_fill = temp_fill_percent;
				}
				*( float * ) this->value = ( temp_fill_percent / 100 ) * max_value;
				if ( this->key_bind_controller ) {
					if ( !this->key_bind_controller->old_value )
					  this->key_bind_controller->old_value = malloc ( sizeof ( this->value ) );

					std::memcpy ( this->key_bind_controller->old_value, this->value, sizeof ( this->value ));// = this->value;
				}
				if ( !this->children.empty() )
				this->children.front ( )->update ( );

		

			}
			else if ( this->old_value && (float)*( float * ) this->value != ( float ) *( float * ) this->old_value ) {


				this->updated_last_time = true;

				this->in_animation = true;

				std::memcpy ( this->old_value, this->value, sizeof ( this->value ) );
				float cur_val = ( float ) *( float * ) this->value;
				float max_value = ( float ) *( float * )&this->value_maxs;
				this->target_fill = ( cur_val * 100.f ) / max_value;
				float tfl = this->target_fill;


			}

			else if ( ui::focused_item == this->_id ) {
				out_of_focus ( );
			}
		
	}
	void slider::update ( ) {
		bool is_parent_panel = this->parrent->type == panel_element || this->parrent->type == panel_cotainer_element;
		
		
		if ( this->index > 0 ) {
			auto & back = this->parrent->children.at ( this->index - 1 );
			this->mins = ImVec2 ( this->parrent->mins.x + this->parrent->padding, back->maxs.y + 8 );
			this->maxs = ImVec2 ( this->parrent->maxs.x - this->parrent->padding, this->mins.y + 20 );


			this->bb_min = ImVec2 ( (this->maxs.x + this->mins.x) / 2.f - 10.f , this->mins.y + 7 );
			this->bb_max = ImVec2 ( this->maxs.x - 35, this->maxs.y - 7 );

			if ( this->value )
				this->animation_step = this->bb_max.x;
			else
				this->animation_step = this->bb_min.x;
		}
		else {
			if ( is_parent_panel ) {

				/*if ( this->parrent->parrent && this->parrent->parrent->children.size() > 0 ) {
					auto previous_element = this->parrent->parrent->children.at ( this->parrent->index  - 1);

					this->mins = ImVec2 ( this->parrent->mins.x + this->parrent->padding, previous_element->maxs.y + 8 );
				}
				else {
					this->mins = ImVec2 ( this->parrent->mins.x + this->parrent->padding, this->parrent->mins.y + 8);
				}*/
				this->mins = ImVec2 ( this->parrent->mins.x + this->parrent->padding, this->parrent->mins.y );
				this->maxs = ImVec2 ( this->parrent->maxs.x - this->parrent->padding, this->mins.y + 20 );

			}
			else {
				this->mins = ImVec2 ( this->parrent->mins.x + this->parrent->padding, this->parrent->mins.y + 40 );
				this->maxs = ImVec2 ( this->parrent->maxs.x - this->parrent->padding, this->mins.y + 20 );
			}

			this->bb_min = ImVec2 ( ( this->maxs.x + this->mins.x ) / 2.f - 10.f, this->mins.y + 7 );
			this->bb_max = ImVec2 ( this->maxs.x - 35, this->maxs.y - 7 );

			if ( this->value )
				this->animation_step = this->bb_max.x;
			else
				this->animation_step = this->bb_min.x;

		}

		for ( auto & child : this->children )
			child->update ( );
	}
}