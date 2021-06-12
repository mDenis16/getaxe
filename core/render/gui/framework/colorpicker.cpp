
#pragma once

#include <Windows.h>
#include <iostream>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <WinBase.h>
#include <stdio.h>
#include <tchar.h>
#include <clip.h>
#include "../includes.h"

namespace ui {

	colorpicker::colorpicker ( object * parent, ImColor & color ) {


		this->parrent = parent; 
		this->renderer = this->parrent->renderer;
		this->type = colorpicker_element;

		this->color = &color;
		update ( );
		this->add_children ( new text_input ( this, text_type::hash ) );
		this->parrent->add_children ( this );

		update ( );
	}
	std::string rgb2hex ( int r, int g, int b, bool with_head ) {
		std::stringstream ss;
		if ( with_head )
			ss << "#";
		ss << std::hex << ( r << 16 | g << 8 | b );
		return ss.str ( );
	}
	std::vector<std::string> SplitWithCharacters ( const std::string & str, int splitLength ) {
		int NumSubstrings = str.length ( ) / splitLength;
		std::vector<std::string> ret;

		for ( int i = 0; i < NumSubstrings; i++ ) {
			ret.push_back ( str.substr ( i * splitLength, splitLength ) );
		}

		// If there are leftover characters, create a shorter item at the end.
		if ( str.length ( ) % splitLength != 0 ) {
			ret.push_back ( str.substr ( splitLength * NumSubstrings ) );
		}


		return ret;
	}

	struct COLOR {
		short r;
		short g;
		short b;
	};

