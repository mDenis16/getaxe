

#include <Windows.h>
#include <iostream>
#include <cmath>

#include "../includes.h"

namespace ui {

	double easeOutCubicc ( double t ) {
		return 1 + ( --t ) * t * t;
	}float degrees_to_radians ( const float deg ) {
		return deg * ( 3.14f / 180.f );
	}

	void rotate_triangle ( std::vector<ImVec2> & points, float rotation ) {
		ImVec2 points_center;
		points_center.x = ( points.at ( 0 ).x + points.at ( 1 ).x + points.at ( 2 ).x ) / 3.f;
		points_center.y = ( points.at ( 0 ).y + points.at ( 1 ).y + points.at ( 2 ).y ) / 3.f;

		for ( auto & point : points ) {
			point.x -= points_center.x;
			point.y -= points_center.y;

			const auto temp_x = point.x;
			const auto temp_y = point.y;

			const auto theta = degrees_to_radians ( rotation );
			const auto c = cosf ( theta );
			const auto s = sinf ( theta );

			point.x = temp_x * c - temp_y * s;
			point.y = temp_x * s + temp_y * c;

			point.x += points_center.x;
			point.y += points_center.y;
		}
	}
	void combobox::handle_scroll ( ) {
		if ( !this->opened )return;
		if ( !should_be_scrollable ) return;

		float windowSize = 120;

		float maxLimitSize =  this->children.size ( )  * 18.f;

		float contentSize =  this->children.size ( )  * 18.f;
		float trackSize = windowSize / 3; //80 unknown units

		float windowContentRatio = windowSize / contentSize;
		float gripSize = trackSize * windowContentRatio;


		float percentage = windowSize / contentSize;
		this->thumb_length = windowSize * percentage;


		this->thumb_path_mins = ImVec2 ( this->bb_max.x - 7, this->bb_min.y );
		this->thumb_path_maxs = ImVec2 ( this->bb_max.x - 2, this->bb_max.y );




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

		if ( ImGui::GetIO ( ).MouseWheel != 0.f ) {
			float old = this->thumb_progress;

			if ( ImGui::GetIO ( ).MouseWheel > 0.f )
				this->thumb_progress -= 0.1f;
			else if ( ImGui::GetIO ( ).MouseWheel < 0.f )
				this->thumb_progress += 0.1f;


			this->thumb_progress = std::lerp ( old, this->thumb_progress, ImGui::GetIO ( ).DeltaTime * 13.5f );
			this->thumb_progress = std::clamp ( this->thumb_progress, 0.f, 1.f );

			this->max_thumb_progress = 1.f - this->thumb_length / windowSize;
			if ( this->max_thumb_progress < 0.f )
				return; /*dont handle unmodifiable scrollbar*/
			this->thumb_progress = std::clamp ( this->thumb_progress, 0.f, this->max_thumb_progress );


			this->target_scroll_progress = maximumContentSize * this->thumb_progress;
			lastUpdateTime = ImGui::GetTime ( );



		}

		if ( this->target_scroll_progress != this->scroll_progress ) {

			this->scroll_progress = std::lerp ( this->scroll_progress, this->target_scroll_progress, ( float ) ( ImGui::GetTime ( ) - lastUpdateTime ) * 1.5f );
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
			maximumContentSize = this->children.size ( )   * 18.f;
		}

		draw_scrollbar ( );
	}

	void combobox::draw_scrollbar ( ) {
		this->renderer->AddRectFilledMultiColor ( ImVec2 ( this->mins.x, this->mins.y ), ImVec2 ( this->maxs.x, this->mins.y + 25.f ), ImColor ( 0, 0, 0, 25 ), ImColor ( 0, 0, 0, 25 ), ImColor ( 0, 0, 0, 15 ), ImColor ( 0, 0, 0, 15 ) );

		this->renderer->PushClipRect ( this->thumb_path_mins, this->thumb_path_maxs );
		//this->renderer->AddRectFilled ( this->thumb_path_mins, this->thumb_path_maxs, ImColor ( 255, 0, 0, 125 ) );
		this->renderer->AddRectFilled ( this->thumb_mins, this->thumb_maxs, ImColor ( 34, 46, 80, 255 ), 3.f );
		this->renderer->PopClipRect ( );
		//this->renderer->AddRect ( this->bb_min, this->bb_max, ImColor ( 255, 255, 255, 15 ), 3.5f );


		//this->renderer->AddText ( ui::font_widgets, 14.f, ImVec2 ( this->maxs.x + 14, this->mins.y + 14 ), ImColor ( 255, 255, 255, 225 ), std::to_string ( this->thumb_progress ).c_str ( ) );

	}


