
#pragma once
#include <iostream>
#include "../includes.h"

#include "../../../../dependencies/utilities/base64.h"
#include "../../../../dependencies/utilities/crypt_str.h"
#include <shlobj.h>
#include <iostream>
#include <fstream>
#include  <iomanip>
#include "../../../../dependencies/utilities/json.hpp"
#include "../config.h"
#include <sstream>
#include <random>
#include <string>
#include <tchar.h>
#undef config

namespace ui {
	unsigned int random_char ( ) {
		std::random_device rd;
		std::mt19937 gen ( rd ( ) );
		std::uniform_int_distribution<> dis ( 0, 255 );
		return dis ( gen );
	}

	std::string generate_hex ( const unsigned int len ) {
		std::stringstream ss;
		for ( auto i = 0; i < len; i++ ) {
			const auto rc = random_char ( );
			std::stringstream hexstream;
			hexstream << std::hex << rc;
			auto hex = hexstream.str ( );
			ss << ( hex.length ( ) < 2 ? '0' + hex : hex );
		}
		return ss.str ( );
	}

	config_box::config_box ( object * parent ) {

		this->parrent = parent;
		this->renderer = this->parrent->renderer;
		this->type = config_box_element;
		
		

		this->parrent->add_children ( this );
		update ( );



	}

