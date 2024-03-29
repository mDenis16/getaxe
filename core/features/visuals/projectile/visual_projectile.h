#pragma once
namespace visuals {
	class visual_projectile : public visual_data {
	public:
		grenade_t * projectile;
		vec3_t initial_origin;
		projectiles_visual * cfg;
		std::vector<vec3_t> path;
		std::vector<ImVec2> points_w2s;
		int owner_type = 0;
		float alpha = 0.f;
		int tick = 0;
		float initial_distance = 0.f;
		float current_step = 0.f;
		float total_distance = 0.f;
		float spawn_curtime = 0.f;
		int last_tick = 0;
		ImVec2 base_pos;
		std::vector<ImVec2> offscreen_points;

		bool should_animate_alpha = true;
	
		float frame_time = 0.f;
		float progress = 0.f;
		float explode_time = 0.f;
		int current_indx = 0;
		float path_ratio = 0.f;
		float radius = 27.f;
		bool pulse = false;
		float pulse_value = 1.f;

		int last_tickcount = 0;

		int  start_tick = 0;
		float current_distance = 0.f;
		std::string grenade_icon;
		void render_name ( ) ;
		void render_box ( ) ;

		void render_warning ( );
		void render_path ( );

		void render_offscreen ( );

		void on_update ( ) override;
		void on_render ( ) override;


		void reset ( ) {
			valid = false;
			path.clear ( );
			initial_origin = vec3_t ( );
			projectile = nullptr;
			index = 0;
		}
	};
}