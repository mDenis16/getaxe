#pragma once
#include <vector>
#include <memory>
#include "../config.h"

namespace ui {
	inline int last_element_id = 0;


	enum key_bind_mode {
		HOLD,
		TOGGLE
	};

	enum key_bind_value_type {
		key_float,
		key_bool,
		key_int
	};

	class key_bind_component {
	public:

		void * value;
		void * new_value;
		void * old_value;

		bool was_hold = false;

		int * key;
		key_bind_value_type type;
		key_bind_mode * mode;

		key_bind_component ( void * value, void * new_value, int & _key, int & _mode, int type ) {
			this->type = ( key_bind_value_type)type;
			this->key = &_key;
			this->value = &value;
			this->new_value = new_value;
			this->mode = ( key_bind_mode* ) &_mode;
			//key_bind_list.push_back ( this );
		}
		~key_bind_component ( ) {
			
		}

		void update_bind ( );

	};

    extern std::vector< key_bind_component*> key_bind_list;
	
	enum object_type {
		window_element,
		tab_element,
		child_window_element,
		sub_tab_element,
		checkbox_element,
		slider_element,
		small_text_input_element,
		text_input_element,
		combobox_element,
		combo_element,
		multibox_element,
		colorpicker_element,
		keybind_element,
		key_selector_element,
		config_box_element,
		config_item_element,
		button_element,
		panel_element,
		panel_cotainer_element,
		element_extender_element,
		bezier_editor_element

	};

	enum flex_direction {
		line,
		block,
		automatic
	};
	enum float_side {
		none,
		top,
		left,
		right,
		bottom
	};
	enum flags {
		position_absolute = 1 << 0,
		position_relative = 1 << 1,
		align_center = 1 << 2,
		align_normal = 1 << 3,
		vertical_align_center = 1 << 4,
		align_left = 1 << 5,
		align_right = 1 << 6,
		no_background = 1 << 7,
		hide_overflow = 1 << 8,
		fullscreen = 1 << 9,
		scrollbar = 1 << 10,
		render_forward = 1 << 11,
		text_input_as_element = 1 << 12,
		big_inventory_item = 1 << 13
	};
	
	class object {
	public:
	
		

		object * parrent = nullptr;
		int flags = 0;
		int width = 0;
		int index = 0;
		float padding = 0.f;
		int children_index = 0;
		int height = 70;
		
		bool should_reanimate = true;
		bool should_reset = true;
		bool hovering = false; 
	
		bool in_animation = false;


		bool hold_down = false;
		bool registered = false;
		bool was_hovering = false;
		float hover_start = 0.f;
		float hover_start_time = 0.f;
		float hsv_val = 0.f;
		float last_open = 0.f;
		ImVec2 mins = ImVec2(0,0), maxs = ImVec2(0,0);
		ImVec2 old_mins = ImVec2(0,0), old_maxs = ImVec2(0,0);
		int _id = -1;
		double animation_step = 1.0;
		flex_direction flex = flex_direction::line;
		float_side _float = none;
		ImDrawList * renderer = nullptr;
		object_type type;
		void focus_it ( );
		std::vector< object* > children;

		void set_parent (  object *  _parrent ) {
			this->parrent = _parrent;
		}

		void empty_children ( ) {
			for ( auto& child : children ) {
				delete child;
			}
			children_index = 0;
			children.clear ( );
		}


		void add_children ( object*  child ) {
			child->index = this->children_index;

			last_element_id++;
			child->_id = last_element_id;

			children.emplace_back ( child );
		
			this->children_index++;

	

			update ( );
		}
		bool is_focused ( );
		void out_of_focus ( );
		bool can_focus ( );

		virtual void update ( ) = 0;
		virtual void handle ( ) = 0;
		virtual void draw ( ) = 0;
		virtual void handle_mouse_input ( ) = 0;
	
	};
}
#define PRIMARY_COLOR ImColor(9,12,16)
#define SECONDARY_COLOR ImColor(27,27,35)
#define TERTIARY_COLOR ImColor(0,129,138,255)
#define TERTIARY_COLOR_OPEN ImColor(219,237,243,255)