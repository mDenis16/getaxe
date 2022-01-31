#pragma once
#include <atomic>
#include <utility>


class animations_manager
{
public:
	animations_manager() {};
	~animations_manager() {};

	std::array<bool, 64> allow_bones;

	std::array<bool, 64> allow_animation;
	std::array<vec3_t, 64> velocities;
	struct local
	{
		int lastTick = 0;
		bool bUpdateThisFrame = false;
		anim_state* realState = nullptr;
		anim_state* fakeState = nullptr;

		std::array< float, 24 > m_realPoses;
		std::array< animationlayer, 13 > m_realLayers;
		float properAbsYaw = 0.f;

	} localanim;

	void update_local_player();

	void update_anim_angle(anim_state* state, vec3_t ang);

	void on_frame_stage_before(client_frame_stage_t stage);
	void on_frame_stage_after(client_frame_stage_t stage);

	void on_create_move();

	void animsamp();

	void call_before(client_frame_stage_t stage);
	void call_after(client_frame_stage_t stage);
	void on_create_move(bool& sendPacket);

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

	 global_data globals;

	 void backup();
	 void restore();
	 void update_globals(player_t* player, float time);

	struct anim_record
	{
		vec3_t velocity;
		int tickcount;
		vec3_t viewangle;
		bool bSend = false;
	};

	std::vector<anim_record> records;
	std::mutex recordsLock;


};

inline animations_manager anim_manager;