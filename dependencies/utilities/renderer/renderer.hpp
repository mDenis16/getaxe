#pragma once
#include "../../utilities/csgo.hpp"
#include "../../../core/menu/zgui/zgui.hpp"

enum font_flags {
	fontflag_none,
	fontflag_italic = 0x001,
	fontflag_underline = 0x002,
	fontflag_strikeout = 0x004,
	fontflag_symbol = 0x008,
	fontflag_antialias = 0x010,
	fontflag_gaussianblur = 0x020,
	fontflag_rotary = 0x040,
	fontflag_dropshadow = 0x080,
	fontflag_additive = 0x100,
	fontflag_outline = 0x200,
	fontflag_custom = 0x400,
	fontflag_bitmap = 0x800,
};

namespace render {
	void initialize();
	void draw_3d_filled_circle( int x, int y, float radius );
	void draw_line(int x1, int y1, int x2, int y2, color colour);
	void draw_text_wchar(int x, int y, unsigned long font, const wchar_t* string, color colour);
	void draw_text_string(int x, int y, unsigned long font, std::string string, bool text_centered, color colour);
	void draw_rect(int x, int y, int w, int h, color color);
	void draw_filled_rect(int x, int y, int w, int h, color colour);
	void draw_outline(int x, int y, int w, int h, color colour);
	void draw_textured_polygon(int n, vertex_t* vertice, color col);
	void draw_circle(int x, int y, int r, int s, color col);

	void draw_3d_filled_circle( vec3_t origin, float radius );

	vec2_t get_text_size(unsigned long font, std::string text);

	//zgui
	void zgui_line( int x, int y, int x2, int y2, color c ) noexcept;
	void zgui_rect( int x, int y, int x2, int y2, color c ) noexcept;
	void zgui_filled_rect( int x, int y, int x2, int y2, color c ) noexcept;
	void zgui_text( int x, int y, color color, int font, bool center, std::string str ) noexcept;
    void get_text_size( unsigned long font, std::string str, int& width, int& height ) noexcept;
	float get_frametime( ) noexcept;

	namespace fonts {
		extern unsigned long menu_font;
		extern unsigned long watermark_font;
	}

}