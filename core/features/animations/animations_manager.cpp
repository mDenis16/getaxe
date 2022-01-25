#include "../features.hpp"

#include "animations_manager.h"

void animations_manager::update_anim_angle(anim_state* state, vec3_t ang) {
	using fn = void(__vectorcall*)(void*, void*, float, float, float, void*);
	static auto ret = reinterpret_cast<fn>(utilities::pattern_scan("client.dll", ("55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3")));

	if (!ret || !state)
		return;

	ret(state, nullptr, 0.f, ang.y, ang.x, nullptr);
}
void animations_manager::update_local_player()
{
	if (local_pointer && local_pointer->is_alive())
	{

	}

}
void animations_manager::backup() {

	globals.cur_time = interfaces::globals->cur_time;
	globals.real_time = interfaces::globals->realtime;
	globals.frame_time = interfaces::globals->frame_time;
	globals.abs_frametime = interfaces::globals->absolute_frametime;
	globals.interpolation_amount = interfaces::globals->interpolation_amount;
	globals.tickcount = interfaces::globals->tick_count;
	globals.absolute_frame_start_time = interfaces::globals->absolute_frame_start_time;
	globals.framecount = interfaces::globals->frame_count;
}

void animations_manager::restore() {
	interfaces::globals->cur_time = globals.cur_time;
	interfaces::globals->realtime = globals.real_time;
	interfaces::globals->frame_time = globals.frame_time;
	interfaces::globals->absolute_frametime = globals.abs_frametime;
	interfaces::globals->interpolation_amount = globals.interpolation_amount;
	interfaces::globals->absolute_frame_start_time = globals.absolute_frame_start_time;
	interfaces::globals->tick_count = globals.tickcount;
	interfaces::globals->frame_count = globals.framecount;
}

static float leexiePR = 0.f;
static bool validShit = false;

void animations_manager::update_globals(player_t* player, float time) {
	static auto host_timescale = interfaces::console->get_convar(("host_timescale"));

	int tick = math::time_to_ticks(time);

	interfaces::globals->cur_time = time;
	interfaces::globals->realtime = time;
	interfaces::globals->frame_time = interfaces::globals->interval_per_tick * host_timescale->get_float();
	interfaces::globals->absolute_frametime = interfaces::globals->interval_per_tick * host_timescale->get_float();
	interfaces::globals->absolute_frame_start_time = time - interfaces::globals->interval_per_tick * host_timescale->get_float();
	interfaces::globals->interpolation_amount = 0;

	interfaces::globals->frame_count = tick;
	interfaces::globals->tick_count = tick;




}
#undef max
void animations_manager::on_frame_stage_before(client_frame_stage_t stage)
{

	//if (local_pointer && local_pointer->is_alive())
	//{
	//	if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_END)
	//	{

	//		if (!localanim.realState) {
	//			localanim.realState = (anim_state*)(interfaces::memalloc->Alloc(sizeof(anim_state)));
	//			*localanim.realState = *local_pointer->get_anim_state();
	//			//std::memcpy(&localanim.realState, local_pointer->get_anim_state(), sizeof(anim_state));
	//		}

	//		auto state = local_pointer->get_anim_state();


	//		if (state) {

	//			localanim.bUpdateThisFrame = interfaces::globals->tick_count != localanim.lastTick;


	//			if (localanim.bUpdateThisFrame)
	//			{

	//				if (state->m_last_update_frame == interfaces::globals->frame_count) {
	//					state->m_last_update_frame -= 1;
	//				}

	//				std::memcpy(localanim.m_realLayers.data(), local_pointer->anim_layers().data(), sizeof(animationlayer) * 13);

	//				//backup();
	//				//update_globals(local_pointer, local_pointer->get_tick_base() * interfaces::globals->interval_per_tick);
	//				allow_animation[local_pointer->index()] = true;
	//				update_anim_angle(state, localdata.lastViewangle);
	//				allow_animation[local_pointer->index()] = false;

	//				if (!interfaces::clientstate->m_choked_commands) {
	//					leexiePR = state->m_foot_yaw;
	//					localanim.properAbsYaw = state->m_foot_yaw;
	//					std::memcpy(localanim.m_realPoses.data(), local_player::m_data.pointer->m_flPoseParameter().data(), sizeof(float) * 24);
	//					validShit = true;
	//				}

	//				*localanim.realState = *local_pointer->get_anim_state();
	//				//memcpy(local_pointer->anim_layers().data(), localanim.m_realLayers.data(), sizeof(animationlayer) * 13);
	//				//restore();

	//			}

	//			localanim.lastTick = interfaces::globals->tick_count;
	//		}
	//	}


	//}
	
}
float SmoothStepBounds(float edge0, float edge1, float x)
{
	float v1 = std::clamp((x - edge0) / (edge1 - edge0), 0.f, 1.f);
	return v1 * v1 * (3 - 2 * v1);
}
enum animstate_pose_param_idx_t
{
	PLAYER_POSE_PARAM_FIRST = 0,
	PLAYER_POSE_PARAM_LEAN_YAW = PLAYER_POSE_PARAM_FIRST,
	PLAYER_POSE_PARAM_SPEED,
	PLAYER_POSE_PARAM_LADDER_SPEED,
	PLAYER_POSE_PARAM_LADDER_YAW,
	PLAYER_POSE_PARAM_MOVE_YAW,
	PLAYER_POSE_PARAM_RUN,
	PLAYER_POSE_PARAM_BODY_YAW,
	PLAYER_POSE_PARAM_BODY_PITCH,
	PLAYER_POSE_PARAM_DEATH_YAW,
	PLAYER_POSE_PARAM_STAND,
	PLAYER_POSE_PARAM_JUMP_FALL,
	PLAYER_POSE_PARAM_AIM_BLEND_STAND_IDLE,
	PLAYER_POSE_PARAM_AIM_BLEND_CROUCH_IDLE,
	PLAYER_POSE_PARAM_STRAFE_DIR,
	PLAYER_POSE_PARAM_AIM_BLEND_STAND_WALK,
	PLAYER_POSE_PARAM_AIM_BLEND_STAND_RUN,
	PLAYER_POSE_PARAM_AIM_BLEND_CROUCH_WALK,
	PLAYER_POSE_PARAM_MOVE_BLEND_WALK,
	PLAYER_POSE_PARAM_MOVE_BLEND_RUN,
	PLAYER_POSE_PARAM_MOVE_BLEND_CROUCH_WALK,
	//PLAYER_POSE_PARAM_STRAFE_CROSS,
	PLAYER_POSE_PARAM_COUNT,
};
void __declspec(naked) feet_wobble_fix()
{
	__asm
	{
		push edi
		xchg dword ptr[esp], edi
		push eax
		mov eax, 77
		mov eax, dword ptr[esp]
		add esp, 4
		pop edi

		cmp esp, 0
		jne fixentity

		_emit 0x88
		_emit 0xFF

		invlpg dword ptr[eax]

		int 2

		fixentity:
		sub esp, 4
			mov dword ptr[esp], ebp

			call cleanup

			pop ebp

			ret

			cleanup :

		ret
	}
}

