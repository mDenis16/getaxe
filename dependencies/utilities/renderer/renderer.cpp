#include "renderer.hpp"

unsigned long render::fonts::menu_font;
unsigned long render::fonts::watermark_font;

void render::initialize() {
	render::fonts::menu_font = interfaces::surface->font_create( );
	render::fonts::watermark_font = interfaces::surface->font_create();

	interfaces::surface->set_font_glyph( render::fonts::menu_font, "Verdana", 12, 0, 0, 0, font_flags::fontflag_dropshadow );
	interfaces::surface->set_font_glyph(render::fonts::watermark_font, "Tahoma", 12, 500, 0, 0, font_flags::fontflag_dropshadow);

	console::log("[setup] render initialized!\n");

	zgui::functions.draw_line = zgui_line;
	zgui::functions.draw_rect = zgui_rect;
	zgui::functions.draw_filled_rect = zgui_filled_rect;
	zgui::functions.draw_text = zgui_text;
	zgui::functions.get_text_size = get_text_size;
	zgui::functions.get_frametime = get_frametime;

	console::log( "[setup] zgui initialized!\n" );
}

void render::draw_line(std::int32_t x1, std::int32_t y1, std::int32_t x2, std::int32_t y2, color colour) {
	interfaces::surface->set_drawing_color(colour.r, colour.g, colour.b, colour.a);
	interfaces::surface->draw_line(x1, y1, x2, y2);
}

void render::draw_text_wchar(std::int32_t x, std::int32_t y, unsigned long font, const wchar_t* string, color colour) {
	interfaces::surface->set_text_color(colour.r, colour.g, colour.b, colour.a);
	interfaces::surface->draw_text_font(font);
	interfaces::surface->draw_text_pos(x, y);
	interfaces::surface->draw_render_text(string, wcslen(string));
}

void render::draw_text_string(std::int32_t x, std::int32_t y, unsigned long font, std::string string, bool text_centered, color colour) {
	const auto converted_text = std::wstring(string.begin(), string.end());

	int width, height;
	interfaces::surface->get_text_size(font, converted_text.c_str(), width, height);

	interfaces::surface->set_text_color(colour.r, colour.g, colour.b, colour.a);
	interfaces::surface->draw_text_font(font);
	if (text_centered)
		interfaces::surface->draw_text_pos(x - (width / 2), y);
	else
		interfaces::surface->draw_text_pos(x, y);
	interfaces::surface->draw_render_text(converted_text.c_str(), wcslen(converted_text.c_str()));
}

void render::draw_rect(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, color color) {
	interfaces::surface->set_drawing_color(color.r, color.g, color.b, color.a);
	interfaces::surface->draw_outlined_rect(x, y, w, h);
}

void render::draw_filled_rect(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, color colour) {
	interfaces::surface->set_drawing_color(colour.r, colour.g, colour.b, colour.a);
	interfaces::surface->draw_filled_rectangle(x, y, w, h);
}

void render::draw_outline(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, color colour) {
	interfaces::surface->set_drawing_color(colour.r, colour.g, colour.b, colour.a);
	interfaces::surface->draw_outlined_rect(x, y, w, h);
}

void render::draw_textured_polygon(std::int32_t n, vertex_t* vertice, color col) {
	static unsigned char buf[4] = { 255, 255, 255, 255 };
	unsigned int texture_id{};
	if (!texture_id) {
		texture_id = interfaces::surface->create_new_texture_id(true);
		interfaces::surface->set_texture_rgba(texture_id, buf, 1, 1);
	}
	interfaces::surface->set_drawing_color(col.r, col.g, col.b, col.a);
	interfaces::surface->set_texture(texture_id);
	interfaces::surface->draw_polygon(n, vertice);
}
void render::draw_3d_filled_circle( vec3_t origin, float radius ) {
	constexpr int quality = 20;

	static int texture = interfaces::surface->create_new_texture_id( true );
	
	unsigned char Buffer [ 4 ] = { 255, 0, 0, 100 };

	interfaces::surface->set_texture_rgba( texture, Buffer, 1, 1 );
	interfaces::surface->set_texture( texture );

	float step = M_PI * 2.0 / 8;
	static vertex_t verts [ 8 ];
	int step_count = 0;
	for ( float a = 0; a < ( M_PI * 2.0 ); a += step )
	{
		vec3_t location( radius * cos( a ) + origin.x, radius * sin( a ) + origin.y, origin.z );
		vec3_t location_w2s = vec3_t( );  math::world_to_screen( location, location_w2s );
		verts [ step_count ].position = vec2_t( location_w2s.x, location_w2s.y );

		step_count++;
		
	}

  interfaces::surface->draw_textured_polygon( quality, verts, true );
}

void render::draw_circle(std::int32_t x, std::int32_t y, std::int32_t r, std::int32_t s, color col) {
	float Step = M_PI * 2.0 / s;
	for (float a = 0; a < (M_PI * 2.0); a += Step) {
		float x1 = r * cos(a) + x;
		float y1 = r * sin(a) + y;
		float x2 = r * cos(a + Step) + x;
		float y2 = r * sin(a + Step) + y;
		interfaces::surface->set_drawing_color(col.r, col.g, col.b, col.a);
		interfaces::surface->draw_line(x1, y1, x2, y2);
	}
}

vec2_t render::get_text_size(unsigned long font, std::string text) {
	std::wstring a(text.begin(), text.end());
	const wchar_t* wstr = a.c_str();
	static int w, h;

	interfaces::surface->get_text_size(font, wstr, w, h);
	return { static_cast<float>(w), static_cast<float>(h) };
}

//zgui
void render::zgui_line( int x, int y, int x2, int y2, color c ) noexcept {
	interfaces::surface->set_drawing_color( c.r, c.g, c.b, c.a );
	interfaces::surface->draw_line( x, y, x2, y2 );
}

void render::zgui_rect( int x, int y, int x2, int y2, color c ) noexcept {
	interfaces::surface->set_drawing_color( c.r, c.g, c.b, c.a );
	interfaces::surface->draw_outlined_rect( x, y, x2, y2 );
}

void render::zgui_filled_rect( int x, int y, int x2, int y2, color c ) noexcept {
	interfaces::surface->set_drawing_color( c.r, c.g, c.b, c.a );
	interfaces::surface->draw_filled_rectangle( x, y, x2, y2 );
}

void render::zgui_text( int x, int y, color color, int font, bool center, std::string str ) noexcept {
	std::wstring text = std::wstring( str.begin( ), str.end( ) );
	const wchar_t* converted_text = text.c_str( );

	int width, height;
	interfaces::surface->get_text_size( font, converted_text, width, height );

	interfaces::surface->set_text_color( color.r, color.g, color.b, color.a );
	interfaces::surface->draw_text_font( font );
	if ( center )
		interfaces::surface->draw_text_pos( x - ( width / 2 ), y );
	else
		interfaces::surface->draw_text_pos( x, y );

	interfaces::surface->draw_render_text( converted_text, wcslen( converted_text ) );
}

void render::get_text_size( unsigned long font, std::string str, int& width, int& height ) noexcept {
	std::wstring text = std::wstring( str.begin( ), str.end( ) );
	const wchar_t* out = text.c_str( );

	interfaces::surface->get_text_size( font, out, width, height );
}

float render::get_frametime( ) noexcept {
	return interfaces::globals->frame_time;
}