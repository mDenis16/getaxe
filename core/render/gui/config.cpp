#pragma once

#include "../../../dependencies/utilities/base64.h"
#include "../../../dependencies/utilities/crypt_str.h"
#include <shlobj.h>
#include <renderlib/imgui/imgui.h>
#include <UIFramework/elements/element.h>
#include <iostream>
#include <fstream>
#include  <iomanip>
#include "gui.h"
#include "config.h"

#include <UIFramework/framework_public.h>


bool menu_opened = true;
std::unordered_map <std::string, float [ 4 ]> colors;
config_manager * cfg_manager = new config_manager ( );

#define per_player(x) crypt_str ( player_type + "." + x )

void config_manager::setup ( ) {

	/*
	setup_item ( &config.local.thirdperson, false, "local.thirdperson", &config.local.key_bind_thirdperson );
	setup_item ( &config.local.thirdperson_distance, 0.f, "local.thirdperson.distance", &config.local.key_bind_thirdperson_distance );


	for ( size_t i = 0; i < 2; i++ ) {

		auto player_type = std::string(i > 0 ? "team" : "enemy");

		setup_item ( &config.player_visual [ i ].chams_material, 0, per_player ( "visual.chams_material" ), &config.player_visual [ i ].chams_material_keybind );

		setup_item ( &config.player_visual [ i ].bounding_box, false, per_player ( "visual.bounding_box" ), &config.player_visual [ i ].bound_box_keybind );
		setup_item ( &config.player_visual [ i ].bound_box_color, ImColor(255,0,255), per_player ( "visual.bounding_box_color" ));


		std::vector<int> d( FLAGS_MAX);
	


		setup_item_multiarray ( &config.player_visual [ i ].flags_input, &d, per_player ( "visual.flags_input" ), & config.player_visual [ i ].flags_keybind );
	}*/
	


}
#undef config


void config_manager::add_item ( void * pointer, const char * name, const variable_type & type, key_bind_item* key_bind ) {
	if (key_bind )
	key_bind->value = pointer;
	items.push_back ( new config_item ( std::string ( name ), pointer, type, key_bind ));
}
/*
void config_manager::setup_item ( int * pointer, int value, const std::string & name, key_bind_item * key_bind ) {
	add_item ( pointer, name.c_str ( ), crypt_str ( "int" ) );
	*pointer = value;
}
*/
void config_manager::setup_item ( bool * pointer, bool value, const std::string & name, config_manager::key_bind_item * key_bind ) {
	key_bind->var_type = bool_type;
	add_item ( pointer, name.c_str ( ), variable_type::bool_type, key_bind );
	*pointer = value;
}
void config_manager::setup_item ( float * pointer, float value, const std::string & name, config_manager::key_bind_item * key_bind ) {
	key_bind->var_type = float_type;
	add_item ( pointer, name.c_str ( ), variable_type::float_type, key_bind );
	*pointer = value;
}
void config_manager::setup_item ( int * pointer, int value, const std::string & name, config_manager::key_bind_item * key_bind ) {
	key_bind->var_type = int_type;
	add_item ( pointer, name.c_str ( ), variable_type::int_type, key_bind );
	*pointer = value;
}
void config_manager::setup_item_multiarray ( std::vector<int> * pointer, std::vector<int> * value, const std::string & name, config_manager::key_bind_item * key_bind ) {
	key_bind->var_type = multibox_type;
	add_item ( pointer, name.c_str ( ), variable_type::multibox_type, key_bind );
	*pointer = *value;
	//std::memcpy ( pointer, value, sizeof ( pointer ) );
}
void config_manager::setup_item ( ImColor * pointer, ImColor value, const std::string & name ) {

	add_item ( pointer, name.c_str ( ), variable_type::color, nullptr );
	*pointer = value;
}