	combobox::combobox ( SecureString& text, object * parent, std::vector<std::string> _items, int & _value, float _bb_width ) {

		this->items = _items;
		this->title = &text;
		this->value = &_value;
	

		this->parrent = parent;
		this->renderer = this->parrent->renderer;
		this->type = combobox_element;

		if (_bb_width> 0.f )
		this->bb_width = _bb_width;
		

		for ( size_t i = 0; i < _items.size ( ); i++ )
			new combo_item ( _items.at ( i ), static_cast< object * >( this ) );


		this->parrent->add_children ( this );
	
		update ( );

	}
	combobox::combobox ( SecureString& text, object * parent, std::vector<std::string> _items, int * _value, float _bb_width ) {

		this->items = _items;
		this->title = &text;
		this->value = _value;


		this->parrent = parent;
		this->renderer = this->parrent->renderer;
		this->type = combobox_element;

		if ( _bb_width > 0.f )
			this->bb_width = _bb_width;


		for ( size_t i = 0; i < _items.size ( ); i++ )
			new combo_item ( _items.at ( i ), static_cast< object * >( this ) );


		this->parrent->add_children ( this );

		update ( );

	}
	combobox::combobox ( SecureString& text, object * parent, std::vector<std::string> _items, int & _value, config_manager::key_bind_item & key_bind_item, float _bb_width )
	{

		this->items = _items;
		this->title = &text;
		this->value = &_value;


		this->parrent = parent;
		this->renderer = this->parrent->renderer;
		this->type = combobox_element;
		this->key_bind_controller = &key_bind_item;
		if ( _bb_width > 0.f )
			this->bb_width = _bb_width;


		for ( size_t i = 0; i < _items.size ( ); i++ )
			new combo_item ( _items.at ( i ), static_cast< object * >( this ) );

	//	new key_bind_muie ( this, this->key_bind_controller );

		this->parrent->add_children ( this );

		update ( );

	}

	combobox::combobox ( SecureString& text, object * parent, std::vector<std::string> _items, int & _value ) {

		this->items = _items;
		this->title = &text;
		this->value = &_value;


		this->parrent = parent;
		this->renderer = this->parrent->renderer;
		this->type = combobox_element;

	
		for ( size_t i = 0; i < _items.size ( ); i++ ) 
			new combo_item ( _items.at ( i ), static_cast< object * >( this ) );
		

		this->parrent->add_children ( this );

		update ( );

	}
	double easeInOutQuint ( double t ) {
		double t2;
		if ( t < 0.5 ) {
			t2 = t * t;
			return 16 * t * t2 * t2;
		}
		else {
			t2 = ( --t ) * t;
			return 1 + 16 * t * t2 * t2;
		}
	}

	void triangle ( ImVec2 pos, ImDrawList* render, ImColor color, float progress, bool b ) {

		
		auto left = ImVec2 ( pos.x - 5.f, pos.y );
		auto right = ImVec2 ( pos.x + 5.f, pos.y );

		auto bottom = ImVec2 ( pos.x, pos.y + 7.5f );


		std::vector<ImVec2> points; points.reserve ( 3 );
		points.push_back ( left );
		points.push_back ( right );
		points.push_back ( bottom );
		rotate_triangle ( points, ( float ) std::lerp ( 0.f, 90.f, ( float ) easeInOutQuint ( progress ) ) );
		render->AddConvexPolyFilled ( points.data(), points.size ( ), color );
		

	}
	void combobox::set_value ( int index ) {
		if ( this->key_bind_controller ) {
			if ( !this->key_bind_controller->old_value )
				this->key_bind_controller->old_value = malloc ( sizeof ( this->value ) );

			*( int * ) this->key_bind_controller->old_value = index;
		
		}
		*(int*)this->value = index;

	}

