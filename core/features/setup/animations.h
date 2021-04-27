namespace animations {
	struct anim_data {
		bool should_animate = false;
		animationlayer networked_layers [ 15 ];
		std::array< float, 24 > m_poses;
		std::array< float, 24 > m_current_posses;
		std::array< float, 24 > m_old_poses;
		std::array< animationlayer, 13 > m_layers;
		std::array< animationlayer, 13 > m_old_layers;
		bool full_init = false;
		anim_state * fake_anim_state = nullptr;
		std::array< float, 24 > m_fake_poses;
		std::array< animationlayer, 13 > m_fake_layers;
		float last_update_time = 0.f;
		float proper_abs_yaw = 0.f;
		float fake_abs_yaw = 0.f;
		bool init = false;
	};
	struct anim_info {


		vec3_t perdicted_velocity = vec3_t ( );
		vec3_t predicted_origin = vec3_t ( );
		vec3_t simulated_origin = vec3_t ( );
		vec3_t interpolated_origin = vec3_t ( );
		float time_delta = 0.f;
		struct backup_data {
			anim_state state;
			std::array<animationlayer, 13> layers;
			std::array<float, 24U> poses;
		};  backup_data backup;


		struct network_data {
			vec3_t angle = vec3_t ( );
			vec3_t origin = vec3_t ( );
			vec3_t velocity = vec3_t ( );
		};  network_data network;


		bool network_update = false;
		bool outdated_anims = false;

		bool update_anims = false;
		bool update_bones = false;

		bool full_setup = false;


	};

	inline anim_data m_data;
	void update_animations_local ( );
	void update_animations_update_end ( );
	void update_anim_angle ( player_t * ent, anim_state * state, vec3_t ang );
	void predict_velocity ( player_t * player );
	void post_data_end ( player_t * player );
	void update_player_animation ( player_t * player );
	void update_globals ( player_t * player, float time );
	void restore ( );
	void backup ( );
	void update_fake_animation ( );

	void update_local_update_start ( client_frame_stage_t state );

	inline std::array<anim_info, 64> player_data;


	struct global_data {
		float cur_time = 0.f;
		float real_time = 0.f;
		float frame_time = 0.f;
		float abs_frametime = 0.f;
		float absolute_frame_start_time = 0.f;
		float interpolation_amount = 0.f;

		int tickcount = 0;
		int framecount = 0;

	};

	inline global_data globals;
}