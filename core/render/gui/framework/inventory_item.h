#pragma once
#include <string>
#include <vector>
#include <memory>
#include <d3d9.h>

namespace ui {

	class inventory_item : public object {

	public:

		std::string text;
		weapon * weap;
		int alpha = 0;
		void * archive;
		std::string image_path ;
		IDirect3DTexture9*  image_buffer;
	
		bool _default = false;

	
		inventory_item ( object * parent, weapon * weapon, void * _arch );

		void generate_preview ( );

		void draw ( ) override;
		void handle ( ) override;
		void update ( ) override;
		void handle_mouse_input ( ) override;

	};


}