	void combobox::draw_scroll_thumb ( ) {
		if ( !this->opened )
			return;


		this->renderer->AddRectFilled ( this->thumb_min, this->thumb_max, ImColor ( 34, 46, 80, 255 ), 3.f );

	}
	void combobox::draw ( ) {

		

		auto cur_text = this->items.at ( *this->value ).c_str ( );

		this->handle ( );


		//this->renderer->AddRectFilled ( this->mins, this->maxs, ImColor ( 255, 0, 255, 255 ) );
		ImVec2 middle = ImVec2 ( ( this->bb_min.x + this->bb_max.x ) / 2.f, (  this->bb_min.y * 2.f + 18.f ) / 2.f );
		middle.y -= ImGui::CalcTextSize ( cur_text, 13.f, ui::font_widgets ).y / 2.f;



		

		if ( this->in_animation ) {

			animation_progress = std::clamp ( ( float ) ( ImGui::GetTime ( ) - click_time ) * 2.3f, 0.f, 1.f );

			if ( animation_progress >= 1.f )
				this->in_animation = false;

			if ( this->bb_max.y != this->target_bb_y ) {

				if ( this->opened )
					this->bb_max.y = this->bb_min.y + std::lerp ( 18.f, this->bb_max_calc, ( float ) easeOutCubicc ( animation_progress ) );
				else
					this->bb_max.y = this->bb_min.y + std::lerp ( 18.f, this->bb_max_calc, 1.f - ( float ) easeOutCubicc ( animation_progress ) );
			}


		}

		float progress_alpha = ImGui::GetTime ( ) - hover_start_time;  progress_alpha = std::clamp ( progress_alpha * 3.5f, 0.f, 1.f );


		int alpha;

		if ( this->hovering ) {
			alpha = ( int ) std::lerp ( 20.f, 35.f, progress_alpha );

		}
		else {
			alpha = ( int ) std::lerp ( 20.f, 35.f, 1.f - progress_alpha );

		}

		this->renderer->AddRectFilled ( this->bb_min, this->bb_max, ImColor ( 27, 28, 31, (int)std::lerp(50, 160, (float )easeOutCubicc ( animation_progress )) ), 3.5f );
		auto prog = ( int ) ( !this->opened ? alpha : std::lerp ( 35.f, 70.f, (float)easeOutCubicc(animation_progress)) );

		this->renderer->AddRect ( this->bb_min, this->bb_max, ImColor ( 255, 255, 255, prog ), 4.5f );


		
		bool first = false;

		this->renderer->PushClipRect ( ImVec2 ( this->bb_min.x, this->bb_min.y + 2 ), ImVec2 ( this->bb_max.x, this->bb_max.y - 2 ) );
		for ( size_t i = 0; i < children.size ( ); i++ ) {

			auto & child = children.at ( i );

			if ( child->type == keybind_element )
				continue;

			if ( i == 0 ) {
				if (this->opened )
				   child->draw ( );
				else
					this->renderer->AddText ( ui::font_widgets, 13.f, ImVec2 ( this->bb_min.x + 5.f, middle.y ), ImColor ( 255, 255, 255, 215 ), cur_text );
			}
			else
				child->draw ( );

		}
		this->renderer->PopClipRect ( );


	


		this->renderer->AddText ( ui::font_widgets, 13.f, ImVec2 ( this->mins.x, middle.y ), ImColor ( 255, 255, 255, 225 ), this->title->data ( ) );



		if ( !this->children.empty ( ) && this->children.back ( )->type == keybind_element )
			this->children.back ( )->draw ( );

		/*if ( !this->opened ) {
			this->renderer->AddConvexPolyFilled ( triangle_buffer.data ( ), triangle_buffer.size ( ), ImColor ( 255, 255, 255, 255 ) );
			return;
		}*/

		auto pos = ImVec2 ( this->bb_max.x - 10.f, ( this->bb_min.y * 2.f + 18.f ) / 2.f - 7.5f / 2.f);
		

		triangle ( pos, renderer, ImColor(255,255,255,255), this->opened ?  animation_progress : 1.f - animation_progress, opened  );


		//this->renderer->AddRect ( this->mins, ImVec2 ( this->maxs.x + 4.5f, this->maxs.y ), ImColor ( 255, 0, 0, 255 ) );

		handle_scroll ( );
		
	}
	void combobox::update_triangle ( ) {

		auto pos = ImVec2 ( this->bb_max.x - 10.f, this->bb_min.y + 5.1f );
		auto left = ImVec2 ( pos.x - 5.f, pos.y );
		auto right = ImVec2 ( pos.x + 5.f, pos.y );

		auto bottom = ImVec2 ( pos.x, pos.y + 7.f );


		triangle_buffer.clear ( );
		triangle_buffer.reserve ( 3 );

		triangle_buffer.push_back ( left );
		triangle_buffer.push_back ( right );
		triangle_buffer.push_back ( bottom );



	}
	void combobox::handle_mouse_input ( ) {
	
	

		if ( in_animation )
			return;


		auto mouse_pos = ui::get_cursor ( );
	
		this->hovering = ( mouse_pos.x > this->bb_min.x && mouse_pos.y > this->bb_min.y && mouse_pos.x < this->bb_max.x && mouse_pos.y < this->bb_max.y );


	}

