#pragma once
namespace visuals {
	class visual_grenade : public visual_data {
	public:
		bool warning = false;
		
	
		bool should_animate_alpha = true;
		grenade_t * grenade;
	     
		projectiles_visual * cfg;

		std::vector<vec3_t> fire_points;
		std::vector<ImVec2> points_w2s;
		std::vector<ImVec2> offscreen_points;
		std::vector<vec3_t> circle_points;

		int owner_type = 0;
		float initial_distance = 0.f;
		float current_step = 0.f;
		float alpha = 0.f;
		float total_distance = 0.f;
		float spawn_curtime = 0.f;
		float frame_time = 0.f;
		float explode_time = 0.f;
		int current_indx = 0;
		float path_ratio = 0.f;
		float lastRadius = 0.f;
		float radius = 27.f;
		float base_radius = 300.f;

		bool pulse = false;
		float pulse_value = 1.f;

		int last_tickcount = 0;

		int  start_tick = 0;
		float current_distance = 0.f;
		std::string grenade_icon;
		
		void render_indicator ( );

		void render_offscreen ( );

		bool is_intersected ( );

		void render_smoke ( );

		void render_inferno ( );

		void on_update ( ) override;
		void on_render ( ) override;


		void reset ( ) {
			valid = false;
			
		}
	};
}