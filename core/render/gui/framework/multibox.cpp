

#include <Windows.h>
#include <iostream>
#include <cmath>

#include "../includes.h"

namespace ui {
	double easeOutCirc ( double t ) {
		return sqrt ( t );
	}
	double easeOutCubiccc ( double t ) {
		return 1 + ( --t ) * t * t;
	}

	void multibox::handle_scroll ( ) {
		if ( !this->opened )return;

		if ( !should_be_scrollable ) return;

		float windowSize = 120;

		float maxLimitSize = this->children.size ( ) * 18.f;

		float contentSize = this->children.size ( ) * 18.f;
		float trackSize = windowSize / 3; //80 unknown units

		float windowContentRatio = windowSize / contentSize;
		float gripSize = trackSize * windowContentRatio;


		float percentage = windowSize / contentSize;
		this->thumb_length = windowSize * percentage;


		this->thumb_path_mins = ImVec2 ( this->bb_max.x - 5, this->bb_min.y  );
		this->thumb_path_maxs = ImVec2 ( this->bb_max.x, this->bb_max.y );


		

		float s = ( contentSize / windowSize );
		float location = windowSize / 0.5f;

		float pos = ( this->bb_max.y - this->bb_min.y ) * this->thumb_progress;
		//m_rctThumb.Size.Height = m_rctShaft.Height * m_flThumbPerc;
		//   thumb.Location.Y += m_rctBounds.Size.Width + m_rctThumb.Location.Y;
		this->thumb_mins = ImVec2 ( this->bb_max.x - 5, this->bb_min.y + pos );
		this->thumb_maxs = ImVec2 ( this->bb_max.x, this->thumb_mins.y + this->thumb_length );

		auto mouse_pos = ui::get_cursor ( );

		bool hovering_thumb = ( mouse_pos.x > this->thumb_path_mins.x && mouse_pos.y > this->thumb_path_mins.y && mouse_pos.x < this->thumb_path_maxs.x && mouse_pos.y < this->thumb_path_maxs.y );


		if ( ui::key_pressed ( VK_LBUTTON ) && hovering_thumb ) {
			this->modifying_thumb = true;
		}
		else if ( this->modifying_thumb && ui::key_released ( VK_LBUTTON ) ) {
			this->modifying_thumb = false;
		}

		 if ( ImGui::GetIO ( ).MouseWheel != 0.f) {
			float old = this->thumb_progress;

			if ( ImGui::GetIO ( ).MouseWheel  > 0.f)
		  	  this->thumb_progress -= 0.1f;
			else if ( ImGui::GetIO ( ).MouseWheel < 0.f)
				this->thumb_progress += 0.1f;

	    	
           this->thumb_progress = std::lerp ( old, this->thumb_progress, ImGui::GetIO ( ).DeltaTime * 13.5f );
		   this->thumb_progress =  std::clamp ( this->thumb_progress, 0.f, 1.f );

			this->max_thumb_progress = 1.f - this->thumb_length / windowSize;
			if ( this->max_thumb_progress < 0.f )
				return; /*dont handle unmodifiable scrollbar*/
			this->thumb_progress = std::clamp ( this->thumb_progress, 0.f, this->max_thumb_progress );


			this->target_scroll_progress = maximumContentSize * this->thumb_progress;
			lastUpdateTime = ImGui::GetTime ( );

		

		}
		 
		 if ( this->target_scroll_progress != this->scroll_progress ) {

			 this->scroll_progress =  std::lerp ( this->scroll_progress, this->target_scroll_progress, (float)(ImGui::GetTime() - lastUpdateTime) * 1.5f);
			 update ( );
		 }

		if ( this->modifying_thumb ) {

			float dist = mouse_pos.y - ( this->thumb_length ) / 2.f - this->mins.y;
			dist /= ( this->maxs.y - this->mins.y );

			float old = this->thumb_progress;
			this->max_thumb_progress = 1.f - this->thumb_length / contentSize;
			if ( this->max_thumb_progress < 0.f )
				return; /*dont handle unmodifiable scrollbar*/

			this->thumb_progress = dist;

			this->thumb_progress = std::lerp ( old, this->thumb_progress, ImGui::GetIO ( ).DeltaTime * 13.5f );
			this->thumb_progress = std::clamp ( this->thumb_progress, 0.f, 1.f );

			this->max_thumb_progress = 1.f - this->thumb_length / windowSize;
			if ( this->max_thumb_progress < 0.f )
				return; /*dont handle unmodifiable scrollbar*/
			this->thumb_progress = std::clamp ( this->thumb_progress, 0.f, this->max_thumb_progress );


			this->scroll_progress = maximumContentSize * this->thumb_progress;

			update ( );
		}
		else {
			maximumContentSize = 18.f * this->children.size ( );
		}

		draw_scrollbar ( );
	}

