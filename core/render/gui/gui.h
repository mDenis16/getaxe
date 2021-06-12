

#define PREVIEW
#include <deque>
#include <string>

namespace ui {

	extern void * window_pointer_cheat;

	void initialize ( );
	void render ( ImDrawList* render );

	void render_debug ( );

	bool key_pressed ( const int key );

	bool key_down ( const int key );


	bool key_released ( const int key );

	ImVec2 calculate_center ( ImVec2 a, ImVec2 b );

	class debug_log {
	public:
		~debug_log ( ) {
			
		}
		debug_log ( std::string msg ) { text = msg; time = ImGui::GetTime ( ); }
		std::string text;
		float time;
	};
	extern std::deque<debug_log *> debug_logs;

	extern ImFont * font_title;
	extern ImFont * font_widgets;
	extern ImFont * font_tab;
	extern ImFont * font_icons;
	extern ImFont * font_menu_icons;
	extern ImFont * menuicons;
	extern ImFont * weapon_font;
	extern ImFont * weapon_icons;
	extern ImFont * test;
	extern void * radar_window_extern;
	extern void * window_device;

	extern int focused_item;
	extern long last_time_clicked_element;
	extern bool key_state [ 256 ];
	extern bool prev_key_state [ 256 ];
	ImVec2 get_cursor ( );

	void to_rgb ( ImColor & original );

}

#define ICON_PLUS "a"
#define ICON_OK_SHIELD "b"
#define ICON_COLOR_PALLETE "c"
#define ICON_LEGITBOT "d"
#define VISUALS_ENEMIES "e"
#define VISUALS_TEAMMATES "f"
#define ICON_COPY "g"
#define ICON_PASTE "h"
#define ICON_STACK "i"
#define ICON_DOWNLOAD "j"
#define ICON_UPLOAD "k"
#define ICON_UNDO "l"
#define ICON_REDO "m"
#define ICON_WRENCH "n"
#define ICON_EQUALIZER "o"
#define ICON_BIN "p"
#define ICON_RAGEBOT "q"
#define ICON_ANTIAIM "r"
#define ICON_TAZER "s"
#define ICON_CLOUD "t"
#define ICON_WORLD "u"
#define ICON_VISUALS "v"
#define ICON_VISUALS_BLOCKED "w"
#define ICON_WARNING "x"
#define ICON_NOTIFICATION "y"
#define ICON_QUESTION "z"
#define ICON_INFO "A"
#define ICON_SETTINGS "B"
#define ICON_FIRE "C"
#define ICON_LOCAL "D"
#define ICON_MOVEMENT "E"
