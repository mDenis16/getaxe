
namespace ui {

	class multibox : public object {

	public:
		bool should_be_scrollable = false;
		float maximumContentSize = 0.f;
		float click_time = 0.f;
		std::string title;
		config_manager::key_bind_item * key_bind_controller = nullptr;
		ImVec2 bb_min;
		bool key_bind_open = false;
		ImVec2 bb_max;
		bool hovering_element = false;
		float target_bb_y = 0.f;
		float open_progress = 0.f;
		float open_target_progress = 0.f;
		float target_progress = 0.f;
		float original_bb_height = 0.f;
		float bb_max_calc = 0.f;
	
		float original_bb_max_y;
		ImVec2 temp_min;
		std::string preview;
		std::string preview_animated;
		ImVec2 temp_max;
		
		ImVec2 preview_min;
	
		float thumb_length = 0.f;
		float thumb_progress = 0.f;
		float thumb_lerp_progress = 0.f;
		float max_thumb_progress = 0.f;
		ImVec2 thumb_path_mins = ImVec2 ( 0, 0 ), thumb_path_maxs = ImVec2 ( 0, 0 );

		ImVec2 thumb_mins = ImVec2 ( 0, 0 ), thumb_maxs = ImVec2 ( 0, 0 );

		bool modifying_thumb = false;

		float scroll_progress = 0.f;
		float target_scroll_progress = 0.f;

		bool animating_preview = false;
		float thumb_percent = 0.f;
		float last_access_frame = 999.f;
		float lastUpdateTime = 0.f;

		bool thumb_locked = false;
		float thumb_height = 5.f;
		float bb_width = 111;
		float max_height = 180;
		bool did_update = false;
		bool opened = false;
		bool hovering_thumb = false;
		long open_time = 0;
		bool in_animation = false;
		std::vector<std::string> items;
		float animation_progress = 0.f;

		std::vector<ImVec2> triangle_buffer;
		std::vector<int> * value_array;

		void * old_value;
		void on_update ( ) {
			if ( this->key_bind_controller ) {
				if ( !this->key_bind_controller->old_value ) 
					this->key_bind_controller->old_value = new std::vector<int> ( this->items.size ( ) );
				

				auto old_value_vector = ( std::vector<int>* )this->key_bind_controller->old_value;

				size_t buffer_size = sizeof ( int ) * old_value_vector->size ( );

				std::memcpy ( old_value_vector->data ( ), this->value_array->data ( ), buffer_size );

				std::memcpy ( this->key_bind_controller->old_value, this->value_array, sizeof ( this->value_array ) );// = this->value;



			}
			



		}
		void refresh_preview ( ) {



			preview.clear ( );
			preview_animated.clear ( );

			bool none_selected = true;
			int count_elements = 0;


			if ( preview.length ( ) > 13 )
				preview = preview.substr ( 0, 13 ) + "...";


			preview_animated = "";

			for ( size_t i = 0; i < this->items.size ( ); i++ ) {

				if ( this->value_array->at ( i ) > 0 ) {
					if ( preview.length ( ) > 0 )
						preview += ", ";
					preview += this->items.at ( i );
					none_selected = false;
					if ( preview_animated.length ( ) > 0 )
						preview_animated += ", ";
					preview_animated += this->items.at ( i );
				}
			}

			if ( preview.length ( ) > 13 )
				preview = preview.substr ( 0, 13 ) + "...";

			if ( none_selected ) {
				preview = "select";
				preview_animated = "click to open";
			}
		}
		void handle_scroll ( );
		void draw_scrollbar ( );
		multibox ( std::string text, object * parent, std::vector<std::string> _items, void * _value_array, float _bb_width = 0.f );
		multibox ( std::string text, object * parent, std::vector<std::string> _items, std::vector<int> & _value_array, float _bb_width = 0.f );
		multibox ( std::string text, object * parent, std::vector<std::string> _items, std::vector<int> & _value_array, config_manager::key_bind_item & key_bind_item, float _bb_width = 0.f );

		void update_triangle ( );



		void set_value ( int index );
		void draw_scroll_thumb ( );
		void draw ( ) override;
		void animate_preview ( );
		void handle ( ) override;
		void update ( ) override;
		void handle_mouse_input ( ) override;

	};
}
