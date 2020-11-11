#pragma once

namespace variables {
	namespace menu {
		inline bool opened = false;
	}
	namespace cheat {
		inline bool in_setup_bones = false;
		inline int interval_per_tick = 0;
		inline bool antiaim_dirrection = false;
		inline bool should_update_anims = false;
		inline float goal_feet = 0.f;
	
		inline bool in_update_client_side_animation = false;
	}
	namespace antiaim {
		inline bool enable = false;
		inline int yaw = 0;
		inline int pitch = 0;
		inline float jitter = 0;
		inline bool invert_real = false;
		inline bool autoepeek = false;
		inline bool invert_fake = false;
		inline int invert_key = 0;
		inline int fake_mode = 0;
		inline float fake_left = 0.0f;
		inline float fake_right = 0.0f;
		inline float body_lean = 0.0f;
		inline int inverted_body_lean = 0.0f;
		inline int real_jitter = 120.f;
		inline int fake_jitter = 120.f;
		inline int fakelag = 0;
		inline int db_tap_ticks = 12;
		inline bool on_peek = false;
		inline bool moving = false;
		inline bool Hitinit = false;
		inline bool canHit = false;
		inline bool chokeAfterShoot = false;
		inline bool antiaimAfterShoot = false;
		inline bool standing = false;
	
	}
	namespace legit_bot {
		inline bool enable = false;
		inline bool mouse_move = false;
		inline float fov = 0.f;
		inline float smooth_x = 0.f;
		inline float smooth_y = 0.f;
		inline int type = 0;
		inline int rcs_type = 0;
		inline float time_y = 0.f;
		inline float time = 0.f;
		inline float time_x = 0.f;
		inline float rcs = 0.f;
		inline float rcs_x = 0.f;
		inline float rcs_y = 0.f;
		inline int smooth_type = 0;
		inline float cancel_fov = 0.f;
		inline bool hitscan[ 20 ];
		inline int hitbox = 0;
		inline bool backtrack = false;
		inline bool aim_at_backtrack = false;
		inline float backtrack_ms = 0.f;
	}
	namespace ragebot {
		inline float point_scale = 0.f;
		inline float head_scale = 0.f;
		inline bool safe_point = false;
		inline bool resolver = false;
		inline int prioritize_hitbox = 0;
		inline bool head_scan = false;
		inline bool body_scan = false;
		inline bool feet_scan = false;
		inline bool arms_scan = false;
		inline bool prioritize_pitch_flip = false;
		inline bool enabled = false;
		inline bool extrapolate = false;
		inline int prioritize_record = 0;
		inline bool fatal_pitch_flip = false;
		inline int target_selection = 0;
		inline bool double_tap = false;
		inline float hitchance = 0.f;
		inline bool auto_fire = false;
		inline bool silent_aim = false;
		inline bool hitscan [ 20 ];
		inline int auto_peek_ticks = 6;
		inline bool autostop;
		inline bool auto_slow_down = false;
		inline bool multipointhitscan [ 20 ];
		inline float min_dmg = 0.f;
		inline int max_targets = 2;
	}
	namespace visuals {
		namespace enemy {
			inline bool enabled = false;
			inline bool name = false;
			inline bool box = false;
			inline bool health = false;
			inline bool weapon = false;

			inline float name_color [ 4 ] = { 0,0,0, 255 };
			inline float box_color [ 4 ] = { 0,24,0, 255 };
			inline float health_color [ 4 ] = { 0,24,0, 255 };
			inline float weapon_color [ 4 ] = { 0,24,0, 255 };
		}
		namespace team {
			inline bool enabled = false;
			inline bool name = false;
			inline bool box = false;
			inline bool health = false;
			inline bool weapon = false;

			inline float name_color [ 4 ] = { 0,0,0, 255 };
			inline float box_color [ 4 ] = { 0,24,0, 255 };
			inline float health_color [ 4 ] = { 0,24,0, 255 };
			inline float weapon_color [ 4 ] = { 0,24,0, 255 };
		}
		namespace modulation {
			namespace enemy {
				inline bool visible;
				inline bool xyz;
				inline bool glow;
				inline bool enabled = false;
				inline float visible_color [ 4 ];
				inline float xyz_color [ 4 ];
				inline float glow_color [ 4 ];
			}
			namespace team {
				inline bool visible;
				inline bool xyz;
				inline bool glow;
				inline bool enabled = false;
				inline float visible_color [ 4 ];
				inline float xyz_color [ 4 ];
				inline float glow_color [ 4 ];
			}

		}
		/*namespace chams {
			inline int material = 0;
			inline bool view_backtrack = false;
			inline bool enemy_chams = false;
			inline bool teammate_chams = false;
			inline color enemy_color = { 255, 0, 0, 255 };
			inline color enemy_visible_color = { 255, 0, 255, 255 };
			inline color backtrack_colro = { 25, 0, 25, 255 };
			inline color team_color = { 0, 255, 0, 255 };
			inline color team_color_visible = { 55, 255, 33, 255 };
			inline bool xyz = false;
		}*/
	}

	namespace misc {
		inline bool bhop = false;
		namespace removals {
			inline bool smoke = false;
			inline bool flash = false;
		}
	}
}