/*
void config_manager::setup_item ( float * pointer, float value, const std::string & name ) {
	add_item ( pointer, name.c_str ( ), crypt_str ( "float" ) );
	*pointer = value;
}*/
/*
void config_manager::setup_item ( key_bind * pointer, key_bind value, const std::string & name ) //-V813
{
	add_item ( pointer, name.c_str ( ), crypt_str ( "KeyBind" ) );
	*pointer = value;
} not yet*/
/*
void config_manager::setup_item ( ImColor * pointer, ImColor value, const std::string & name ) {

	add_item ( pointer, name.c_str ( ), crypt_str ( "color" ) );
	*pointer = value;
}

void config_manager::setup_item ( std::vector< int > * pointer, int size, const std::string & name ) {
	add_item ( pointer, name.c_str ( ), crypt_str ( "vector<int>" ) );
	pointer->clear ( );

	for ( int i = 0; i < size; i++ )
		pointer->push_back ( FALSE );
}

void config_manager::setup_item ( std::vector< std::string > * pointer, const std::string & name ) {
	add_item ( pointer, name.c_str ( ), crypt_str ( "vector<string>" ) );
}

void config_manager::setup_item ( std::string * pointer, const std::string & value, const std::string & name ) {
	add_item ( pointer, name.c_str ( ), crypt_str ( "string" ) );
	*pointer = value; //-V820
}
*/
void config_manager::save ( std::string config ) {
	std::string folder, file;

	auto get_dir = [ &folder, &file, &config ] ( ) -> void {
		static TCHAR path [ MAX_PATH ];

		if ( SUCCEEDED ( SHGetFolderPath ( NULL, CSIDL_APPDATA, NULL, NULL, path ) ) ) {
			folder =  crypt_str ( "C:\\axeus\\" );
			file = folder + config + ".cfg";
		}

		CreateDirectory ( folder.c_str ( ), NULL );
	};

	get_dir ( );

	json allJson;
	allJson [ crypt_str ( "cfg_name" ) ] = config;
	allJson [ crypt_str ( "author" ) ] = "unknown";


	for ( auto it : items ) {
		json j;

		j [ crypt_str ( "name" ) ] = it->name;
		j [ crypt_str ( "type" ) ] = it->type;

		/*if 
			( !it->type.compare ( crypt_str ( "int" ) ) )
			j [ crypt_str ( "value" ) ] = ( int ) *( int * ) it->pointer; //-V206
		else if ( !it->type.compare ( crypt_str ( "float" ) ) )
			j [ crypt_str ( "value" ) ] = ( float ) *( float * ) it->pointer;
		else if ( !it->type.compare ( crypt_str ( "bool" ) ) )
			j [ crypt_str ( "value" ) ] = ( bool ) *( bool * ) it->pointer;
	/*	else if ( !it->type.compare ( crypt_str ( "KeyBind" ) ) ) {
			auto k = *( key_bind * ) ( it->pointer );

			std::vector <int> a = { k.key, k.mode };
			json ja;

			for ( auto & i : a )
				ja.push_back ( i );

			j [ crypt_str ( "value" ) ] = ja.dump ( );
		}*//*not implemented yet*/
		/*else if ( !it->type.compare ( crypt_str ( "color" ) ) ) {
			auto c = *( ImColor * ) ( it->pointer );
			c.get_rgb ( );

			std::vector<int> a = { (int)c.rgb.x, ( int ) c.rgb.y, ( int ) c.rgb.z, ( int ) c.rgb.w };
			json ja;

			for ( auto & i : a )
				ja.push_back ( i );

			j [ crypt_str ( "value" ) ] = ja.dump ( );
		}
		else if ( !it->type.compare ( crypt_str ( "vector<int>" ) ) ) {
			auto & ptr = *( std::vector<int>* )( it->pointer );
			json ja;

			for ( auto & i : ptr )
				ja.push_back ( i );

			j [ crypt_str ( "value" ) ] = ja.dump ( );
		}
		else if ( !it->type.compare ( crypt_str ( "vector<string>" ) ) ) {
			auto & ptr = *( std::vector<std::string>* )( it->pointer );
			json ja;

			for ( auto & i : ptr )
				ja.push_back ( i );

			j [ crypt_str ( "value" ) ] = ja.dump ( );
		}
		else if ( !it->type.compare ( crypt_str ( "string" ) ) )
			j [ crypt_str ( "value" ) ] = ( std::string ) * ( std::string * ) it->pointer;
			*/
		allJson["data"].push_back ( j );
	}

	auto get_type = [ ] ( ui::object_type type ) {
		switch ( type ) //-V719
		{
		case ui::object_type::checkbox_element:
			return "bool";
		case ui::object_type::combobox_element:
		case ui::object_type::combo_element:
			return "int";
		case ui::object_type::slider_element:
			return "float";
		case ui::object_type::colorpicker_element:
			return "color";
		}
	};


	std::string data;

	Base64 base64;
	data = allJson.dump ( );

	//base64.encode ( dmp, &data );

	std::ofstream ofs;
	ofs.open ( file + '\0', std::ios::out | std::ios::trunc );

	ofs << std::setw ( 4 ) << data << std::endl;
	ofs.close ( );
}

