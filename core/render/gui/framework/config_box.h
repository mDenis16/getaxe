
namespace ui {

	class config_box : public object {

	public:

		ImVec2 thumb_path_mins = ImVec2 ( 0, 0 ), thumb_path_maxs = ImVec2 ( 0, 0 );
		ImVec2 bb_min;
		ImVec2 bb_max;
		float thumb_length = 0.f;
		float max_scroll_progress = 0.f;
		float scroll_progress = 0.f;
		float thumb_percent = 0.f;
		config_box ( object * parent );
		float thumb_progress = 0.f;
		bool modifying_thumb = false;
		float max_thumb_progress = 0.f;
		bool should_reload = false;
		float target_scroll_progress = 0.f;
		object * get_raw_pointer ( );
		float maximumContentSize = 0.f;
		std::vector<std::string> local_files ( );
		float lastUpdateTime = 0.f;
		 void refresh_list ( );

		 void load_config ( std::string hash, bool cloud );

		void save_config ( std::string name, bool cloud, std::string hash = "" );

		ImVec2 thumb_mins, thumb_maxs;

		void draw ( ) override;
		void handle ( ) override;
		void update ( ) override;
		void handle_scroll ( );
		void draw_scrollbar ( );
		void handle_mouse_input ( ) override;



	};
}
