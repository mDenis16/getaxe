#include "../includes.h"

#include <Windows.h>
#include <iostream>
#include <cmath>
#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream

namespace ImGui
{
	template<int steps>
	void bezier_table ( ImVec2 P [ 4 ], ImVec2 results [ steps + 1 ] ) {
		static float C [ ( steps + 1 ) * 4 ], * K = 0;
		if ( !K ) {
			K = C;
			for ( unsigned step = 0; step <= steps; ++step ) {
				float t = ( float ) step / ( float ) steps;
				C [ step * 4 + 0 ] = ( 1 - t ) * ( 1 - t ) * ( 1 - t );   // * P0
				C [ step * 4 + 1 ] = 3 * ( 1 - t ) * ( 1 - t ) * t; // * P1
				C [ step * 4 + 2 ] = 3 * ( 1 - t ) * t * t;     // * P2
				C [ step * 4 + 3 ] = t * t * t;               // * P3
			}
		}
		for ( unsigned step = 0; step <= steps; ++step ) {
			ImVec2 point = {
				K [ step * 4 + 0 ] * P [ 0 ].x + K [ step * 4 + 1 ] * P [ 1 ].x + K [ step * 4 + 2 ] * P [ 2 ].x + K [ step * 4 + 3 ] * P [ 3 ].x,
				K [ step * 4 + 0 ] * P [ 0 ].y + K [ step * 4 + 1 ] * P [ 1 ].y + K [ step * 4 + 2 ] * P [ 2 ].y + K [ step * 4 + 3 ] * P [ 3 ].y
			};
			results [ step ] = point;
		}
	}

	float BezierValue ( float dt01, float P [ 4 ] ) {
		enum { STEPS = 256 };
		ImVec2 Q [ 4 ] = { { 0, 0 }, { P [ 0 ], P [ 1 ] }, { P [ 2 ], P [ 3 ] }, { 1, 1 } };
		ImVec2 results [ STEPS + 1 ];
		bezier_table<STEPS> ( Q, results );
		return results [ ( int ) ( ( dt01 < 0 ? 0 : dt01 > 1 ? 1 : dt01 ) * STEPS ) ].y;
	}
}
namespace ui {

	bezier_dot::bezier_dot ( object * parent, float & value_x, float & value_y ) {

		this->parrent = parent;
		this->renderer = this->parrent->renderer;
		this->value_x = &value_x;
		this->value_y = &value_y;

		this->parrent->add_children ( this );

		update ( );

	}

