
namespace ui {

	class key_selector : public object {

	public:

	
		ImVec2 bb_min;
		ImVec2 bb_max;

		bool pulse = false;
		float alpha = 0;


		std::string text; 
		int *bind_key;
		int key = 0;
		float height = 20;
		bool listening = false;

		key_selector ( std::string text, object * parent, int * key_bind);
	
		void draw ( ) override;
		void handle ( ) override;
		void update ( ) override;
		void handle_mouse_input ( ) override;

	};
}
