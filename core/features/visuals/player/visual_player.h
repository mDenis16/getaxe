#pragma once
namespace visuals {
	class visual_player : public visual_data {
	public:
		std::string weapon_icon;
		std::string weapon_name;
		vec3_t barrel_start, barrel_end;
		player_visual_base * cfg;

		std::vector<ImVec2> warning_range;

		player_t * player;
		int health;
		int old_health;
		int health_on_change;
		float health_animation_start = 0.f;
		bool animating_health = false;
		bool alive;
		bool is_knife = false;
		bool enemy;
		int type = 0;
		int ammo = 0;
		int money = 0;
		bool taze = false;
		bool kevlar = false;
		bool helmet = false;
		bool scoped = false;
		bool defuser = false;
		bool fake_duck = false;
		int clip = 0;
		int max_clip = 0;
		int last_tick_update = 0;
		float l1_cycle = 0.f;
		int last_tick = 0;
		int act = 0;
		float l1_weight = 0.f;
		std::vector<bone_data> bones;
		std::vector<ImVec2> offscreen_points;
		ImVec2 base_pos;
		ImVec2 barrel_start_w2s, barrel_end_w2s;
		ImVec2 animated_clip_mins;
		ImVec2 animated_clip_maxs;
		int last_flags_update_tickcount = 0;
		bool never_seen = true;
		bool in_animation = false;
		bool bomb_carrier = false;
		std::vector<flag_struct> flags_list;
		float alpha = 255;
		player_info_t player_info;
		bool out_of_pov = false;
		void render_range ( );
		void render_name ( ) ;
		void render_box ( ) ;
		void render_ammo ( ) ;
		void on_update ( ) override;
		void on_render (  ) override;

		void  render_health ( );
		void  render_weapon ( );
		void  render_offscreen ( );
		void render_flags ( );


		void render_skeleton ( );
		void render_barrel ( );
		void visibility_check ( );

		void animate ( );

		void handle_flags ( );

		void invalidate ( ) {
			valid = false;
			out_of_pov = false;
			alpha = 255;
			health = 0;
			alive = false;
			type = 0;
			never_seen = true;
			in_animation = false;
		}
	};
}