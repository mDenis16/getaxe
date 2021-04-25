#pragma once
#define MENU 1
#define GAME 0

#include <limits>
#include <unordered_map>
#include <array>
#include <algorithm>
#include <vector>
#include "../../../dependencies/utilities/json.hpp"
#include "../../../dependencies/imgui/imgui.h"
#include <Windows.h>
#include <string>
#include <vector>
#include "../../../dependencies/utilities/singleton.hpp"

/*
*/


using json = nlohmann::json;

enum variable_type {
	int_type,
	float_type,
	bool_type,
	multibox_type,
	color
};


class config_manager {
public:
	

	class key_bind_item {
	public:
		void * new_value;
		void * value;
		int mode = 0;
		int key = 0;
		bool was_hold = false;
		bool is_active = false;
		void* old_value;

		variable_type var_type;
		key_bind_item ( ) {

		}
		
		void update ( );
		void update_array ( );
	};

	class config_item {
	public:
		std::string name;
		void * pointer;
		variable_type type;
		key_bind_item* key_bind = nullptr;

		config_item ( std::string name, void * pointer, variable_type type, key_bind_item* _key_bind )
		{
			this->key_bind = _key_bind; 
			this->name = name; 
			this->pointer = pointer;
			this->type = type; 
		}
	};

	void add_item ( void * pointer, const char * name, const variable_type & type, key_bind_item * key_bind );

	void setup_item ( bool * pointer, bool value, const std::string & name, config_manager::key_bind_item * key_bind );

	void setup_item ( float * pointer, float value, const std::string & name, config_manager::key_bind_item * key_bind );

	void setup_item ( int * pointer, int value, const std::string & name, config_manager::key_bind_item * key_bind );

	void setup_item_multiarray ( std::vector<int> * pointer, std::vector<int> * value, const std::string & name, config_manager::key_bind_item * key_bind );

	void setup_item ( ImColor * pointer, ImColor value, const std::string & name );

	std::vector <config_item *> items;

	config_manager ( ) {
		setup ( );
	};

	void setup ( );
	void save ( std::string config );
	void load ( std::string config, bool load_script_items );
	void remove ( std::string config );
	std::vector<std::string> files;
	
};

extern config_manager * cfg_manager;



#define config c_config::get ( )

enum player_visual_type {
	ENEMY,
	TEAM
};



struct player_visual {

	bool bounding_box = false;
	ImColor bound_box_color = ImColor ( 255, 0, 0, 255 );
	config_manager::key_bind_item bound_box_keybind;

	bool glow = false;
	ImColor glow_color = ImColor ( 255, 255, 0, 255 );

	bool health = false;


	bool name = false;
	ImColor name_color = ImColor ( 255, 0, 0, 255 );

	bool weapon = false;

	int weapon_mode = 0;

	bool ammo = false;
	ImColor ammo_color = ImColor ( 255, 0, 0, 255 );

	bool dormant = false;

	bool p2p_esp = false;

	bool skeleton = false;
	ImColor skeleton_color = ImColor ( 200, 15, 15, 255 );

	int chams_material = 0;
	config_manager::key_bind_item chams_material_keybind;

	bool visible_chams = false;
	ImColor visible_chams_color = ImColor ( 255, 0, 100, 255 );

	bool hidden_chams = false;
	ImColor  hidden_chams_color = ImColor ( 255, 0, 100, 255 );

	bool history_chams = false;
	ImColor history_chams_color = ImColor ( 255, 0, 100, 255 );

	bool out_of_pov = false;
	ImColor out_of_pov_color = ImColor ( 255, 200, 21, 225 );

	bool foot_steps = false;
	ImColor foot_steps_color = ImColor ( 255, 200, 21, 225 );

	bool force_radar_reveal = false;

	std::vector<int> flags = { 0, 0 };



	config_manager::key_bind_item flags_keybind;


};

struct local {

	bool thirdperson = false;
	config_manager::key_bind_item key_bind_thirdperson;


	float thirdperson_distance = 0.f;
	config_manager::key_bind_item key_bind_thirdperson_distance;

};

class c_config : public singleton< c_config > {
public:
	player_visual player_visual [ 2 ];
	local local;

};
