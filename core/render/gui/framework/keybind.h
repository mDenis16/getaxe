
namespace ui {

	class key_bind_muie : public object {

	public:




		key_bind_muie ( object * parent, config_manager::key_bind_item * );

		bool* open;
		int  * key;
		int * key_mode ;
		ImVec2 bb_min;
		ImVec2 bb_max;
		void* new_value;
		int new_value_int = 0;
		

		float new_float_value;

		float width = 170;
		float height = 130;



		void draw ( ) override;

		void handle ( ) override;

		void update ( ) override;
		void handle_mouse_input ( ) override;


	};
}
