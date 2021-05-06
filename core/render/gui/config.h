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

enum local_chams_model {
	LOCAL,
	DESYNC,
	FAKELAG,
	ARMS,
	WEAPON,
	CHAMS_LOCAL_MAX
};

enum chams_material {
	CHAMS_SOLID,
	CHAMS_FLAT,
	CHAMS_REFLECTIVE,
	CHAMS_GLOW,
	CHAMS_MAX
};

enum removals {
	REMOVALS_SCOPE,
	REMOVALS_ZOOM,
	REMOVALS_SMOKE,
	REMOVALS_FLASH,
	REMOVALS_RECOIL,
	REMOVALS_LANDING_BOB,
	REMOVALS_POSTPROCESSING,
	REMOVALS_FOGS,
	REMOVALS_MAX
};

struct chams_visual {
	ImColor color = ImColor( 255, 255, 255, 255);
	int material = 0;
	bool enable = false;
	config_manager::key_bind_item enable_keybind;
	bool force_z = false;
};

struct glow_visual {
	ImColor color = ImColor ( 255, 255, 255 );
	bool render_when_occluded = true;
	bool render_when_unoccluded = false;
	float bloom_amount = 1.f;
	bool enable = false;
	config_manager::key_bind_item keybind;
};


class player_visual_base {
public:
	player_visual_base ( ){ }

	bool bounding_box = false; 
	config_manager::key_bind_item bound_box_keybind;
	bool dynamic_box = false;

	int box_type = 0;
	ImColor bound_box_color = ImColor ( 255, 0, 0, 255 );
	float bound_box_thickness = 1.f;
	float bound_box_border_thickness = 3.f;
	float bound_box_border_rounding = 0.f;



	glow_visual glow;

	bool health = false;
	config_manager::key_bind_item health_keybind;
	ImColor health_color = ImColor ( 255, 255, 0, 255 );



	bool name = false;
	config_manager::key_bind_item name_keybind;
	ImColor name_color = ImColor ( 255, 0, 0, 255 );
	float name_size = 12.f;
	bool shadow_name = false;
	bool dynamic_name_size = false;
	int name_render_type = 0;

	bool weapon = false;
	config_manager::key_bind_item weapon_keybind;
	ImColor weapon_color = ImColor ( 255, 0, 0, 255 );
	int weapon_mode = 0;

	bool ammo = false;
	config_manager::key_bind_item ammo_keybind;
	ImColor ammo_color = ImColor ( 255, 0, 0, 255 );

	bool dormant = false;

	bool p2p_esp = false;

	bool skeleton = false;
	config_manager::key_bind_item skeleton_keybind;
	ImColor skeleton_color = ImColor ( 200, 15, 15, 255 );

	float elasticty = 1.f;

	bool out_of_pov = false;
	config_manager::key_bind_item out_of_pov_keybind;
	ImColor out_of_pov_color = ImColor ( 255, 200, 21, 225 );
	float out_of_pov_radius = 10.f;
	float out_of_pov_base_size = 13.f;
	float out_of_pov_circle_radius_distance = 13.f;

	

	bool flags = false;
	ImColor flags_color = ImColor ( 255, 255, 255, 255 );
	config_manager::key_bind_item flags_keybind;
	std::vector<int> flags_input;


	bool bullet_impacts = false;
	ImColor bullet_impacts_color = ImColor ( 255, 255, 255, 255 );
	config_manager::key_bind_item bullet_impacts_keybind;

	bool bullet_tracers = false;
	ImColor bullet_tracers_color = ImColor ( 255, 255, 255, 255 );
	config_manager::key_bind_item bullet_tracers_keybind;


	bool taser_range = false;
	ImColor taser_range_color = ImColor ( 255, 255, 255, 255 );
	config_manager::key_bind_item taser_range_keybind;

	bool knife_range = false;
	ImColor knife_range_color = ImColor ( 255, 255, 255, 255 );
	config_manager::key_bind_item knife_range_keybind;


};



class player_visual_local : public player_visual_base {
public:
	chams_visual chams [ CHAMS_LOCAL_MAX ];


	bool thirdperson = false;
	float thirdperson_distance = 80.f;
	config_manager::key_bind_item key_bind_thirdperson;



	float view_model_distance = 25.f;
	config_manager::key_bind_item view_model_distance_distance;


	float view_model_x_offset = 0.f;
	float view_model_y_offset = 0.f;
	float view_model_z_offset = 0.f;
	float view_model_roll = 0.f;

	float aspect_ratio = 0.f;

	bool weapon_spread = false;
	config_manager::key_bind_item weapon_spread_keybind;
	ImColor weapon_spread_color;


	bool weapon_recoil = false;
	config_manager::key_bind_item weapon_recoil_keybind;
	ImColor weapon_recoil_color;
	
	bool weapon_force_corsshair = false;

	bool vulnerable_warning = false;
	config_manager::key_bind_item vulnerable_warning_keybind;
	ImColor vulnerable_warning_color;