	void multibox::draw_scrollbar ( ) {
		this->renderer->AddRectFilledMultiColor ( ImVec2 ( this->mins.x, this->mins.y ), ImVec2 ( this->maxs.x, this->mins.y + 25.f ), ImColor ( 0, 0, 0, 25 ), ImColor ( 0, 0, 0, 25 ), ImColor ( 0, 0, 0, 15 ), ImColor ( 0, 0, 0, 15 ) );

		this->renderer->PushClipRect ( this->thumb_path_mins, this->thumb_path_maxs );
		//this->renderer->AddRectFilled ( this->thumb_path_mins, this->thumb_path_maxs, ImColor ( 255, 0, 0, 125 ) );
		this->renderer->AddRectFilled ( this->thumb_mins, this->thumb_maxs, ImColor ( 34, 46, 80, 255 ), 3.f );
		this->renderer->PopClipRect ( );
		//this->renderer->AddRect ( this->bb_min, this->bb_max, ImColor ( 255, 255, 255, 15 ), 3.5f );


		this->renderer->AddText ( ui::font_widgets, 14.f, ImVec2 ( this->maxs.x + 14, this->mins.y + 14 ), ImColor ( 255, 255, 255, 225 ), std::to_string ( this->thumb_progress ).c_str ( ) );

	}

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
	
	
		this->value_array = &_value_array;
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


		for ( size_t i = 0; i < _items.size ( ); i++ )
			new combo_item ( _items.at ( i ), static_cast< object * >( this ), this->value_array->at ( i ) );

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

	double easeOutCubic ( double t ) {
		return 1 + ( --t ) * t * t;
	}
	void multibox::draw ( ) {

		this->handle ( );


		//this->renderer->AddRectFilled ( this->mins, this->maxs, ImColor ( 255, 0, 255, 255 ) );
		ImVec2 middle = ImVec2 ( ( this->mins.x + this->maxs.x ) / 2.f, ( this->maxs.y + this->mins.y ) / 2.f );
		middle.y -= ImGui::CalcTextSize ( this->title.c_str ( ), 13.f, ui::font_widgets ).y / 2.f;


	

		if ( this->in_animation ) {
		
			 animation_progress = std::clamp ( ( float ) ( ImGui::GetTime ( ) - click_time ) * 2.3f, 0.f, 1.f );

			if ( animation_progress >= 1.f )
				this->in_animation = false;

			if ( this->bb_max.y != this->target_bb_y ) {
			
				if (this->opened )
					this->bb_max.y = this->bb_min.y + std::lerp ( 0.f, this->bb_max_calc, ( float ) easeOutCubic ( animation_progress ) );
				else
					this->bb_max.y = this->bb_min.y + std::lerp ( 19.0833333333f, this->bb_max_calc, 1.f - ( float ) easeOutCubic ( animation_progress ) );
			}

		
		}

		this->renderer->PushClipRect ( this->bb_min, ImVec2 ( this->bb_max.x, this->bb_max.y + 2 ) );
		float progress_alpha = ImGui::GetTime ( ) - hover_start_time;  progress_alpha = std::clamp ( progress_alpha * 3.5f, 0.f, 1.f );


		int alpha;

		if ( this->hovering ) {
			alpha = ( int ) std::lerp ( 20.f, 35.f, progress_alpha );

		}
		else {
			alpha = ( int ) std::lerp ( 20.f, 35.f, 1.f - progress_alpha );

		}

		this->renderer->AddRectFilled ( this->bb_min, this->bb_max, ImColor ( 27, 28, 31, ( int ) std::lerp ( 50, 160, ( float ) easeOutCubic ( animation_progress ) ) ), 3.5f );
		auto prog = ( int ) ( !this->opened ? alpha : std::lerp ( 35.f, 70.f, ( float ) easeOutCubic ( animation_progress ) ) );

		this->renderer->AddRect ( this->bb_min, this->bb_max, ImColor ( 255, 255, 255, prog ), 4.5f );

	
			//this->renderer->PushClipRect ( this->bb_min, this->bb_max );
			bool first = false;
			for ( size_t i = 0; i < children.size ( ); i++ ) {
				auto & child = children.at ( i );

				if ( child->type == keybind_element )
					continue;

				if ( i == 0 && !this->opened ) 					{
				//	child->draw ( );
				}
				else
					child->draw ( );
			}
		
			this->renderer->PopClipRect ( );
		
			if ( !this->opened ) {
				auto mouse_pos = ui::get_cursor ( );

				bool hovering_bb = ( mouse_pos.x > this->bb_min.x && mouse_pos.y > this->bb_min.y && mouse_pos.x < this->bb_max.x && mouse_pos.y < this->bb_max.y );
				if ( hovering_bb ) {
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
					this->renderer->AddText ( ui::font_widgets, 13.f, ImVec2 ( this->bb_min.x + 5.f, middle.y ), ImColor ( 255, 255, 255, 215 ), this->preview.c_str ( ) );
				}
			}
			
		

		if ( !this->children.empty ( ) && this->children.back ( )->type == keybind_element )
			this->children.back ( )->draw ( );

		if ( !this->opened ) {
			this->renderer->AddConvexPolyFilled ( triangle_buffer.data ( ), triangle_buffer.size ( ), ImColor ( 255, 255, 255, 255 ) );
			return;
		}

		if ( this->animation_progress >= 1.f ) {
			for ( auto & child : this->children )
				child->handle ( );
		}

		handle_scroll ( );
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

		this->hovering = ( mouse_pos.x > this->bb_min.x && mouse_pos.y > this->bb_min.y && mouse_pos.x < this->bb_max.x && mouse_pos.y < original_bb_max_y );
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

		if ( !this->can_focus ( ) && !this->is_focused() )
			return;

		auto mouse_pos = ui::get_cursor ( );

		this->hovering_element = ( mouse_pos.x > this->mins.x && mouse_pos.y > this->mins.y && mouse_pos.x < this->maxs.x && mouse_pos.y < this->original_bb_max_y );

		if ( this->hovering_element && key_released ( VK_RBUTTON ) ) {
			this->key_bind_open = !this->key_bind_open;

		}
		
		


		handle_mouse_input ( );

		if ( this->was_hovering != this->hovering )
			this->hover_start_time = ImGui::GetTime ( );

		bool hovering_bb =  ( mouse_pos.x > this->bb_min.x && mouse_pos.y > this->bb_min.y && mouse_pos.x < this->bb_max.x && mouse_pos.y < this->original_bb_max_y );

		if ( this->opened )
			hovering_bb = !( ( mouse_pos.x > this->bb_min.x && mouse_pos.y > this->bb_min.y && mouse_pos.x < this->bb_max.x && mouse_pos.y < this->bb_max.y ) );

		if (  key_pressed ( VK_LBUTTON ) && hovering_bb ) {
		
			
			if ( !this->hovering_thumb  ) {
				
				this->opened = !this->opened; 

				this->scroll_progress = this->target_scroll_progress = thumb_progress = 0.f;

				for ( auto & child : children ) child->update ( );

				if ( this->opened )
					this->target_bb_y = this->bb_max_calc;
				else
					this->target_bb_y = 53.f;

				click_time = ImGui::GetTime ( );

				this->in_animation = true;
				
				if ( this->opened )
					this->focus_it ( );
				else
					this->out_of_focus ( );

			}

		}
	}

