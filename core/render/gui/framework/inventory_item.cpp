#include "../includes.h"

#include <Windows.h>
#include <iostream>
#include <cmath>
#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream
#include <d3dx9.h>
#include "../../../../dependencies/utilities/crypt_str.h"




namespace ui {

	static auto gloves = { 5027, 5030, 5031, 5032,5033, 5034, 5034, 5035 };

	inventory_item::~inventory_item ( ) {

		if ( this->is_copy ) {
			this->image_buffer->Release ( );
			delete this->image_buffer;
		}
	}
	inventory_item::inventory_item ( object * parent, paintkit * _paintkit, void * _arch, std::function<void ( )> func ) {
		this->create_time = ImGui::GetTime ( );
		this->call_back = func;
		this->ppaintkit = _paintkit;
		this->archive = _arch;
		this->parrent = parent;
		this->renderer = this->parrent->renderer;


		this->parrent->add_children ( this );


		update ( );
	}
	inventory_item::inventory_item ( object * parent, paintkit * _paintkit, void * _arch ) {
		this->create_time = ImGui::GetTime ( );

		this->ppaintkit = _paintkit;
		this->archive = _arch;
		this->parrent = parent;
		this->renderer = this->parrent->renderer;


		this->parrent->add_children ( this );


		update ( );
	}
	inventory_item::inventory_item ( object * parent, weapon * weapon, void * _arch, std::function<void ( )> func ) {
		this->create_time = ImGui::GetTime ( );

		this->call_back = func;
		this->weap = weapon;
		this->archive = _arch;
		this->parrent = parent;
		this->renderer = this->parrent->renderer;


		this->parrent->add_children ( this );


		update ( );
	}

	bool inventory_item::is_glove ( ) {
		return this->weap && this->weap->item_definition_index >= 5027 && this->weap->item_definition_index <= 5035;

	}
	void inventory_item::generate_preview ( ) {
		//esource\flash\econ\weapons\base_weapons\weapon_cz75a.png


		if ( !this->try_generate ) {
			std::string preview;

			if ( this->weap ) {
				if ( is_glove ( ) )
					preview = "resource/flash/econ/wearables/gloves/ct_defaultgloves.png";
				else
					preview = "resource/flash/econ/weapons/base_weapons/" + this->weap->weapon_name + ".png";


			}
			else if ( this->ppaintkit )
				preview = this->ppaintkit->image_path;


			while ( !mutex_image_data.try_lock ( ) ) {
				std::this_thread::sleep_for ( std::chrono::milliseconds ( 1 ) );
			}
			ui::images_data.push ( new ui::image_queue ( &this->image_buffer, this->archive, preview ) );
			mutex_image_data.unlock ( );
			this->try_generate = true;
		}

	}

	void inventory_item::draw ( ) {
		handle ( );



		//std::optional<c_vpk_entry> entry = static_cast< c_vpk_archive * >( this->archive )->get_file ( preview );

		/*if ( !this->try_generate && ImGui::GetTime() > (this->create_time + 0.1f * this->index) ) {
			if ( entry ) {

				std::optional<std::vector<uint8_t>> png_data = entry.value ( ).get_data ( );
				if ( png_data.has_value ( ) ) {
				//	D3DXCreateTextureFromFileInMemory ( reinterpret_cast< IDirect3DDevice9 * >( ui::window_device ), ( char * ) png_data.value ( ).data ( ), png_data.value ( ).size ( ), &this->image_buffer );
					D3DXCreateTextureFromFileInMemoryEx ( reinterpret_cast< IDirect3DDevice9 * >( ui::window_device ), ( char * ) png_data.value ( ).data ( ), png_data.value ( ).size ( ),
						200, 200, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, NULL, NULL, &this->image_buffer );

				}
			}

			this->try_generate = true;
		}*/

		this->handle ( );



		this->renderer->AddRectFilled ( this->mins, this->maxs, ImColor ( 27, 28, 31, 255 ), 3.5f );
		this->renderer->AddRect ( this->mins, this->maxs, ImColor ( 255, 255, 255, 15 ), 4.5f );

		if ( this->weap )
			this->renderer->AddText ( ui::font_widgets, 14.f, ImVec2 ( this->mins.x, this->maxs.y + 6 ), ImColor ( 255, 255, 255, 255 ), this->weap->display_name.c_str ( ) );
		else
			this->renderer->AddText ( ui::font_widgets, 14.f, ImVec2 ( this->mins.x, this->maxs.y + 6 ), ImColor ( 255, 255, 255, 255 ), this->ppaintkit->display_name.data ( ) );


		if ( this->image_buffer != nullptr ) {
			ui::mutex_image_data.lock ( );
			this->renderer->AddImage ( this->image_buffer, ImVec2 ( this->mins.x + 15, this->mins.y + 15 ), ImVec2 ( this->maxs.x - 15, this->maxs.y - 15 ) );
			ui::mutex_image_data.unlock ( );
		}
		else {

			renderer->PathClear ( );

			static float const radius = 15;
			ImGuiContext & g = *GImGui;
			float gtime = g.Time + this->index * 0.001f;

			int num_segments = 30;
			int start = abs ( sinf ( gtime ) * ( num_segments - 5 ) );

			const float a_min = IM_PI * 2.0f * ( ( float ) start ) / ( float ) num_segments;
			const float a_max = IM_PI * 2.0f * ( ( float ) num_segments - 3 ) / ( float ) num_segments;

			const ImVec2 centre = ImVec2 ( ( this->maxs.x + this->mins.x ) / 2.f, ( this->maxs.y + this->mins.y ) / 2.f );

			for ( int i = 0; i < num_segments; i++ ) {
				const float a = a_min + ( ( float ) i / ( float ) num_segments ) * ( a_max - a_min );
				renderer->PathLineTo ( ImVec2 ( centre.x + cosf ( a + gtime * 8 ) * radius,
					centre.y + ImSin ( a + gtime * 8 ) * radius ) );
			}

			renderer->PathStroke ( ImColor ( 255, 255, 255, 125 ), false, 3.f );

		}
		//this->renderer->AddShadowRect ( this->mins, this->maxs, ImColor ( 255, 255, 255, 255 ), 15.f, ImVec2 ( 0, 0 ), ImDrawShadowFlags_CutOutShapeBackground );

		//for ( auto & child : children ) {
	//		child->draw ( );
	//	}


		generate_preview ( );


	}
	void inventory_item::handle_mouse_input ( ) {


		auto mouse_pos = ui::get_cursor ( );
		this->hovering = ( mouse_pos.x > this->mins.x && mouse_pos.y > this->mins.y && mouse_pos.x < this->maxs.x && mouse_pos.y < this->maxs.y );
	}