	config_manager::key_bind_item removals_keybind;
	std::vector<int> removals_input;
	
	bool interpolate_angles = false;
	float interpolation_amount = 0.f;
};

class player_visual : public player_visual_base {
public:
	bool foot_steps = false;
	config_manager::key_bind_item foot_steps_keybind;
	ImColor foot_steps_color = ImColor ( 255, 200, 21, 225 );

	bool force_radar_reveal = false;
	config_manager::key_bind_item force_radar_reveal_keybind;

	bool view_barrel = false;
	config_manager::key_bind_item view_barrel_keybind;
	ImColor view_barrel_color = ImColor ( 255, 0, 0, 255 );

	chams_visual chams [ CHAMS_MODEL_MAX ];
	bool ragdoll_chams = false;


};

class projectiles_visual {
public:

	bool name = false;
	ImColor name_color = false;
	int name_type = 0;
	config_manager::key_bind_item name_keybind;

	bool bounding_box = false;
	config_manager::key_bind_item bound_box_keybind;
	bool dynamic_box = false;



	int box_type = 0;
	ImColor bound_box_color = ImColor ( 255, 0, 0, 255 );
	float bound_box_thickness = 1.f;
	float bound_box_border_thickness = 3.f;
	float bound_box_border_rounding = 0.f;


	bool grenade_path = false;
	bool grenade_warning = false;

	bool grenade_radius = false;
	bool grenade_radius_warning = false;

	glow_visual glow;



	bool out_of_pov = false;
	config_manager::key_bind_item out_of_pov_keybind;
	ImColor out_of_pov_color = ImColor ( 255, 200, 21, 225 );
	float out_of_pov_radius = 10.f;
	float out_of_pov_base_size = 13.f;
	float out_of_pov_circle_radius_distance = 13.f;

};

class weapon_visual {
public:

	bool name = false;
	ImColor name_color = false;
	int name_type = 0;
	config_manager::key_bind_item name_keybind;

	bool bounding_box = false;
	config_manager::key_bind_item bound_box_keybind;
	bool dynamic_box = false;

	int box_type = 0;
	ImColor bound_box_color = ImColor ( 255, 0, 0, 255 );
	float bound_box_thickness = 1.f;
	float bound_box_border_thickness = 3.f;
	float bound_box_border_rounding = 0.f;




	glow_visual glow;



	bool out_of_pov = false;
	config_manager::key_bind_item out_of_pov_keybind;
	ImColor out_of_pov_color = ImColor ( 255, 200, 21, 225 );
	float out_of_pov_radius = 10.f;
	float out_of_pov_base_size = 13.f;
	float out_of_pov_circle_radius_distance = 13.f;


	weapon_visual ( ) { };
};


struct world_visuals {

	bool fog_controller = false;
	ImColor fog_color = ImColor ( 255, 255, 255, 255 );

	int fog_start = 0;

	bool bloom_controller = false;
	float bloom_scale = 0.f;
	float world_exposure = 0.f;
	float model_ambient = 0.f;

};

struct c_movement {
	bool bhop = false;
	config_manager::key_bind_item bhop_keybind;

	bool auto_strafe = false;
	config_manager::key_bind_item auto_strafe_keybind;

	bool wasd_strafe = false;
	config_manager::key_bind_item wasd_strafe_keybind;

	bool a_strafe = false;
	config_manager::key_bind_item a_strafe_keybind;

	bool circle_strafe = false;
	config_manager::key_bind_item circle_keybind;

	bool z_strafe = false;
	config_manager::key_bind_item z_keybind;


	float z_strafe_frequency = 1.f;
	float z_strafer_distance = 1.f;

	bool slow_walk = false;
	config_manager::key_bind_item slow_walk_keybind;


	bool air_duck = false;

};

struct c_misc {
	c_movement movement;

};

class c_config : public singleton< c_config > {
public:
	c_config ( ) {
		for ( size_t i = 0; i < 2; i++ ) {
			player_visual [ i ].flags_input.resize ( FLAGS_MAX );
		}
		
		local_visual.removals_input.resize ( REMOVALS_MAX );
		local_visual.flags_input.resize ( FLAGS_MAX );
	}

	player_visual player_visual [ 2 ];
	player_visual_local local_visual;
	weapon_visual weapons_visual;
	projectiles_visual projectiles_visual;
	c_misc misc;

	std::vector<std::string> flags_list = { "Money", "Armor", "Kit",  "Scoped", "Flashed", "Fakeduck", "Bomb", "Break LC", "Taser", "Hit", "Exploit", "Ping", "Hostage", "Defusing", "Reload", "Dormant", "Distance" };
	std::vector<std::string> removals_list = {"Scope", "Zoom", "Smoke", "Flash", "Recoil", "Landing bob", "Post processing", "Fogs"};

	std::vector<std::pair<std::string, std::string> >  player_types =
	{
	{ "Team", "f" },
	{ "Enemy", "e" },
	{ "Local", "D" }
	};

};



extern bool menu_opened;
