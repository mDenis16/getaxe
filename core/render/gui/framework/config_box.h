
namespace ui {

	class config_box : public object {

	public:


		ImVec2 bb_min;
		ImVec2 bb_max;
	
		float max_scroll_progress = 0.f;
		float scroll_progress = 0.f;
		float thumb_percent = 0.f;
		config_box ( object * parent );

	
	
		bool should_reload = false;

		object * get_raw_pointer ( );

		std::vector<std::string> local_files ( );

		 void refresh_list ( );

		 void load_config ( std::string hash, bool cloud );

		void save_config ( std::string name, bool cloud, std::string hash = "" );

		ImVec2 thumb_mins, thumb_maxs;

		void draw ( ) override;
		void handle ( ) override;
		void update ( ) override;
		void handle_mouse_input ( ) override;



	};
}
