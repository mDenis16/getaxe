#include <string>
#include <vector>
#include <memory>
namespace ui {

	class bezier_editor : public object {

	public:

		std::string text;
		float value [ 4 ] = { 0.f, 1.f, 1.f, 1.f };

		int animated_alpha = 255;
		ImVec2 animation_position;
		bezier_editor (  object * parent, ImVec4& bezier );
	
		void draw ( ) override;
		void handle ( ) override;
		bool mouse_in_radius ( float radius, ImVec2 pos );
		void update ( ) override;
		void handle_mouse_input ( ) override;

	};

	class  bezier_dot : public object {
	public:
		bezier_dot ( object * parent, float& value_x, float& value_y );

		float * value_x;
		float * value_y;

		bool is_down = false;

		void draw ( ) override;
		void handle ( ) override;
		void update ( ) override;
		void handle_mouse_input ( ) override;
	};


}
