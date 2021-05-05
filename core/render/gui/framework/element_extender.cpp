#include "../includes.h"


namespace ui {




	element_extender::element_extender ( object * parent  ) {
		this->parrent = parent;
		this->renderer = this->parrent->renderer;
		this->type = element_extender_element;
		
		this->padding = 15.f;

		update ( );

		this->parrent->add_children ( this );

		update ( );
	}

	void element_extender::draw ( ) {

	
		auto text_size = ImGui::CalcTextSize ( ICON_SETTINGS, 18.f, ui::font_icons );

		this->renderer->AddText ( ui::font_icons, 18.f, ImVec2 ( ( this->icon_mins.x + this->icon_maxs.x ) / 2.f - text_size.x / 2.f, ( ( this->icon_mins.y + this->icon_maxs.y ) / 2.f ) - text_size.y / 2.f ), ImColor(255, 255, 255 , 245), ICON_SETTINGS );


		this->handle ( );


		//this->renderer->AddRectFilled ( this->icon_mins, this->icon_maxs, ImColor ( 255, 28, 31, 255 ), 3.5f );

		if ( !this->open )
			return;

		renderer->PushClipRect ( this->mins, this->maxs, false );
		//this->renderer->AddRect ( ImVec2( this->mins.x + 1, this->mins.y + 1), ImVec2 ( this->maxs.x + 2, this->maxs.y + 2 ), ImColor ( 255, 255, 255, 25 ), 3.5f );
		this->renderer->AddRectFilled ( this->mins, this->maxs, ImColor ( 27, 28, 31,  255 ), 3.5f );
		this->renderer->AddRect ( this->mins, this->maxs, ImColor ( 255, 255, 255, 25 ), 3.5f );


		this->renderer->AddText ( ui::font_widgets, 14.f, ImVec2 ( this->mins.x + 14, this->mins.y + 14 ), ImColor ( 0, 0, 0, 225 ), "Advanced settings" );
		this->renderer->AddText ( ui::font_widgets, 14.f, ImVec2 ( this->mins.x + 13, this->mins.y + 13 ), ImColor ( 255, 255, 255, 225 ), "Advanced settings" );

	

		for ( size_t i = this->children.size ( ) - 1; i != ( size_t ) -1; i-- )
			this->children.at ( i )->draw ( );

		renderer->PopClipRect ( );

	}

	void element_extender::handle_mouse_input ( ) {

		auto mouse_pos = ui::get_cursor ( );
		this->hovering = ( mouse_pos.x > this->icon_mins.x && mouse_pos.y > this->icon_mins.y && mouse_pos.x < this->icon_maxs.x && mouse_pos.y < this->icon_maxs.y );
	}





	void element_extender::handle ( ) {

		if ( ui::focused_item != -1 && ui::focused_item != this->_id )
			return;

		if ( this->in_animation ) {

			if ( this->maxs.y >= this->target_max_y )
				this->maxs.y -= ( 1000.0 / ( double ) ImGui::GetIO ( ).Framerate ) / 3.f;
			else if ( this->maxs.y <= this->target_max_y ) 
				this->maxs.y += ( 1000.0 / ( double ) ImGui::GetIO ( ).Framerate ) / 3.f;


				if ( std::fabs ( this->maxs.y - this->target_max_y ) <= 3.f ) {
					this->maxs.y = this->target_max_y;

					this->in_animation = false;

				}
			
			return;
		}
	



		handle_mouse_input ( );

		if ( this->hovering && key_pressed ( VK_LBUTTON ) ) {
			auto time = ImGui::GetIO ( ).DeltaTime;

			if ( time == this->last_open )
				return;

			this->last_open = time;

			
			this->open = !this->open;

			if ( this->open ) {
				this->maxs.y = this->mins.y;
				this->in_animation = true;
			}

			if ( this->open ) {
				ui::focused_item = this->_id;
			}
			else
				ui::focused_item = -1;

		}
		else if ( !this->hovering && key_pressed ( VK_LBUTTON ) ) {
		//	this->open = false;
			//this->maxs.y = this->mins.y;
			///out_of_focus ( );
		}

	}


	void element_extender::update_isolate ( ) {
		for ( auto & child : this->children )
			child->update ( );

		if ( !this->children.empty ( ) ) {
			this->target_max_y = this->children.back ( )->maxs.y + 10.f;
			this->in_animation = true;
		}
	}

	void element_extender::update ( ) {

		if ( this->parrent->type != slider_element && this->index > 0 && this->parrent->children.size() && this->parrent->children.at ( this->index - 1 )->type != keybind_element ) {
			this->parrent->children.at ( this->index - 1 )->update ( );
			this->icon_maxs.x = this->parrent->children.at(this->index - 1)->mins.x - 4;
		}
		else {
		
			if ( this->parrent->type == slider_element ) {
				auto parent = static_cast< ui::slider * >( this->parrent );
				float mata = parent->bb_max.x + 30;

				this->icon_maxs.x = parent->bb_min.x;
			}
			else {
				this->icon_maxs.x = this->parrent->maxs.x - 33;
			}
		}

		this->icon_mins.x = this->icon_maxs.x - 25;

		this->icon_mins.y = ( this->parrent->mins.y + this->parrent->maxs.y ) / 2.f - 8.f;
		this->icon_maxs.y = ( this->parrent->mins.y + this->parrent->maxs.y ) / 2.f + 8.f;
	

		
		this->mins.x = (this->icon_mins.x + this->icon_maxs.x) / 2.f;
		this->maxs.x = this->mins.x + this->width;

		this->mins.y = this->parrent->maxs.y;

		
		for ( auto & child : this->children )
			child->update ( );

		if ( !this->children.empty ( ) ) {
			this->target_max_y = this->children.back ( )->maxs.y + 10.f;
			if (this->open )
				this->maxs.y = this->target_max_y;
		}



	}
}