	bezier_editor::bezier_editor ( object * parent, float* bezier ) {

		this->value = bezier;

		this->text = text;
		this->parrent = parent;
		this->renderer = this->parrent->renderer;
		this->type = bezier_editor_element;

		height = 30;
		width = ( 85.f / 100.f ) * this->parrent->width;


		this->parrent->add_children ( this );
		update ( );


		new bezier_dot ( this, this->value [ 0 ], this->value [ 1 ] );
		new bezier_dot ( this, this->value [ 2 ], this->value [ 3 ] );

		for ( auto & child : children )
			child->update ( );

	}
	void bezier_editor::draw ( ) {
		handle ( );

		static struct { const char * name; float points [ 4 ]; } presets [ ] = {
		  { "Linear", 0.000f, 0.000f, 1.000f, 1.000f },

		  { "In Sine", 0.470f, 0.000f, 0.745f, 0.715f },
		  { "In Quad", 0.550f, 0.085f, 0.680f, 0.530f },
		  { "In Cubic", 0.550f, 0.055f, 0.675f, 0.190f },
		  { "In Quart", 0.895f, 0.030f, 0.685f, 0.220f },
		  { "In Quint", 0.755f, 0.050f, 0.855f, 0.060f },
		  { "In Expo", 0.950f, 0.050f, 0.795f, 0.035f },
		  { "In Circ", 0.600f, 0.040f, 0.980f, 0.335f },
		  { "In Back", 0.600f, -0.28f, 0.735f, 0.045f },

		  { "Out Sine", 0.390f, 0.575f, 0.565f, 1.000f },
		  { "Out Quad", 0.250f, 0.460f, 0.450f, 0.940f },
		  { "Out Cubic", 0.215f, 0.610f, 0.355f, 1.000f },
		  { "Out Quart", 0.165f, 0.840f, 0.440f, 1.000f },
		  { "Out Quint", 0.230f, 1.000f, 0.320f, 1.000f },
		  { "Out Expo", 0.190f, 1.000f, 0.220f, 1.000f },
		  { "Out Circ", 0.075f, 0.820f, 0.165f, 1.000f },
		  { "Out Back", 0.175f, 0.885f, 0.320f, 1.275f },

		  { "InOut Sine", 0.445f, 0.050f, 0.550f, 0.950f },
		  { "InOut Quad", 0.455f, 0.030f, 0.515f, 0.955f },
		  { "InOut Cubic", 0.645f, 0.045f, 0.355f, 1.000f },
		  { "InOut Quart", 0.770f, 0.000f, 0.175f, 1.000f },
		  { "InOut Quint", 0.860f, 0.000f, 0.070f, 1.000f },
		  { "InOut Expo", 1.000f, 0.000f, 0.000f, 1.000f },
		  { "InOut Circ", 0.785f, 0.135f, 0.150f, 0.860f },
		  { "InOut Back", 0.680f, -0.55f, 0.265f, 1.550f },

		  // easeInElastic: not a bezier
		  // easeOutElastic: not a bezier
		  // easeInOutElastic: not a bezier
		  // easeInBounce: not a bezier
		  // easeOutBounce: not a bezier
		  // easeInOutBounce: not a bezier
		};

		enum { SMOOTHNESS = 64 }; // curve smoothness: the higher number of segments, the smoother curve
		enum { CURVE_WIDTH = 2 }; // main curved line width
		enum { LINE_WIDTH = 1 }; // handlers: small lines width
		enum { GRAB_RADIUS = 6 }; // handlers: circle radius
		enum { GRAB_BORDER = 2 }; // handlers: circle border width

		this->handle ( );

		std::stringstream ss{};

		ss << "cubic-bezier(" << this->value [ 0 ] << "," << this->value [ 1 ] << "," << this->value [ 2 ] << "," << this->value [ 3 ] << ",";

		renderer->AddText ( ui::font_widgets, 13.f, ImVec2 ( this->maxs.x + 20.f, this->mins.y ), ImColor ( 255, 255, 255, 225 ), ss.str().c_str() );
		renderer->AddRectFilled ( this->mins, this->maxs, ImColor ( 25, 125, 123, 15 ) );


		for ( int i = 0; i <= (this->maxs.x - this->mins.x); i += ( (this->maxs.x - this->mins.x) / 4 ) ) 
			renderer->AddLine ( ImVec2 ( this->mins.x + i, this->mins.y ),
				ImVec2 ( this->mins.x + i, this->maxs.y ),
				
ImColor ( 255, 255, 255, 25 ) );

		for ( int i = 0; i <= ( this->maxs.y - this->mins.y ); i += ( ( this->maxs.y - this->mins.y ) / 4 ) )
			renderer->AddLine ( ImVec2 ( this->mins.x, this->mins.y + i),
				ImVec2 ( this->maxs.x, this->mins.y + i ),
				ImColor ( 255, 255, 255, 25 ) );

		float P [ 4 ];

	//	memcpy ( P, presets [ 3 ].points, sizeof ( float ) * 4 );
		ImVec2 Q [ 4 ] = { { 0, 0 }, { this->value [ 0 ], this->value [ 1 ] }, { this->value [ 2 ], this->value [ 3 ] }, { 1, 1 } };
		ImVec2 results [ SMOOTHNESS + 1 ];
		ImGui::bezier_table<SMOOTHNESS> ( Q, results );

		 
	     	ImColor color = ImColor ( 255, 255, 255, 255 );

			for ( int i = 0; i < SMOOTHNESS; ++i ) {
				ImVec2 p = { results [ i + 0 ].x, 1 - results [ i + 0 ].y };
				ImVec2 q = { results [ i + 1 ].x, 1 - results [ i + 1 ].y };
				ImVec2 r ( p.x * ( maxs.x - mins.x ) + mins.x, p.y * ( maxs.y - mins.y ) + mins.y );
				ImVec2 s ( q.x * ( maxs.x - mins.x ) + mins.x, q.y * ( maxs.y - mins.y ) + mins.y );
				renderer->AddLine ( r, s, color, CURVE_WIDTH );
			}
		
	
		
			for ( auto & child : children ) {
				child->draw ( );
			}

	

	}
	void bezier_editor::handle_mouse_input ( ) {
		if ( ui::focused_item != -1 )
			return;

		auto mouse_pos = ui::get_cursor ( );

		this->hovering = ( mouse_pos.x > this->mins.x && mouse_pos.y > this->mins.y && mouse_pos.x < this->maxs.x && mouse_pos.y < this->maxs.y );
	}

