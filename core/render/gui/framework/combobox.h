
namespace ui {

	class combobox : public object {

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
		float max_thumb_progress = 0.f;
		float scroll_progress = 0.f;
		float max_scroll_progress = 0.f;
		float thumb_length = 0.f;
		float thumb_progress = 0.f;
		float thumb_lerp_progress = 0.f;
	
		float original_bb_max_y;
		float lastUpdateTime = 0.f;
		float animation_progress = 1.f;
		ImVec2 thumb_path_mins = ImVec2 ( 0, 0 ), thumb_path_maxs = ImVec2 ( 0, 0 );
		bool modifying_thumb = false;
		ImVec2 thumb_mins = ImVec2 ( 0, 0 ), thumb_maxs = ImVec2 ( 0, 0 );
		
		float target_scroll_progress = 0.f;
		ImVec2 temp_min;
		ImVec2 temp_max;
		ImVec2 thumb_min;
		ImVec2 thumb_max;
		float thumb_percent = 0.f;
		bool thumb_locked = false;
		float thumb_height = 5.f;
		float last_delta_time = 0.f;
		float bb_width = 111;
		float max_height = 180;
		
		bool opened = false;
		bool hovering_thumb = false;
		long open_time = 0;
		bool in_animation = false;
		std::vector<std::string> items;
		int* value;
		void * old_value;

		void handle_scroll ( );

		void draw_scrollbar ( );
		std::vector<ImVec2> triangle_buffer;
		combobox ( std::string text, object * parent, std::vector<std::string> _items, int & _value, float _bb_width = 0.f );
		combobox ( std::string text, object * parent, std::vector<std::string> _items, int* _value, float _bb_width = 0.f );
		combobox ( std::string text, object * parent, std::vector<std::string> _items,  int & _value, config_manager::key_bind_item & key_bind_item, float _bb_width = 0.f );

		combobox ( std::string text, object * parent, std::vector<std::string> _items, int & _value );


		void set_value ( int index );
		void draw_scroll_thumb ( );
		void draw ( ) override;
		void update_triangle ( );
		void handle ( ) override;
		void update ( ) override;
		void handle_mouse_input ( ) override;

	};
}