	void inventory_item::handle ( ) {

		if ( ui::focused_item != -1 || static_cast< child_window * >( this->parrent )->modifying_thumb )
			return;


		this->renderer->AddRect ( this->parrent->mins, this->parrent->maxs, ImColor ( 255, 255, 255, 255 ) );


		if ( this->maxs.y > this->parrent->mins.y && this->mins.y < this->parrent->maxs.y ) {
			handle_mouse_input ( );

			if ( ui::key_pressed ( VK_LBUTTON ) && this->hovering ) {
				if ( this->weap || this->ppaintkit ) {
					this->call_back ( );
				}
			}
		}

	}


	void inventory_item::update ( ) {


		if ( this->flags & big_inventory_item ) {
			this->width = 180;
			this->height = 144;
			this->padding = 12;

			this->mins.x = this->parrent->mins.x + ( this->parrent->maxs.x - ( this->parrent->mins.x + this->width ) ) / 2.f;

			this->maxs.x = this->mins.x + this->width;

			this->mins.y = this->parrent->mins.y + 20;
			this->maxs.y = this->mins.y + this->height;

		}
		else {
			this->width = this->width > 800 ?  this->parrent->width * 0.22f : 119;
			this->height = this->width > 800 ? this->parrent->width * 0.19f : 96;
			this->padding = 12;

			int max_elements_on_line = ( this->parrent->maxs.x - this->parrent->mins.x ) / this->width;

			max_elements_on_line = ( ( ( max_elements_on_line - 1 ) * this->padding ) + this->width * max_elements_on_line ) / (this->width + 24);


			if ( this->index == 0 ) {
				float end_point = this->parrent->mins.x + ( ( max_elements_on_line - 1 ) * this->padding ) + this->width * max_elements_on_line;
				float start_point = ( this->parrent->maxs.x - end_point ) / 2.f;

				this->mins.x = this->parrent->mins.x + start_point;
				this->mins.y = this->parrent->mins.y - static_cast< ui::child_window * >( this->parrent )->scroll_progress + padding;

				this->maxs.x = this->mins.x + this->width;
				this->maxs.y = this->mins.y + this->height;

			}
			else {
				auto & last = this->parrent->children.at ( this->index - 1 );



				this->mins.x = last->maxs.x + this->padding;
				this->maxs.x = this->mins.x + this->width;

				this->mins.y = last->mins.y;
				this->maxs.y = this->mins.y + this->height;

				if ( this->maxs.x > this->parrent->maxs.x && this->index >= max_elements_on_line ) {

					auto & first_in_row = this->parrent->children.at ( this->index - max_elements_on_line );
					this->mins.y = first_in_row->maxs.y + 55;
					this->maxs.y = this->mins.y + this->height;
					this->mins.x = first_in_row->mins.x;
					this->maxs.x = this->mins.x + this->width;
				}

			}
		}
		for ( auto & child : children ) {
			child->update ( );
		}
	}
}