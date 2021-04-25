

#include <Windows.h>
#include <iostream>
#include <cmath>

#include "../includes.h"

namespace ui {

	multibox::multibox ( std::string text, object * parent, std::vector<std::string> _items, void * _value_array, float _bb_width ) {


		this->items = _items;
		this->title = text;

		this->value_array = ( std::vector<int>*)(_value_array);

		this->parrent = parent;
		this->renderer = this->parrent->renderer;
		this->type = multibox_element;

		if ( _bb_width > 0.f )
			this->bb_width = _bb_width;


		for ( size_t i = 0; i < _items.size ( ); i++ )
			new combo_item ( _items.at ( i ), static_cast< object * >( this ), this->value_array->at ( i ) );


		this->parrent->add_children ( this );

		update ( );

	}

	/*implementare fara array-ul de bool uri trebuie sa fie neaparat pointer /shrug */
	multibox::multibox ( std::string text, object * parent, std::vector<std::string> _items, std::vector<int> & _value_array, float _bb_width ) {


		this->items = _items;
		this->title = text;
	
	

		this->parrent = parent;
		this->renderer = this->parrent->renderer;
		this->type = multibox_element;

		if ( _bb_width > 0.f )
			this->bb_width = _bb_width;


		for ( size_t i = 0; i < _items.size ( ); i++ )
			new combo_item ( _items.at ( i ), static_cast< object * >( this ), this->value_array->at(i ));


		this->parrent->add_children ( this );

		update ( );

	}
	/*
	 implementare cu keybind
	*/
	multibox::multibox ( std::string text, object * parent, std::vector<std::string> _items, std::vector<int> & _value_array, config_manager::key_bind_item & key_bind_item, float _bb_width ) {

		this->items = _items;
		this->title = text;
		this->value_array = &_value_array;


		this->parrent = parent;
		this->renderer = this->parrent->renderer;
		this->type = multibox_element;
		this->key_bind_controller = &key_bind_item;

		if ( _bb_width > 0.f )
			this->bb_width = _bb_width;


		for ( size_t i = 0; i < _items.size ( ); i++ ) {
			auto ref = this->value_array->at ( i );
			new combo_item ( _items.at ( i ), static_cast< object * >( this ), this->value_array->at ( i ) );
		}
		new key_bind_muie ( this, this->key_bind_controller );

		this->parrent->add_children ( this );

		update ( );

	}


	void multibox::update_triangle ( ) {

		auto pos = ImVec2 ( this->bb_max.x - 10.f, this->bb_min.y + 5.1f );
		auto left = ImVec2 ( pos.x - 5.f, pos.y );
		auto right = ImVec2 ( pos.x + 5.f, pos.y );

		auto bottom = ImVec2 ( pos.x, pos.y + 7.f );


		triangle_buffer.clear ( );

		triangle_buffer.push_back ( left );
		triangle_buffer.push_back ( right );
		triangle_buffer.push_back ( bottom );



	}