void config_manager::load ( std::string config, bool load_script_items ) {
	static auto find_item = [ ] ( std::vector< config_item * > items, std::string name ) -> config_item * {
		for ( int i = 0; i < ( int ) items.size ( ); i++ ) //-V202
			if ( !items [ i ]->name.compare ( name ) )
				return items [ i ];

		return nullptr;
	};

	std::string folder, file;

	auto get_dir = [ &folder, &file, &config ] ( ) ->void {
		static TCHAR path [ MAX_PATH ];

		if ( SUCCEEDED ( SHGetFolderPath ( NULL, CSIDL_APPDATA, NULL, NULL, path ) ) ) {
			folder = crypt_str ( "C:\\axeus\\" );
			file = folder + config + ".cfg";
		}

		CreateDirectory ( folder.c_str ( ), NULL );
	};

	get_dir ( );
	std::string data;

	std::ifstream ifs;
	ifs.open ( file + '\0' );

	ifs >> data;
	ifs.close ( );

	if ( data.empty ( ) )
		return;

	Base64 base64;

	std::string decoded_data;
	base64.decode ( data, &decoded_data );

	std::ofstream ofs;
	ofs.open ( file + '\0', std::ios::out | std::ios::trunc );

	ofs << decoded_data;
	ofs.close ( );

	json allJson;

	std::ifstream ifs_final;
	ifs_final.open ( file + '\0' );

	ifs_final >> allJson;
	ifs_final.close ( );

	data = allJson.dump ( );
	//base64.encode ( allJson.dump ( ), &data );

	std::ofstream final_ofs;
	final_ofs.open ( file + '\0', std::ios::out | std::ios::trunc );

	final_ofs << data;
	final_ofs.close ( );

	for ( auto it = allJson.begin ( ); it != allJson.end ( ); ++it ) {
		json j = *it;

		std::string name = j [ crypt_str ( "name" ) ];
		std::string type = j [ crypt_str ( "type" ) ];

		auto script_item = std::count_if ( name.begin ( ), name.end ( ),
			[ ] ( char & c ) {
			return c == '.';
		}
		) >= 2;


			auto item = find_item ( items, name );

			if ( item ) {
				if ( !type.compare ( crypt_str ( "int" ) ) )
					*( int * ) item->pointer = j [ crypt_str ( "value" ) ].get<int> ( ); //-V206
				else if ( !type.compare ( crypt_str ( "float" ) ) )
					*( float * ) item->pointer = j [ crypt_str ( "value" ) ].get<float> ( );
				else if ( !type.compare ( crypt_str ( "bool" ) ) )
					*( bool * ) item->pointer = j [ crypt_str ( "value" ) ].get<bool> ( );
				/*else if ( !type.compare ( crypt_str ( "KeyBind" ) ) ) {
					std::vector<int> a;
					json ja = json::parse ( j [ crypt_str ( "value" ) ].get<std::string> ( ).c_str ( ) );

					for ( json::iterator it = ja.begin ( ); it != ja.end ( ); ++it )
						a.push_back ( *it );

					*( key_bind * ) item->pointer = key_bind ( ( ButtonCode_t ) a [ 0 ], ( key_bind_mode ) a [ 1 ] );
				}*//*not yet*/
				else if ( !type.compare ( crypt_str ( "Color" ) ) ) {
					std::vector<int> a;
					json ja = json::parse ( j [ crypt_str ( "value" ) ].get<std::string> ( ).c_str ( ) );

					for ( json::iterator it = ja.begin ( ); it != ja.end ( ); ++it )
						a.push_back ( *it );

					colors.erase ( item->name );
					*( ImColor * ) item->pointer = ImColor ( a [ 0 ], a [ 1 ], a [ 2 ], a [ 3 ] );
				}
				else if ( !type.compare ( crypt_str ( "vector<int>" ) ) ) {
					auto ptr = static_cast< std::vector <int>* > ( item->pointer );
					ptr->clear ( );

					json ja = json::parse ( j [ crypt_str ( "value" ) ].get<std::string> ( ).c_str ( ) );

					for ( json::iterator it = ja.begin ( ); it != ja.end ( ); ++it )
						ptr->push_back ( *it );
				}
				else if ( !type.compare ( crypt_str ( "vector<string>" ) ) ) {
					auto ptr = static_cast< std::vector <std::string>* > ( item->pointer );
					ptr->clear ( );

					json ja = json::parse ( j [ crypt_str ( "value" ) ].get<std::string> ( ).c_str ( ) );

					for ( json::iterator it = ja.begin ( ); it != ja.end ( ); ++it )
						ptr->push_back ( *it );
				}
				else if ( !type.compare ( crypt_str ( "string" ) ) )
					*( std::string * ) item->pointer = j [ crypt_str ( "value" ) ].get<std::string> ( );
			}
		}
	
}