	void multibox::update ( ) {



		if ( this->index > 0 ) {
			if ( this->parrent->type == tab_element ) {
				this->bb_width = 140;

				this->mins = ImVec2 ( this->parrent->parrent->maxs.x - this->bb_width - 20, this->parrent->parrent->mins.y + 60 );
				this->maxs = ImVec2 ( this->parrent->parrent->maxs.x, this->mins.y + original_bb_height );


				this->bb_min = ImVec2 ( this->maxs.x - this->bb_width, this->mins.y + 3 );
				this->bb_max = ImVec2 ( this->maxs.x + 4.5f, this->maxs.y );

				this->original_bb_max_y = this->bb_max.y;
			}
			else {
				auto & back = this->parrent->children.at ( this->index - 1 );
				this->mins = ImVec2 ( this->parrent->mins.x + this->parrent->padding, back->maxs.y + 8 );
				this->maxs = ImVec2 ( this->parrent->maxs.x - this->parrent->padding, this->mins.y + 20 );

				this->bb_max = ImVec2 ( this->maxs.x + 4.5f, this->maxs.y );
				this->bb_min = ImVec2 ( this->maxs.x - bb_width - this->parrent->padding, this->mins.y );


				this->original_bb_max_y = this->bb_max.y;
			}
		}
		else {


			this->mins = ImVec2 ( this->parrent->mins.x + this->parrent->padding, this->parrent->mins.y + 40 );
			this->maxs = ImVec2 ( this->parrent->maxs.x - this->parrent->padding, this->mins.y + 22.9f );


			this->bb_min = ImVec2 ( this->maxs.x - this->bb_width, this->mins.y + 3 );
			this->bb_max = ImVec2 ( this->maxs.x + 4.5f, this->maxs.y );

			this->original_bb_max_y = this->bb_max.y;
		}

		should_be_scrollable = children.size ( ) >= 6;

		if ( !should_be_scrollable )
			this->bb_max_calc = ( this->children.size ( ) - 1 ) * 18.f;
		else
			this->bb_max_calc = 120.f;

		if ( this->opened )
			this->bb_max.y = this->bb_min.y + std::lerp ( 0.f, this->bb_max_calc, ( float ) easeOutCubic ( animation_progress ) );
		else
			this->bb_max.y = this->bb_min.y +  std::lerp ( 19.0833333333f, this->bb_max_calc, 0.f );

		this->update_triangle ( );


		for ( auto & children : this->children )
			children->update ( );

		this->refresh_preview ( );
		this->preview_min.x = this->bb_min.x + 5.f;

	}
}

