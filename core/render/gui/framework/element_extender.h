
namespace ui {

	class element_extender : public object {

	public:


		ImVec2 icon_mins, icon_maxs;


		element_extender ( object * parent );

		bool  open;

		float width = 185;
		float height = 130;

		bool updated_last_time = false;
		bool in_animation = false;
		float target_max_y = 0.f;


		void draw ( ) override;

		void handle ( ) override;

		void update_isolate ( );

		void update ( ) override;
		void handle_mouse_input ( ) override;


	};
}
