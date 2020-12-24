#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "../features/features.hpp"

#include "../../core/features/nade prediction/nade_prediction.hpp"
#include "../menu/menu.hpp"
#include "../menu/render/menu_render.hpp"

#include <d3dx9.h>
#include "../menu/ImGui/imgui.h"
#include "../menu/ImGui/impl/imgui_impl_dx9.h"

aimbot::target aimbot::last_target;
aimbot::target aimbot::best_target;


hooks::frame_stage::fn frame_stage_original = nullptr;
hooks::create_move::fn create_move_original = nullptr;
hooks::paint_traverse::fn paint_traverse_original = nullptr;
hooks::post_screen_space_fx::fn post_screen_space_original = nullptr;
hooks::crc_server_check::fn crc_server_check_original = nullptr;
hooks::scene_end::fn scene_end_original = nullptr;
hooks::setup_bones::fn setup_bones_original = nullptr;
hooks::do_extra_bone_processing::fn do_extra_bone_processing_original = nullptr;
hooks::standard_blending_rules::fn standard_blending_rules_original = nullptr;
hooks::update_client_side_animation::fn update_client_side_animation_original = nullptr;
hooks::render_smoke_overlay::fn render_smoke_overlay_original = nullptr;
hooks::trace_ray::fn trace_ray_original = nullptr;
hooks::fire_event::fn fire_event_original = nullptr;
hooks::write_user_cmd::fn write_user_cmd_original = nullptr;
hooks::override_view::fn override_view_original = nullptr;
hooks::cl_move::fn cl_move_original = nullptr;
hooks::run_commmand::fn run_commmand_original = nullptr;
hooks::present::fn present_original = nullptr;
hooks::reset::fn reset_original = nullptr;
hooks::get_eye_angles::fn get_eye_angles_original = nullptr;
hooks::build_transformations::fn build_transformations_original = nullptr;
void * do_procedural_foot_plant_original = nullptr;
void * should_skip_animation_frame_original = nullptr;
void * file_system_original = nullptr;
hooks::modify_eye_position::fn  modify_eye_position_original = nullptr;
hooks::check_for_sequence_change::fn check_for_sequence_change_original = nullptr;
hooks::calculate_view::fn calculate_view_original = nullptr;
hooks::update_animation_state::fn update_animation_state_original = nullptr;
hooks::is_hltv::fn is_hltv_original = nullptr;
hooks::in_prediction::fn in_prediction_original = nullptr;
HWND hooks::window;
WNDPROC hooks::wndproc_original = NULL;


static constexpr auto BONE_USED_BY_SERVER = BONE_USED_BY_HITBOX | BONE_USED_BY_VERTEX_LOD0 | BONE_USED_BY_VERTEX_LOD1 | BONE_USED_BY_VERTEX_LOD2
| BONE_USED_BY_VERTEX_LOD3 | BONE_USED_BY_VERTEX_LOD4 | BONE_USED_BY_VERTEX_LOD5 | BONE_USED_BY_VERTEX_LOD6 | BONE_USED_BY_VERTEX_LOD7;

