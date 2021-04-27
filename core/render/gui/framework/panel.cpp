#include "../includes.h"
#include <WinUser.h>

namespace ui {
	void panel::draw ( ) {
	
		this->renderer->AddRectFilled ( this->mins, this->maxs, ImColor ( 255, 23, 255, 150 ) );

		for ( auto & child : this->children )
			child->draw ( );
	}
	void panel::handle ( ) {
		


	}


	void panel::handle_mouse_input ( ) {


	}
	void panel::update ( ) {
	
		this->padding = this->parrent->padding;

		this->mins = this->parrent->mins;
	
		this->maxs.x = this->parrent->maxs.x;

		for ( auto & child : this->children )
			child->update ( );

		if ( !this->children.empty ( ) )
			this->maxs.y = this->children.back ( )->maxs.y;
	}
}