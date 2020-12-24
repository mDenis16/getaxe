#include "../features.hpp"

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
	std::array<anim_info, 64> player_data;
	void update_anim_angle ( anim_state * state, vec3_t ang ) {
		using fn = void ( __vectorcall * )( void *, void *, float, float, float, void * );
		static auto ret = reinterpret_cast< fn >( utilities::pattern_scan ( "client.dll", ( "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24" ) ) );

		if ( !ret || !state )
			return;

		ret ( state, nullptr, 0.f, ang.y, ang.x, nullptr );
	}

	void post_data_end ( player_t * player ) {
		auto & anim = player_data [ player->index ( ) ];
		if ( player->simulation_time ( ) != player->get_old_simulation_time ( ) ) {
			anim.last_networked_angle = player->eye_angles ( );
			anim.last_networked_origin = player->origin ( );
			anim.last_networked_duck_amount = player->duck_amount ( );
			anim.network_update = true;
		}
	}

	vec3_t predict_position ( player_t * player ) {
		auto & anim = player_data [ player->index ( ) ];
		return anim.predicted_origin = anim.last_networked_origin + ( player->velocity ( ) * ( interfaces::globals->interval_per_tick * ( float ) math::time_to_ticks ( player->simulation_time ( ) - player->get_old_simulation_time ( ) ) ) );
	}
	void __declspec( naked ) feet_wobble_fix ( ) {
		__asm
		{
			push edi
			xchg dword ptr [ esp ], edi
			push eax
			mov eax, 77
			mov eax, dword ptr [ esp ]
			add esp, 4
			pop edi

			cmp esp, 0
			jne fixentity

			_emit 0x88
			_emit 0xFF

			invlpg dword ptr [ eax ]

			int 2

			fixentity:
			sub esp, 4
				mov dword ptr [ esp ], ebp

				call cleanup

				pop ebp

				ret

				cleanup :

			ret
		}
	}
	void update_player_animation ( player_t * player ) {

		auto & anim = player_data [ player->index ( ) ];



		const auto o_eflags = player->get_ieflags ( );
		auto animstate = player->get_anim_state ( );






		if ( anim.last_tick != interfaces::globals->tick_count ) {

			animstate->m_force_update = true;
			animstate->m_last_clientside_anim_framecount = 0;

			predict_position ( player );

			float flTimeDelta = player->simulation_time ( ) - player->get_old_simulation_time ( );
			vec3_t newVelo = ( anim.predicted_origin - anim.last_networked_origin ) / flTimeDelta;

			anim.interpolated_origin = math::lerp ( anim.last_networked_origin, anim.predicted_origin, flTimeDelta );

			player->get_abs_velocity ( ) = newVelo;
			player->get_ieflags ( ) &= ~4096;

			float predicted_duck = anim.last_networked_duck_amount + ( interfaces::globals->frame_time * ( interfaces::globals->interval_per_tick * ( float ) math::time_to_ticks ( player->simulation_time ( ) - player->get_old_simulation_time ( ) ) ) );
	       // player->duck_amount ( ) = std::lerp ( anim.last_networked_duck_amount, predicted_duck, flTimeDelta );

			anim.update_anims = true;
			csgo::m_update = true;
			feet_wobble_fix ( );
			update_anim_angle ( animstate, anim.last_networked_angle );
			player->update_client_side_animations ( );
			csgo::m_update = false;
			anim.update_anims = false;

	
				float feet_yaw = resolver::resolver_data [ player->index ( ) ].goal_feet_yaw;
				
					if ( resolver::resolver_data [ player->index ( ) ].side == resolver::desync_side::left )
						feet_yaw -= resolver::resolver_data [ player->index ( ) ].brute_angle;
					else if ( resolver::resolver_data [ player->index ( ) ].side == resolver::desync_side::right )
						feet_yaw += resolver::resolver_data [ player->index ( ) ].brute_angle;
			
				if (  resolver::resolver_data [ player->index ( ) ].side > 0 )
					player->set_abs_angles ( vec3_t ( 0, feet_yaw, 0.f ) );

			
			std::memcpy ( anim.m_layers.data ( ), player->get_animoverlays ( ), sizeof ( anim.m_layers ) );
			std::memcpy ( anim.m_poses.data ( ), player->m_flPoseParameter ( ).data ( ), sizeof ( anim.m_poses ) );

		
			player->get_ieflags ( ) = o_eflags;
			anim.init = true;
		}


		if ( anim.init ) {
			std::memcpy ( player->get_animoverlays ( ), anim.m_layers.data ( ), sizeof ( anim.m_layers ) );
			std::memcpy ( player->m_flPoseParameter ( ).data ( ), anim.m_poses.data ( ), sizeof ( anim.m_poses ) );
		}

		vec3_t back_origin = player->abs_origin ( );
		player->set_abs_origin ( anim.interpolated_origin );
		anim.update_bones = true;
		player->setup_bones ( nullptr, 128, 0x7FF00, interfaces::globals->cur_time );
		anim.update_bones = false;
	
		anim.last_tick = interfaces::globals->tick_count;

	}
	void update_animations_local ( ) {



		if ( !interfaces::engine->is_in_game ( ) )
			return;

		if ( !interfaces::engine->is_connected ( ) )
			return;

		local_player::m_data.pointer = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( interfaces::engine->get_local_player ( ) ) );
		csgo::local_player = local_player::m_data.pointer;
		if ( !local_player::m_data.pointer )
			return;

		if ( !csgo::cmd || !local_player::m_data.pointer->is_alive ( ) )
			return;

		/* vars */
		const auto o_eflags = local_player::m_data.pointer->get_ieflags ( );
		auto animstate = local_player::m_data.pointer->get_anim_state ( );


		if ( !animstate ) return;



		std::memcpy ( m_data.m_layers.data ( ), local_player::m_data.pointer->get_animoverlays ( ), sizeof ( m_data.m_layers ) );


		animstate->m_force_update = true;
		animstate->m_last_clientside_anim_framecount = 0;


		local_player::m_data.pointer->get_ieflags ( ) &= ~4096;
		local_player::m_data.pointer->get_abs_velocity ( ) = local_player::m_data.pointer->velocity ( );

		m_data.init = true;



		m_data.should_animate = true;
		update_anim_angle ( local_player::m_data.pointer->get_anim_state ( ), csgo::cmd->viewangles	 );
		local_player::m_data.pointer->update_client_side_animations ( );
		m_data.should_animate = false;

		local_player::m_data.pointer->get_ieflags ( ) = o_eflags;

		if ( !interfaces::clientstate->m_choked_commands ) {
			m_data.proper_abs_yaw = animstate->m_abs_yaw;
			std::memcpy ( m_data.m_poses.data ( ), local_player::m_data.pointer->m_flPoseParameter ( ).data ( ), sizeof ( m_data.m_poses ) );
		}






		//		local_player::m_data.pointer->invalidate_bone_cache ( );
			//	local_player::m_data.pointer->setup_bones ( nullptr, 128, 0x7FF00, interfaces::globals->cur_time );

	}
	void reset_animation_state ( anim_state * state ) {

		using ResetAnimState_t = void ( __thiscall * )( anim_state * );

		static auto func = reinterpret_cast< ResetAnimState_t > ( utilities::pattern_scan ( "client.dll", ( "56 6A 01 68 ? ? ? ? 8B F1" ) ) );
		if ( !func )
			return;

		func ( state );
	}
	void update_fake_animations ( ) {

		if ( !interfaces::engine->is_in_game ( ) )
			return;

		if ( !interfaces::engine->is_connected ( ) )
			return;


		local_player::m_data.pointer = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( interfaces::engine->get_local_player ( ) ) );
		csgo::local_player = local_player::m_data.pointer;

		if ( !local_player::m_data.pointer )
			return;

		if ( !csgo::cmd || !local_player::m_data.pointer->is_alive ( ) )
			return;

		
			m_data.fake_anim_state = local_player::m_data.pointer->get_anim_state ( );
			if ( csgo::send_packet ) {

				m_data.fake_anim_state->m_force_update = true;
				m_data.fake_anim_state->m_last_clientside_anim_framecount = 0;



				m_data.should_animate = true;
				update_anim_angle ( m_data.fake_anim_state, csgo::real_angle );
				m_data.should_animate = false;


			}
		
			local_player::m_data.pointer->set_abs_angles ( vec3_t ( 0, m_data.fake_anim_state->m_abs_yaw, 0 ) );

			local_player::m_data.pointer->invalidate_bone_cache ( );
			local_player::m_data.pointer->setup_bones ( csgo::fake_matrix, 128, 0x7FF00, interfaces::globals->cur_time );

		std::memcpy ( local_player::m_data.pointer->get_animoverlays ( ), m_data.m_layers.data ( ), sizeof ( m_data.m_layers ) );
		std::memcpy ( local_player::m_data.pointer->m_flPoseParameter ( ).data ( ), m_data.m_poses.data ( ), sizeof ( m_data.m_poses ) );

		local_player::m_data.pointer->set_abs_angles ( vec3_t ( 0, m_data.proper_abs_yaw, 0 ) );
	}
}