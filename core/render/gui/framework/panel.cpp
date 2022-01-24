#include "../includes.h"
#include <WinUser.h>

namespace ui {
	void panel::draw ( ) {
	
		


		for ( size_t i = this->children.size ( ) - 1; i != ( size_t ) -1; i-- )
			this->children.at ( i )->draw ( );

		this->renderer->AddRect ( this->mins, this->maxs, ImColor ( 255, 0, 0, 255 ), 0, 0, 1.f );
	}
	void panel::handle ( ) {
		


	}


	void panel::handle_mouse_input ( ) {


	}
	void panel::update ( ) {
		if ( this->flags & flags::fullscreen ) {

			this->padding = 0;
			this->maxs = this->parrent->maxs;
			this->mins = this->parrent->mins;

			this->height = this->maxs.y - this->mins.y;
			this->width = this->maxs.x - this->mins.x;
		

			for ( auto & child : this->children )
				child->update ( );


		
		}
		else {
			this->padding = this->parrent->padding;

			this->mins = this->parrent->mins;

			this->maxs.x = this->parrent->maxs.x;





			for ( auto & child : this->children )
				child->update ( );


			if ( !this->children.empty ( ) )
				this->maxs.y = this->children.back ( )->maxs.y;
			else
				this->maxs.y = this->mins.y;

			this->width = this->maxs.x - this->mins.x;
			this->height = this->maxs.y - this->mins.y;

			if ( this->flags & flags::fullscreen ) {

				this->padding = 0;
				this->maxs = this->parrent->maxs;
				this->mins = this->parrent->mins;

			}
			this->width = this->maxs.x - this->mins.x;
			this->height = this->maxs.y - this->mins.y;
			if (this->children.empty())
			{
				this->width = this->height = 0;
			}

		}
	}
}