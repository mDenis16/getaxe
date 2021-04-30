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

enum FLAGS_LIST {
	FLAGS_MONEY,
	FLAGS_ARMOR,
	FLAGS_KIT,
	FLAGS_SCOPED,
	FLAGS_FLASHED,
	FLAGS_FAKEDUCKING,
	FLAGS_C4,
	FLAGS_LC,
	FLAGS_TASER,
	FLAGS_HIT,
	FLAGS_EXPLOIT,
	FLAGS_PING,
	FLAGS_HOSTAGE,
	FLAGS_DEFUSING,
	FLAGS_RELOAD,
	FLAGS_DORMANT,
	FLAGS_DISTANCE,
	FLAGS_MAX
};


enum player_visual_type {
	ENEMY,
	TEAM
};

enum bounding_box_type {
    NORMAL, 
	CORNERED,
    THREE_DIMENSIONAL
};

enum chams_model {
	VISIBLE,
	HIDDEN,
	BACKTRACK,
	HIT,
	ATTACHMENTS,
	CHAMS_MODEL_MAX
};

enum chams_material {
	CHAMS_SOLID,
	CHAMS_FLAT,
	CHAMS_REFLECTIVE,
	CHAMS_GLOW,
	CHAMS_MAX
};


struct chams_visual {
	ImColor color = ImColor( 255, 255, 255, 255);
	int material = 0;
	bool enable = false;
	bool force_z = false;
};

struct player_visual {

	bool bounding_box = false;
	bool dynamic_box = false;

	int box_type = 0;
	ImColor bound_box_color = ImColor ( 255, 0, 0, 255 );
	float bound_box_thickness = 1.f;
	float bound_box_border_thickness = 3.f;
	float bound_box_border_rounding = 0.f;

	config_manager::key_bind_item bound_box_keybind;

	bool glow = false;
	ImColor glow_color = ImColor ( 255, 255, 0, 255 );

	bool health = false;
	ImColor health_color = ImColor ( 255, 255, 0, 255 );

	chams_visual chams [ CHAMS_MODEL_MAX ];
	bool ragdoll_chams = false;

	bool name = false;
	ImColor name_color = ImColor ( 255, 0, 0, 255 );
	float name_size = 12.f;
	bool shadow_name = false;
	bool dynamic_name_size = false;
	int name_render_type = 0;

	bool weapon = false;
	ImColor weapon_color = ImColor ( 255, 0, 0, 255 );
	int weapon_mode = 0;

	bool ammo = false;
	ImColor ammo_color = ImColor ( 255, 0, 0, 255 );

	bool dormant = false;

	bool p2p_esp = false;

	bool skeleton = false;
	ImColor skeleton_color = ImColor ( 200, 15, 15, 255 );

	int chams_material = 0;
	config_manager::key_bind_item chams_material_keybind;



	bool out_of_pov = false;
	ImColor out_of_pov_color = ImColor ( 255, 200, 21, 225 );
	float out_of_pov_radius = 10.f;
	float out_of_pov_base_size = 13.f;
	float out_of_pov_circle_radius_distance = 13.f;
	bool foot_steps = false;
	ImColor foot_steps_color = ImColor ( 255, 200, 21, 225 );

	bool force_radar_reveal = false;

	bool view_barrel = false;
	ImColor view_barrel_color = ImColor ( 255, 0, 0, 255 );

	bool flags = false;

	float slider_a = 1.f;
	float slider_b = 1.f;
	std::vector<int> flags_input;
	
	
	ImColor flags_color = ImColor ( 255, 200, 21, 225 );
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
	c_config ( ) {
		for ( size_t i = 0; i < 2; i++ ) {
			player_visual [ i ].flags_input.resize ( FLAGS_MAX );
		}
	}
	player_visual player_visual [ 2 ]; 
	local local;



	std::vector<std::string> flags_list = { "Money", "Armor", "Kit",  "Scoped", "Flashed", "Fakeduck", "Bomb", "Break LC", "Taser", "Hit", "Exploit", "Ping", "Hostage", "Defusing", "Reload", "Dormant", "Distance" };

	/*
	
		FLAGS_MONEY,
	FLAGS_ARMOR,
	FLAGS_KIT,
	FLAGS_SCOPED,
	FLAGS_FLASHED,
	FLAGS_FAKEDUCKING,
	FLAGS_C4,
	FLAGS_LC,
	FLAGS_TASER,
	FLAGS_HIT,
	FLAGS_EXPLOIT,
	FLAGS_PING,
	FLAGS_HOSTAGE,
	FLAGS_DEFUSING,
	FLAGS_RELOAD,
	FLAGS_DORMANT,
	FLAGS_DISTANCE,
	FLAGS_MAX*/
};



extern bool menu_opened;
