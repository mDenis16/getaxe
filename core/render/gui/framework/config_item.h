

namespace ui {

	class config_item : public object {

		

		

	public:
		void draw ( ) override;
		void handle ( ) override;
		void update ( ) override;
		void handle_mouse_input ( ) override;

		ImVec2 button_mins, button_maxs;
		ImVec2 delete_mins, delete_maxs;
		ImVec2 copy_mins, copy_maxs;
		ImVec2 copy_pos, delete_pos;




		bool loaded = false;

		bool hovering_button = false;

		config_item ( object * parent, std::string _name, std::string _hash );

		std::string hash;
		bool cloud = false;
		std::string name;
		std::string author;
		std::string date;

		
		


	};
}

