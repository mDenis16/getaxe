#pragma once

#include <string>
#include <vector>
#include <memory>
#include <queue>
#include <d3d9.h>
#include <mutex>

namespace ui {
	void handle_image_data ( );
	extern std::mutex mutex_image_data;

	class image_queue {
	public:
		image_queue ( IDirect3DTexture9 ** _image_buffer, void * _archive, std::string _path_location ) {
			this->image_buffer = _image_buffer;

			this->archive = _archive;
			this->location = _path_location;

			this->done = false;
		}
		void remove ( int index );

		std::string location = "none";
		IDirect3DTexture9 ** image_buffer;
		void * archive;

		bool done = false;

	};

	extern std::vector<image_queue*> images_data;

	class paintkit {
	public:
		int id = -1;
		std::string game_name;
		std::string display_name;
		int rarity;

		std::string image_path;
		std::string weapon_name;

		void generate_image_path ( );


	};

	class weapon {
	public:
		weapon ( int item_definition_index );
		~weapon() {
			for ( auto & paint : paintkits )
		    	delete paint;


			paintkits.clear ( );
		}
		int item_definition_index = -1;
		
        std::string weapon_name;
		std::string display_name;

		std::vector<paintkit*> paintkits;


	};



	class inventory_changer : public object {

	public:

		~inventory_changer ( ) {
			for ( auto & weapon : weapons )
				delete weapon;

	//		delete archive;

			weapons.clear ( );
		}
		void asdad ( );

		void * archive;

		std::string text;
		std::string search_text;
		int stage = 0;

		std::vector< weapon* > weapons;

		int animated_alpha = 255;
		ImVec2 animation_position;
		
		void on_start ( );
		inventory_changer ( object * parent );
		void parse_data ( );

		void draw ( ) override;
		void handle ( ) override;
		void update ( ) override;
		void handle_mouse_input ( ) override;

	};

	
}