	object * config_box::get_raw_pointer ( ) {
		return this->parrent->children.at ( this->index );

	}
	std::vector<std::string> config_box::local_files ( ) {
		std::vector<std::string> files;

		std::string folder;

		auto get_dir = [ &folder ] ( ) -> void {
			static TCHAR path [ MAX_PATH ];

			if ( SUCCEEDED ( SHGetFolderPath ( NULL, CSIDL_APPDATA, NULL, NULL, path ) ) )
				folder = crypt_str ( "C:\\axeus\\" );

			CreateDirectory ( folder.c_str ( ), NULL );
		};

		get_dir ( );


		std::string path = folder + crypt_str ( "/*.json" );
		WIN32_FIND_DATA fd;

		HANDLE hFind = FindFirstFile ( path.c_str ( ), &fd );

		if ( hFind != INVALID_HANDLE_VALUE ) {
			do {
				if ( !( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
					files.push_back ( fd.cFileName );
			} while ( FindNextFile ( hFind, &fd ) );

			FindClose ( hFind );
		}

		return files;

	}

	void config_box::refresh_list ( ) {


		auto files = local_files ( );
		if ( !this->children.empty ( ) ) {
			this->children_index = 0;
			//for ( auto & child : this->children )
			//	delete child;

			this->children.clear ( );
		}

		for ( auto & file : files ) {
			std::string folder, file_in_folder;

			auto get_dir = [ &folder, &file_in_folder, &file ] ( ) ->void {
				static TCHAR path [ MAX_PATH ];

				if ( SUCCEEDED ( SHGetFolderPath ( NULL, CSIDL_APPDATA, NULL, NULL, path ) ) ) {
					folder = crypt_str ( "C:\\axeus\\" );
					file_in_folder = folder + file;
				}

				CreateDirectory ( folder.c_str ( ), NULL );
			};

			get_dir ( );


			std::ifstream in ( file_in_folder );

			nlohmann::json j; in >> j;
			try {
				new config_item ( this, j [ "name" ], j [ "hash" ] );
			}
			catch ( int ex  ) {

			}
		  

			/*config->hash = j [ "hash" ];
			config->cloud = j [ "cloud" ];
			config->name = j [ "name" ];
			config->author = j [ "author" ];*/




		}



	}

	void config_box::load_config ( std::string hash, bool cloud ) {
		static auto find_item = [ ] ( std::vector< config_manager::config_item * > items, std::string name ) -> config_manager::config_item * {
			for ( int i = 0; i < ( int ) items.size ( ); i++ ) //-V202
				if ( !items [ i ]->name.compare ( name ) )
					return items [ i ];

			return nullptr;
		};


		std::string folder, file;
	
		auto get_dir = [ &folder, &file, &hash ] ( ) -> void {
			static TCHAR path [ MAX_PATH ];

			if ( SUCCEEDED ( SHGetFolderPath ( NULL, CSIDL_APPDATA, NULL, NULL, path ) ) ) {
				folder = crypt_str ( "C:\\axeus\\" );
				file = folder + hash + ".json";
			}

			CreateDirectory ( folder.c_str ( ), NULL );
		};

		get_dir ( );

		std::ifstream in ( file );

		nlohmann::json json_data; in >> json_data;


		for ( auto it = json_data["data"].begin ( ); it != json_data [ "data" ].end ( ); ++it ) {
			json j = *it;

			std::string name = j [ crypt_str ( "name" ) ];
			variable_type type = ( variable_type)j [ crypt_str ( "type" ) ].get<int>();

			auto item = find_item ( cfg_manager->items, name );

			if ( item ) {
			
				switch ( type ) {
				case int_type:
				{
					*( int * ) item->pointer = j [ crypt_str ( "value" ) ].get<int> ( );

					json & kb = j [ crypt_str ( "keybind" ) ];
					if ( !kb.is_null ( ) && item->key_bind ) {

						if ( !kb [ crypt_str ( "key" ) ].is_null ( ) )
							 item->key_bind->key = kb [ crypt_str ( "key" ) ].get<int> ( );

						if ( !kb [ crypt_str ( "mode" ) ].is_null ( ) )
							item->key_bind->mode = kb [ crypt_str ( "mode" ) ].get<int> ( );

						if ( !kb [ crypt_str ( "new_value" ) ].is_null ( ) )
							*( int * ) item->key_bind->new_value = kb [ crypt_str ( "new_value" ) ].get<int> ( );
					}

					break;
				}
				case float_type:
				{
					*( float * ) item->pointer = j [ crypt_str ( "value" ) ].get<float> ( );

					json & kb = j [ crypt_str ( "keybind" ) ];
					if ( !kb.is_null ( ) && item->key_bind ) {

						if ( !kb [ crypt_str ( "key" ) ].is_null ( ) )
							 item->key_bind->key = kb [ crypt_str ( "key" ) ].get<int> ( );

						if ( !kb [ crypt_str ( "mode" ) ].is_null ( ) )
							 item->key_bind->mode = kb [ crypt_str ( "mode" ) ].get<int> ( );

						if ( !kb [ crypt_str ( "new_value" ) ].is_null ( ) )
							*( float * ) item->key_bind->new_value = kb [ crypt_str ( "new_value" ) ].get<float> ( );
					}
					break;
				}
				case bool_type:
				{
					*( bool * ) item->pointer = j [ crypt_str ( "value" ) ].get<bool> ( );

					json & kb = j [ crypt_str ( "keybind" ) ];
					if ( !kb.is_null ( ) && item->key_bind ) {

						if ( !kb [ crypt_str ( "key" ) ].is_null ( ) )
							 item->key_bind->key = kb [ crypt_str ( "key" ) ].get<bool> ( );

						if ( !kb [ crypt_str ( "mode" ) ].is_null ( ) )
							 item->key_bind->mode = kb [ crypt_str ( "mode" ) ].get<bool> ( );

						if ( !kb [ crypt_str ( "new_value" ) ].is_null ( ) )
							*( bool * ) item->key_bind->new_value = kb [ crypt_str ( "new_value" ) ].get<bool> ( );
					}
					break;
				}
				case multibox_type:
				{
					auto ptr = static_cast< std::vector <int>* > ( item->pointer );
					ptr->clear ( );

					json ja = json::parse ( j [ crypt_str ( "value" ) ].get<std::string> ( ).c_str ( ) );

					for ( json::iterator it = ja.begin ( ); it != ja.end ( ); ++it )
						ptr->push_back ( *it );

					json & kb = j [ crypt_str ( "keybind" ) ];
					if ( !kb.is_null ( ) && item->key_bind ) {


						if ( !kb [ crypt_str ( "key" ) ].is_null ( ) )
							 item->key_bind->key = kb [ crypt_str ( "key" ) ].get<int> ( );

						if ( !kb [ crypt_str ( "mode" ) ].is_null ( ) )
							 item->key_bind->mode = kb [ crypt_str ( "mode" ) ].get<int> ( );

						if ( !kb [ crypt_str ( "new_value" ) ].is_null ( ) ) {


							auto ptr_new_val = static_cast< std::vector <int>* > ( item->key_bind->new_value );
						
							std::vector<int> values;
							json ja = json::parse ( kb [ crypt_str ( "new_value" ) ].get<std::string> ( ).c_str ( ) );

							for ( json::iterator it = ja.begin ( ); it != ja.end ( ); ++it )
								values.push_back ( *it );

							assert ( values.size ( ) == ptr_new_val->size ( ) );

							for ( size_t i = 0; i < ptr_new_val->size(); i++ ) {
								ptr_new_val->at ( i ) = values.at ( i );
							}
						}
					}

				}
				break;
				case color:
				{
					std::vector<int> a;
					json ja = json::parse ( j [ crypt_str ( "value" ) ].get<std::string> ( ).c_str ( ) );

					for ( json::iterator it = ja.begin ( ); it != ja.end ( ); ++it )
						a.push_back ( *it );

					*( ImColor * ) item->pointer = ImColor ( a [ 0 ], a [ 1 ], a [ 2 ], a [ 3 ] );
					break;
				}
				default:
					break;
				}
			}
		}
	}

	void config_box::save_config ( std::string name, bool cloud, std::string hash ) {
		std::string folder, file;
		const auto hash_name = !hash.empty()? hash : generate_hex ( 12 );

		auto get_dir = [ &folder, &file, &hash_name ] ( ) -> void {
			static TCHAR path [ MAX_PATH ];

			if ( SUCCEEDED ( SHGetFolderPath ( NULL, CSIDL_APPDATA, NULL, NULL, path ) ) ) {
				folder = crypt_str ( "C:\\axeus\\" );
				file = folder + hash_name + ".json";
			}

			CreateDirectory ( folder.c_str ( ), NULL );
		};

		get_dir ( );

		json allJson;

		allJson [ crypt_str ( "hash" ) ] = hash_name;
		allJson [ crypt_str ( "cloud" ) ] = cloud;
		allJson [ crypt_str ( "name" ) ] = name;
		allJson [ crypt_str ( "author" ) ] = "unknown";


		for ( auto it : cfg_manager->items ) {
			json j;

			j [ crypt_str ( "name" ) ] = it->name;
			j [ crypt_str ( "type" ) ] = (int)it->type;

			if ( it->key_bind && it->key_bind->key > 0 ) {
				j [ crypt_str ( "keybind" ) ][ crypt_str ( "key" ) ] = it->key_bind->key;
				j [ crypt_str ( "keybind" ) ][ crypt_str ( "mode" ) ] = it->key_bind->mode;
			}


			switch ( it->type ) {
			case variable_type::bool_type:
				j [ crypt_str ( "value" ) ] = ( bool ) *( bool * ) it->pointer;
				if ( it->key_bind ) 
					j [ crypt_str ( "keybind" ) ][ crypt_str ( "new_value" ) ] = ( bool ) * ( bool * ) it->key_bind->new_value;
				

				break;
			case variable_type::float_type:
				j [ crypt_str ( "value" ) ] = ( bool ) *( bool * ) it->pointer;
				if ( it->key_bind )
					j [ crypt_str ( "keybind" ) ][ crypt_str ( "new_value" ) ] = ( float ) *( float * ) it->key_bind->new_value;


				break;
			case variable_type::int_type:
				j [ crypt_str ( "value" ) ] = ( bool ) *( bool * ) it->pointer;
				if ( it->key_bind )
					j [ crypt_str ( "keybind" ) ][ crypt_str ( "new_value" ) ] = ( int ) *( int * ) it->key_bind->new_value;


				break;
			case variable_type::multibox_type:
			{

				auto & ptr = *( std::vector<int>* )( it->pointer );

				int size = ptr.size ( );

				json ja;
				for ( size_t i = 0; i < size ; i++ ) {
					ja.push_back ( (int)ptr.at(i) );
				}


				j [ crypt_str ( "value" ) ] = ja.dump ( );

				if ( it->key_bind ) {
					auto & ptr_keybind = *( std::vector<int>* )( it->key_bind->new_value );
					json ja_keybind_new_value;
					for ( size_t i = 0; i < ptr_keybind.size(); i++ ) {
						ja_keybind_new_value.push_back ( ( int ) ptr_keybind.at ( i ) );
					}

					j [ crypt_str ( "keybind" ) ][ crypt_str ( "new_value" ) ] = ja_keybind_new_value.dump ( );
				}


				break;
			}
		

			case variable_type::color:
			{
				auto c = *( ImColor * ) ( it->pointer );
				c.get_rgb ( );

				std::vector<int> a = { ( int ) c.rgb.x, ( int ) c.rgb.y, ( int ) c.rgb.z, ( int ) c.rgb.w };
				json ja;

				for ( auto & i : a )
					ja.push_back ( i );

				j [ crypt_str ( "value" ) ] = ja.dump ( );
			}
				break;
			default:
				break;
			}
	

			allJson [ "data" ].push_back ( j );
		}


		std::string data;

		Base64 base64;
		data = allJson.dump ( );

		//base64.encode ( dmp, &data );

		std::ofstream ofs;
		ofs.open ( file + '\0', std::ios::out | std::ios::trunc );

		ofs << std::setw ( 4 ) << data << std::endl;
		ofs.close ( );
	}


	void config_box::draw ( ) {

		this->handle ( );


		//	this->renderer->AddRectFilled ( this->mins, this->maxs, ImColor ( 255, 255, 0, 255 ) );


		this->renderer->PushClipRect ( this->mins, this->maxs, true );
		for ( auto & child : this->children ) {
			//if ( child->maxs.y < this->maxs.y && child->mins.y > this->mins.y )
			child->draw ( );


		}




		auto address = std::addressof ( *this );


	
		this->renderer->AddText ( this->mins, ImColor ( 255, 0, 0, 255 ), std::to_string ( this->_id ).c_str ( ) );
		
		if ( this->should_reload ) {

			this->refresh_list ( );
			this->should_reload = false;
		}

		this->renderer->AddRectFilled ( this->thumb_mins, this->thumb_maxs, ImColor ( 34, 46, 80, 255 ), 3.f );
		this->renderer->PopClipRect ( );
	}
	void config_box::handle_mouse_input ( ) {



		auto mouse_pos = ui::get_cursor ( );

		this->hovering = ( mouse_pos.x > this->mins.x && mouse_pos.y > this->mins.y && mouse_pos.x < this->maxs.x && mouse_pos.y < this->maxs.y );


	}

	void config_box::handle ( ) {

		//if ( ui::focused_item != -1 && ui::focused_item != this->_id )
			//return;


		handle_mouse_input ( );

		ImGuiIO & io = ImGui::GetIO ( );

		const float scroll_speed = ImFloor ( 1296.f * io.DeltaTime );


		if ( this->hovering) {

			if ( this->children.empty ( ) ) 			
			{
				//MessageBox ( NULL,  ( std::to_string ( this->_id ).c_str ( ) ),  ( "message" ), MB_OK | MB_SYSTEMMODAL );
				return;
			}
			this->max_scroll_progress = 60.f * this->children.size ( );


			if ( ImGui::GetIO ( ).MouseWheel > 0.0f && this->scroll_progress > 0.f ) {
				this->scroll_progress -= scroll_speed;

			}
			else if ( ImGui::GetIO ( ).MouseWheel < 0.0f && this->children.back ( )->maxs.y > this->maxs.y ) {
				this->scroll_progress += scroll_speed;

			}
			this->thumb_percent = ( this->scroll_progress / this->max_scroll_progress ) * 100.f;



			update ( );
		}



	}

	void config_box::update ( ) {

		this->mins.x = this->parrent->mins.x + this->padding;
		this->maxs.x = this->parrent->maxs.x - this->padding;

		this->mins.y = this->parrent->mins.y + this->padding;
		this->maxs.y = this->parrent->maxs.y - this->padding;

		this->bb_min = this->mins;
		this->bb_min.y = this->mins.y + 15;


		this->bb_max = this->maxs;
		this->bb_max.y = this->maxs.y - 15;

		float thumb_center_y = this->bb_min.y + this->thumb_percent * ( ( this->bb_max.y - this->bb_min.y ) ) / 100.f;
		this->thumb_mins = ImVec2 ( this->bb_max.x - 8.f, thumb_center_y );
		this->thumb_maxs = ImVec2 ( this->bb_max.x - 2.f, thumb_center_y + 30.f );
		//this->hovering_thumb = ( mouse_pos.x > this->thumb_min.x && mouse_pos.y > this->bb_min.y && mouse_pos.x < this->thumb_max.x && mouse_pos.y < this->bb_max.y );



		for ( auto & children : this->children )
			children->update ( );
	}
}