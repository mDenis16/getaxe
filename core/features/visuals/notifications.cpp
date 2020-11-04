#include "../features.hpp"
#include "../../menu/render/menu_render.hpp"
#include "../../menu/ImGui/imgui.h"
#include "../../menu/ImGui/imgui_internal.h"

float dsdsDdfdfdffdrawText ( ImFont * pFont, const std::string & text, const ImVec2 & pos, float size, color color, float shadow, bool center, bool bold ) {
	ImGuiWindow * window = ImGui::GetCurrentWindow ( );


	std::stringstream stream ( text );
	std::string line;

	float y = 0.0f;
	int i = 0;

	while ( std::getline ( stream, line ) ) {
		ImVec2 textSize = pFont->CalcTextSizeA ( size, FLT_MAX, 0.0f, line.c_str ( ) );

		if ( center ) {
			if ( bold ) {
				window->DrawList->AddText ( pFont, size, ImVec2 ( ( pos.x - textSize.x / 2.0f ) + shadow, ( pos.y + textSize.y * i ) + shadow ), ImColor ( 0, 0, 0, color.a ), line.c_str ( ) );
				window->DrawList->AddText ( pFont, size, ImVec2 ( ( pos.x - textSize.x / 2.0f ) - shadow, ( pos.y + textSize.y * i ) - shadow ), ImColor ( 0, 0, 0, color.a ), line.c_str ( ) );
				window->DrawList->AddText ( pFont, size, ImVec2 ( ( pos.x - textSize.x / 2.0f ) + shadow, ( pos.y + textSize.y * i ) - shadow ), ImColor ( 0, 0, 0, color.a ), line.c_str ( ) );
				window->DrawList->AddText ( pFont, size, ImVec2 ( ( pos.x - textSize.x / 2.0f ) - shadow, ( pos.y + textSize.y * i ) + shadow ), ImColor ( 0, 0, 0, color.a ), line.c_str ( ) );

			}
			window->DrawList->AddText ( pFont, size, ImVec2 ( pos.x - textSize.x / 2.0f, pos.y + textSize.y * i ), ImColor ( color.r, color.g, color.b, color.a ), line.c_str ( ) );
		}
		else {
			window->DrawList->AddText ( pFont, size, ImVec2 ( ( pos.x ) + 1, ( pos.y + textSize.y * i ) + shadow ), ImColor ( 0, 0, 0, color.a ), line.c_str ( ) );
			window->DrawList->AddText ( pFont, size, ImVec2 ( ( pos.x ) - 1, ( pos.y + textSize.y * i ) - shadow ), ImColor ( 0, 0, 0, color.a ), line.c_str ( ) );
			window->DrawList->AddText ( pFont, size, ImVec2 ( ( pos.x ) + 1, ( pos.y + textSize.y * i ) - shadow ), ImColor ( 0, 0, 0, color.a ), line.c_str ( ) );
			window->DrawList->AddText ( pFont, size, ImVec2 ( ( pos.x ) - 1, ( pos.y + textSize.y * i ) + shadow ), ImColor ( 0, 0, 0, color.a ), line.c_str ( ) );

			window->DrawList->AddText ( pFont, size, ImVec2 ( pos.x, pos.y + textSize.y * i ), ImColor ( color.r, color.g, color.b, color.a ), line.c_str ( ) );
		}

		y = pos.y + textSize.y * ( i + 1 );
		i++;
	}

	return y;
}
namespace visuals {
	namespace notifications {
		std::vector<notify> list;
		void add ( std::string _text ) {
			list.push_back ( { _text, interfaces::globals->cur_time } );
		}
	

		void fne ( ) {

		}
		void present ( ) {
			for ( size_t i = 0; i < list.size(); i++ ) {
				auto & notification = list.at ( i );
				auto dif = interfaces::globals->cur_time - notification.time;

				if ( !notification.seen) {
					if ( notification.current < notification.to_end )
					  notification.current += 1.f / 0.5f * interfaces::globals->frame_time * 150.f;
					else if ( dif > 3.f ) {
						notification.seen = true;
						continue;
					}
				}
				else {
					if ( notification.current > notification.from_start ) {
						notification.current -= 1.f / 0.5f * interfaces::globals->frame_time * 150.f;
					   
						
					}
					if ( notification.current < notification.from_start ) {
						list.erase ( list.begin ( ) + i );
						continue;
					}
				}

				
				ImVec2 textSize = c_menu::get ( ).normalf->CalcTextSizeA ( 13.f, FLT_MAX, 0.0f, notification.text.c_str ( ) );
				notification.min = ImVec2 ( notification.current, 25.f * i  );
				notification.max = ImVec2 ( notification.current + textSize.x + 15, 25.f * i + 25.f );
				
				//if ( notification.width && dif > 3.f )
					//notification.width--;
			
				c_menu::get ( ).draw->AddRectFilled (notification.min, notification.max, ImColor( 4, 19, 37,255) );
				c_menu::get ( ).draw->AddRectFilled ( ImVec2( notification.max.x - 3, notification.min.y), notification.max, ImColor ( 255, 255, 255, 100 ) );
				c_menu::get ( ).draw->AddRect ( notification.min, notification.max, ImColor ( 0, 0, 0, 15 ) );
				auto text_pos = ImVec2 ( notification.min.x + 5.f, ( notification.min.y + notification.max.y ) / 2.f );
				
				text_pos.y -= textSize.y / 2;

				dsdsDdfdfdffdrawText ( c_menu::get ( ).normalf, notification.text, text_pos, 13.f, color ( 255, 255, 255, 255 ), 0.f, false, false );
			}
		}
	}
}