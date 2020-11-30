#include "../features.hpp"
#include "../../helpers/helpers.h"
bool fresh_tick ( ) {
	static int old_tick_count;

	if ( old_tick_count != interfaces::globals->tick_count ) {
		old_tick_count = interfaces::globals->tick_count;
		return true;
	}
}
enum e_anim_state_poses {
	ACT_DIE_STAND = 953,
	ACT_DIE_STAND_HEADSHOT,
	ACT_DIE_CROUCH,
	ACT_DIE_CROUCH_HEADSHOT,
	ACT_NULL,
	ACT_DEFUSE,
	ACT_DEFUSE_WITH_KIT,
	ACT_FLASHBANG_REACTION,
	ACT_FIRE_PRIMARY,
	ACT_FIRE_PRIMARY_OPT_1,
	ACT_FIRE_PRIMARY_OPT_2,
	ACT_FIRE_SECONDARY,
	ACT_FIRE_SECONDARY_OPT_1,
	ACT_FIRE_SECONDARY_OPT_2,
	ACT_RELOAD,
	ACT_RELOAD_START,
	ACT_RELOAD_LOOP,
	ACT_RELOAD_END,
	ACT_OPERATE,
	ACT_DEPLOY,
	ACT_CATCH,
	ACT_SILENCER_DETACH,
	ACT_SILENCER_ATTACH,
	ACT_TWITCH,
	ACT_TWITCH_BUYZONE,
	ACT_PLANT_BOMB,
	ACT_IDLE_ADJUST_TURN,
	ACT_IDLE_ADJUST_STOPPEDMOVING,
	ACT_ALIVE_LOOP,
	ACT_FLINCH,
	ACT_FLINCH_HEAD,
	ACT_FLINCH_MOLOTOV,
	ACT_JUMP,
	ACT_FALL,
	ACT_CLIMB_LADDER,
	ACT_LAND_LIGHT,
	ACT_LAND_HEAVY,
	ACT_EXIT_LADDER_TOP,
	ACT_EXIT_LADDER_BOTTOM
};
enum e_anim_layer {
	ANIMATION_LAYER_AIMMATRIX,
	ANIMATION_LAYER_WEAPON_ACTION,
	ANIMATION_LAYER_WEAPON_ACTION_RECROUCH,
	ANIMATION_LAYER_ADJUST,
	ANIMATION_LAYER_MOVEMENT_JUMP_OR_FALL,
	ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB,
	ANIMATION_LAYER_MOVEMENT_MOVE,
	ANIMATION_LAYER_MOVEMENT_STRAFECHANGE,
	ANIMATION_LAYER_WHOLE_BODY,
	ANIMATION_LAYER_FLASHED,
	ANIMATION_LAYER_FLINCH,
	ANIMATION_LAYER_ALIVELOOP,
	ANIMATION_LAYER_LEAN
};

namespace animations {
	anim_data m_data;
	void update_anim_angle ( anim_state * state, vec3_t ang ) {
		using fn = void ( __vectorcall * )( void *, void *, float, float, float, void * );
		static auto ret = reinterpret_cast< fn >( utilities::pattern_scan ( "client.dll", ( "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24" ) ) );

		if ( !ret || !state )
			return;

		ret ( state, nullptr, 0.f, ang.y, ang.x, nullptr );
	}
	void update_animations ( ) {
		if ( !local_player::m_data.pointer )
			return;

		if ( local_player::m_data.pointer->health ( ) <= 0 )
			return;
		anim_state * state = local_player::m_data.pointer->get_anim_state ( );

		if ( !state )
			return;

		// update time.
		m_data.m_anim_frame = interfaces::globals->cur_time - m_data.m_anim_time;
		m_data.m_anim_time = interfaces::globals->cur_time;

		// CCSGOPlayerAnimState::Update, bypass already animated checks.
		if ( state->m_iLastClientSideAnimationUpdateFramecount == interfaces::globals->frame_count )
			state->m_iLastClientSideAnimationUpdateFramecount -= 1;

		// save updated data.
		m_data.m_speed = state->m_flSpeedNormalized;
		m_data.m_ground = state->m_bOnGround;
	}
	void update_animations_local ( ) {
		if ( !csgo::cmd || !local_player::m_data.alive )
			return;

		auto state = local_player::m_data.pointer->get_anim_state ( );
		if ( !state ) {
			return;
		}

		if ( !interfaces::clientstate ) {
			return;
		}

		// allow re-animating in the same frame.
		if ( state->m_iLastClientSideAnimationUpdateFramecount == interfaces::globals->frame_count ) {
			state->m_iLastClientSideAnimationUpdateFramecount -= 1;
		}
#undef max
		// update anim update delta as server build.
		state->m_flLastClientSideAnimationUpdateTime = std::max ( 0.0f, interfaces::globals->cur_time - state->m_flLastClientSideAnimationUpdateTime ); // negative values possible when clocks on client and server go out of sync..
		static bool init = false;
		if ( m_data.m_animate ) {
			// get layers.
			local_player::m_data.pointer->GetAnimLayers ( m_data.m_real_layers );

			// allow the game to update animations this tick.
			m_data.should_update = true;

			// update animations.
			update_anim_angle ( state, csgo::cmd->viewangles );
			local_player::m_data.pointer->update_client_side_animations ( );

			// disallow the game from updating animations this tick.
			m_data.should_update = false;

			// save data when our choke cycle resets.
			if ( !interfaces::clientstate->m_choked_commands ) {
				m_data.m_rotation.y = state->m_flGoalFeetYaw;
				local_player::m_data.pointer->GetPoseParameters ( m_data.m_real_poses );
			}

			// remove model sway.
			m_data.m_real_layers [ 12 ].weight = 0.f;

			// make sure to only animate once per tick.
			m_data.m_animate = false;
		
		}

		// update our layers and poses with the saved ones.
		local_player::m_data.pointer->SetAnimLayers ( m_data.m_real_layers );
		local_player::m_data.pointer->SetPoseParameters ( m_data.m_real_poses );

		// update our real rotation.
		if ( !m_data.m_rotation.is_zero() )
			local_player::m_data.pointer->set_abs_angles ( m_data.m_rotation );

		m_data.should_setup_bones = true;
		local_player::m_data.pointer->invalidate_bone_cache ( );
		local_player::m_data.pointer->setup_bones ( nullptr, -1, 0x7FF00, interfaces::globals->cur_time );
		m_data.should_setup_bones = false;
	}

}