

namespace ui {

	enum slider_type {
		degrees,
		integer,
		floates
	};
	class slider : public object {

	public:

		std::string title;
		bool key_bind_open = false;
		ImVec2 bb_min;
		ImVec2 bb_max;
		void*  value;
		void * old_value;
		bool updated_last_time = false;
		long last_access = 0;
		void * value_mins = nullptr;
		void * value_maxs = nullptr;
		slider_type slide_type;
		float fill_percent = 0.f;
		float old_fill_percent = 0.f;
		float target_fill = 0.f;
		config_manager::key_bind_item * key_bind_controller = nullptr;
		float width_limit = 120;
		bool in_animation = false;
		bool animate_left = false;
		
		bool animate_right = false;
		slider ( std::string text, object * parent, float & _value, float mins, float maxs, slider_type _type, config_manager::key_bind_item * key_bind_item );
		slider ( std::string text, object * parent, float & _value, float mins, float maxs, slider_type _type );

		void draw ( ) override;
		void handle ( ) override;
		void update ( ) override;
		void handle_mouse_input ( ) override;

	};
}