	ImColor hex2rgb ( std::string hex ) {
		ImColor color;

		if ( hex.at ( 0 ) == '#' ) {
			hex.erase ( 0, 1 );
		}

		while ( hex.length ( ) != 6 ) {
			hex += "0";
		}

		std::vector<std::string> colori = SplitWithCharacters ( hex, 2 );

		int r = stoi ( colori [ 0 ], nullptr, 16 );
		int g = stoi ( colori [ 1 ], nullptr, 16 );
		int b = stoi ( colori [ 2 ], nullptr, 16 );

		return ImColor(r,g,b);
	}
	void colorpicker::draw ( ) {

		this->handle ( );

		static ImColor hueColors [ 7 ] =
		{
			{ 255, 0, 0   },
			{ 255, 255, 0 },
			{ 0, 255, 0   },
			{ 0, 255, 255 },
			{ 0, 0, 255   },
			{ 255, 0, 255 },
			{ 255, 0, 0   }
		};

		if ( this->in_animation ) {

			this->animation_step += ( ( 1000.0 / ( double ) ImGui::GetIO ( ).Framerate ) / 800.0);
			if ( this->animation_step >= 1.f ) {
				this->in_animation = false;
				this->animation_step = 1.f;
			}
			this->color->get_rgb ( );

			int r = static_cast< int >( std::lerp ( ( int ) this->color->rgb.x, 255, this->animation_step ) );
			int g = static_cast< int >( std::lerp ( ( int ) this->color->rgb.y, 255, this->animation_step ) );
			int b = static_cast< int >( std::lerp ( ( int ) this->color->rgb.z, 255, this->animation_step ) );
			int a = static_cast< int >( std::lerp ( 200, 140, this->animation_step ) );


			this->progress_color = ImColor ( r, g, b, a );

		}
		

		
	
		if ( this->parrent->type == checkbox_element ) {
			//L"Hello, \u0444!\n";
			//this->renderer->AddRectFilled ( this->mins, this->maxs, ImColor ( 255, 0, 255, 255 ) );
			auto text_size = ImGui::CalcTextSize ( ICON_COLOR_PALLETE, 18.f, ui::font_icons );

			this->renderer->AddText ( ui::font_icons, 18.f, ImVec2((this->mins.x + this->maxs.x) / 2.f - text_size.x / 2.f, ((this->mins.y + this->maxs.y) / 2.f) - text_size.y / 2.f), progress_color, ICON_COLOR_PALLETE );



			//this->renderer->AddText ( ui::font_widgets, 16.f, ImVec2 ( ( this->parrent->mins.x + this->parrent->maxs.x ) / 2.f - text_size.x / 2.f, ( this->parrent->mins.y + this->parrent->maxs.y ) / 2.f ), *this->color, std::to_string ( this->color->Value.w ).c_str ( ) );

		}
		if ( this->open ) {
			this->renderer->AddRectFilled ( this->bb_min, this->bb_max, ImColor ( 27, 28, 31, 255 ), 3.5f );
			this->renderer->AddRect ( this->bb_min, this->bb_max, ImColor ( 255, 255, 255, 15 ), 4.5f );

	
			
		

			auto mouse_pos = ui::get_cursor ( );
			
			picker_gradient ( );

			this->renderer->AddCircleFilled ( ImVec2 ( this->picker_bb_min.x + this->picker_cursor.x, this->picker_bb_min.y + this->picker_cursor.y ), 5.0f, ImColor(255, 255, 255, 255) );
			this->renderer->AddCircleFilled (ImVec2(this->picker_bb_min.x + this->picker_cursor.x, this->picker_bb_min.y + this->picker_cursor.y), 4.5f, *this->color );

			this->renderer->AddCircle ( ImVec2 ( this->picker_bb_min.x + this->picker_cursor.x, this->picker_bb_min.y + this->picker_cursor.y ), 5.5f, ImColor ( 0, 0, 0, 255 ) );

			auto hue_height = this->hue_bb_max.y - this->hue_bb_min.y;

			auto left_triangle = ImVec2 ( this->hue_bb_min.x - 1, this->hue_bb_min.y + (  (1.f - colHSV [ 0 ]) / 100.0 ) * hue_height * 100 );

			auto alpha_right_triangle = ImVec2 ( this->alpha_bb_max.x, this->hue_bb_min.y + ( ( 1.f - this->alpha  ) / 100.0 ) * hue_height * 100 );


			this->renderer->AddRectFilledMultiColor ( this->alpha_bb_min, this->alpha_bb_max, *this->color, *this->color, ImColor(0,0,0,255), ImColor ( 0, 0, 0, 255 ) );
			this->renderer->AddRect ( this->alpha_bb_min, this->alpha_bb_max, ImColor ( 255, 255, 255, 15 ) );

			this->renderer->AddTriangleFilled ( ImVec2 ( left_triangle.x, left_triangle.y + 3.5f ), ImVec2 ( ( this->hue_bb_min.x + this->hue_bb_max.x )  / 2.f, left_triangle.y), ImVec2 ( left_triangle.x, left_triangle.y - 3.5f ), ImColor ( 255, 255, 255, 255 ) );
			this->renderer->AddTriangle ( ImVec2 ( left_triangle.x, left_triangle.y + 3.5f ), ImVec2 ( ( this->hue_bb_min.x + this->hue_bb_max.x ) / 2.f, left_triangle.y ), ImVec2 ( left_triangle.x, left_triangle.y - 3.5f ), ImColor ( 0, 0, 0, 255 ) );


			this->renderer->AddTriangleFilled ( ImVec2 ( ( this->alpha_bb_min.x + this->alpha_bb_max.x ) / 2.f - 0.5f, alpha_right_triangle.y ), ImVec2 ( alpha_right_triangle.x, alpha_right_triangle.y + 3.5f ), ImVec2 ( alpha_right_triangle.x, alpha_right_triangle.y - 3.5f ), ImColor ( 255, 255, 255, 255 ) );

			this->renderer->AddTriangle ( ImVec2 ( ( this->alpha_bb_min.x + this->alpha_bb_max.x ) / 2.f - 0.5f, alpha_right_triangle.y ), ImVec2 ( alpha_right_triangle.x, alpha_right_triangle.y + 3.5f ), ImVec2 ( alpha_right_triangle.x, alpha_right_triangle.y - 3.5f ), ImColor ( 0, 0, 0, 255 ) );
		

			//this->renderer->AddRectFilled ( this->copy_bb_min, this->copy_bb_max, ImColor ( 255, 0, 255, 255 ) );

			auto text_size = ImGui::CalcTextSize ( ICON_COPY, 12.f, ui::font_icons );
			this->renderer->AddText ( ui::font_icons, 12.f, ImVec2 ( (( this->copy_bb_min.x + this->copy_bb_max.x ) / 2.f - text_size.x / 2.f), ( this->copy_bb_min.y + this->copy_bb_max.y ) / 2.f - text_size.y / 2.f ), ImColor(255, 255, 255, 200), ICON_COPY );

			auto text_size2 = ImGui::CalcTextSize ( ICON_PASTE, 12.f, ui::font_icons );
			this->renderer->AddText ( ui::font_icons, 12.f, ImVec2 ( ( ( this->paste_bb_min.x + this->paste_bb_max.x ) / 2.f - text_size2.x / 2.f ), ( this->paste_bb_min.y + this->paste_bb_max.y ) / 2.f - text_size2.y / 2.f ), ImColor ( 255, 255, 255, 200 ), ICON_PASTE );

			auto text_size_hex = ImGui::CalcTextSize ( this->hex.c_str() );

			//this->renderer->AddText ( ui::font_widgets, 11.f, ImVec2 ( this->text_bb_min.x + 5.f, (this->text_bb_min.y + text_size_hex.y / 4.f )), ImColor ( 255, 255, 255, 200 ), this->hex.c_str() );

			this->renderer->AddText ( ui::font_widgets, 12.f, ImVec2 ( this->text_bb_min.x + 5.f, (( this->text_bb_min.y + this->text_bb_max.y ) / 2.f) - 4.6f ), *this->color, this->hex.c_str ( ) );
		}
	}
	unsigned long colorpicker::create_hex ( int r, int g, int b ) {
		return ( ( r & 0xff ) << 16 ) + ( ( g & 0xff ) << 8 ) + ( b & 0xff );
	}
	void colorpicker::picker_gradient ( ) {
		static ImColor hueColors [ 7 ] =
		{
			{ 255, 0, 0   },
			{ 255, 255, 0 },
			{ 0, 255, 0   },
			{ 0, 255, 255 },
			{ 0, 0, 255   },
			{ 255, 0, 255 },
			{ 255, 0, 0   }
		};

		/* Render hue bar */
		
		
		ImColor picker;

		ImGui::ColorConvertHSVtoRGB ( 1.f - this->colHSV [ 0 ], 1.f, 1.f, picker.Value.x, picker.Value.y, picker.Value.z ); picker.Value.w = 255;
		
		this->renderer->AddRectFilledMultiColor ( this->picker_bb_min, this->picker_bb_max, ImColor ( 255, 255, 255, 255 ), picker, picker, ImColor ( 255, 255, 255, 255 ) );
		this->renderer->AddRectFilledMultiColor ( this->picker_bb_min, this->picker_bb_max, ImColor ( 0, 0, 0, 0 ), ImColor ( 0, 0, 0, 0 ), ImColor ( 0, 0, 0, 255 ), ImColor ( 0, 0, 0, 255 ) );
	

		for ( auto i = 0; i < 6; ++i ) {

			auto temp_hue_min = ImVec2 ( this->hue_bb_min.x, int ( hue_bb_min.y + i * ( float ( this->picker_height ) / 6.f ) ) );
		    auto temp_hue_max = ImVec2 ( this->hue_bb_max.x, int ( hue_bb_min.y + ( i + 1 ) * ( float ( this->picker_height ) / 6.f ) ) );

			this->renderer->AddRectFilledMultiColor ( temp_hue_min, temp_hue_max, hueColors [ i ], hueColors [ i ], hueColors [ i + 1 ], hueColors [ i + 1 ] );

		}
	

		
		this->renderer->AddRectFilled ( this->text_bb_min, this->text_bb_max, ImColor ( 46, 49, 52, 45 ), 3.5f );

		this->renderer->AddRect ( this->text_bb_min, this->text_bb_max, ImColor ( 255, 255, 255, 15 ), 3.5f );
		//this->renderer->AddRectFilled ( this->text_bb_min, this->text_bb_max, ImColor ( 255, 0, 0, 255 ) );
	//	this->renderer->AddText ( ui::font_icons, 6.f, ImVec2 ( ( this->text_bb_min.x + this->text_bb_max.x ) / 2.f - text_size.x / 2.f, ( this->text_bb_min.y + this->text_bb_max.y ) / 2.f ), *this->color, ICON_FA_PAINT_BRUSH );
	}
	void colorpicker::handle_mouse_input ( ) {
		

		auto mouse_pos = ui::get_cursor ( );

		this->hovering = ( mouse_pos.x > this->mins.x && mouse_pos.y > this->mins.y && mouse_pos.x < this->maxs.x && mouse_pos.y < this->maxs.y );
	}

