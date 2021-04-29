#include "../features.hpp"

namespace visuals {
	void initialize ( ) {
		

	}
	int string_height ( ID3DXFont * font, const char * string ) {
		RECT rect = RECT ( );
		font->DrawTextA ( nullptr, string, strlen ( string ), &rect, DT_CALCRECT, D3DCOLOR_RGBA ( 0, 0, 0, 0 ) );
		return rect.bottom - rect.top;
	}
	RECT string_rect ( ID3DXFont * font, const char * string ) {
		RECT rect = RECT ( );
		font->DrawTextA ( nullptr, string, strlen ( string ), &rect, DT_CALCRECT, D3DCOLOR_RGBA ( 0, 0, 0, 0 ) );
		return rect;
	}

	void primitive_string ( bool borderedl, ID3DXFont * font, float x, float y, ImColor _color, ImColor background, int orientation, const char * input, ... ) {
		char szBuffer [ MAX_PATH ];

		if ( !input )
			return;

		auto b_color = D3DCOLOR_COLORVALUE ( background.Value.x, background.Value.y, background.Value.z, background.Value.w );

		vsprintf_s ( szBuffer, input, ( char * ) &input + _INTSIZEOF ( input ) );

		RECT rect;
		

		switch ( orientation ) {
		case font_left:
		{
			if ( borderedl ) {
				SetRect ( &rect, x - 1, y, x - 1, y );
				font->DrawTextA ( nullptr, szBuffer, -1, &rect, DT_LEFT | DT_NOCLIP, b_color );
				SetRect ( &rect, x + 1, y, x + 1, y );
				font->DrawTextA ( nullptr, szBuffer, -1, &rect, DT_LEFT | DT_NOCLIP, b_color );
				SetRect ( &rect, x, y - 1, x, y - 1 );
				font->DrawTextA ( nullptr, szBuffer, -1, &rect, DT_LEFT | DT_NOCLIP, b_color );
				SetRect ( &rect, x, y + 1, x, y + 1 );
				font->DrawTextA ( nullptr, szBuffer, -1, &rect, DT_LEFT | DT_NOCLIP, b_color );
			}
			SetRect ( &rect, x, y, x, y );
			font->DrawTextA ( nullptr, szBuffer, -1, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_COLORVALUE ( _color.Value.x, _color.Value.y, _color.Value.z,_color.Value.w) );
		} break;
		case font_center:
		{
			y -= string_height ( font, szBuffer ) / 2;

			if ( borderedl ) {
				SetRect ( &rect, x - 1, y, x - 1, y );
				font->DrawTextA ( nullptr, szBuffer, -1, &rect, DT_CENTER | DT_NOCLIP, b_color );
				SetRect ( &rect, x + 1, y, x + 1, y );
				font->DrawTextA ( nullptr, szBuffer, -1, &rect, DT_CENTER | DT_NOCLIP, b_color );
				SetRect ( &rect, x, y - 1, x, y - 1 );
				font->DrawTextA ( nullptr, szBuffer, -1, &rect, DT_CENTER | DT_NOCLIP, b_color );
				SetRect ( &rect, x, y + 1, x, y + 1 );
				font->DrawTextA ( nullptr, szBuffer, -1, &rect, DT_CENTER | DT_NOCLIP, b_color );
			}
			SetRect ( &rect, x, y, x, y );
			font->DrawTextA ( nullptr, szBuffer, -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_COLORVALUE ( _color.Value.x, _color.Value.y, _color.Value.z, _color.Value.w ) );
		} break;
		case font_right:
		{
			if ( borderedl ) {
				SetRect ( &rect, x - 1, y, x - 1, y );
				font->DrawTextA ( nullptr, szBuffer, -1, &rect, DT_RIGHT | DT_NOCLIP, b_color );
				SetRect ( &rect, x + 1, y, x + 1, y );
				font->DrawTextA ( nullptr, szBuffer, -1, &rect, DT_RIGHT | DT_NOCLIP, b_color );
				SetRect ( &rect, x, y - 1, x, y - 1 );
				font->DrawTextA ( nullptr, szBuffer, -1, &rect, DT_RIGHT | DT_NOCLIP, b_color );
				SetRect ( &rect, x, y + 1, x, y + 1 );
				font->DrawTextA ( nullptr, szBuffer, -1, &rect, DT_RIGHT | DT_NOCLIP, b_color );
			}
			SetRect ( &rect, x, y, x, y );
			font->DrawTextA ( nullptr, szBuffer, -1, &rect, DT_RIGHT | DT_NOCLIP, D3DCOLOR_COLORVALUE ( _color.Value.x, _color.Value.y, _color.Value.z, _color.Value.w ) );
		} break;
		}
	}
	void rotate_triangle ( std::array<ImVec2, 3> & points, float rotation ) {

		const auto pointsCenter = ImVec2 ( ( points.at ( 0 ).x + points.at ( 1 ).x + points.at ( 2 ).x ) / 3, ( points.at ( 0 ).y + points.at ( 1 ).y + points.at ( 2 ).y ) / 3 );

		for ( auto & point : points ) {
			point.x -= pointsCenter.x;	point.y -= pointsCenter.y;

			const auto tempX = point.x;
			const auto tempY = point.y;

			const auto theta = DEG2RAD ( rotation );
			const auto c = cos ( theta );
			const auto s = sin ( theta );

			point.x = tempX * c - tempY * s;
			point.y = tempX * s + tempY * c;

			point.x += pointsCenter.x; point.y += pointsCenter.y;
		}
	}
	bool world_to_screen ( const vec3_t & origin, ImVec2 & screen ) {
		vec3_t screen_game = vec3_t ( );
		const auto screen_transform = [ &origin, &screen ] ( ) -> bool {
			static std::uint8_t * matrix;
			if ( !matrix ) {
				matrix = static_cast< std::uint8_t * >( utilities::pattern_scan ( "client.dll", "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9" ) );
				matrix += 3;
				matrix = *reinterpret_cast< std::uint8_t ** >( matrix );
				matrix += 176;
			}

			const matrix3x4_t & w2s_matrix = *reinterpret_cast< matrix3x4_t * >( matrix );
			screen.x = w2s_matrix.m_flMatVal [ 0 ][ 0 ] * origin.x + w2s_matrix.m_flMatVal [ 0 ][ 1 ] * origin.y + w2s_matrix.m_flMatVal [ 0 ][ 2 ] * origin.z + w2s_matrix.m_flMatVal [ 0 ][ 3 ];
			screen.y = w2s_matrix.m_flMatVal [ 1 ][ 0 ] * origin.x + w2s_matrix.m_flMatVal [ 1 ][ 1 ] * origin.y + w2s_matrix.m_flMatVal [ 1 ][ 2 ] * origin.z + w2s_matrix.m_flMatVal [ 1 ][ 3 ];


			float w = w2s_matrix.m_flMatVal [ 3 ][ 0 ] * origin.x + w2s_matrix.m_flMatVal [ 3 ][ 1 ] * origin.y + w2s_matrix.m_flMatVal [ 3 ][ 2 ] * origin.z + w2s_matrix.m_flMatVal [ 3 ][ 3 ];

			if ( w < 0.001f ) {
				screen.x *= 100000;
				screen.y *= 100000;
				return true;
			}

			float invw = 1.f / w;
			screen.x *= invw;
			screen.y *= invw;

			return false;
		};

		if ( !screen_transform ( ) ) {
			int screen_width, screen_height;
			interfaces::engine->get_screen_size ( screen_width, screen_height );

			screen.x = ( screen_width * 0.5f ) + ( screen.x * screen_width ) * 0.5f;
			screen.y = ( screen_height * 0.5f ) - ( screen.y * screen_height ) * 0.5f;

			return true;
		}

		return false;

	}
	bool visual_data::calculate_box(  ) {
		matrix3x4_t & tran_frame = ent->coord_frame ( );

		const vec3_t min = ent->mins ( );
		const vec3_t max = ent->maxs ( );

		ImVec2 screen_boxes [ 8 ];

		vec3_t points [ ] = {
			vec3_t ( min.x, min.y, min.z ),
			vec3_t ( min.x, max.y, min.z ),
			vec3_t ( max.x, max.y, min.z ),
			vec3_t ( max.x, min.y, min.z ),
			vec3_t ( max.x, max.y, max.z ),
			vec3_t ( min.x, max.y, max.z ),
			vec3_t ( min.x, min.y, max.z ),
			vec3_t ( max.x, min.y, max.z )
		};

		for ( int i = 0; i <= 7; i++ )
			if ( !world_to_screen ( math::vector_transform ( points [ i ], tran_frame ), screen_boxes [ i ] ) )
				return false;

		ImVec2 box_array [ ] = {
			screen_boxes [ 3 ], // fl
			screen_boxes [ 5 ], // br
			screen_boxes [ 0 ], // bl
			screen_boxes [ 4 ], // fr
			screen_boxes [ 2 ], // fr
			screen_boxes [ 1 ], // br
			screen_boxes [ 6 ], // bl
			screen_boxes [ 7 ] // fl
		};

		float left = screen_boxes [ 3 ].x, bottom = screen_boxes [ 3 ].y, right = screen_boxes [ 3 ].x, top = screen_boxes [ 3 ].y - 5.f;

		for ( int i = 0; i <= 7; i++ ) {
			if ( left > box_array [ i ].x )
				left = box_array [ i ].x;

			if ( bottom < box_array [ i ].y )
				bottom = box_array [ i ].y;

			if ( right < box_array [ i ].x )
				right = box_array [ i ].x;

			if ( top > box_array [ i ].y )
				top = box_array [ i ].y;
		}

		this->box_data.x = left;
		this->box_data.y = top;
		this->box_data.w = right - left;
		this->box_data.h = bottom - top;

		this->box_data.top_center = ImVec2 ( ( this->box_data.x + this->box_data.w / 2.f ), top );
		this->box_data.bottom_center = ImVec2 ( ( this->box_data.x + this->box_data.w / 2.f ), bottom );

		this->box_data.mins = ImVec2 ( left, top );
		this->box_data.maxs = ImVec2 ( right, bottom );

		return true;
	}

	void on_render ( ) {
		initialize ( );
		
		render = ImGui::GetBackgroundDrawList ( );


		for ( size_t i = 1; i < interfaces::globals->max_clients; i++ ) {

			auto entity = static_cast< player_t * >( interfaces::entity_list->get_client_entity ( i ) );

			auto & data = player_data.at ( i );

			if ( !data.valid )
				continue;

			data.render_entity ( );

		}

	}

	void on_queue ( ) {
		for ( size_t i = 1; i < interfaces::globals->max_clients; i++ ) {
			
			auto entity = static_cast<player_t*>(interfaces::entity_list->get_client_entity ( i ));
			auto & data = player_data.at ( i );

			if ( !entity || !entity->is_alive() ) {
				if ( data.valid )
					data.valid = false;

				data.never_seen = true;
				data.ent = nullptr;
				data.player = nullptr;

				continue;
			}

			data.queue_entity ( entity );
		}
	}
	
}
