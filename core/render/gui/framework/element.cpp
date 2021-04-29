
#pragma once
#include <iostream>
#include "../includes.h"


namespace ui {
	void object::handle_mouse_input ( ) {
		
	

	}

	void object::out_of_focus ( ) {
		
		if ( this->parrent->type == element_extender_element )
			ui::focused_item = this->parrent->_id; /*restore id to parent if element is extender*/
		else if ( this->parrent->type == panel_element && this->parrent->parrent->type == panel_cotainer_element && this->parrent->parrent->parrent->type == element_extender_element )
			ui::focused_item = this->parrent->parrent->parrent->_id;
		else
			ui::focused_item = -1;

	}
	bool object::can_focus ( ) {

		if ( this->parrent->type == element_extender_element || this->parrent->type == panel_element && this->parrent->parrent->type == panel_cotainer_element ) {
			if ( ui::focused_item != -1 && ui::focused_item == this->parrent->parrent->parrent->_id ) /*handling when object in inside panel in extend box*/
				return true;
			else if ( ui::focused_item != -1 && ui::focused_item != this->parrent->_id )
				return false;
		}
		else if ( ui::focused_item != -1 )
			return false;

		return true;
	}
	bool object::is_focused ( ) {
		
		return  ui::focused_item == this->_id;
	}
}


