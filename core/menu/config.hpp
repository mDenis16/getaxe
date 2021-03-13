

#include <Windows.h>
#include <string>
#include <vector>
#include "../../dependencies/utilities/singleton.hpp"

class c_config : public singleton< c_config > {
public:
	int counts;
	bool save_config ( std::string file_name );
	bool load_config ( std::string file_name );
	bool remove_config ( std::string file_name );
	void create_config ( std::string name );
	std::vector<std::string> get_configs ( );

	/*antiaim*/
	 bool antiaim_enable = false;
	 int antiaim_yaw = 0;
	 int antiaim_pitch = 0;
	 int antiaim_fakelag = 0;
	 bool antiaim_fakelag_flags[5];
	 bool antiaim_3way = false;
	 bool antiaim_antibruteforce = false;
	 bool antiaim_at_targets = false;
	/* end */

	 /*ragebot*/
	 bool ragebot_enabled = false;

	  float ragebot_point_scale = 0.f;
	  float ragebot_head_scale = 0.f;

	  int ragebot_prioritize_hitbox = 0;
	  int ragebot_safe_point = 0;
	  bool ragebot_head_scan = false;
	  bool ragebot_body_scan = false;
	  bool ragebot_feet_scan = false;
	  bool ragebot_arms_scan = false;


	  bool ragebot_auto_fire = false;
	  int ragebot_record_selection = 0;
	  bool ragebot_double_tap = false;
	  bool ragebot_hideshots = false;
	  int ragebot_double_tap_ticks = 12;
	  float ragebot_hitchance = 0.f;
	  float ragebot_min_dmg = 0.f;
	  
	  bool ragebot_autostop [ 3 ];
	  /*end */

	  /*visuals team*/
	   bool visuals_team_enabled = false;
	   bool visuals_team_name = false;
	   bool visuals_team_box = false;
	   bool visuals_team_health = false;
	   bool visuals_team_weapon = false;

	   float visuals_team_name_color [ 4 ] = { 0,0,0, 255 };
	   float visuals_team_box_color [ 4 ] = { 0,24,0, 255 };
	   float visuals_team_health_color [ 4 ] = { 0,24,0, 255 };
	   float visuals_team_weapon_color [ 4 ] = { 0,24,0, 255 };

	   /*end*/

		 /*visuals enemy*/
	   bool visuals_enemy_enabled = false;
	   bool visuals_enemy_name = false;
	   bool visuals_enemy_box = false;
	   bool visuals_enemy_health = false;
	   bool visuals_enemy_weapon = false;
	   bool visuals_enemy_flags[6];

	   float visuals_enemy_name_color [ 4 ] = { 0,0,0, 255 };
	   float visuals_enemy_box_color [ 4 ] = { 0,24,0, 255 };
	   float visuals_enemy_health_color [ 4 ] = { 0,24,0, 255 };
	   float visuals_enemy_weapon_color [ 4 ] = { 0,24,0, 255 };
	   float visuals_enemy_flags_color [ 4 ] = { 0,24, 0, 255 };
	   /*end*/

	   /*
        visuals world modulation
	   */

	   bool visuals_modulation_world_nightmode = false;

	   /*
	   */
	   /*visuals team modulation*/
	    bool visuals_modulation_team_visible = false;
	    bool visuals_modulation_team_xyz = false;
	    bool visuals_modulation_team_glow = false;
	    bool visuals_modulation_team_enabled = false;
		int visuals_modulation_team_material = 0;
	    float visuals_modulation_team_visible_color [ 4 ]= { 0,0,0, 255 };
	    float visuals_modulation_team_xyz_color [ 4 ] = { 0,0,0, 255 };
	    float visuals_modulation_team_glow_color [ 4 ] = { 0,0,0, 255 };

	   /*end*/

		/*
		   
		*/
		   /*visuals enemy modulation*/
		bool visuals_modulation_enemy_visible = false;
		bool visuals_modulation_enemy_xyz = false;
		bool visuals_modulation_enemy_lagcompensation = false;
		bool visuals_modulation_enemy_glow = false;
		bool visuals_modulation_enemy_enabled = false;
		float visuals_modulation_enemy_lagcompensation_color [ 4 ] = { 110, 116, 184, 200 };
		float visuals_modulation_enemy_visible_color [ 4 ] = { 174, 149, 184, 255 };
		float visuals_modulation_enemy_xyz_color [ 4 ] = { 51, 62, 189, 255 };
		float visuals_modulation_enemy_glow_color [ 4 ] = { 114, 64, 133, 255 };
		float visuals_modulation_local_weapon_color [ 4 ] = { 114, 64, 133, 255 };
		float visuals_modulation_local_arms_color [ 4 ] = { 114, 64, 133, 255 };
		int visuals_modulation_enemy_material = 0;
		int visuals_modulation_local_arms_material = 0;
		int visuals_modulation_local_weapon_material = 0;
		bool visuals_modulation_local_weapon = false;
		bool visuals_modulation_local_arms = false;
		bool visuals_modulation_local_enabled = false;
		/*end*/

		/*visuals world*/
		 bool visuals_world_grenade_prediction = false;
		 float visuals_world_grenade_prediction_color [ 4 ] = { 0,24,0, 255 };


		 bool visuals_world_draw_grenades = false;
		 bool visuals_world_draw_weapons = false;
		 bool visuals_world_draw_grenades_proximity = false;
		 bool visuals_world_removals [ 4 ];
		 bool visuals_world_autowall_crosshair = false;
		 float visuals_world_draw_weapons_color [ 4 ] = { 255, 255, 255, 255 };

		/* end*/
		 bool ragebot_resolver = false;
		 bool ragebot_double_tap_filters [ 3 ] ;


		 /*legitbot*/
		 float legitbot_recoil_x = 0.f;
		 float legitbot_recoil_y = 0.f;
		 float legitbot_curve_factor = 0.f;
		 bool legitbot_recoil = false;
		 float legitbot_smooth = 0.f;
		 bool legitbot_enable = false;
		 float legitbot_fov = 0.f;
		 bool triggerbot_enable = false;
		 int triggerbot_delay_between_shots = 0;
		 bool ragebot_extend_lag_compensation = false;
		 int double_tap_key = 0;
		 int hide_shots_key = 0;


		 /*hooks*/
		 bool should_skip_animation_frame_target = true;
		 bool is_hltv_target = true;
		 bool build_transformations_target = true;
		 bool setup_bones = true;
		 bool do_procedural_foot_plant_target = true;
		 bool standard_blending_rules_target = true;
		 bool update_client_side_animation = true;

		 bool abs_angles = false;

		 int real_offset = 0;
		 int fake_offset = 0;
		 int lby_offset = 0;
};

#define config c_config::get ( )