void animations_manager::on_frame_stage_after(client_frame_stage_t stage)
{
	//if (local_pointer && local_pointer->is_alive())
	//{
	//	if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_END && validShit)
	//	{
	//		std::array< float, 24 > m_backupPoses;
	//		std::array< animationlayer, 13 > m_backupLayers;
	//		anim_state m_backupState;


	//		/*std::memcpy(m_backupPoses.data(), local_player::m_data.pointer->m_flPoseParameter().data(), sizeof(float) * 24);
	//		memcpy(m_backupLayers.data(), local_pointer->anim_layers().data(), sizeof(animationlayer) * 13);
	//		m_backupState = *local_pointer->get_anim_state();
	//		*/

	//		//std::memcpy(local_player::m_data.pointer->m_flPoseParameter().data(), localanim.m_realPoses.data(), sizeof(float) * 24);
	//		memcpy(local_pointer->anim_layers().data(), localanim.m_realLayers.data(), sizeof(animationlayer) * 13);
	//		//*local_pointer->get_anim_state() = *localanim.realState;

	//		


	//		allow_bones[local_pointer->index()] = true;

	//		std::cout << " leexiePR << " << leexiePR << std::endl;

	//		local_pointer->set_abs_angles(vec3_t(0, leexiePR, 0));
	//		
	//		local_player::m_data.pointer->setup_bones(nullptr, 128, 0x7FF00, interfaces::globals->cur_time);


	//	/*	std::memcpy(local_player::m_data.pointer->m_flPoseParameter().data(), m_backupPoses.data(), sizeof(float) * 24);
	//		memcpy(local_pointer->anim_layers().data(), m_backupLayers.data(), sizeof(animationlayer) * 13);
	//		*local_pointer->get_anim_state() = m_backupState;*/

	//		allow_bones[local_pointer->index()] = false;

	//	}

	//}
	if (local_pointer && local_pointer->is_alive()) {
		if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_END) {
			if (!localanim.realState) {
				localanim.realState = (anim_state*)(interfaces::memalloc->Alloc(sizeof(anim_state)));
				*localanim.realState = *local_pointer->get_anim_state();
				//std::memcpy(&localanim.realState, local_pointer->get_anim_state(), sizeof(anim_state));
			}

			auto state = local_pointer->get_anim_state();


			if (state) {

				localanim.bUpdateThisFrame = interfaces::globals->tick_count != localanim.lastTick;


				if (localanim.bUpdateThisFrame)
				{

					if (state->m_last_update_frame == interfaces::globals->frame_count) {
						state->m_last_update_frame -= 1;
					}

					state->m_last_update_increment = std::max(0.0f, interfaces::globals->cur_time - state->m_last_update_time); // negative values possible when clocks on client and server go out of sync..


					std::memcpy(localanim.m_realLayers.data(), local_pointer->anim_layers().data(), sizeof(animationlayer) * 13);


					allow_animation[local_pointer->index()] = true;
					//anim_manager.velocities[local_pointer->index()] = local_pointer->velocity();

					feet_wobble_fix();

					update_anim_angle(state, localdata.lastViewangle);
					allow_animation[local_pointer->index()] = false;



					if (!interfaces::clientstate->m_choked_commands) {
						localanim.properAbsYaw = state->m_abs_yaw;
						std::memcpy(localanim.m_realPoses.data(), local_player::m_data.pointer->m_flPoseParameter().data(), sizeof(float) * 24);
					}


					
					static float flAirTime[65] = { 0.f };
					static float flLastAircheckTime[65] = { -1.f };

					bool bOnGround = localdata.lastFlags & fl_onground;

					if (!bOnGround)
					{
						if (flLastAircheckTime[local_pointer->index()] != -1.f)
						{
							flAirTime[local_pointer->index()] += interfaces::globals->cur_time - flLastAircheckTime[local_pointer->index()];
							localanim.m_realPoses[PLAYER_POSE_PARAM_JUMP_FALL] = std::clamp(SmoothStepBounds(.72f, 1.52f, flAirTime[local_pointer->index()]), 0.f, 1.f);
						}

						flLastAircheckTime[local_pointer->index()] = interfaces::globals->cur_time;
					}
					else
					{
						flAirTime[local_pointer->index()] = 0.f;
						flLastAircheckTime[local_pointer->index()] = -1.f;
						localanim.m_realPoses[PLAYER_POSE_PARAM_JUMP_FALL] = 0.f;
					}



					//std::cout << "  localanim.realState->m_on_ground " << localanim.realState->m_on_ground << " flags on ground " << (local_pointer->flags() & (fl_onground | fl_partialground | fl_conveyor)) << std::endl;

					memcpy(local_pointer->anim_layers().data(), localanim.m_realLayers.data(), sizeof(animationlayer) * 13);


				}
				
				localanim.lastTick = interfaces::globals->tick_count;
			}
		}

		if (stage == FRAME_RENDER_START)
		{

			allow_bones[local_pointer->index()] = true;

            std::array< float, 24 > m_backupPoses;
			std::array< animationlayer, 13 > m_backupLayers;
			anim_state m_backupState;

			std::memcpy(m_backupPoses.data(), local_player::m_data.pointer->m_flPoseParameter().data(), sizeof(float) * 24);
			memcpy(m_backupLayers.data(), local_pointer->anim_layers().data(), sizeof(animationlayer) * 13);
			m_backupState = *local_pointer->get_anim_state();


			std::memcpy(local_player::m_data.pointer->m_flPoseParameter().data(), localanim.m_realPoses.data(), sizeof(float) * 24);
			memcpy(local_pointer->anim_layers().data(), localanim.m_realLayers.data(), sizeof(animationlayer) * 13);
			*local_pointer->get_anim_state() = *localanim.realState;

			/*local_player::m_data.pointer->GetBoneAccessor ( )->m_ReadableBones = local_player::m_data.pointer->GetBoneAccessor ( )->m_WritableBones = 0;

			local_player::m_data.pointer->invalidate_bone_cache();*/

			local_pointer->set_abs_angles(vec3_t(0, localanim.properAbsYaw, 0));
			*reinterpret_cast<int*>(local_pointer + 0xA68) = interfaces::globals->frame_time;
			float frametime = interfaces::globals->frame_time;
			interfaces::globals->frame_time = 23.91753135f; // :^)

			local_player::m_data.pointer->setup_bones(nullptr, 128, 0x7FF00, interfaces::globals->cur_time);

			interfaces::globals->frame_time = frametime;

			allow_bones[local_pointer->index()] = false;


			std::memcpy(local_player::m_data.pointer->m_flPoseParameter().data(), m_backupPoses.data(), sizeof(float) * 24);
			memcpy(local_pointer->anim_layers().data(), m_backupLayers.data(), sizeof(animationlayer) * 13);
			*local_pointer->get_anim_state() = m_backupState;
		}
	}
}