	void combobox::handle ( ) {
		if ( !this->old_value ) {
			this->old_value = std::malloc ( sizeof ( this->value ) );
			std::memcpy ( this->old_value, this->value, sizeof ( this->value ) );
		}

		auto mouse_pos = ui::get_cursor ( );

		this->hovering_element = ( mouse_pos.x > this->mins.x && mouse_pos.y > this->mins.y && mouse_pos.x < this->maxs.x && mouse_pos.y < this->original_bb_max_y );

		if ( this->hovering_element && key_released ( VK_RBUTTON ) ) {
			this->key_bind_open = !this->key_bind_open;

		}

		if ( !this->can_focus ( ) && !this->is_focused())
			return;


		handle_mouse_input ( );

		if ( this->was_hovering != this->hovering )
			this->hover_start_time = ImGui::GetTime ( );


		this->was_hovering = this->hovering;

		bool hovering_bb = ( mouse_pos.x > this->bb_min.x && mouse_pos.y > this->bb_min.y && mouse_pos.x < this->bb_max.x && mouse_pos.y < this->original_bb_max_y );

		if ( this->opened )
			hovering_bb = true;

		if ( this->opened && !this->in_animation ) {
			for ( auto & child : this->children )
				if ( child->mins.y > this->bb_min.y - 20 && child->maxs.y < this->bb_max.y + 20 )
					child->handle ( );
		}

		if ( hovering_bb && key_pressed ( VK_LBUTTON )  ) {


			if ( !this->hovering_thumb ) {

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
	void combobox::update ( ) {

	

		if ( this->index > 0 ) {
			if ( this->parrent->type == tab_element ) {
				this->bb_width = 140;

				this->mins = ImVec2 ( this->parrent->parrent->maxs.x - this->bb_width - 20, this->parrent->parrent->mins.y + 60 );
				this->maxs = ImVec2 ( this->parrent->parrent->maxs.x, this->mins.y + 22.9f );


				this->bb_min = ImVec2 ( this->maxs.x - this->bb_width, this->mins.y  );
				this->bb_max = ImVec2 ( this->maxs.x, this->maxs.y );

				this->original_bb_max_y = this->bb_max.y;
			}
			else {
				auto & back = this->parrent->children.at ( this->index - 1 );
				this->mins = ImVec2 ( this->parrent->mins.x + this->parrent->padding, back->maxs.y + 8 );
				this->maxs = ImVec2 ( this->parrent->maxs.x - this->parrent->padding, this->mins.y + 20 );

				this->bb_max = ImVec2 ( this->maxs.x + 4.5f, this->maxs.y );
				this->bb_min = ImVec2 ( this->maxs.x - bb_width - this->parrent->padding , this->mins.y );


				this->original_bb_max_y = this->bb_max.y;
			}
		}
		else {
			bool is_parent_panel = this->parrent->type == panel_element || this->parrent->type == panel_cotainer_element;
			if ( is_parent_panel ) {

				/*if ( this->parrent->parrent && this->parrent->parrent->children.size() > 0 ) {
					auto previous_element = this->parrent->parrent->children.at ( this->parrent->index  - 1);

					this->mins = ImVec2 ( this->parrent->mins.x + this->parrent->padding, previous_element->maxs.y + 8 );
				}
				else {
					this->mins = ImVec2 ( this->parrent->mins.x + this->parrent->padding, this->parrent->mins.y + 8);
				}*/

				this->mins = ImVec2 ( this->parrent->mins.x + this->parrent->padding, this->parrent->mins.y );
				this->maxs = ImVec2 ( this->parrent->maxs.x - this->parrent->padding, this->mins.y + 22.9f );



				this->bb_min = ImVec2 ( this->maxs.x - this->bb_width, this->mins.y );
				this->bb_max = ImVec2 ( this->maxs.x, this->maxs.y );

				this->original_bb_max_y = this->bb_max.y;
			}
			else {


				this->mins = ImVec2(this->parrent->mins.x + this->parrent->padding, this->parrent->mins.y + 40);
				this->maxs = ImVec2(this->parrent->maxs.x - this->parrent->padding, this->mins.y + 20);


				this->bb_max = ImVec2(this->maxs.x, this->maxs.y);
				this->bb_min = ImVec2(this->bb_max.x - this->bb_width, this->mins.y);


				this->original_bb_max_y = this->bb_max.y;
			}
		}

		should_be_scrollable = children.size ( ) >= 6;

		
		    this->bb_max_calc = std::clamp( this->children.size ( ) * 18.f, 18.f, 120.f );

			if ( this->opened )
				this->bb_max.y = this->bb_min.y + std::lerp ( 0.f, this->bb_max_calc, ( float ) easeOutCubicc ( animation_progress ) );
			else
				this->bb_max.y = this->bb_min.y + 18.f;

		this->update_triangle ( );


		for ( auto & children : this->children )
			children->update ( );
		
	
	}
}