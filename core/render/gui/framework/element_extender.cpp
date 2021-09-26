#include "../includes.h"


namespace ui {




	element_extender::element_extender ( object * parent ) {
		this->parrent = parent;
		this->renderer = this->parrent->renderer;
		this->type = element_extender_element;

		this->padding = 15.f;
		this->flags = flags::hide_overflow;
		update ( );

		this->parrent->add_children ( this );

		update ( );
	}

	double easeOutCubicSS ( double t ) {
		return 1 + ( --t ) * t * t;
	}

	void element_extender::draw ( ) {


		auto text_size = ImGui::CalcTextSize ( ICON_SETTINGS, 18.f, ui::font_icons );

		


		this->handle ( );


		//this->renderer->AddRectFilled ( this->icon_mins, this->icon_maxs, ImColor ( 255, 28, 31, 255 ), 3.5f );
		float progress_height = ImGui::GetTime ( ) - state_change_time;  progress_height = std::clamp ( progress_height * 3.5f, 0.f, 1.f );
		if ( this->open ) {
			
			max_alpha = std::lerp ( 0, 255, easeOutCubicSS (progress_height) );
			this->maxs.y = this->target_max_y;
		}
		else {
		
			this->maxs.y = 0;
			max_alpha = std::lerp ( 0, 255, easeOutCubicSS (1.f -   progress_height) );
		}
		this->renderer->AddText ( ui::font_icons, 18.f, ImVec2 ( ( this->icon_mins.x + this->icon_maxs.x ) / 2.f - text_size.x / 2.f, ( ( this->icon_mins.y + this->icon_maxs.y ) / 2.f ) - text_size.y / 2.f ), ImColor ( 255, 255, 255, 225 ), ICON_SETTINGS );

		//renderer->PushClipRect ( this->mins, this->maxs, false );
		//this->renderer->AddRect ( ImVec2( this->mins.x + 1, this->mins.y + 1), ImVec2 ( this->maxs.x + 2, this->maxs.y + 2 ), ImColor ( 255, 255, 255, 25 ), 3.5f );

		if ( ( this->maxs.y - this->mins.y ) > 3 ) {
			this->renderer->AddRectFilled ( this->mins, this->maxs, ImColor ( 0, 0, 0, ( int ) std::lerp ( 50, 220, ( float ) easeOutCubicSS ( progress_height ) ) ), 3.5f );
			this->renderer->AddRectFilled ( this->mins, this->maxs, ImColor ( 27, 28, 31, ( int ) std::lerp ( 50, 160, ( float ) easeOutCubicSS ( progress_height ) ) ), 3.5f );
	
			this->renderer->AddRect ( this->mins, this->maxs, ImColor ( 255, 255, 255, (int)std::lerp ( 35.f, 70.f, ( float ) easeOutCubicSS ( progress_height ) )), 3.5f );

			if ( this->maxs.y > this->mins.y + 14 ) {
				this->renderer->AddText ( ui::font_widgets, 14.f, ImVec2 ( this->mins.x + 14, this->mins.y + 14 ), ImColor ( 0, 0, 0, max_alpha ), "Advanced settings" );
				this->renderer->AddText ( ui::font_widgets, 14.f, ImVec2 ( this->mins.x + 13, this->mins.y + 13 ), ImColor ( 255, 255, 255, max_alpha ), "Advanced settings" );
			}
			//ImVec2 ( this->mins.x + 14, this->mins.y + 14 )

			/*
			
			new ui::combobox ( "Type", name_box_extender, std::vector<std::string>{"rasterize", "native"}, & cfg->name_render_type, 80.f );

					auto panel_container = new ui::panel_container ( name_box_extender, cfg->name_render_type );
					{
						auto panel_rasterize = new ui::panel ( panel_container );
			*/

			for ( size_t i = this->children.size ( ) - 1; i != ( size_t ) -1; i-- ) {
				auto & child = this->children.at ( i );

				if ( child->mins.y > this->mins.y && child->maxs.y < this->maxs.y ) {

					if ( child->type == panel_cotainer_element ) {
					
						for ( auto & p : child->children ) {
							if ( p->type == panel_element ) {

								for ( auto & d : p->children )
								{
									d->max_alpha = max_alpha;
								}
							}
						}
					}
					else {

						child->max_alpha = max_alpha;

					}

					child->draw ( );
				}

			}
		}
		//renderer->PopClipRect ( );

	}

	void element_extender::handle_mouse_input ( ) {

		auto mouse_pos = ui::get_cursor ( );
		this->hovering = ( mouse_pos.x > this->icon_mins.x && mouse_pos.y > this->icon_mins.y && mouse_pos.x < this->icon_maxs.x && mouse_pos.y < this->icon_maxs.y );
	}





	void element_extender::handle ( ) {


		
		if ( !this->can_focus ( ) && !this->is_focused ( ) )
			return;


		handle_mouse_input ( );

		if ( !this->open && this->hovering && key_pressed ( VK_LBUTTON ) ) {


			this->open = true;

				this->focus_it ( );
		

			this->state_change_time = ImGui::GetTime ( );

		}else if (this->open && key_pressed(VK_LBUTTON))
		{
			auto mouse_pos = ui::get_cursor();
			hovering_extender = (mouse_pos.x > this->mins.x && mouse_pos.y > this->mins.y && mouse_pos.x < this->maxs.x&& mouse_pos.y < this->maxs.y);

			if (!hovering_extender)
			{
				this->open = false;
				this->out_of_focus();
			}
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

		if ( this->parrent->type != slider_element && this->index > 0 && this->parrent->children.size ( ) && this->parrent->children.at ( this->index - 1 )->type != keybind_element ) {
			this->parrent->children.at ( this->index - 1 )->update ( );
			this->icon_maxs.x = this->parrent->children.at ( this->index - 1 )->mins.x - 5;
		}
		else {

			if ( this->parrent->type == slider_element ) {
				auto parent = static_cast< ui::slider * >( this->parrent );
				float mata = parent->bb_max.x + 30;

				this->icon_maxs.x = parent->bb_min.x;
			}
			else {
				this->icon_maxs.x = this->parrent->maxs.x - 39;
			}
		}

		this->icon_mins.x = this->icon_maxs.x - 25;

		this->icon_mins.y = ( this->parrent->mins.y + this->parrent->maxs.y ) / 2.f - 8.f;
		this->icon_maxs.y = ( this->parrent->mins.y + this->parrent->maxs.y ) / 2.f + 8.f;



		this->mins.x = ( this->icon_mins.x + this->icon_maxs.x ) / 2.f;
		this->maxs.x = this->mins.x + this->width;

		this->mins.y = this->parrent->maxs.y;


		for ( auto & child : this->children )
			child->update ( );

		if ( !this->children.empty ( ) ) {
			this->target_max_y = this->children.back ( )->maxs.y + 10.f;
			//if (this->open )
				//this->maxs.y = this->target_max_y;
		}
	}
}