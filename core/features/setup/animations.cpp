#include "../features.hpp"

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
			
			predict_velocity ( player );

			anim.network.angle = player->eye_angles ( );
			anim.network.origin = player->origin ( );
			anim.outdated_anims = anim.network_update = true;

			anim.simulated_origin = anim.network.origin + ( player->velocity ( ) * ( interfaces::globals->interval_per_tick  ) );

		}
	}

	void predict_velocity ( player_t * player ) {

		 auto & anim = player_data [ player->index ( ) ];
		 anim.time_delta = player->simulation_time ( ) - player->get_old_simulation_time ( );

		 anim.predicted_origin = anim.network.origin + ( anim.perdicted_velocity * ( interfaces::globals->interval_per_tick * ( float ) math::time_to_ticks ( anim.time_delta ) ) );
		 anim.perdicted_velocity = ( player->velocity() - anim.network.origin ) / anim.time_delta;


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


	void backup ( ) {

		globals.cur_time = interfaces::globals->cur_time;
		globals.real_time = interfaces::globals->realtime;
		globals.frame_time = interfaces::globals->frame_time;
		globals.abs_frametime = interfaces::globals->absolute_frametime;
		globals.interpolation_amount = interfaces::globals->interpolation_amount;
		globals.tickcount = interfaces::globals->tick_count;
		globals.framecount = interfaces::globals->frame_count;
	}

	void backup_animations ( player_t * player ) {
		auto & backup = player_data [ player->index ( ) ].backup;

		auto anim_pointer = player->get_anim_state ( );

		std::memcpy ( &backup.state, anim_pointer, sizeof ( anim_state ) );
		std::memcpy ( backup.layers.data ( ), player->anim_layers ( ).data ( ), sizeof ( animationlayer ) * 13 );
		std::memcpy ( backup.poses.data ( ), player->m_flPoseParameter ( ).data ( ), sizeof ( float ) * 24 );
	}

	void restore_animations ( player_t * player ) {
		auto & backup = player_data [ player->index ( ) ].backup;
		
		auto anim_pointer = player->get_anim_state ( );


		memcpy ( player->m_flPoseParameter ( ).data(), backup.poses.data(), sizeof ( float ) * 24 );
		memcpy ( player->get_anim_state(), &backup.state, sizeof ( anim_state ) );
		memcpy ( player->anim_layers ().data(), backup.layers.data(), sizeof ( animationlayer ) * 13 );

	}
	void restore (  ) {
		interfaces::globals->cur_time = globals.cur_time;
		interfaces::globals->realtime = globals.real_time;
		interfaces::globals->frame_time = globals.frame_time;
		interfaces::globals->absolute_frametime = globals.abs_frametime;
		interfaces::globals->interpolation_amount = globals.interpolation_amount;

		interfaces::globals->tick_count = globals.tickcount;
		interfaces::globals->frame_count = globals.framecount;
	}

	void update_globals ( player_t * player, float time ) {
		static auto host_timescale = interfaces::console->get_convar ( ( "host_timescale" ) );

		int tick = math::time_to_ticks ( time );

		interfaces::globals->cur_time = time;
		interfaces::globals->realtime = time;
		interfaces::globals->frame_time = interfaces::globals->interval_per_tick * host_timescale->get_float ( );
		interfaces::globals->absolute_frametime = interfaces::globals->interval_per_tick * host_timescale->get_float ( );
		interfaces::globals->absolute_frame_start_time = time - interfaces::globals->interval_per_tick * host_timescale->get_float ( );
		interfaces::globals->interpolation_amount = 0;

		interfaces::globals->frame_count = tick;
		interfaces::globals->tick_count = tick;



	}
	void update_player_animation ( player_t * player ) {

	

	}
	/*local player*/
	void update_animations_local ( ) {


		static int last_tick = 0;
	
		/*if ( local_pointer &&  local_pointer->is_alive ( ) ) {
		/*	if ( local_pointer->get_anim_state ( )->m_hit_ground ) {
				*reinterpret_cast< float * >( reinterpret_cast< uintptr_t >( local_pointer->get_anim_state ( ) ) + 0x110 ) = 0.0f;
			}*/
			//local_pointer->m_flPoseParameter ( ) [ 12 ] = 0.f;
			//local_pointer->m_flPoseParameter ( ) [ 11 ] = 120.f;
			//if ( local_pointer->get_anim_state ( )->m_hit_ground ) {
			//	float height = static_cast< float >( reinterpret_cast< uintptr_t >( local_pointer->get_anim_state ( ) ) + 0x118 );

				//if ( height ) {
			//		update_anim_angle ( local_pointer->get_anim_state ( ), vec3_t ( 0.f, local_pointer->get_anim_state ( )->m_eye_yaw, 0.f ) );
				//}
			//}
				
			

		//}
		/*if ( local_pointer->is_in_air ( ) ) {
		  
			//if ( local_pointer->get_anim_state ( )->m_hit_ground )
				

			local_pointer->get_anim_state ( )->m_time_in_air = 999.f;


			//entity.set_prop ( entity.get_local_player ( ), "m_flPoseParameter", 0.5, 12 )
		}*/

//		return;

		bool should_update = true;
		if ( interfaces::globals->tick_count != last_tick ) {
			should_update = true;
			last_tick = interfaces::globals->tick_count;
		}

		
		if ( !local_player::m_data.pointer )
			return;

		if ( !csgo::cmd || !local_player::m_data.pointer->is_alive ( ) )
			return;


		/* vars */
		const auto o_eflags = local_player::m_data.pointer->get_ieflags ( );
		auto animstate = local_player::m_data.pointer->get_anim_state ( );

		auto local_index = local_player::m_data.pointer->index ( );
		auto& anim_data = player_data [ local_index ];
		if ( !animstate ) return;
		std::memcpy ( m_data.m_layers.data ( ), local_player::m_data.pointer->get_animoverlays ( ), sizeof ( m_data.m_layers ) );
		if ( should_update ) {
			
			*reinterpret_cast< bool * >( uintptr_t ( animstate ) + 0x5 ) = true;
			animstate->m_last_clientside_anim_framecount = 0;



			m_data.should_animate = true;
			anim_data.update_anims = true;
			//local_player::m_data.pointer->update_client_side_animations ( );
			update_anim_angle ( local_player::m_data.pointer->get_anim_state ( ), csgo::cmd->viewangles );
			anim_data.update_anims = false;
			m_data.should_animate = false;

			local_player::m_data.pointer->get_ieflags ( ) = o_eflags;


		
			if ( !interfaces::clientstate->m_choked_commands ) {
				m_data.proper_abs_yaw = animstate->m_abs_yaw;
				std::memcpy ( m_data.m_poses.data ( ), local_player::m_data.pointer->m_flPoseParameter ( ).data ( ), sizeof ( m_data.m_poses ) );
			}

	
			

			m_data.init = true;
		}

		if ( m_data.init ) {
		
			std::memcpy ( local_player::m_data.pointer->get_animoverlays ( ), m_data.m_layers.data ( ), sizeof ( m_data.m_layers ) );
			std::memcpy ( local_player::m_data.pointer->m_flPoseParameter ( ).data ( ), m_data.m_poses.data ( ), sizeof ( m_data.m_poses ) );

			//local_player::m_data.pointer->GetBoneAccessor ( )->m_ReadableBones = local_player::m_data.pointer->GetBoneAccessor ( )->m_WritableBones = 0;
			auto delta = math::normalize_yaw ( ( csgo::real_angle.y - m_data.proper_abs_yaw ) ) / 2.f;
			delta = math::normalize_yaw ( delta );

			local_player::m_data.pointer->set_abs_angles ( vec3_t ( 0, math::normalize_yaw( m_data.proper_abs_yaw + delta) , 0 ) );

			anim_data.update_bones = true;
			auto old = local_pointer->get_anim_state ( )->m_duck_amount;
			local_pointer->get_anim_state ( )->m_duck_amount = 0.f;
			local_player::m_data.pointer->setup_bones ( csgo::fake_matrix, 128, 0x7FF00, interfaces::globals->cur_time );
			local_pointer->get_anim_state ( )->m_duck_amount = old;
			anim_data.update_bones = false;

			for ( auto & i : csgo::fake_matrix ) {
				i [ 0 ][ 3 ] -= local_player::m_data.pointer->abs_origin ( ).x; //-V807
				i [ 1 ][ 3 ] -= local_player::m_data.pointer->abs_origin ( ).y;
				i [ 2 ][ 3 ] -= local_player::m_data.pointer->abs_origin ( ).z;
			}
			std::memcpy ( local_player::m_data.pointer->get_animoverlays ( ), m_data.m_layers.data ( ), sizeof ( m_data.m_layers ) );
			std::memcpy ( local_player::m_data.pointer->m_flPoseParameter ( ).data ( ), m_data.m_poses.data ( ), sizeof ( m_data.m_poses ) );
		

			local_player::m_data.pointer->set_abs_angles ( vec3_t ( 0, m_data.proper_abs_yaw, 0 ) );

			anim_data.update_bones = true;
			local_player::m_data.pointer->setup_bones ( csgo::real_matrix, 128, 0x7FF00, interfaces::globals->cur_time );
			anim_data.update_bones = false;

			for ( auto & i : csgo::real_matrix ) {
				i [ 0 ][ 3 ] -= local_player::m_data.pointer->abs_origin ( ).x; //-V807
				i [ 1 ][ 3 ] -= local_player::m_data.pointer->abs_origin ( ).y;
				i [ 2 ][ 3 ] -= local_player::m_data.pointer->abs_origin ( ).z;
			}
		
		}
	}
	void reset_animation_state ( anim_state * state ) {

		using ResetAnimState_t = void ( __thiscall * )( anim_state * );

		static auto func = reinterpret_cast< ResetAnimState_t > ( utilities::pattern_scan ( "client.dll", ( "56 6A 01 68 ? ? ? ? 8B F1" ) ) );
		if ( !func )
			return;

		func ( state );
	}
	
}