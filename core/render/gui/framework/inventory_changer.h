#pragma once

#include <string>
#include <vector>
#include <memory>
#include <queue>
#include <d3d9.h>
#include <mutex>
#include <atomic>


namespace ui {
	void handle_image_data ( );

	extern std::mutex mutex_image_data;
	extern std::atomic<bool> done;


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
		bool failed = false;

	};


	extern std::queue<ui::image_queue*> images_data;

	class paintkit {
	public:
		paintkit ( ) {

		}
		~paintkit( ){ }
		int id = -1;
		int weapon_id = -1;
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
		int index_in_list = -1;
		
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
		void change_page ( int page, int weapon = 0 );
		void painkit_modify ( paintkit * pkit );

		void * archive;

		std::string text;
		std::string search_text;
		int last_search_size = 0;
		int stage = 0;

		std::vector< weapon* > weapons;

		std::vector<object *> original_children;


		int animated_alpha = 255;
		ImVec2 animation_position;
		
		void on_start ( );
		void update_player ( );
		void add_item_to_inventory ( weapon * weap, paintkit * pkit );
		inventory_changer ( object * parent );
		void parse_data ( );

		void draw ( ) override;
		void handle ( ) override;
		void update ( ) override;
		void handle_mouse_input ( ) override;

	};

	
}
