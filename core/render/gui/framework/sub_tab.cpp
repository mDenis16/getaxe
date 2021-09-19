
#include "../includes.h"
#include <queue>

namespace ui {
	void sub_tab::draw ( ) {
		if (!this->text) return;

		handle ( );


		auto text_size = ImGui::CalcTextSize ( this->icon_text.data(), this->icon_size, this->icon_font );
		auto text_size_name = ImGui::CalcTextSize ( this->text->data ( ), 13.f, ui::font_widgets );
		
		
		FLOAT center_Y = ( this->mins.y + this->maxs.y ) / 2.f;
		FLOAT center_X = ( this->mins.x + this->maxs.x ) / 2.f;

		ImVec2  text_mins = ImVec2 ( center_X - text_size.x / 2.f, center_Y - text_size.y / 2.f );
		ImVec2 text_maxs = ImVec2 ( center_X + text_size.x / 2.f, center_Y + text_size.y / 2.f + text_size_name.y );

		float middle_y = ( text_mins.y + text_maxs.y ) / 2.f;

		
		float progress = ImGui::GetTime ( ) - hover_start_time;  progress = std::clamp ( progress * 3.5f, 0.f, 1.f );


		int alpha = 0;
		if ( this->selected )alpha = 255;
		else {
			if ( this->hovering )
				alpha = ( int ) std::lerp ( 155.f, 255.f, progress );
			else
				alpha = ( int ) std::lerp ( 155.f, 255.f, 1.f - progress );
		}

		if (this->selected )
		this->renderer->AddText ( this->icon_font, this->icon_size, ImVec2 ( center_X - text_size.x / 2.f + 2.f, middle_y - text_size.y - 1.175f + offset_y + 2.f), ImColor ( 0, 0, 0, alpha ), this->icon_text.data ( ) );

		this->renderer->AddText ( this->icon_font, this->icon_size, ImVec2 ( center_X - text_size.x / 2.f, middle_y - text_size.y - 1.175f + offset_y) , ImColor ( 255, 255, 255, alpha ), this->icon_text.data() );
		if ( this->selected )
		this->renderer->AddText ( ui::font_widgets, 13.f, ImVec2 ( ( this->mins.x + this->maxs.x ) / 2.f - text_size_name.x / 2.f + 2.f, middle_y - text_size.y + text_size.y + 1.175f + 2.f), ImColor ( 0, 0, 0, alpha ), this->text->data ( ) );

		this->renderer->AddText ( ui::font_widgets, 13.f, ImVec2 ( ( this->mins.x + this->maxs.x ) / 2.f - text_size_name.x / 2.f, middle_y - text_size.y + text_size.y + 1.175f  ), ImColor ( 255, 255, 255, alpha ), this->text->data ( ) );
	

		//this->renderer->AddRect (this->mins, this->maxs, ImColor(255, 0,255, 255));

		for ( auto & child : this->children )
			child->draw ( );
	}
	void sub_tab::handle ( ) {
		this->handle_mouse_input ( );
		if ( this->hovering && !this->was_hovering )
			hover_start_time = ImGui::GetTime ( );
		else if ( this->was_hovering && !this->hovering )
			hover_start_time = ImGui::GetTime ( );

		if ( this->hovering && key_down ( VK_LBUTTON ) ) {
			if ( this->selected )
				return;


			bool old_val = this->selected;
			if ( !this->parrent->children.empty ( ) ) {
				for ( object * child : this->parrent->children ) {
					auto sb = static_cast< sub_tab * >( child );
					if ( sb->selected ) {
						sb->selected = false;

						if ( sb->child_assign ) {

							std::queue<ui::object *> q;
							q.push ( sb->child_assign );

							while ( !q.empty ( ) ) {
								int n = q.size ( );


								while ( n > 0 ) {		
									ui::object * p = q.front ( );
									p->should_reset = true;

									q.pop ( );

									for ( int i = 0; i < p->children.size ( ); i++ )
										q.push ( p->children [ i ] );
									n--;
								}
							}
						}
					}
				}
			}
			this->selected = !old_val;

			if ( this->selected && this->child_assign ) {
				this->last_selected = this->last_selected;

				std::queue<ui::object *> q;
				q.push ( this->child_assign );

				while ( !q.empty ( ) ) {
					int n = q.size ( );

					// If this node has children
					while ( n > 0 ) {

						// Dequeue an item from queue and
						// check if it is equal to x
						// If YES, then return number of children
						ui::object * p = q.front ( );
						p->should_reanimate = true;
						p->should_reset = true;

						q.pop ( );


						// Enqueue all children of the dequeued item
						for ( int i = 0; i < p->children.size ( ); i++ )
							q.push ( p->children [ i ] );
						n--;
					}
				}

				this->last_selected = this->selected;
			}
		}
		
		


		this->was_hovering = this->hovering;
		
	}
	void sub_tab::handle_mouse_input ( ) {
		auto mouse_pos = ui::get_cursor ( );
		this->hovering = ( mouse_pos.x > this->mins.x && mouse_pos.y > this->mins.y && mouse_pos.x < this->maxs.x && mouse_pos.y < this->maxs.y );
	}
	void sub_tab::update ( ) {
		auto text_size = ImGui::CalcTextSize ( this->text->data ( ), 15.f, ui::font_widgets );

		if ( this->index > 0 ) {
			auto & back = this->parrent->children.at ( this->index - 1 );

			this->mins.x = back->maxs.x;
			this->mins.y = back->mins.y;

			this->maxs.x = this->mins.x + text_size.x + 35;
			this->maxs.y = back->maxs.y;


		}
		else {
			this->mins.x = this->parrent->parrent->mins.x;
			this->maxs.x = this->mins.x + text_size.x + 35;
			this->maxs.y = this->parrent->parrent->maxs.y;
			this->mins.y = this->parrent->maxs.y;
			
		}

	}
}