	void bezier_editor::handle ( ) {

		handle_mouse_input ( );
		auto mouse_pos = ui::get_cursor ( );

	
		
		
	}


	void bezier_editor::update ( ) {


	

		this->mins.x = this->parrent->mins.x + 30;
		this->maxs.x = this->parrent->maxs.x - 30;

		this->mins.y = this->parrent->mins.y + 50;
		this->maxs.y = this->mins.y + (( this->parrent->maxs.y - this->parrent->mins.y ) / 100 * 60.f);
			

		for ( auto & child : children ) {
			child->update ( );
		}

	}
	void bezier_dot::draw ( ) {
		
		handle ( );

		const ImVec2 circle_center = ImVec2 ( this->mins.x + 5.5f, this->mins.y + 5.5f );

		//renderer->AddRectFilled ( this->mins, this->maxs, ImColor ( 255, 255, 255, 255 ) );
		renderer->AddCircleFilled ( circle_center , 5.5f, ImColor ( this->index > 0 ? 255 : 0, 125, 123, 255 ) );

		if (this->index > 0 )
		   renderer->AddLine ( ImVec2 ( this->parrent->mins.x, this->parrent->maxs.y ), circle_center, ImColor(255, 255, 255,25) );
		else
			renderer->AddLine ( ImVec2 ( this->parrent->maxs.x, this->parrent->mins.y ), circle_center, ImColor ( 255, 255, 255, 25 ) );

	

	}
	void bezier_dot::handle ( ) {

		auto mouse_pos = ui::get_cursor ( );


		this->hovering = ( mouse_pos.x > this->mins.x && mouse_pos.y > this->mins.y && mouse_pos.x < this->maxs.x && mouse_pos.y < this->maxs.y );

		if ( this->hovering && key_down ( VK_LBUTTON ) && this->can_focus ( ) ) {
			this->hold_down = true;
			this->was_hovering = true;
			ui::focused_item = this->_id;
		}
		else if ( key_released ( VK_LBUTTON ) && was_hovering ) 			{
			this->hold_down = false;
			was_hovering = false;
			this->out_of_focus ( );
		}

		if ( this->hold_down ) {
			

			float percent_value_x = ( mouse_pos.x - this->parrent->mins.x - 5.f );
			percent_value_x /= ( this->parrent->maxs.x - this->parrent->mins.x );

			percent_value_x = ( percent_value_x * 100.f / 100 ) ;
			percent_value_x = std::clamp ( percent_value_x, 0.f, 1.f );

			if ( this->index > 0 ) {
				*this->value_x =  std::clamp ( percent_value_x, 0.f, 1.f );
			}
			else {
				*this->value_x =  std::clamp ( percent_value_x, 0.f, 1.f );
			}



			float percent_value_y = ( mouse_pos.y - this->parrent->mins.y - 5.f );
			percent_value_y /= ( this->parrent->maxs.y - this->parrent->mins.y );

			percent_value_y = ( percent_value_y * 100.f / 100 );

			if ( this->index > 0 ) {
				*this->value_y = 1.f - std::clamp ( percent_value_y, 0.f, 1.f );
			}
			else {
				*this->value_y = 1.f - std::clamp ( percent_value_y, 0.f, 1.f );
			}
			
			update ( );

		}

	}
	void bezier_dot::update ( ) {


		if ( this->index > 0 ) {
			this->mins.x = this->parrent->mins.x + ( ( this->parrent->maxs.x - 11.f - this->parrent->mins.x ) * (  *this->value_x ) );
			this->maxs.x = this->mins.x + 5.5f * 2.f;

			this->mins.y = this->parrent->mins.y + ( ( this->parrent->maxs.y - 11.f - this->parrent->mins.y ) * ( 1.f -  *this->value_y ) );
			this->maxs.y = this->mins.y + 5.5f * 2.f;
		}
		else {
			this->mins.x = this->parrent->mins.x + ( ( this->parrent->maxs.x - 11.f - this->parrent->mins.x ) * (  *this->value_x) );
			this->maxs.x = this->mins.x + 5.5f * 2.f;

			this->mins.y = this->parrent->mins.y + ( ( this->parrent->maxs.y - 11.f - this->parrent->mins.y ) * ( 1.f - *this->value_y) );
			this->maxs.y = this->mins.y + 5.5f * 2.f;
		}




	}
	void bezier_dot::handle_mouse_input ( ) {
	}
}