	void colorpicker::handle_text ( ) {

	}

	template<class T>
	const T & clamp ( const T & x, const T & upper, const T & lower ) {
		return min ( upper, max ( x, lower ) );
	}

	void colorpicker::handle ( ) {

		if ( this->should_reset ) {
			this->animation_step = this->bb_min.x;
			this->should_reset = false;
		}
		if ( this->should_reanimate ) {
			this->in_animation = true;
			this->animation_step = 0.f;
			this->should_reanimate = false;
		}

		if ( !this->can_focus ( ) && !this->is_focused ( ) )
			return;



		



		handle_mouse_input ( );

		if ( this->hovering && key_pressed ( VK_LBUTTON ) ) {
			auto time = ImGui::GetIO ( ).DeltaTime;

			if ( time == this->last_open )
				return;

			this->last_open = time;

			this->open = !this->open;
			if ( this->open )
				this->focus_it ( );
			else
				this->out_of_focus ( );

			return;
		}

	

		if ( !this->open )
			return;

		auto mouse_pos = ui::get_cursor ( );
		bool hovering_bb = ( mouse_pos.x > this->bb_min.x && mouse_pos.y > this->bb_min.y && mouse_pos.x < this->bb_max.x && mouse_pos.y < this->bb_max.y );

		if ( key_pressed ( VK_LBUTTON ) && !hovering_bb ) {
			this->open = false;
			this->out_of_focus ( );
			return;
		}



		bool hovering_picker = ( mouse_pos.x > this->picker_bb_min.x && mouse_pos.y > this->picker_bb_min.y && mouse_pos.x < this->picker_bb_max.x && mouse_pos.y < this->picker_bb_max.y );

		if ( hovering_picker && key_pressed ( VK_LBUTTON ) )
			modifying_picker = true;
		else if ( modifying_picker && key_released ( VK_LBUTTON ) )
			modifying_picker = false;

		
		 bool hovering_hue = !( modifying_alpha || modifying_picker) && ( mouse_pos.x > this->hue_bb_min.x && mouse_pos.y > this->hue_bb_min.y && mouse_pos.x < this->hue_bb_max.x && mouse_pos.y < this->hue_bb_max.y );

		 if ( hovering_hue && key_pressed ( VK_LBUTTON ) )
			 modififing_hue = true;
		 else if ( modififing_hue && key_released ( VK_LBUTTON ) )
			 modififing_hue = false;

		 if ( modififing_hue ) {
		
			 this->colHSV[0] = ( this->hue_bb_max.y - mouse_pos.y  );
			 this->colHSV [ 0 ] /= ( this->hue_bb_max.y - this->hue_bb_min.y  );
			 this->colHSV [ 0 ] = ( this->colHSV[0] * 100.f / 100.f ) * 1.f;

			 this->colHSV [ 0 ] =  std::clamp ( this->colHSV [ 0 ], 0.f, 1.f );

			 ImGui::ColorConvertHSVtoRGB ( 1.f - colHSV [ 0 ], colHSV [ 1 ], 1.f - colHSV [ 2 ], this->color->Value.x, this->color->Value.y, this->color->Value.z );
			 //this->color->Value.w = this->alpha * ( 255 / 255 ) * 255; bugg

			 this->picker_cursor.x = ( colHSV [ 1 ] / 100.0 ) * this->picker_width * 100;
			 this->picker_cursor.y = ( colHSV [ 2 ] / 100.0 ) * this->picker_height * 100;

			 this->color->get_rgb ( );

			 this->hex = rgb2hex ( this->color->rgb.x, this->color->rgb.y, this->color->rgb.z, true );
			 return;
		 }
		 bool hovering_alpha = ( mouse_pos.x > this->alpha_bb_min.x && mouse_pos.y > this->alpha_bb_min.y && mouse_pos.x < this->alpha_bb_max.x && mouse_pos.y < this->alpha_bb_max.y );

		 if ( hovering_alpha && key_pressed ( VK_LBUTTON ) )
			 modifying_alpha = true;
		 else if ( modifying_alpha && key_released ( VK_LBUTTON ) )
			 modifying_alpha = false;

		 if ( modifying_alpha && key_down ( VK_LBUTTON ) ) {

			 this->alpha = ( this->alpha_bb_max.y - mouse_pos.y );
			 this->alpha /= ( this->alpha_bb_max.y - this->alpha_bb_min.y );
			 this->alpha = ( this->alpha * 100.f / 100.f );
			 this->alpha = std::clamp ( this->alpha, 0.f, 1.f );
			 this->color->Value.w = this->alpha;// this->alpha * ( 255 / 255 ) * 255;
			  //this->color->Value.w = 0;
			 return;

		 }
		 if ( modifying_picker  ) {

			 this->colHSV [ 1 ] = ( mouse_pos.x  -  this->picker_bb_min.x);
			 this->colHSV [ 1 ] /= ( this->picker_bb_max.x - this->picker_bb_min.x );
			 this->colHSV [ 1 ] = ( this->colHSV[1] * 100.f / 100.f );

			 this->colHSV [ 2 ] = ( mouse_pos.y - this->picker_bb_min.y );
			 this->colHSV [ 2 ] /= ( this->picker_bb_max.y - this->picker_bb_min.y );
			 this->colHSV [ 2 ] = ( this->colHSV [ 2 ] * 100.f / 100.f ) ;

			 this->colHSV [ 1 ] =  std::clamp ( this->colHSV [ 1 ], 0.f, 1.f );
			 this->colHSV [ 2 ] = std::clamp ( this->colHSV [ 2 ], 0.f, 1.f );

			 ImGui::ColorConvertHSVtoRGB ( 1.f - colHSV [ 0 ], colHSV [ 1 ],  1.f - colHSV [ 2 ], this->color->Value.x, this->color->Value.y, this->color->Value.z ); //this->color->Value.w = this->alpha * ( 255 / 255 ) * 255; //nu mstiu ce e cu asta aici dar de aici pare sa fie bugul

			 this->picker_cursor.x = ( colHSV [ 1 ] / 100.0 ) * this->picker_width * 100;
			 this->picker_cursor.y = ( colHSV [ 2 ] / 100.0 ) * this->picker_height * 100;
		
		    this->color->get_rgb ( );

			this->hex = rgb2hex ( this->color->rgb.x, this->color->rgb.y, this->color->rgb.z, true );

		 }

		 bool hovering_copy = ( mouse_pos.x > this->copy_bb_min.x && mouse_pos.y > this->copy_bb_min.y && mouse_pos.x < this->copy_bb_max.x && mouse_pos.y < this->copy_bb_max.y );
		 bool hovering_paste = ( mouse_pos.x > this->paste_bb_min.x && mouse_pos.y > this->paste_bb_min.y && mouse_pos.x < this->paste_bb_max.x && mouse_pos.y < this->paste_bb_max.y );
		 if ( hovering_copy && key_released ( VK_LBUTTON ) ) {
			 clipboard_copy ( );
		 }
		 else if ( hovering_paste && key_released ( VK_LBUTTON ) ) 			 {
			 this->hex = get_clipboard ( );
			 *this->color = hex2rgb ( this->hex );
		
			 this->picker_cursor.x = ( this->color->Value.y / 100.0 ) * this->picker_width * 100;
			 this->picker_cursor.y = ( this->color->Value.z / 100.0 ) * this->picker_height * 100;

		 }
	}