	void multibox::draw_scroll_thumb ( ) {
		if ( !this->opened )
			return;


		this->renderer->AddRectFilled ( this->thumb_min, this->thumb_max, ImColor ( 34, 46, 80, 255 ), 3.f );

	}
	void multibox::draw ( ) {

		this->handle ( );


		//this->renderer->AddRectFilled ( this->mins, this->maxs, ImColor ( 255, 0, 255, 255 ) );
		ImVec2 middle = ImVec2 ( ( this->mins.x + this->maxs.x ) / 2.f, ( this->maxs.y + this->mins.y ) / 2.f );
		middle.y -= ImGui::CalcTextSize ( this->title.c_str ( ), 13.f, ui::font_widgets ).y / 2.f;


		this->renderer->AddText ( ui::font_widgets, 13.f, ImVec2 ( this->mins.x, middle.y ), ImColor ( 255, 255, 255, 225 ), this->title.c_str ( ) );

		this->max_scroll_progress = 18.f * this->children.size ( );
		if ( ImGui::GetIO ( ).MouseWheel > 0.0f ) {
			this->scroll_progress -= 18;
			this->thumb_percent = ( this->scroll_progress / this->max_scroll_progress ) * 100.f;
		}
		else if ( ImGui::GetIO ( ).MouseWheel < 0.0f && children.back ( )->maxs.y >= this->bb_max.y ) {
			this->scroll_progress += 18;
			this->thumb_percent = ( this->scroll_progress / this->max_scroll_progress ) * 100.f;
		}

		if ( this->scroll_progress <= 0.f )
			this->scroll_progress = 0.f;
		else if ( this->scroll_progress >= this->max_scroll_progress )
			this->scroll_progress = this->max_scroll_progress;



		float limit_max = this->children.empty ( ) ? this->bb_min.y + 120 : this->children.back ( )->maxs.y + 10;

		if ( limit_max > this->original_bb_max_y + 120 ) {
			limit_max = this->original_bb_max_y + 120;
		}

		if ( this->opened ) {
			if ( !this->children.empty ( ) ) {



				this->bb_max.y += ( 1000.0 / ( double ) ImGui::GetIO ( ).Framerate ) / 5.f;
				//	this->scroll_progress = ImGui::GetIO ( ).MouseWheel;
				if ( this->bb_max.y > limit_max )
					this->bb_max.y = limit_max;



				this->renderer->AddRectFilled ( this->bb_min, this->bb_max, ImColor ( 27, 28, 31, 255 ), 3.5f );
				this->renderer->AddRect ( this->bb_min, this->bb_max, ImColor ( 255, 255, 255, 15 ), 4.5f );


				float progress = ( limit_max - this->bb_max.y ) * 100 / ( limit_max - this->original_bb_max_y ) / 100;


				int current_child = this->children.size ( ) - static_cast< int >( std::roundf ( ( progress / 1.f ) * static_cast< float >( this->children.size ( ) ) ) );

				if ( current_child == 0 ) {
					auto t_mins = ImVec2 ( this->bb_min.x + 3, this->bb_min.y + 1 );
					auto t_maxs = ImVec2 ( this->bb_max.x + 3, this->bb_min.y + 15 );

					ImVec2 middle = ImVec2 ( ( t_mins.x + t_maxs.x ) / 2.f, ( t_maxs.y + t_mins.y ) / 2.f );

					middle.y -= ImGui::CalcTextSize ( this->title.c_str ( ), 13.f, ui::font_widgets ).y / 2.f;



				}

				for ( auto & child : children ) {
					if ( child->type == keybind_element )
						continue;

					if ( child->maxs.y <= this->bb_max.y && child->mins.y >= this->bb_min.y )
						child->draw ( );

					child->update ( );
				}

			}
		}
		else if ( !opened && in_animation ) {



			this->bb_max.y -= ( 1000.0 / ( double ) ImGui::GetIO ( ).Framerate ) / 5.f;
			this->scroll_progress -= 18;
			if ( this->scroll_progress <= 0.f )
				this->scroll_progress = 0.f;

			if ( this->bb_max.y < this->original_bb_max_y ) {
				this->bb_max.y = this->original_bb_max_y;
				//update ( );
				ui::focused_item = -1;
				opened = false;
				in_animation = false;
			}



			this->renderer->AddRectFilled ( this->bb_min, this->bb_max, ImColor ( 27, 28, 31, 255 ), 3.5f );
			this->renderer->AddRect ( this->bb_min, this->bb_max, ImColor ( 255, 255, 255, 15 ), 4.5f );

			float progress = ( limit_max - this->bb_max.y ) * 100 / ( limit_max - this->original_bb_max_y ) / 100;


			int current_child = this->children.size ( ) - static_cast< int >( std::roundf ( ( progress / 1.f ) * static_cast< float >( this->children.size ( ) ) ) );

			if ( current_child == 0 ) {
				auto t_mins = ImVec2 ( this->bb_min.x + 3, this->bb_min.y + 1 );
				auto t_maxs = ImVec2 ( this->bb_max.x + 3, this->bb_min.y + 15 );

				ImVec2 middle = ImVec2 ( ( t_mins.x + t_maxs.x ) / 2.f, ( t_maxs.y + t_mins.y ) / 2.f );

				middle.y -= ImGui::CalcTextSize ( this->title.c_str ( ) ).y / 6.f;

				this->renderer->AddText ( ui::font_widgets, 13.f, ImVec2 ( this->bb_min.x + 5.f, middle.y ), ImColor ( 255, 255, 255, 215 ), this->preview.data ( ) );

			}
			for ( auto & child : children ) {
				if ( child->maxs.y <= this->bb_max.y && child->mins.y >= this->bb_min.y )
					child->draw ( );

				child->update ( );
			}



		}
		else {
			scroll_progress = 0.f;
			auto t_mins = ImVec2 ( this->bb_min.x + 3, this->bb_min.y );
			auto t_maxs = ImVec2 ( this->bb_max.x + 3, this->original_bb_max_y );

			ImVec2 middle = ImVec2 ( ( t_mins.x + t_maxs.x ) / 2.f, ( t_maxs.y + t_mins.y ) / 2.f );

			middle.y -= ImGui::CalcTextSize ( this->title.c_str ( ), 13.f, ui::font_widgets ).y / 2.f;


			this->renderer->AddRectFilled ( this->bb_min, this->bb_max, ImColor ( 27, 28, 31, 255 ), 3.5f );




			if ( this->hovering ) {
				this->animate_preview ( );
				auto text_size = ImGui::CalcTextSize ( this->preview_animated.c_str ( ), 13.f, ui::font_widgets );
				float maxim = ( this->bb_min.x + text_size.x );

				if ( !this->animating_preview && ( this->bb_min.x + text_size.x ) > this->bb_max.x )
					this->animating_preview = true;

				ImVec4 fine_clip_rect ( this->bb_min.x + 3.f, this->bb_min.y, this->bb_max.x - 20.f, this->bb_max.y );


				this->renderer->AddText ( ui::font_widgets, 13.f, ImVec2 ( this->preview_min.x, middle.y ), ImColor ( 255, 255, 255, 215 ), this->preview_animated.data ( ), NULL, 0.f, &fine_clip_rect );
			}
			else {
				if ( this->animating_preview ) {
					this->animating_preview = false;
					this->preview_min.x = this->bb_min.x + 5.f;
				}
				this->renderer->AddText ( ui::font_widgets, 13.f, ImVec2 ( this->bb_min.x + 5.f, middle.y ), ImColor ( 255, 255, 255, 215 ), this->preview.data ( ) );
			}


		}
		this->renderer->AddRect ( this->bb_min, this->bb_max, ImColor ( 255, 255, 255, 15 ), 3.5f );

		draw_scroll_thumb ( );

		if ( !this->children.empty ( ) && this->children.back ( )->type == keybind_element )
			this->children.back ( )->draw ( );

		if ( !this->opened ) {
			this->renderer->AddConvexPolyFilled ( triangle_buffer.data ( ), triangle_buffer.size ( ), ImColor ( 255, 255, 255, 255 ) );
			return;
		}


		if ( this->last_access_frame == ImGui::GetIO ( ).DeltaTime )
			return;

		for ( auto & child : this->children )
			child->handle ( );
	}
	void multibox::animate_preview ( ) {

		if ( this->animating_preview ) {

			auto text_size = ImGui::CalcTextSize ( this->preview.c_str ( ), 13.f, ui::font_widgets );

			if ( this->preview_min.x + text_size.x * 2.f - 22.f > this->bb_min.x ) {
				this->preview_min.x -= ( 1000.0 / ( double ) ImGui::GetIO ( ).Framerate ) / 55.f;
			}
		}
	}
	void multibox::handle_mouse_input ( ) {



		if ( in_animation )
			return;


		auto mouse_pos = ui::get_cursor ( );

		this->hovering = ( mouse_pos.x > this->bb_min.x && mouse_pos.y > this->bb_min.y && mouse_pos.x < this->bb_max.x && mouse_pos.y < this->bb_max.y );
	}
	void multibox::handle ( ) {


		if ( !this->old_value ) {
			this->old_value = std::malloc ( sizeof ( this->value_array ) );
			std::memcpy ( this->old_value, this->value_array, sizeof ( this->value_array ) );
		}
		

		/*if ( *(bool*)this->old_value != *this->value_array->data() ) {
			this->refresh_preview ( );
			std::memcpy ( this->old_value, this->value_array, sizeof ( this->value_array ) );
			
		}*/

		auto mouse_pos = ui::get_cursor ( );

		this->hovering_element = ( mouse_pos.x > this->mins.x && mouse_pos.y > this->mins.y && mouse_pos.x < this->maxs.x && mouse_pos.y < this->original_bb_max_y );

		if ( this->hovering_element && key_released ( VK_RBUTTON ) ) {
			this->key_bind_open = !this->key_bind_open;

		}
		if ( ui::focused_item != -1 && ui::focused_item != this->_id )
			return;

		handle_mouse_input ( );




		
		float thumb_center_y = this->bb_min.y + 18.f + this->thumb_percent * ( ( this->bb_max.y - this->bb_min.y ) ) / 100.f;
		this->thumb_min = ImVec2 ( this->bb_max.x - 8.f, thumb_center_y - 10.f );
		this->thumb_max = ImVec2 ( this->bb_max.x - 2.f, thumb_center_y + 10.f );
		this->hovering_thumb = ( mouse_pos.x > this->thumb_min.x && mouse_pos.y > this->bb_min.y && mouse_pos.x < this->thumb_max.x && mouse_pos.y < this->bb_max.y );

		if ( this->thumb_max.y > this->bb_max.y )
			this->thumb_max.y = this->bb_max.y;

		bool hovering_bb = ( mouse_pos.x > this->bb_min.x && mouse_pos.y > this->bb_min.y && mouse_pos.x < this->bb_max.x && mouse_pos.y < this->original_bb_max_y );

		if ( key_released ( VK_LBUTTON ) ) {
			if ( this->hovering_thumb ) {

			}
			else {
				if ( this->hovering && !this->opened ) {

					this->opened = !this->opened;
					this->last_access_frame = ImGui::GetIO ( ).DeltaTime;

					if ( this->opened ) {
						ui::focused_item = this->_id;
					}
					else {
						in_animation = true;
						ui::focused_item = -1;
					}
				}
				else if ( this->opened && !this->hovering ) {
					this->opened = false;
					in_animation = true;
					ui::focused_item = -1;
				}
			}
			thumb_locked = false;
		}
		if ( this->opened ) {
			if ( ( this->hovering_thumb && key_down ( VK_LBUTTON ) || thumb_locked ) ) {

				thumb_locked = true;

				this->thumb_percent = ( mouse_pos.y - this->bb_min.y );
				this->thumb_percent /= ( this->bb_max.y - this->bb_min.y );
				this->thumb_percent = ( this->thumb_percent * 100.f / 100 ) * 100;
				this->scroll_progress = ( this->thumb_percent / 100 ) * this->max_scroll_progress;

				if ( children.back ( )->maxs.y <= this->bb_max.y ) {
					this->thumb_percent = 99.f;
				}
				if ( this->scroll_progress > this->max_scroll_progress )
					this->scroll_progress = this->max_scroll_progress;



			}
		}




	}
	void multibox::update ( ) {



		if ( this->index > 0 ) {
			if ( this->parrent->type == tab_element ) {
				this->bb_width = 140;

				this->mins = ImVec2 ( this->parrent->parrent->maxs.x - this->bb_width - 20, this->parrent->parrent->mins.y + 60 );
				this->maxs = ImVec2 ( this->parrent->parrent->maxs.x, this->mins.y + 22.9f );


				this->bb_min = ImVec2 ( this->maxs.x - this->bb_width, this->mins.y + 3 );
				this->bb_max = ImVec2 ( this->maxs.x, this->maxs.y );

				this->original_bb_max_y = this->bb_max.y;
			}
			else {
				auto & back = this->parrent->children.at ( this->index - 1 );
				this->mins = ImVec2 ( this->parrent->mins.x + this->parrent->padding, back->maxs.y + 8 );
				this->maxs = ImVec2 ( this->parrent->maxs.x - this->parrent->padding, this->mins.y + 20 );

				this->bb_max = ImVec2 ( this->maxs.x, this->maxs.y );
				this->bb_min = ImVec2 ( this->maxs.x - bb_width - this->parrent->padding, this->mins.y );


				this->original_bb_max_y = this->bb_max.y;
			}
		}
		else {


			this->mins = ImVec2 ( this->parrent->mins.x + this->parrent->padding, this->parrent->mins.y + 40 );
			this->maxs = ImVec2 ( this->parrent->maxs.x - this->parrent->padding, this->mins.y + 22.9f );


			this->bb_min = ImVec2 ( this->maxs.x - this->bb_width, this->mins.y + 3 );
			this->bb_max = ImVec2 ( this->maxs.x, this->maxs.y );

			this->original_bb_max_y = this->bb_max.y;
		}


		this->update_triangle ( );


		for ( auto & children : this->children )
			children->update ( );

		this->refresh_preview ( );
		this->preview_min.x = this->bb_min.x + 5.f;

	}
}