void config_manager::remove ( std::string config ) {
	std::string folder, file;

	auto get_dir = [ &folder, &file, &config ] ( ) -> void {
		static TCHAR path [ MAX_PATH ];

		if ( SUCCEEDED ( SHGetFolderPath ( NULL, CSIDL_APPDATA, NULL, NULL, path ) ) ) {
			folder = crypt_str ( "C:\\axeus\\" );
			file = folder + config + ".cfg";
		}

		CreateDirectory ( folder.c_str ( ), NULL );
	};

	get_dir ( );

	std::string path = file + '\0';
	std::remove ( path.c_str ( ) );
}
void config_manager::key_bind_item::update_array ( ) {
	if ( !this->old_value ) {
		auto ar_old =  ( std::vector<int>* )this->value;
		this->old_value = new std::vector<int> ( );
		 ( ( std::vector<int>* )this->old_value )->resize ( ar_old->size ( ) );

	}


		if ( ui::key_down ( this->key ) ) {

			

			auto old_value_vector = ( std::vector<int>* )this->old_value;
			auto value_vector = ( std::vector<int>* )this->value;
			auto new_value = ( std::vector<int>* )this->new_value;

			size_t buffer_size = sizeof ( int ) * old_value_vector->size ( );


			if ( !this->was_hold )
				std::memcpy ( old_value_vector->data ( ), value_vector->data ( ), buffer_size );

			std::memcpy ( value_vector->data ( ), new_value->data ( ), buffer_size );


			this->was_hold = true;
		}
		else if ( this->was_hold ) {
			auto old_value_vector = ( std::vector<int>* )this->old_value;
			auto value_vector = ( std::vector<int>* )this->value;
			auto new_value = ( std::vector<int>* )this->new_value;

			size_t buffer_size = sizeof ( int ) * old_value_vector->size ( );

			std::memcpy ( value_vector->data ( ), old_value_vector->data ( ), buffer_size );
			this->was_hold = false;
		}
		

}

void config_manager::key_bind_item::update ( ) {

	if ( this->var_type == multibox_type ) {
		this->update_array ( );
		return;
	}
	
	if ( !this->old_value )
		this->old_value = malloc ( sizeof(this->value) );


	switch ( this->mode ) {
	case 0:
		if ( ui::key_down ( this->key ) ) {

		
			if ( !this->was_hold )
				std::memcpy ( this->old_value, this->value, sizeof ( this->value ) );

			std::memcpy ( this->value, this->new_value, sizeof ( this->new_value ) );


			this->was_hold = true;
		}
		else if ( this->was_hold ) {

			std::memcpy ( this->value, this->old_value, sizeof ( this->value ) );
			this->was_hold = false;
		}
		break;

	case 1:

		if ( ui::key_pressed ( this->key ) ) {

			bool was_active = this->is_active;

			this->is_active = !this->is_active;

			if ( this->is_active ) {
				if ( !was_active )
					std::memcpy ( this->old_value, this->value, sizeof ( this->value ) );

				std::memcpy ( this->value, this->new_value, sizeof ( this->new_value ) );
			}
			else {
				if ( !was_active )
					std::memcpy ( this->old_value, this->new_value, sizeof ( this->value ) );
				std::memcpy ( this->value, this->old_value, sizeof ( this->value ) );
			}
		}


		break;
	default:
		break;
	}
	/*
		if ( ui::key_down ( this->key ) ) {
			
			

			if ( !this->was_hold )
				std::memcpy(this->old_value, this->value, sizeof(this->value));
			
			std::memcpy ( this->value, this->new_value, sizeof ( this->new_value ) );


			this->was_hold = true;
		}
		else if ( this->was_hold ) {

			  std::memcpy ( this->value, this->old_value, sizeof ( this->value ) );
			this->was_hold = false;
		}*/


}