	std::string colorpicker::get_clipboard ( ) {
		// Try opening the clipboard
		if ( !OpenClipboard ( nullptr ) )
			return "#fffff";

		  // Get handle of clipboard object for ANSI text
			HANDLE hData = GetClipboardData ( CF_TEXT );
		if ( hData == nullptr )
			return "#fffff";

		  // Lock the handle to get the actual text pointer
			char * pszText = static_cast< char * >( GlobalLock ( hData ) );
		if ( pszText == nullptr )
			return "#fffff";

		  // Save text in a string class instance
			std::string text ( pszText );

		// Release the lock
		GlobalUnlock ( hData );

		// Release the clipboard
		CloseClipboard ( );

		return text;
	}
	void colorpicker::clipboard_copy ( ) {
		const size_t len = strlen ( this->hex.c_str() ) + 1;
		HGLOBAL hMem = GlobalAlloc ( GMEM_MOVEABLE, len );
		memcpy ( GlobalLock ( hMem ), this->hex.c_str(), len );
		GlobalUnlock ( hMem );
		OpenClipboard ( 0 );
		EmptyClipboard ( );
		SetClipboardData ( CF_TEXT, hMem );
		CloseClipboard ( );
	}
	void colorpicker::update ( ) {

	
		this->maxs.x = this->parrent->maxs.x - 33;
		this->mins.x = this->maxs.x - 25;

		this->mins.y = ( this->parrent->mins.y + this->parrent->maxs.y ) / 2.f - 8.f;
		this->maxs.y = ( this->parrent->mins.y + this->parrent->maxs.y ) / 2.f + 8.f;

		this->bb_min = this->maxs;
	
	
		this->picker_bb_min.x = this->bb_min.x + 5.f;
		this->picker_bb_min.y = this->bb_min.y + 5.f;

		this->picker_bb_max.x = this->picker_bb_min.x + this->picker_width;
		this->picker_bb_max.y = this->picker_bb_min.y + this->picker_height;



		this->hue_bb_min.x = this->picker_bb_max.x + 4.f;
		this->hue_bb_max.x = this->hue_bb_min.x + 12.f;

		this->hue_bb_min.y = this->picker_bb_min.y;
		this->hue_bb_max.y = this->picker_bb_max.y;


		this->alpha_bb_min.y = this->picker_bb_min.y;
		this->alpha_bb_max.y = this->picker_bb_max.y;

		this->alpha_bb_min.x = this->hue_bb_max.x + 5.f;
		this->alpha_bb_max.x = this->alpha_bb_min.x + 12.f;


		this->picker_bb_max.x = this->picker_bb_min.x + this->picker_width;
		this->picker_bb_max.y = this->picker_bb_min.y + this->picker_height;

		this->text_bb_min = ImVec2 ( this->picker_bb_min.x, this->picker_bb_max.y + 5.f );
		this->text_bb_max = ImVec2 ( this->picker_bb_max.x, this->text_bb_min.y + 15.f );

		this->copy_bb_min = ImVec2 ( this->hue_bb_min.x, this->text_bb_min.y);
		this->copy_bb_max = ImVec2 ( this->hue_bb_max.x, this->text_bb_max.y );

		this->paste_bb_min = ImVec2 ( this->alpha_bb_min.x, this->text_bb_min.y );
		this->paste_bb_max = ImVec2 ( this->alpha_bb_max.x, this->text_bb_max.y );


		this->bb_max = ImVec2 ( this->alpha_bb_max.x + 5.f, this->text_bb_max.y + 5.f);


		this->picker_cursor.x = ( colHSV [ 1 ] / 100.0 ) * this->picker_width * 100;
		this->picker_cursor.y = ( colHSV [ 2 ] / 100.0 ) * this->picker_height * 100;


		this->color->get_rgb ( );

		this->hex = rgb2hex ( this->color->rgb.x, this->color->rgb.y, this->color->rgb.z, true );
	}
}