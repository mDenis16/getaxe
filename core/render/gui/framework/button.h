#include <string>
#include <vector>
#include <memory>
namespace ui {

	class button : public object {

	public:

		SecureString* text;
		bool * value;

		int animated_alpha = 255;
		ImVec2 animation_position;
		button ( SecureString& text, object * parent, std::function<void ( )> func );
		button ( SecureString& text, object * parent, float percent_width, float percent_height, std::function<void ( )> func );
		
		
		std::function<void ( )> call_back;


		void draw ( ) override;
		void handle ( ) override;
		void update ( ) override;
		void handle_mouse_input ( ) override;

	};
}
