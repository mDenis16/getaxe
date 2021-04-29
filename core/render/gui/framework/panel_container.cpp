#include "../includes.h"
#include <WinUser.h>
#include <queue>

namespace ui {
	void panel_container::draw ( ) {

		this->handle ( );

	
		if ( *this->selected_index < this->children.size() ) 
			this->children.at ( *this->selected_index )->draw ( );
		
		
	}

	void panel_container::handle ( ) {

		if ( *this->selected_index != this->old_index ) {

			if (this->parrent->type == element_extender_element )
			 static_cast<ui::element_extender*>(this->parrent)->update_isolate ( );
			else
				this->parrent->update ( );

			for ( object * child : this->children ) {

				std::queue<ui::object *> q;
				q.push ( child );

				while ( !q.empty ( ) ) {
					int n = q.size ( );


					while ( n > 0 ) {
						ui::object * p = q.front ( );
						p->should_reset = true;
						p->should_reanimate = true;

						q.pop ( );

						for ( int i = 0; i < p->children.size ( ); i++ )
							q.push ( p->children [ i ] );
						n--;
					}
				}
			}
			this->old_index = *this->selected_index;
		}
	}


	void panel_container::handle_mouse_input ( ) {


	}
	void panel_container::update ( ) {

		this->padding = this->parrent->padding;

		if ( this->index > 0 ) {
			auto & old = this->parrent->children.at ( this->index - 1 );

			this->mins.x = this->parrent->mins.x;
			this->mins.y = old->maxs.y + 8.f;
		}
		else {
			this->mins = this->parrent->mins;

		}
		this->maxs.x = this->parrent->maxs.x;

		for ( auto & child : this->children )
			child->update ( );

		if ( !this->children.empty ( ) ) {
			if ( *this->selected_index < this->children.size ( ) )
				this->maxs.y = this->children.at ( *this->selected_index )->maxs.y;
		}
	}
}