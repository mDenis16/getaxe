#include "../../helpers/helpers.h"
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
	if (local_player::ptr() && local_player::ptr()->is_alive())
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

	/*if (local_player::ptr() && local_player::ptr()->is_alive())*/
		/*local_player::ptr()->m_bClientSideAnimation() = false;*/

	
	//if (local_player::ptr() && local_player::ptr()->is_alive())
	//{
	//	if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_END)
	//	{

	//		if (!localanim.realState) {
	//			localanim.realState = (anim_state*)(interfaces::memalloc->Alloc(sizeof(anim_state)));
	//			*localanim.realState = *local_player::ptr()->get_anim_state();
	//			//std::memcpy(&localanim.realState, local_player::ptr()->get_anim_state(), sizeof(anim_state));
	//		}

	//		auto state = local_player::ptr()->get_anim_state();


	//		if (state) {

	//			localanim.bUpdateThisFrame = interfaces::globals->tick_count != localanim.lastTick;


	//			if (localanim.bUpdateThisFrame)
	//			{

	//				if (state->m_last_update_frame == interfaces::globals->frame_count) {
	//					state->m_last_update_frame -= 1;
	//				}

	//				std::memcpy(localanim.m_realLayers.data(), local_player::ptr()->anim_layers().data(), sizeof(animationlayer) * 13);

	//				//backup();
	//				//update_globals(local_player::ptr(), local_player::ptr()->get_tick_base() * interfaces::globals->interval_per_tick);
	//				allow_animation[local_player::ptr()->index()] = true;
	//				update_anim_angle(state, local_player::data().lastViewangle);
	//				allow_animation[local_player::ptr()->index()] = false;

	//				if (!interfaces::clientstate->m_choked_commands) {
	//					leexiePR = state->m_foot_yaw;
	//					localanim.properAbsYaw = state->m_foot_yaw;
	//					std::memcpy(localanim.m_realPoses.data(), local_player::ptr()->m_flPoseParameter().data(), sizeof(float) * 24);
	//					validShit = true;
	//				}

	//				*localanim.realState = *local_player::ptr()->get_anim_state();
	//				//memcpy(local_player::ptr()->anim_layers().data(), localanim.m_realLayers.data(), sizeof(animationlayer) * 13);
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
void SetPoseParameter(void* player, int index, float value)
{
	static auto func = utilities::pattern_scan("client.dll", ("E8 ? ? ? ? D9 45 08 5F"));  

	CMDLCacheCriticalSection criticalSection(interfaces::modelcache);
	__asm
	{
		push index
		movss xmm2, value
		mov ecx, player
		call func
	}
}

void animations_manager::animsamp()
{
	
	if (!local_player::ptr()) return;

	if (!local_player::ptr()->is_alive()) return;


 
	static std::array< float, 24 > realPoses;
	static std::array< animationlayer, 13 > realLayers;
	static float rotationReal = 0.f;




		memcpy(realLayers.data(), local_player::ptr()->anim_layers().data(), sizeof(animationlayer) * 13);
		memcpy(realPoses.data(), local_player::ptr()->m_flPoseParameter().data(), sizeof(float) * 24);



		/*allow_animation[local_player::ptr()->index()] = true;
		update_anim_angle(local_player::ptr()->get_anim_state(), local_player::data().lastViewangle);
		allow_animation[local_player::ptr()->index()] = false;*/


	/*	if (!interfaces::clientstate->m_choked_commands) {
			rotationReal = local_player::data().lastViewangle.y;
			std::memcpy(realPoses.data(), local_player::ptr()->m_flPoseParameter().data(), sizeof(float) * 24);
		}*/

	/*	local_player::data().should_animate = false;
	}*/

	
	
	
	memcpy(local_player::ptr()->anim_layers().data(), realLayers.data(), sizeof(animationlayer) * 13);
	std::memcpy(local_player::ptr()->m_flPoseParameter().data(), realPoses.data(), sizeof(float) * 24);



	local_player::ptr()->set_abs_angles(vec3_t(0, local_player::data().lastViewangle.y, 0));


	allow_bones[local_player::ptr()->index()] = true;
	local_player::ptr()->invalidate_bone_cache();
	local_player::ptr()->setup_bones(nullptr, 128, 0x7FF00, local_player::ptr()->simulation_time());
	allow_bones[local_player::ptr()->index()] = false;
}
void animations_manager::on_create_move()
{

	auto animstate = local_player::ptr()->get_anim_state();
	if (!animstate) return;

	
		
	if (animstate->m_iLastClientSideAnimationUpdateFramecount == interfaces::globals->frame_count)
	{
		animstate->m_iLastClientSideAnimationUpdateFramecount -= 1;
	}

	animstate->m_flUpdateTimeDelta = std::max(0.0f, interfaces::globals->cur_time - animstate->m_flLastClientSideAnimationUpdateTime); // negative values possible when clocks on client and server go out of sync..

	update_anim_angle(local_player::ptr()->get_anim_state(), local_player::data().lastViewangle);



	allow_bones[local_player::ptr()->index()] = true;
	local_player::ptr()->setup_bones(nullptr, 128, 0x7FF00, local_player::ptr()->simulation_time());
	allow_bones[local_player::ptr()->index()] = false;

}
void animations_manager::on_frame_stage_after(client_frame_stage_t stage)
{

	if (stage != FRAME_NET_UPDATE_POSTDATAUPDATE_END)return;


	if (!local_player::ptr())
		return;

	if (!local_player::ptr()->is_alive())
		return;

	std::memcpy(localanim.m_realLayers.data(), local_player::ptr()->get_animoverlays(), sizeof(localanim.m_realLayers));
	memcpy(localanim.m_realPoses.data(), local_player::ptr()->m_flPoseParameter().data(), sizeof(float) * 24);

}