bool hooks::initialize ( ) {


	static const auto c_cs_player_table = reinterpret_cast< uint32_t >( utilities::pattern_scan ( "client.dll",
		"55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 89 7C 24 0C" ) ) + 0x47;



	void * pt = reinterpret_cast< void * >( c_cs_player_table );

	printf ( "c_cs_player_table ptr = %p \n", pt );
	static const auto _sbf = utilities::pattern_scan ( "client.dll", ( "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 89 7C 24 0C" ) ) + 0x4E;
	void * sbf = reinterpret_cast< void * > ( _sbf );
	void * d3d_device = **( void *** ) ( ( uintptr_t ) utilities::pattern_scan ( "shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C" ) + 0x1 );


	auto frame_stage_target = reinterpret_cast< void * >( get_virtual ( interfaces::client, 37 ) );
	auto create_move_target = reinterpret_cast< void * >( get_virtual ( interfaces::clientmode, 24 ) );
	auto paint_traverse_target = reinterpret_cast< void * >( get_virtual ( interfaces::panel, 41 ) );
	auto post_screen_space_fx = reinterpret_cast< void * >( get_virtual ( interfaces::clientmode, 44 ) );
	auto draw_model_execute_target = reinterpret_cast< void * >( get_virtual ( interfaces::model_render, 21 ) );
	auto scene_end_target = reinterpret_cast< void * >( get_virtual ( interfaces::render_view, 9 ) );
	auto setup_bones_target = reinterpret_cast< void * >( get_virtual ( sbf, 13 ) );
	auto do_procedural_foot_plant_target = reinterpret_cast< void * >( utilities::pattern_scan ( "client.dll", "55 8B EC 83 E4 F0 83 EC 78 56 8B F1 57 8B 56 60" ) );
	auto do_extra_bone_processing_target = reinterpret_cast< void * >( get_virtual ( pt, 197 ) );
	auto should_skip_animation_frame_target = reinterpret_cast< void * >( utilities::pattern_scan ( "client.dll", "57 8B F9 8B 07 8B 80 ? ? ? ? FF D0 84 C0 75 02" ) );

	auto in_prediction_target = ( void * ) get_virtual ( interfaces::prediction, 14 ); // 14

	auto check_for_sequence_change_target = reinterpret_cast< void * >( utilities::pattern_scan ( "client.dll", "55 8B EC 51 53 8B 5D 08 56 8B F1 57 85" ) );
	auto modify_eye_position_target = reinterpret_cast< void * >( utilities::pattern_scan ( "client.dll", "55 8B EC 83 E4 F8 83 EC 5C 53 8B D9 56 57 83" ) );

	auto accumulate_layers_target = reinterpret_cast< void * >( get_virtual ( pt, 9 ) );
	auto get_eye_angles_target = reinterpret_cast< void * >( get_virtual ( pt, 169 ) );
	auto standard_blending_rules_target = reinterpret_cast< void * >( get_virtual ( pt, 205 ) );
	auto update_client_side_animation_target = reinterpret_cast< void * >( get_virtual ( pt, 223 ) );
	auto render_smoke_overlay_target = reinterpret_cast< void * >( get_virtual ( interfaces::render_view, 41 ) );
	auto trace_ray_target = reinterpret_cast< void * >( get_virtual ( interfaces::trace_ray, 8 ) );
	auto fire_event_target = reinterpret_cast< void * >( get_virtual ( interfaces::engine, 59 ) ); // working

	auto override_view_target = reinterpret_cast< void * >( get_virtual ( interfaces::clientmode, 18 ) );
	auto write_user_cmd_target = reinterpret_cast< void * >( get_virtual ( interfaces::client, 24 ) );
	auto cl_move_target = reinterpret_cast< void * >( utilities::pattern_scan ( "engine.dll", "55 8B EC 81 EC ? ? ? ? 53 56 57 8B 3D ? ? ? ? 8A" ) );
	auto run_commmand_target = reinterpret_cast< void * >( get_virtual ( interfaces::prediction, 19 ) );
	auto crc_server_check_target = reinterpret_cast< void * >( utilities::pattern_scan ( "engine.dll", "55 8B EC 81 EC ? ? ? ? 53 8B D9 89 5D F8 80" ) );
	auto file_system_target = reinterpret_cast< void * >( get_virtual ( interfaces::file_system, 128 ) );

	auto is_hltv_target = reinterpret_cast< void * >( get_virtual ( interfaces::engine, 93 ) );

	auto build_transformations_target = reinterpret_cast< void * >( utilities::pattern_scan ( "client.dll", "55 8B EC 83 E4 F0 81 EC ? ? ? ? 56 57 8B F9 8B 0D ? ? ? ? 89 7C 24 1C" ) );

	auto calculate_view_target = reinterpret_cast< void * >( utilities::pattern_scan ( "client.dll", "55 8B EC 83 EC 14 53 56 57 FF 75 18" ) );
	auto update_animation_state_target = reinterpret_cast< void * >( utilities::pattern_scan ( "client.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3" ) );

	auto present_target = reinterpret_cast< void * >( get_virtual ( d3d_device, 42 ) );
	auto reset_target = reinterpret_cast< void * >( get_virtual ( d3d_device, 16 ) );
	printf ( "check_for_sequence_change_target ptr = %p \n", check_for_sequence_change_target );

	if ( MH_Initialize ( ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize MH_Initialize." );
		return false;
	}

	if ( MH_CreateHook ( frame_stage_target, &frame_stage::hook, reinterpret_cast< void ** >( &frame_stage_original ) ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize frame_stage. (outdated index?)" );
		return false;
	}


	if ( MH_CreateHook ( crc_server_check_target, &crc_server_check::hook, reinterpret_cast< void ** >( &crc_server_check_original ) ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize crc_server_check. (outdated index?)" );
		return false;
	}


	 if ( MH_CreateHook ( should_skip_animation_frame_target, &should_skip_animation_frame::hook, reinterpret_cast< void ** >( &should_skip_animation_frame_original ) ) != MH_OK ) {
		  throw std::runtime_error ( "failed to initialize should_skip_animation_frame. (outdated index?)" );
		  return false;
	  }
	/*  if ( MH_CreateHook ( update_animation_state_target, &update_animation_state::hook, reinterpret_cast< void ** >( &update_animation_state_original ) ) != MH_OK ) {
		  throw std::runtime_error ( "failed to initialize update_animation_state. (outdated index?)" );
		  return false;
	  }*/
	  /*if ( MH_CreateHook ( get_eye_angles_target, &get_eye_angles::hook, reinterpret_cast< void ** >( &get_eye_angles_original ) ) != MH_OK ) {
		  throw std::runtime_error ( "failed to initialize get_eye_angles. (outdated index?)" );
		  return false;
	  
	 }*/


	if ( MH_CreateHook ( build_transformations_target, &build_transformations::hook, reinterpret_cast< void ** >( &build_transformations_original ) ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize build_transformations. (outdated index?)" );
		return false;
	}

	if ( MH_CreateHook ( create_move_target, &create_move::hook, reinterpret_cast< void ** >( &create_move_original ) ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize create_move. (outdated index?)" );
		return false;
	}





	if ( MH_CreateHook ( paint_traverse_target, &paint_traverse::hook, reinterpret_cast< void ** >( &paint_traverse_original ) ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize paint_traverse. (outdated index?)" );
		return false;
	}

	if ( MH_CreateHook ( post_screen_space_fx, &post_screen_space_fx::hook, reinterpret_cast< void ** >( &post_screen_space_original ) ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize post_screen_space_fx. (outdated index?)" );
		return false;
	}


	if ( MH_CreateHook ( scene_end_target, &scene_end::hook, reinterpret_cast< void ** >( &scene_end_original ) ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize scene_end. (outdated index?)" );
		return false;
	}

	if ( MH_CreateHook ( present_target, &present::hook, reinterpret_cast< void ** >( &present_original ) ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize present. (outdated index?)" );
		return false;
	}
	if ( MH_CreateHook ( reset_target, &reset::hook, reinterpret_cast< void ** >( &reset_original ) ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize reset. (outdated index?)" );
		return false;
	}
	if ( MH_CreateHook ( setup_bones_target, &setup_bones::hook, reinterpret_cast< void ** >( &setup_bones_original ) ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize setup_bones. (outdated index?)" );
		return false;
	}

	if ( MH_CreateHook ( do_procedural_foot_plant_target, &do_procedural_foot_plant::hook, ( void ** ) &do_procedural_foot_plant_original ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize prodcedural foot plant. (outdated index?)" );
		return false;
	}
	if ( MH_CreateHook ( file_system_target, &file_system::hook, ( void ** ) &file_system_original ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize file_system (outdated index?)" );
		return false;
	}

    if ( MH_CreateHook ( draw_model_execute_target, &hooks::draw_model_exec::hook, ( void ** ) &o_draw_model_exec ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize prodcedural draw_model_execute. (outdated index?)" );
		return false;
	}

	if ( MH_CreateHook ( update_client_side_animation_target, &update_client_side_animation::hook, ( void ** ) &update_client_side_animation_original ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize update client side. (outdated index?)" );
		return false;
	}

	if ( MH_CreateHook ( standard_blending_rules_target, &standard_blending_rules::hook, ( void ** ) &standard_blending_rules_original ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize standard blending rules. (outdated index?)" );
		return false;
	}

	if ( MH_CreateHook ( fire_event_target, &fire_event::hook, ( void ** ) &fire_event_original ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize fire event. (outdated index?)" );
		return false;
	}
	if ( MH_CreateHook ( override_view_target, &override_view::hook, reinterpret_cast< void ** >( &override_view_original ) ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize override_view. (outdated index?)" );
		return false;
	}
	if ( MH_CreateHook ( write_user_cmd_target, &write_user_cmd::hook, reinterpret_cast< void ** >( &write_user_cmd_original ) ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize write user cmd target. (outdated index?)" );
		return false;
	}
	if ( MH_CreateHook ( cl_move_target, &cl_move::hook, reinterpret_cast< void ** >( &cl_move_original ) ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize cl move. (outdated index?)" );
		return false;
	}

	if ( MH_CreateHook ( run_commmand_target, &run_commmand::hook, reinterpret_cast< void ** >( &run_commmand_original ) ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize run_commmand. (outdated index?)" );
		return false;
	}


	if ( MH_EnableHook ( MH_ALL_HOOKS ) != MH_OK ) {
		throw std::runtime_error ( "failed to enable hooks." );
		return false;
	}


	events.setup ( );

	window = FindWindow ( "Valve001", NULL );

	wndproc_original = ( WNDPROC ) SetWindowLongPtrA ( window, GWL_WNDPROC, ( LONG ) hooks::wnd_proc::hook );

	console::log ( "[setup] hooks initialized!\n" );
	return true;
}

void hooks::release ( ) {

	events.release ( );

	MH_Uninitialize ( );

	MH_DisableHook ( MH_ALL_HOOKS );
}
inline void UpdateAnimationState222 ( anim_state * state, vec3_t ang ) {
	using fn = void ( __vectorcall * )( void *, void *, float, float, float, void * );
	static auto ret = reinterpret_cast< fn >( utilities::pattern_scan ( "client.dll", ( "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24" ) ) );

	if ( !ret || !state )
		return;

	ret ( state, nullptr, 0.f, ang.y, ang.x, nullptr );
}

void __fastcall hooks::calculate_view::hook ( void * ecx, void * edx, vec3_t & eye_origin, vec3_t & eye_angles, float & z_near, float & z_far, float & fov ) {

	//static auto original_calculate_view = m_calculate_view.get_original_function< decltype( &calculate_view ) > ( );

	auto player = reinterpret_cast< player_t * >( ecx );

	if ( player != local_player::m_data.pointer )
		return calculate_view_original ( ecx, edx, eye_origin, eye_angles, z_near, z_far, fov );

	const auto old_use_new_animation_state = player->use_new_animation_state ( );

	//prevent calls to ModifyEyePosition
	player->use_new_animation_state ( ) = false;

	return calculate_view_original ( ecx, edx, eye_origin, eye_angles, z_near, z_far, fov );

	player->use_new_animation_state ( ) = old_use_new_animation_state;

}
bool __fastcall hooks::in_prediction::hook ( void * ecx, void * edx ) {
	static auto maintain_sequence_transitions = ( void * ) ( utilities::pattern_scan ( "client.dll", "84 C0 74 17 8B 87" ) );
	static auto setup_bones_ptr = ( void * ) ( utilities::pattern_scan ( "client.dll", "8B 40 ? FF D0 84 C0 74 ? F3 0F 10 05 ? ? ? ? EB ?" ) + 5 );

	if ( _ReturnAddress ( ) == maintain_sequence_transitions || _ReturnAddress ( ) == setup_bones_ptr )
		return false;

	return in_prediction_original ( interfaces::prediction, 0 );
}

void _fastcall hooks::do_extra_bone_processing::hook ( void * ecx, uint32_t, void * hdr, vec3_t * pos, void * q, void * matrix, uint8_t * bone_computed, void * context ) {
	auto player = ( player_t * ) ecx;


	//if ( player->m_fEffects( ) & 8 )
		//return;
	//printf( " player->m_fEffects( ) ptr = %i \n", player->m_fEffects( ) );
	const auto state = player->get_anim_state ( );

	bool old_on_ground = state->m_on_ground;

	if ( !state )
		return do_extra_bone_processing_original ( player, hdr, pos, q, matrix, bone_computed, context );
	state->m_on_ground = false;

	do_extra_bone_processing_original ( player, hdr, pos, q, matrix, bone_computed, context );

	state->m_on_ground = old_on_ground;
}

/*
void __vectorcall  hooks::update_animation_state::hook ( void * this_pointer, void * unknown, float z, float y, float x, void * unknown1 ) {


	const auto animation_state = reinterpret_cast< anim_state * >( this_pointer );

	// allow animations to be animated in the same frame
	if ( animation_state->m_iLastClientSideAnimationUpdateFramecount == interfaces::globals->frame_count )
		animation_state->m_iLastClientSideAnimationUpdateFramecount -= 1;

	const auto player = animation_state->m_baseEntity;

	if ( player != local_player::m_data.pointer )
		return update_animation_state_original ( this_pointer, unknown, z, y, x, unknown1 );

	const auto angle = animations::m_data.m_angle;

	return update_animation_state_original ( this_pointer, unknown, z, angle.y, angle.x, unknown1 );

}*/
void __fastcall hooks::modify_eye_position::hook ( void * ecx, void * edx, vec3_t & input_eye_position ) {


	const auto animation_state = reinterpret_cast< anim_state * >( ecx );
	if ( !animation_state )
		modify_eye_position_original ( ecx, edx, input_eye_position );
	//*( std::uint32_t * ) ( ( std::uint32_t ) animation_state + 0x32c ) = false;

	return modify_eye_position_original ( ecx, edx, input_eye_position );

}
bool __fastcall hooks::should_skip_animation_frame::hook ( void * ecx, void * edx ) {
	return false;
}

void __stdcall hooks::frame_stage::hook ( int frame_stage ) {




	if ( !interfaces::engine->is_in_game ( ) )
		return;

	if ( !interfaces::engine->is_connected ( ) )
		return;



	switch ( frame_stage ) {
	case FRAME_UNDEFINED:
		break;
	case FRAME_START:
		csgo::screen_matrix = interfaces::engine->world_to_screen_matrix ( );

		break;
	case FRAME_NET_UPDATE_START:
		shot_processor::manage_shots ( );
		break;
	case FRAME_NET_UPDATE_POSTDATAUPDATE_START:
		resolver::frame_stage ( );
		break;
	case FRAME_NET_UPDATE_POSTDATAUPDATE_END:
		player_manager::manage_bones ( );


		break;
	case FRAME_NET_UPDATE_END:
		g_netdata.apply ( );
		player_manager::setup_players ( );

		break;
	case FRAME_RENDER_START:
 
		static auto blur = interfaces::material_system->find_material ( "dev/scope_bluroverlay", TEXTURE_GROUP_OTHER );
		blur->set_material_var_flag ( material_var_no_draw, true );
		csgo::m_rate = ( int ) std::round ( 1.f / interfaces::globals->interval_per_tick );

		if ( !local_player::m_data.pointer )
			return;

		for ( int i = 1; i <= interfaces::globals->max_clients; i++ ) {
			player_t * entity = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( i ) );


			if ( !entity )
				continue;

			if ( entity == local_player::m_data.pointer )
				continue;

			if ( entity->dormant ( ) )
				continue;

			if ( !entity->is_alive() )
				continue;



			animations::update_player_animation ( entity );
		}
		
		animations::update_animations_local ( );
		animations::update_fake_animations ( );

		break;
	case FRAME_RENDER_END:

		break;
	default:
		break;
	}



	static int m_iLastCmdAck = 0;
	static float m_flNextCmdTime = 0.f;

	if ( local_player::m_data.pointer && local_player::m_data.alive ) {
		int framstage_minus2 = frame_stage - 2;

		if ( framstage_minus2 ) {			// do shit onetap does idk
		}
		else {
			if ( interfaces::clientstate && ( m_iLastCmdAck != interfaces::clientstate->m_last_command_ack || m_flNextCmdTime != interfaces::clientstate->m_flNextCmdTime ) ) {
				if ( engine_prediction::m_stored_variables.m_flVelocityModifier != ( reinterpret_cast< uintptr_t >( local_player::m_data.pointer ) + 0xA38C ) ) {
					*reinterpret_cast< int * >( reinterpret_cast< uintptr_t >( interfaces::prediction + 0x24 ) ) = 1;
					engine_prediction::m_stored_variables.m_flVelocityModifier = reinterpret_cast< uintptr_t >( local_player::m_data.pointer ) + 0xA38C;
				}

				m_iLastCmdAck = interfaces::clientstate->m_last_command_ack;
				m_flNextCmdTime = interfaces::clientstate->m_flNextCmdTime;
			}
		}
	}

	frame_stage_original ( interfaces::client, frame_stage );

}



vec3_t * __fastcall  hooks::get_eye_angles::hook ( void * ecx, void * edx ) {
	auto pl = ( player_t * ) ecx;
	if ( !csgo::cmd || !pl || !local_player::m_data.pointer || pl != local_player::m_data.pointer )
		return get_eye_angles_original ( ecx, edx );


	static auto PitchReturn = std::uintptr_t ( utilities::pattern_scan ( "client.dll", "8B CE F3 0F 10 00 8B 06 F3 0F 11 45 ? FF 90 ? ? ? ? F3 0F 10 55" ) );
	static auto YawReturn = std::uintptr_t ( utilities::pattern_scan ( "client.dll", "F3 0F 10 55 ? 51 8B 8E" ) );

	if ( std::uintptr_t ( _ReturnAddress ( ) ) == PitchReturn
		|| std::uintptr_t ( _ReturnAddress ( ) ) == YawReturn )
		return &csgo::real_angle;


	return get_eye_angles_original ( ecx, edx );
}
void __fastcall hooks::override_view::hook ( void * _this, void * _edx, view_setup_t * setup ) {
	/*if ( interfaces::engine->is_in_game( ) ) {
		if ( local_player::m_data.pointer && local_player::m_data.pointer->is_alive( ) && variables::visuals::fov > 0 )
			setup->fov = 90 + variables::visuals::fov;
	}*/

	misc::thirdperson::think ( );

	override_view_original ( interfaces::clientmode, _this, setup );
}
bool __fastcall hooks::create_move::hook ( void * ecx, void * edx, int input_sample_frametime, c_usercmd * cmd ) {


	if ( !cmd || !cmd->command_number )
		return create_move_original ( input_sample_frametime, cmd );

	
	if ( !interfaces::engine->is_in_game ( ) )
		return  create_move_original ( input_sample_frametime, cmd );

	static c_usercmd * last_cmd = nullptr;

	uintptr_t * frame_pointer;
	__asm mov frame_pointer, ebp;
	bool & send_packet = *reinterpret_cast< bool * >( *frame_pointer - 0x1C );

	const auto get_random_seed = [ & ] ( ) {
		using o_fn = unsigned long ( __cdecl * )( std::uintptr_t );
		static auto offset = utilities::pattern_scan ( "client.dll", "55 8B EC 83 E4 F8 83 EC 70 6A 58" );
		static auto MD5_PseudoRandom = reinterpret_cast< o_fn >( offset );
		return MD5_PseudoRandom ( cmd->command_number ) & 0x7FFFFFFF;
	};

	cmd->randomseed = get_random_seed ( );

	csgo::cmd = cmd;
	local_player::begin_tick ( cmd );
	local_player::m_data.pointer = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( interfaces::engine->get_local_player ( ) ) );
	csgo::local_player = local_player::m_data.pointer;

	tickbase_system::pre_movement ( );

	if ( local_player::m_data.alive ) {
		auto count = *( std::uint32_t * ) ( ( std::uint32_t ) local_player::m_data.pointer->get_renderable ( ) + 0x2918 );

		std::memcpy ( &csgo::player_bones [ local_player::m_data.pointer->index ( ) ], *( void ** ) ( ( std::uint32_t ) local_player::m_data.pointer->get_renderable ( ) + 0x290C ), sizeof ( matrix_t ) * count );
	}


	aimbot::autostop::m_data.slow_walk_this_tick = false;
	csgo::m_strafe_angles = cmd->viewangles;
	auto old_viewangles = cmd->viewangles;
	auto old_forwardmove = cmd->forwardmove;
	auto old_sidemove = cmd->sidemove;
	interfaces::engine->get_view_angles ( csgo::original_viewangle );

	cmd->buttons |= in_bullrush;
	bool pressed_right = interfaces::inputsystem->is_button_down ( button_code_t::KEY_D );
	bool pressed_left = interfaces::inputsystem->is_button_down ( button_code_t::KEY_A );

	bool pressed_up = interfaces::inputsystem->is_button_down ( button_code_t::KEY_W );
	bool pressed_down = interfaces::inputsystem->is_button_down ( button_code_t::KEY_S );
	if ( ( local_player::m_data.pointer->flags ( ) & fl_onground ) ) {
		if ( pressed_right )
			cmd->sidemove = 450.f;
		else if ( pressed_left )
			cmd->sidemove = -450.f;

		if ( pressed_down )
			cmd->forwardmove = -450.f;
		else if ( pressed_up )
			cmd->forwardmove = 450.f;
	}

	nade_pred.trace ( cmd );
	visuals::projectiles::grenades::proximity::think ( );
	aimbot::autostop::slow_walk ( cmd );
	misc::movement::bunny_hop ( cmd );

	csgo::m_tick = cmd->tick_count;


	engine_prediction::initialize ( local_player::m_data.pointer, cmd );
	{
		fake_lag::create_move ( cmd, send_packet );

		anti_aim::on_create_move ( cmd, send_packet );

		player_manager::setup_records ( );

		aimbot::create_move ( cmd, send_packet );

		
	


	


		if ( !config.
			antiaim_fakelag_flags [ 4 ] && cmd->buttons & in_attack ) {
			send_packet = true;
		}
	

	} 	engine_prediction::unload ( local_player::m_data.pointer );






	
	misc::movement::fix_move ( cmd, csgo::m_strafe_angles );

	tickbase_system::post_movement ( );


	cmd->viewangles.angle_normalize ( );
	cmd->viewangles.angle_clamp ( );



	csgo::send_packet = send_packet;


			
		

	if ( send_packet )
		csgo::real_angle = cmd->viewangles;


	return false;
}

static bool initialized = false;


long __stdcall hooks::reset::hook ( IDirect3DDevice9 * device, D3DPRESENT_PARAMETERS * present_parameters ) {
	if ( !initialized )
		reset_original ( device, present_parameters );

	c_menu::get ( ).invalidate_objects ( );
	long hr = reset_original ( device, present_parameters );
	c_menu::get ( ).create_objects ( device );

	return hr;
}
extern LRESULT ImGui_ImplDX9_WndProcHandler ( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam );

LRESULT __stdcall hooks::wnd_proc::hook ( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam ) {
	static bool pressed = false;

	if ( !pressed && GetAsyncKeyState ( VK_INSERT ) ) {
		pressed = true;
	}
	else if ( pressed && !GetAsyncKeyState ( VK_INSERT ) ) {
		pressed = false;

		c_menu::get ( ).opened = !c_menu::get ( ).opened;
	}

	if ( c_menu::get ( ).opened && ImGui_ImplDX9_WndProcHandler ( hwnd, message, wparam, lparam ) )
		return true;

	return CallWindowProcA ( wndproc_original, hwnd, message, wparam, lparam );
}

void __stdcall hooks::paint_traverse::hook ( unsigned int panel, bool force_repaint, bool allow_force ) {
	auto panel_to_draw = fnv::hash ( interfaces::panel->get_panel_name ( panel ) );


	bool old_tickbase = tickbase_system::m_shift_data.m_should_attempt_shift;

	switch ( panel_to_draw ) {
	case fnv::hash ( "MatSystemTopPanel" ):
	{
	
		interfaces::engine->get_screen_size ( csgo::screen_width, csgo::screen_height );

	
	}
	break;

	case fnv::hash ( "FocusOverlayPanel" ):
		interfaces::panel->set_keyboard_input_enabled ( panel, c_menu::get ( ).opened );
		interfaces::panel->set_mouse_input_enabled ( panel, c_menu::get ( ).opened );
		break;
	}

	csgo::m_goal_shift = config.ragebot_double_tap_ticks;
	tickbase_system::m_shift_data.m_should_attempt_shift = config.ragebot_double_tap;
	// (!) Temporary - replace with checkbox & hotkey later.
	if ( old_tickbase != tickbase_system::m_shift_data.m_should_attempt_shift ) {
		//g_notify.add( tfm::format( XOR( "Tried shifting tickbase" )) );

		if ( tickbase_system::m_shift_data.m_should_attempt_shift )
			tickbase_system::m_shift_data.m_needs_recharge = csgo::m_goal_shift;
		else
			tickbase_system::m_shift_data.m_needs_recharge = 0;

		tickbase_system::m_shift_data.m_did_shift_before = false;
	}

	static unsigned int panelID, panelHudID;

	if ( !panelHudID )
		if ( fnv::hash ( "HudZoom" ) == panel_to_draw ) {
			panelHudID = panel;
		}

	if ( panelHudID == panel && local_player::m_data.pointer && local_player::m_data.pointer->is_alive ( ) ) {
		if ( local_player::m_data.pointer->is_scoped ( ) )
			return;
	}

	paint_traverse_original ( interfaces::panel, panel, force_repaint, allow_force );
}

void __fastcall hooks::do_procedural_foot_plant::hook ( void * a1, void * _edx, int a2, int a3, int a4, int a5 ) {
	auto orig = *( std::uint32_t * ) ( ( uintptr_t ) a1 + 96 );
	*( std::uint32_t * ) ( ( uintptr_t ) a1 + 96 ) = 0;

	__asm {
		mov ecx, a1
		push a5
		push a4
		push a3
		push a2
		call do_procedural_foot_plant_original
	}

	*( std::uint32_t * ) ( ( uintptr_t ) a1 + 96 ) = orig;
}
bool __fastcall hooks::post_screen_space_fx::hook ( uintptr_t ecx, uintptr_t edx, const view_setup_t * setup ) {
	if ( !local_player::m_data.pointer )
		return post_screen_space_original ( ecx, setup );

	visuals::player::think ( );

	return post_screen_space_original ( ecx, setup );
}
/*
void _fastcall hooks::do_extra_bone_processing::hook( void* ecx, uint32_t, void* hdr, vec3_t* pos, void* q, void* matrix, uint8_t* bone_computed, void* context ) {
	auto player = ( player_t* ) ecx;


	//if ( player->m_fEffects( ) & 8 )
		//return;
	//printf( " player->m_fEffects( ) ptr = %i \n", player->m_fEffects( ) );
	const auto state =  player->get_anim_state( );

	 bool old_on_ground = state->m_bOnGround;

	if ( !state )
		return do_extra_bone_processing_original( player, hdr, pos, q, matrix, bone_computed, context );
	state->m_bOnGround = false;

	do_extra_bone_processing_original( player, hdr, pos, q, matrix, bone_computed, context );

	state->m_bOnGround = old_on_ground;
}*/

void _fastcall hooks::standard_blending_rules::hook ( player_t * player, uint32_t, c_studio_hdr * hdr, vec3_t * pos, quaternion_t * q, const float time, int mask ) {
	if ( local_player::m_data.pointer && player ) {
		if ( player->team ( ) != local_player::m_data.pointer->team ( ) && player != local_player::m_data.pointer )
			mask = BONE_USED_BY_SERVER;

		player->m_fEffects ( ) |= 0x008;
		standard_blending_rules_original ( player, hdr, pos, q, time, mask );
		player->m_fEffects ( ) &= ~0x008;
	}
}


void __fastcall hooks::update_client_side_animation::hook ( player_t * player, uint32_t ) {


	if ( !player )
		return update_client_side_animation_original ( player );


	if ( animations::player_data [ player->index ( ) ].update_anims )
		update_client_side_animation_original ( player );
}

static bool init = false;


bool __fastcall hooks::setup_bones::hook ( void * ecx, void * edx, matrix_t * bone_to_world_out, int max_bones, int bone_mask, float curtime ) {

	
	if ( !interfaces::clientstate )
		return  setup_bones_original ( ecx, bone_to_world_out, max_bones, bone_mask, curtime );
	if ( !local_player::m_data.pointer )
		return  setup_bones_original ( ecx, bone_to_world_out, max_bones, bone_mask, curtime );

	if ( !local_player::m_data.pointer )
		return  setup_bones_original ( ecx, bone_to_world_out, max_bones, bone_mask, curtime );

	auto e = reinterpret_cast < player_t * >( reinterpret_cast < uintptr_t > ( ecx ) - 0x4 );

	if ( !e )
		return setup_bones_original ( ecx, bone_to_world_out, max_bones, bone_mask, curtime );
	
	
	if ( !(e->index ( ) < 64 && e->index ( ) > 0 ))
		return setup_bones_original ( ecx, bone_to_world_out, max_bones, bone_mask, curtime );

	auto anim = animations::player_data [ e->index ( ) ];
	
	

	if ( e == local_player::m_data.pointer ) {
	
		return  setup_bones_original ( ecx, bone_to_world_out, max_bones, bone_mask, curtime );
	}
	else {
		if ( !anim.update_bones ) {
			return  setup_bones_original ( ecx, bone_to_world_out, max_bones, bone_mask, curtime );
		}
		int restore_magic = 0;
		float restore_frametime = 0.f;

		/*
			- C_BasePlayer::SkipAnimationFrame, fix for matrix being 1 tick too late?
			- IDA signature if anyone wants to take a look themselves : 57 8B F9 8B 07 8B 80 ? ? ? ? FF D0 84 C0 75 02 5F C3
		*/

		int & last_animation_framecount = *reinterpret_cast< int * >( uintptr_t ( e ) + 0xA68 );
		int & magic = *reinterpret_cast< int * >( uintptr_t ( e ) + 0x64 );

		restore_frametime = interfaces::globals->frame_time;
		restore_magic = magic;

		if ( !( magic % 3 ) ) {
			magic = 9;
		}

		/*
			- C_BasePlayer::SkipAnimationFrame
			- if ( frametime < 0.0033333334 )
		*/

		if ( interfaces::globals->frame_time > 0.0033333334f ) {
			interfaces::globals->frame_time = 0.003333333f;
		}
		//
			/*
				- C_BasePlayer::SkipAnimationFrame
				- Pseudo code :
					last_animation_framecount = *(player + 0xA68);
					framecount = g_Globals->m_framecount;
					if ( last_animation_framecount ) {
						result = abs(framecount - last_animation_framecount);
						if ( result < 2 )

			*/
		last_animation_framecount = interfaces::globals->frame_count;

		int & occlusion_flag = *reinterpret_cast< int * >( uintptr_t ( e ) + 0xA24 );
		int & prev_bone_mask = *reinterpret_cast< int * >( uintptr_t ( e ) + 0x269C );

		static auto offs_bone_mask = netvar_manager::get_net_var ( fnv::hash ( "DT_BaseAnimating" ), fnv::hash ( "m_nForceBone" ) ) + 0x20;

		*reinterpret_cast< int * >( uintptr_t ( e ) + offs_bone_mask ) = 0;
		prev_bone_mask = 0;

		/*
			- C_CSPlayer::AccumulateLayers, skip the function
			- Pseudo code :
				result = (*(*g_Engine + 372))();
				if ( result || !(*(v5 + 0xA28) & 0xA) )
			- IDA signature : 55 8B EC 57 8B F9 8B 0D ? ? ? ? 8B 01 8B 80
		*/
		occlusion_flag = 0xA;

	

		bool result = setup_bones_original ( ecx, bone_to_world_out, max_bones, bone_mask, curtime );

		magic = restore_magic;
		interfaces::globals->frame_time = restore_frametime;
		return result;
	}
}


void __fastcall hooks::build_transformations::hook ( void * ecx, void * edx, int a2, int a3, int a4, int a5, int a6, int a7 ) {
	auto player = ( player_t * ) ( ecx );
	if ( !player )
		return build_transformations_original ( ecx, edx, a2, a3, a4, a5, a6, a7 );

	auto o_jiggle_bones_enabled = player->JiggleBones ( );

     player->JiggleBones ( ) = false;
	build_transformations_original ( ecx, edx, a2, a3, a4, a5, a6, a7 );
	player->JiggleBones ( ) = o_jiggle_bones_enabled;
}
bool __fastcall hooks::is_hltv::hook ( void * ecx, void * edx ) {


	static const auto AccumulateLayersCall = ( void * ) ( utilities::pattern_scan ( "client.dll", "84 C0 75 0D F6 87" ) );
	static const auto SetupVelocityCall = ( void * ) ( utilities::pattern_scan ( "client.dll", "84 C0 75 38 8B 0D ? ? ? ? 8B 01 8B 80 ? ? ? ? FF D0" ) );

	if ( !interfaces::engine->is_in_game ( ) || !interfaces::engine->is_connected ( ) )
		return is_hltv_original ( ecx );

	if ( _ReturnAddress ( ) == AccumulateLayersCall || _ReturnAddress ( ) == SetupVelocityCall )
		return true;



	return is_hltv_original ( ecx );
}
bool __fastcall hooks::write_user_cmd::hook ( void * ecx, void * edx, int m_nSlot, bf_write * m_pBuffer, int m_nFrom, int m_nTo, bool m_bNewCmd ) {
	if ( tickbase_system::m_shift_data.m_ticks_to_shift <= 0 )
		return write_user_cmd_original ( ecx, m_nSlot, m_pBuffer, m_nFrom, m_nTo, m_bNewCmd );

	if (csgo::cmd && csgo::cmd->weaponselect != 0 )
		return write_user_cmd_original( ecx, m_nSlot, m_pBuffer, m_nFrom, m_nTo, m_bNewCmd );

	if ( m_nFrom != -1 )
		return true;

	m_nFrom = -1;

	int m_nTickbase = tickbase_system::m_shift_data.m_ticks_to_shift;
	tickbase_system::m_shift_data.m_ticks_to_shift = 0;

	int * m_pnNewCmds = ( int * ) ( ( uintptr_t ) m_pBuffer - 0x2C );
	int * m_pnBackupCmds = ( int * ) ( ( uintptr_t ) m_pBuffer - 0x30 );

	*m_pnBackupCmds = 0;

	int m_nNewCmds = *m_pnNewCmds;
	int m_nNextCmd = interfaces::clientstate->m_choked_commands + interfaces::clientstate->m_last_outgoing_command + 1;
	int m_nTotalNewCmds = min ( m_nNewCmds + abs ( m_nTickbase ), 16 );

	*m_pnNewCmds = m_nTotalNewCmds;

	for ( m_nTo = m_nNextCmd - m_nNewCmds + 1; m_nTo <= m_nNextCmd; m_nTo++ ) {
		if ( !write_user_cmd_original ( ecx, m_nSlot, m_pBuffer, m_nFrom, m_nTo, true ) )
			return false;

		m_nFrom = m_nTo;
	}

	c_usercmd * m_pCmd = interfaces::input->get_user_cmd ( m_nSlot, m_nFrom );
	if ( !m_pCmd )
		return true;

	c_usercmd m_ToCmd = *m_pCmd, m_FromCmd = *m_pCmd;
	m_ToCmd.command_number++;
	m_ToCmd.tick_count += 3 * csgo::m_rate;


	//interfaces::console->console_printf( "m_ToCMd_tick_count %i and m_rate is  %i \n", m_ToCmd.tick_count, csgo::m_rate );
	for ( int i = m_nNewCmds; i <= m_nTotalNewCmds; i++ ) {
		tickbase_system::write_user_cmd ( ( bf_write * ) m_pBuffer, &m_ToCmd, &m_FromCmd );
		m_FromCmd = m_ToCmd;

		m_ToCmd.command_number++;
		m_ToCmd.tick_count++;
	}

	tickbase_system::m_shift_data.m_current_shift = m_nTickbase;
	return true;
}


void __fastcall  hooks::cl_move::hook ( void * ecx ) {


	if ( tickbase_system::m_shift_data.m_should_attempt_shift && tickbase_system::m_shift_data.m_needs_recharge ) {
		--tickbase_system::m_shift_data.m_needs_recharge;

		tickbase_system::m_shift_data.m_did_shift_before = false;

		if ( tickbase_system::m_shift_data.m_needs_recharge == 0 ) {
			tickbase_system::m_shift_data.m_should_be_ready = true;
		}
	
		return;
	}

	


	cl_move_original ( ecx );
}
void _fastcall hooks::run_commmand::hook ( void * prediction, void * edx, player_t * player, c_usercmd * cmd, player_move_helper * move_helper ) {
	//return run_commmand_original( interfaces::prediction, player, cmd, move_helper );
	if ( !player )
		return run_commmand_original ( interfaces::prediction, player, cmd, move_helper );

	if ( player->index ( ) != interfaces::engine->get_local_player ( ) )
		return run_commmand_original ( interfaces::prediction, player, cmd, move_helper );
	//interfaces::console->console_printf( "CMD->TICKCOUNT %i  calc %i csgo::m_tick %i\n", cmd->tick_count, ( csgo::m_tick + int( 1 / interfaces::globals->interval_per_tick ) + 8 ), csgo::m_tick );
	if ( cmd->tick_count >= ( csgo::m_tick + int ( 1 / interfaces::globals->interval_per_tick ) + 8 ) ) {
		cmd->hasbeenpredicted = true;
		return;
	}
	int backup_tickbase = player->get_tick_base ( );
	float backup_curtime = interfaces::globals->cur_time;

	if ( cmd->command_number == tickbase_system::m_prediction.m_shifted_command ) {
		player->get_tick_base ( ) = ( tickbase_system::m_prediction.m_original_tickbase - tickbase_system::m_prediction.m_shifted_ticks + 1 );
		++player->get_tick_base ( );

		interfaces::globals->cur_time = math::ticks_to_time ( player->get_tick_base ( ) );
	}
	float m_flVelModBackup = player->m_flVelocityModifier ( );
	if ( csgo::m_update && cmd->command_number == interfaces::clientstate->m_last_command_ack + 1 )
		player->m_flVelocityModifier ( ) = engine_prediction::m_stored_variables.m_flVelocityModifier;


	run_commmand_original ( interfaces::prediction, player, cmd, move_helper );

	if ( !csgo::m_update )
		player->m_flVelocityModifier ( ) = m_flVelModBackup;

	// restore tickbase and curtime.
	if ( cmd->command_number == tickbase_system::m_prediction.m_shifted_command ) {
		player->get_tick_base ( ) = backup_tickbase;

		interfaces::globals->cur_time = backup_curtime;
	}

	g_netdata.store ( );
}
void __fastcall hooks::fire_event::hook ( void * ecx, void * edx ) {
	if ( !local_player::m_data.pointer || !interfaces::engine->is_in_game ( ) || !interfaces::engine->is_connected ( ) )
		return fire_event_original ( ecx );




	/*	auto CL_ParseEventDelta = [ ] ( void* RawData, void* pToData, recv_table* pRecvTable )
		{
			// "RecvTable_DecodeZeros: table '%s' missing a decoder.", look at the function that calls it.
			static uintptr_t CL_ParseEventDeltaF = ( uintptr_t ) utilities::pattern_scan( ( "engine.dll" ), ( "55 8B EC 83 E4 F8 53 57" ) );
			__asm
			{
				mov     ecx, RawData
				mov     edx, pToData
				push	pRecvTable
				call    CL_ParseEventDeltaF
				add     esp, 8
			}
		};*/

		// Filtering events
	if ( !local_player::m_data.pointer || !local_player::m_data.pointer->is_alive ( ) || !config.ragebot_enabled )
		return fire_event_original ( ecx );


	struct event_t {
	public:
		uint8_t uchPad0 [ 4 ];
		float delay;
		uint8_t uchPad2 [ 48 ];
		event_t * next;
	};

	auto ei = interfaces::clientstate->m_Events;

	event_info * next = nullptr;

	if ( !ei )
		return fire_event_original ( ecx );

	do {
		next = ei->pNext;
		ei->flDelay = 0.f;
		ei = next;
	} while ( next );
	return fire_event_original ( ecx );
}

void __fastcall hooks::crc_server_check::hook ( void * ecx, void * edx ) {
	return;
}

void __stdcall hooks::render_smoke_overlay::hook ( void * ctx, bool a ) {
	render_smoke_overlay_original ( ctx, a );
}
void __fastcall hooks::trace_ray::hook ( void * thisptr, void *, const ray_t & ray, unsigned int fMask, trace_filter * pTraceFilter, trace_t * pTrace ) {

	if ( !csgo::in_trace )
		return trace_ray_original ( thisptr, ray, fMask, pTraceFilter, pTrace );

	trace_ray_original ( thisptr, ray, fMask, pTraceFilter, pTrace );

	pTrace->surface.flags |= SURF_SKY;
}
void __stdcall hooks::scene_end::hook ( ) {
	visuals::player::paint_traverse ( );

	visuals::local_esp_think ( );
	nade_pred.paint_traverse ( );
	visuals::projectiles::think ( );

	scene_end_original ( interfaces::render_view );
}

int __fastcall hooks::file_system::hook ( void * ecx, void * edx ) {
	static auto return_to_client_panorama = utilities::pattern_scan ( "client.dll", "83 F8 02 0F 85 ? ? ? ? 8B 3D ? ? ? ? FF D7" );
	static auto return_to_engine = utilities::pattern_scan ( "engine.dll", "83 F8 02 75 6C 68 ? ? ? ? FF 15 ? ? ? ? 8B 4C 24 28 83 C4 04" );

	if ( _ReturnAddress ( ) == return_to_client_panorama || _ReturnAddress ( ) == return_to_engine )
		return 0; //return 0 in some cases to not being kicked from game

	return 1; //return 1 to allow loading of chams materials
}

void __fastcall hooks::check_for_sequence_change::hook ( uintptr_t * ecx, uintptr_t * edx, uintptr_t * hdr, int cur_sequence, bool force_new_sequence, bool interpolate ) {

	return check_for_sequence_change_original ( ecx, edx, hdr, cur_sequence, force_new_sequence, false );

}

long __stdcall hooks::present::hook ( IDirect3DDevice9 * device ) {
	if ( !initialized ) {
		//	D3DXCreateTextureFromFileInMemoryEx ( device
			//	, &asdsadsadad, sizeof ( asdsadsadad ),
				//55, 55, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &menu::buttonimage );

		c_menu::get ( ).apply_fonts ( );

		c_menu::get ( ).setup_resent ( device );
		initialized = true;
	}
	if ( initialized ) {
		c_menu::get ( ).pre_render ( device );
		c_menu::get ( ).post_render ( );

		//	c_menu::get( ).run_popup( );
		c_menu::get ( ).run ( );
		c_menu::get ( ).run_visuals ( );


		c_menu::get ( ).end_present ( device );
		//visuals::circle_filled ( visuals::m_local.autowall_crosshair.end_pos_2d.x, visuals::m_local.autowall_crosshair.end_pos_2d.y, 30, visuals::m_local.autowall_crosshair.plane.x, 100, D3DCOLOR_ARGB ( 205, 000, 000, 200 ) );
	}
	return present_original ( device );
}
