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


	void update_anim_angle ( player_t * ent, anim_state * state, vec3_t ang ) {

		using fn = void ( __vectorcall * )( void *, void *, float, float, float, void * );
		static auto ret = reinterpret_cast< fn >( utilities::pattern_scan ( "client.dll", ( "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24" ) ) );

		if ( !ret || !state )
			return;

		//std::cout << "state entity | " << state->m_entity << " player entity " << ent << std::endl;
		//state->m_entity = ent;

		ret ( state, nullptr, 0.f, ang.y, ang.x, nullptr );

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
		globals.absolute_frame_start_time = interfaces::globals->absolute_frame_start_time;
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


		memcpy ( player->m_flPoseParameter ( ).data ( ), backup.poses.data ( ), sizeof ( float ) * 24 );
		memcpy ( player->get_anim_state ( ), &backup.state, sizeof ( anim_state ) );
		memcpy ( player->anim_layers ( ).data ( ), backup.layers.data ( ), sizeof ( animationlayer ) * 13 );

	}
	void restore ( ) {
		interfaces::globals->cur_time = globals.cur_time;
		interfaces::globals->realtime = globals.real_time;
		interfaces::globals->frame_time = globals.frame_time;
		interfaces::globals->absolute_frametime = globals.abs_frametime;
		interfaces::globals->interpolation_amount = globals.interpolation_amount;
		interfaces::globals->absolute_frame_start_time = globals.absolute_frame_start_time;
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

	void update_fake_animation ( ) {

		return;

		static int tickcount = 0;

		local_player::m_data.pointer = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( interfaces::engine->get_local_player ( ) ) );
		csgo::local_player = local_player::m_data.pointer;


		if ( tickcount != interfaces::globals->tick_count && csgo::send_packet ) {
			std::array<float, 24> pos_params;

			std::memcpy ( pos_params.data ( ), local_pointer->m_flPoseParameter ( ).data ( ),
				sizeof ( float ) * 24 );

			anim_state backup_state;
			std::memcpy ( &backup_state, local_pointer->get_anim_state ( ), sizeof ( anim_state ) );
			animationlayer backup_layers [ 15 ];
			std::memcpy ( backup_layers, local_pointer->get_animoverlays ( ),
				( sizeof ( animationlayer ) * 15 ) );


			matrix3x4_t old_cache [ 128 ];
			std::memcpy ( &old_cache, local_pointer->m_CachedBoneData ( ).base ( ), local_pointer->m_CachedBoneData ( ).count() * sizeof ( matrix3x4_t ) );


			float delta_time = math::ticks_to_time ( 14 );

			vec3_t simulated_fake_velocity = ( localdata.real_origin - localdata.old_real_origin ) / delta_time;
			if ( delta_time ) {
				//interfaces::console->console_printf ( "simulated_fake_velocity %f  tick delta %i\n", simulated_fake_velocity.Length2D ( ), math::time_to_ticks ( delta_time ) );
				localdata.fake_velocity = simulated_fake_velocity;
			}


			auto & anim_data = player_data [ local_pointer->index ( ) ];





			auto backup_frametime = interfaces::globals->frame_time; //-V807
			auto backup_curtime = interfaces::globals->cur_time;

			//	interfaces::globals->frame_time = 300;


			anim_data.update_anims = true;
			localdata.fake_state.m_last_clientside_anim_framecount = 0;
			vec3_t backup_velocity = local_pointer->get_abs_velocity ( );
			local_pointer->get_abs_velocity ( ) = simulated_fake_velocity;
			update_anim_angle ( local_pointer, &localdata.fake_state, csgo::cmd->viewangles );
			local_pointer->get_abs_velocity ( ) = backup_velocity;
			anim_data.update_anims = false;


			//		interfaces::globals->frame_time = backup_frametime;





			anim_data.update_bones = true;
			vec3_t abs_angles = local_pointer->abs_angles ( );


			local_pointer->invalidate_bone_cache ( );
			local_pointer->get_bone_accessor ( )->m_WritableBones = local_pointer->get_bone_accessor ( )->m_ReadableBones = 0;
			local_pointer->set_abs_angles ( vec3_t ( 0, localdata.fake_state.m_abs_yaw, 0 ) );
			std::memcpy ( local_pointer->get_anim_state ( ), &localdata.fake_state, sizeof ( anim_state ) ); /*override with our fake matrix*/
			local_pointer->setup_bones ( csgo::fake_matrix, 128, 0x7FF00, interfaces::globals->cur_time + 1.f );
			anim_data.update_bones = false;


			for ( auto & i : csgo::fake_matrix ) {
				i [ 0 ][ 3 ] -= local_pointer->abs_origin ( ).x; //-V807
				i [ 1 ][ 3 ] -= local_pointer->abs_origin ( ).y;
				i [ 2 ][ 3 ] -= local_pointer->abs_origin ( ).z;
			}

			/*restore*/


			std::memcpy ( local_pointer->m_CachedBoneData ( ).base ( ), &old_cache, local_pointer->m_CachedBoneData ( ).count() * sizeof ( matrix3x4_t ) );
			local_player::m_data.pointer->set_abs_angles ( vec3_t ( 0, m_data.proper_abs_yaw, 0 ) );


			//interfaces::console->console_printf ( "real abs yaw %f  fake abs yaw %f, delta %f \n",local_pointer->get_anim_state()->m_abs_yaw, localdata.fake_state.m_abs_yaw, math::angle_diff (local_pointer->get_anim_state()->m_abs_yaw, localdata.fake_state.m_abs_yaw ) );

			tickcount = interfaces::globals->tick_count;

			std::memcpy ( local_pointer->m_flPoseParameter ( ).data ( ), pos_params.data ( ), sizeof ( float ) * 24 );
			std::memcpy ( local_pointer->get_anim_state ( ), &backup_state, sizeof ( anim_state ) );
			std::memcpy ( local_pointer->get_animoverlays ( ), backup_layers,
				( sizeof ( animationlayer ) * 15 ) ); /*interpolated layers*/

		}

	}

	/*local player*/
	void update_local_update_start ( client_frame_stage_t stage ) {

		return;

		if ( stage != FRAME_RENDER_START )
			return;


		auto predict_origin = [ ] ( vec3_t origin, vec3_t velocity, int ticks ) {
			return origin + ( velocity * ( interfaces::globals->interval_per_tick * static_cast< float >( ticks ) ) );
		};



		static int last_tick = 0;

		if ( !local_player::m_data.pointer )
			return;

		if ( !csgo::cmd || !local_player::m_data.pointer->is_alive ( ) )
			return;

		auto local_index = local_player::m_data.pointer->index ( );
		auto & anim_data = player_data [ local_index ];

		if ( !local_pointer->get_anim_state ( ) ) return;

		auto animstate = local_pointer->get_anim_state ( );


		static bool initials = false;

		if ( !initials ) {
			std::memcpy ( &localdata.fake_state, local_pointer->get_anim_state ( ), sizeof ( anim_state ) );
			std::memcpy ( &localdata.real_state, local_pointer->get_anim_state ( ), sizeof ( anim_state ) );
			initials = true;

			localdata.init_local_anim = true;
		}

		
		std::memcpy ( m_data.m_layers.data ( ), local_pointer->get_animoverlays ( ),
			( sizeof ( animationlayer ) * 15 ) ); /*dont fuck with interpolated layers, restore them*/



            int bkp = local_pointer->get_ieflags ( );
			//local_pointer->get_ieflags ( ) &= ~0x1000; 
			
			//vec3_t vel = local_pointer->get_abs_velocity ( ); local_pointer->get_abs_velocity ( ) = local_pointer->velocity ( );
			//update_globals ( local_pointer, math::ticks_to_time(local_pointer->get_tick_base()) );
			update_anim_angle ( local_pointer, animstate, csgo::cmd->viewangles ); /*update animstate to viewangle*/
			//restore ( );


			//local_pointer->get_abs_velocity ( ) = vel;

			//local_pointer->get_ieflags ( ) = bkp;
			std::memcpy ( local_player::m_data.pointer->get_animoverlays ( ), m_data.m_layers.data ( ), sizeof ( m_data.m_layers ) ); /*restore server layers*/

			if ( !interfaces::clientstate->m_choked_commands ) {
				m_data.proper_abs_yaw = animstate->m_abs_yaw;
				localdata.real_state = *animstate;
				std::memcpy ( m_data.m_poses.data ( ), local_player::m_data.pointer->m_flPoseParameter ( ).data ( ), sizeof ( m_data.m_poses ) ); /*grab uninterpolated poses*/
		
			}

			localdata.init_local_anim = true;
	

	}
	void update_animations_update_end ( client_frame_stage_t stage ) {

		return;
		if ( stage != FRAME_RENDER_START )
			return;

		if ( !local_pointer )
			return;

		if ( !local_pointer->is_alive ( ) )
			return;


		auto local_index = local_player::m_data.pointer->index ( );

		auto & anim_data = player_data [ local_index ];

		if ( !localdata.init_local_anim )
			return;

		auto animstate = local_player::m_data.pointer->get_anim_state ( );


		if ( !animstate ) return;
		
		if ( local_pointer->flags() & fl_onground)
		   animstate->m_time_in_air = 0;




		animationlayer backup_layers [ 15 ];
		std::memcpy ( backup_layers, local_pointer->get_animoverlays ( ),
			( sizeof ( animationlayer ) * 15 ) ); /*dont fuck with interpolated layers, restore them, NEEEDED FOR DESYNC CHAMS*/




		vec3_t old_abs = local_pointer->abs_angles ( );
		local_player::m_data.pointer->set_abs_angles ( vec3_t ( 0, m_data.proper_abs_yaw, 0 ) );

		m_data.m_layers [ 12 ].m_weight = 0.f;


		std::memcpy ( local_player::m_data.pointer->m_flPoseParameter ( ).data ( ), m_data.m_poses.data ( ), sizeof ( m_data.m_poses ) ); /*overide networked poses*/
		std::memcpy ( local_player::m_data.pointer->get_animoverlays ( ), m_data.m_layers.data ( ), sizeof ( m_data.m_layers ) ); /*override uninterpolated layers*/

		/*invalidate */
		//local_pointer->invalidate_bone_cache ( );
		//local_pointer->get_bone_accessor ( )->m_WritableBones = local_pointer->get_bone_accessor ( )->m_ReadableBones = 0;
		/*end*/

		anim_data.update_bones = true;

		//	local_pointer->interpolate ( interfaces::globals->cur_time );
		

		local_player::m_data.pointer->setup_bones ( nullptr, 128, 0x7FF00, local_pointer->simulation_time ( ) );
		

		anim_data.update_bones = false;


		std::memcpy ( local_player::m_data.pointer->get_animoverlays ( ), backup_layers, sizeof ( animationlayer ) * 15 ); /*restore interpolated layers */
		local_pointer->abs_angles ( ) = old_abs;

		
	//	std::memcpy ( local_pointer->get_anim_state ( ), &localdata.real_state, sizeof ( localdata.real_state ) );

		using UpdateAllViewmodelAddons_t = int ( __fastcall * )( base_view_model_t * );
		static UpdateAllViewmodelAddons_t UpdateAllViewmodelAddonsFn = nullptr;

		if ( !UpdateAllViewmodelAddonsFn ) {
			UpdateAllViewmodelAddonsFn = reinterpret_cast< UpdateAllViewmodelAddons_t >( utilities::pattern_scan ( "client.dll", ( "55 8B EC 83 E4 ? 83 EC ? 53 8B D9 56 57 8B 03 FF 90 ? ? ? ? 8B F8 89 7C 24 ? 85 FF 0F 84 ? ? ? ? 8B 17 8B CF" ) ) );
		}
		auto hViewModel = local_pointer->view_model ( );
		if ( hViewModel != 0xFFFFFFFF ) {
			auto pViewModel = reinterpret_cast< base_view_model_t * >( interfaces::entity_list->get_client_entity_handle ( hViewModel ) );
			if ( pViewModel )
				UpdateAllViewmodelAddonsFn ( pViewModel );
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