#pragma once
#include <string>
#include <vector>
#include <memory>
#include <d3d9.h>

namespace ui {

	class inventory_item : public object {

	public:

		bool is_glove ( );
		~inventory_item ( );

		std::function<void ( )> call_back;

		bool try_generate = false;
		float create_time = 0.f;
		
		std::string text;
		weapon * weap;
		paintkit * ppaintkit;
		int alpha = 0;
		void * archive;
		bool is_copy = false;
		std::string image_path ;
		IDirect3DTexture9*  image_buffer;
	
		std::string * search_text;

		bool _default = false;

		inventory_item ( object * parent, weapon * weapon, void * _arch, std::function<void ( )> func );
		inventory_item ( object * parent, paintkit * _paintkit, void * _arch );
		inventory_item ( object * parent, paintkit * _paintkit, void * _arch, std::function<void ( )> func );
		inventory_item ( const inventory_item * B ) {
			this->is_copy = true;
			this->call_back = B->call_back;
			this->try_generate = B->try_generate;
			this->create_time = B->create_time;
			this->text = B->text;
			this->weap = B->weap;
			this->ppaintkit = B->ppaintkit;
			this->alpha = B->alpha;
			this->archive = B->archive;
			this->image_path = B->image_path;
			this->image_buffer = B->image_buffer;
			this->_default = B->_default;

			this->mins = B->mins;
			this->maxs = B->maxs;

			this->flags = B->flags;
			this->width = B->width;
			this->height = B->height;

			this->flex = B->flex;
			this->renderer = B->renderer;
			this->parrent = B->parrent;

			this->type = B->type;
			this->index = B->index;
			this->children_index = B->children_index;
			this->hover_start = B->hover_start;
			this->old_mins = B->old_mins;
			this->maxs = B->old_maxs;
			this->_id = B->_id;

		}
		void generate_preview ( );

		void draw ( ) override;
		void handle ( ) override;
		void update ( ) override;
		void handle_mouse_input ( ) override;

	};


}
