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

#define MULTIPLAYER_BACKUP 150

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
void * sv_cheats_original = nullptr;
hooks::modify_eye_position::fn  modify_eye_position_original = nullptr;
hooks::check_for_sequence_change::fn check_for_sequence_change_original = nullptr;
hooks::calculate_view::fn calculate_view_original = nullptr;
hooks::update_animation_state::fn update_animation_state_original = nullptr;
hooks::is_hltv::fn is_hltv_original = nullptr;
hooks::in_prediction::fn in_prediction_original = nullptr;
hooks::process_packet::fn process_packet_original = nullptr;
hooks::physics_simulate::fn physics_simulate_original = nullptr;
hooks::list_leaves_in_box::fn list_leaves_in_box_original = nullptr;
hooks::process_interpolated_list::fn process_interpolated_list_original = nullptr;

HWND hooks::window;
WNDPROC hooks::wndproc_original = NULL;

hooks::packet_start::fn packet_start_original = nullptr;
hooks::packet_end::fn packet_end_original = nullptr;


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

	auto process_interpolated_list_target = reinterpret_cast< void * >( utilities::pattern_scan ( "client.dll", "53 0F B7 1D ? ? ? ? 56" ) );
	auto frame_stage_target = reinterpret_cast< void * >( get_virtual ( interfaces::client, 37 ) );
	auto create_move_target = reinterpret_cast< void * >( get_virtual ( interfaces::clientmode, 24 ) );
	auto paint_traverse_target = reinterpret_cast< void * >( get_virtual ( interfaces::panel, 41 ) );
	auto post_screen_space_fx = reinterpret_cast< void * >( get_virtual ( interfaces::clientmode, 44 ) );
	auto draw_model_execute_target = reinterpret_cast< void * >( get_virtual ( interfaces::model_render, 21 ) );
	auto scene_end_target = reinterpret_cast< void * >( get_virtual ( interfaces::render_view, 9 ) );
	auto setup_bones_target = reinterpret_cast< void * >( get_virtual ( sbf, 13 ) );
	auto do_procedural_foot_plant_target = reinterpret_cast< void * >( utilities::pattern_scan ( "client.dll", "55 8B EC 83 E4 F0 83 EC 78 56 8B F1 57 8B 56 60" ) );
	//auto do_extra_bone_processing_target = reinterpret_cast< void * >( get_virtual ( pt, 197 ) );
	auto should_skip_animation_frame_target = reinterpret_cast< void * >( utilities::pattern_scan ( "client.dll", "57 8B F9 8B 07 8B 80 ? ? ? ? FF D0 84 C0 75 02" ) );
	auto list_leaves_in_box_target = reinterpret_cast< void * >( get_virtual ( interfaces::engine->get_bsp_tree_query(), 6 ) );

	//auto in_prediction_target = ( void * ) get_virtual ( interfaces::prediction, 14 );

	auto check_for_sequence_change_target = reinterpret_cast< void * >( utilities::pattern_scan ( "client.dll", "55 8B EC 51 53 8B 5D 08 56 8B F1 57 85" ) );
	//auto modify_eye_position_target = reinterpret_cast< void * >( utilities::pattern_scan ( "client.dll", "55 8B EC 83 E4 F8 83 EC 5C 53 8B D9 56 57 83" ) );

	//auto accumulate_layers_target = reinterpret_cast< void * >( get_virtual ( pt, 9 ) );
	//auto get_eye_angles_target = reinterpret_cast< void * >( get_virtual ( pt, 169 ) );
	auto standard_blending_rules_target = reinterpret_cast< void * >( get_virtual ( pt, 205 ) );
	auto update_client_side_animation_target = reinterpret_cast< void * >( get_virtual ( pt, 223 ) );
	//auto render_smoke_overlay_target = reinterpret_cast< void * >( get_virtual ( interfaces::render_view, 41 ) );
	//auto trace_ray_target = reinterpret_cast< void * >( get_virtual ( interfaces::trace_ray, 8 ) );
	auto fire_event_target = reinterpret_cast< void * >( get_virtual ( interfaces::engine, 59 ) ); // working
	auto process_packet_target = reinterpret_cast< void * >( utilities::pattern_scan ( "engine.dll", "55 8B EC 83 E4 C0 81 EC ? ? ? ? 53 56" ));

	//auto physics_simulate_target = reinterpret_cast< void * >( utilities::pattern_scan ( "client.dll", "56 8B F1 8B 8E ? ? ? ? 83 F9 FF 74 21" ) );

	//auto get_color_modulation_target = reinterpret_cast< void * >( utilities::pattern_scan ( "materialsystem.dll", "55 8B EC 83 EC ? 56 8B F1 8A 46" ) );
	
	//auto is_using_static_prop_debug_modes_target = reinterpret_cast< void * >( utilities::pattern_scan ( "engine.dll", "8B 0D ? ? ? ? 81 F9 ? ? ? ? 75 ? A1 ? ? ? ? 35 ? ? ? ? EB ? 8B 01 FF 50 ? 83 F8 ? 0F 85 ? ? ? ? 8B 0D" ) );

	auto packet_end_target = reinterpret_cast< void * >( get_virtual ( ( i_client_state * ) ( uint32_t ( interfaces::clientstate ) + 0x8 ), 6 ) );

	auto packet_start_target = reinterpret_cast< void * >( get_virtual ( ( i_client_state * ) ( uint32_t ( interfaces::clientstate ) + 0x8 ), 5 ) );

	auto override_view_target = reinterpret_cast< void * >( get_virtual ( interfaces::clientmode, 18 ) );
	//auto write_user_cmd_target = reinterpret_cast< void * >( get_virtual ( interfaces::client, 24 ) );
	auto cl_move_target = reinterpret_cast< void * >( utilities::pattern_scan ( "engine.dll", "55 8B EC 81 EC ? ? ? ? 53 56 57 8B 3D ? ? ? ? 8A" ) );
	//auto run_commmand_target = reinterpret_cast< void * >( get_virtual ( interfaces::prediction, 19 ) );
	auto crc_server_check_target = reinterpret_cast< void * >( utilities::pattern_scan ( "engine.dll", "55 8B EC 81 EC ? ? ? ? 53 8B D9 89 5D F8 80" ) );
	auto file_system_target = reinterpret_cast< void * >( get_virtual ( interfaces::file_system, 128 ) );
	auto sv_cheats_target = reinterpret_cast< void * >( get_virtual ( interfaces::console->get_convar ( "sv_cheats" ), 13 ));

	auto is_hltv_target = reinterpret_cast< void * >( get_virtual ( interfaces::engine, 93 ) );

	auto build_transformations_target = reinterpret_cast< void * >( utilities::pattern_scan ( "client.dll", "55 8B EC 83 E4 F0 81 EC ? ? ? ? 56 57 8B F9 8B 0D ? ? ? ? 89 7C 24 1C" ) );

	//auto calculate_view_target = reinterpret_cast< void * >( utilities::pattern_scan ( "client.dll", "55 8B EC 83 EC 14 53 56 57 FF 75 18" ) );
	//auto update_animation_state_target = reinterpret_cast< void * >( utilities::pattern_scan ( "client.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3" ) );

	auto present_target = reinterpret_cast< void * >( get_virtual ( d3d_device, 42 ) );
	auto reset_target = reinterpret_cast< void * >( get_virtual ( d3d_device, 16 ) );


	if ( MH_Initialize ( ) != MH_OK ) {
		//throw std::runtime_error ( "failed to initialize MH_Initialize." );
		
	}

	if ( MH_CreateHook ( frame_stage_target, &frame_stage::hook, reinterpret_cast< void ** >( &frame_stage_original ) ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize frame_stage. (outdated index?)" );
		
	}
/*	if ( MH_CreateHook ( list_leaves_in_box_target, &list_leaves_in_box::hook, reinterpret_cast< void ** >( &list_leaves_in_box_original ) ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize frame_stage. (outdated index?)" );
		
	}*/
	if ( MH_CreateHook ( sv_cheats_target, &sv_cheats::hook, reinterpret_cast< void ** >( &sv_cheats_original ) ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize sv_cheats. (outdated index?)" );
		
	}
	/*if ( MH_CreateHook ( process_interpolated_list_target, &process_interpolated_list::hook, reinterpret_cast< void ** >( &process_interpolated_list_original ) ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize process_interpolated_list. (outdated index?)" );

	}*/
	

	/*if ( MH_CreateHook ( check_for_sequence_change_target, &check_for_sequence_change::hook, reinterpret_cast< void ** >( &check_for_sequence_change_original ) ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize check_for_sequence_change. (outdated index?)" );
		
	}*/



	if ( MH_CreateHook ( crc_server_check_target, &crc_server_check::hook, reinterpret_cast< void ** >( &crc_server_check_original ) ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize crc_server_check. (outdated index?)" );
		
	}

	if ( MH_CreateHook ( packet_start_target, &packet_start::hook, reinterpret_cast< void ** >( &packet_start_original ) ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize packet_start. (outdated index?)" );
		
	}

	if ( MH_CreateHook ( packet_end_target, &packet_end::hook, reinterpret_cast< void ** >( &packet_end_original ) ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize packet_end. (outdated index?)" );
		
	}
	
	 if ( MH_CreateHook ( should_skip_animation_frame_target, &should_skip_animation_frame::hook, reinterpret_cast< void ** >( &should_skip_animation_frame_original ) ) != MH_OK ) {
		  throw std::runtime_error ( "failed to initialize should_skip_animation_frame. (outdated index?)" );
		  
	  }
	  if ( MH_CreateHook ( is_hltv_target, &is_hltv::hook, reinterpret_cast< void ** >( &is_hltv_original ) ) != MH_OK ) {
		 throw std::runtime_error ( "failed to initialize is_hltv. (outdated index?)" );
		 
	 }

	 
	 /*if ( MH_CreateHook ( get_color_modulation_target, &get_color_modulation::hook, reinterpret_cast< void ** >( &get_color_modulation_original ) ) != MH_OK ) {
	 throw std::runtime_error ( "failed to initialize get_color_modulation. (outdated index?)" );
	    
	 }
	 if ( MH_CreateHook ( is_using_static_prop_debug_modes_target, &is_using_static_prop_debug_modes::hook, reinterpret_cast< void ** >( &is_using_static_prop_debug_modes_original ) ) != MH_OK ) {
		 throw std::runtime_error ( "failed to initialize is_using_static_prop_debug_modes. (outdated index?)" );
		 
	 }*/
	 
	if ( MH_CreateHook ( build_transformations_target, &build_transformations::hook, reinterpret_cast< void ** >( &build_transformations_original ) ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize build_transformations. (outdated index?)" );
		
	}

	if ( MH_CreateHook ( create_move_target, &create_move::hook, reinterpret_cast< void ** >( &create_move_original ) ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize create_move. (outdated index?)" );
		
	}
	if ( MH_CreateHook ( process_packet_target, &process_packet::hook, reinterpret_cast< void ** >( &process_packet_original ) ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize process_packet. (outdated index?)" );
		
	}



	if ( MH_CreateHook ( paint_traverse_target, &paint_traverse::hook, reinterpret_cast< void ** >( &paint_traverse_original ) ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize paint_traverse. (outdated index?)" );
		
	}

	if ( MH_CreateHook ( post_screen_space_fx, &post_screen_space_fx::hook, reinterpret_cast< void ** >( &post_screen_space_original ) ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize post_screen_space_fx. (outdated index?)" );
		
	}


	if ( MH_CreateHook ( scene_end_target, &scene_end::hook, reinterpret_cast< void ** >( &scene_end_original ) ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize scene_end. (outdated index?)" );
		
	}

	if ( MH_CreateHook ( present_target, &present::hook, reinterpret_cast< void ** >( &present_original ) ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize present. (outdated index?)" );
		
	}
	if ( MH_CreateHook ( reset_target, &reset::hook, reinterpret_cast< void ** >( &reset_original ) ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize reset. (outdated index?)" );
		
	}
	if ( MH_CreateHook ( setup_bones_target, &setup_bones::hook, reinterpret_cast< void ** >( &setup_bones_original ) ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize setup_bones. (outdated index?)" );
		
	}

	if ( MH_CreateHook ( do_procedural_foot_plant_target, &do_procedural_foot_plant::hook, ( void ** ) &do_procedural_foot_plant_original ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize prodcedural foot plant. (outdated index?)" );
		
	}
	if ( MH_CreateHook ( file_system_target, &file_system::hook, ( void ** ) &file_system_original ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize file_system (outdated index?)" );
		
	}

    if ( MH_CreateHook ( draw_model_execute_target, &hooks::draw_model_exec::hook, ( void ** ) &o_draw_model_exec ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize prodcedural draw_model_execute. (outdated index?)" );
		
	}

	if ( MH_CreateHook ( update_client_side_animation_target, &update_client_side_animation::hook, ( void ** ) &update_client_side_animation_original ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize update client side. (outdated index?)" );
		
	}

	if ( MH_CreateHook ( standard_blending_rules_target, &standard_blending_rules::hook, ( void ** ) &standard_blending_rules_original ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize standard blending rules. (outdated index?)" );
		
	}

	if ( MH_CreateHook ( fire_event_target, &fire_event::hook, ( void ** ) &fire_event_original ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize fire event. (outdated index?)" );
		
	}
	if ( MH_CreateHook ( override_view_target, &override_view::hook, reinterpret_cast< void ** >( &override_view_original ) ) != MH_OK ) {
		throw std::runtime_error ( "failed to initialize override_view. (outdated index?)" );
		
	}


	if ( MH_EnableHook ( MH_ALL_HOOKS ) != MH_OK ) {
		throw std::runtime_error ( "failed to enable hooks." );
		
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


#define MAX_COORD_FLOAT ( 16384.0f )
#define MIN_COORD_FLOAT ( -MAX_COORD_FLOAT )
int __fastcall hooks::list_leaves_in_box::hook ( void * bsp, void * edx, vec3_t & mins, vec3_t & maxs, unsigned short * pList, int listMax ) {

	if ( !interfaces::engine->is_connected ( ) || !interfaces::engine->is_in_game ( ) || !local_player::m_data.pointer )
		return list_leaves_in_box_original ( bsp, edx, mins, maxs, pList, listMax );

	static auto list_leaves_in_box_return = utilities::pattern_scan ( "client.dll", "89 44 24 14 EB 08 C7 44 24 ? ? ? ? ? 8B 45" );
	if ( _ReturnAddress ( ) != ( void * ) list_leaves_in_box_return )
		return list_leaves_in_box_original ( bsp, edx, mins, maxs, pList, listMax );

	struct RenderableInfoT {
		void * renderable;
		void * alpha_property;
		int enum_count;
		int render_frame;
		unsigned short first_shadow;
		unsigned short leaf_list;
		short area;
		uint16_t flags;
		uint16_t flags2;
		vec3_t bloated_abs_mins;
		vec3_t bloated_abs_maxs;
		vec3_t abs_mins;
		vec3_t abs_maxs;
		int pad;
	};

	auto info = *( RenderableInfoT ** ) ( ( uintptr_t ) _AddressOfReturnAddress ( ) + 0x14 );
	if ( !info || !info->renderable )
		return list_leaves_in_box_original ( bsp, edx, mins, maxs, pList, listMax );

	/* made it ghetto. true */
	auto ClientUnknown = [ ] ( void * renderable ) {
		typedef void * ( __thiscall * o_fn )( void * );
		return ( *( o_fn ** ) renderable ) [ 0 ] ( renderable );
	};

	auto BaseEntity = [ ] ( void * c_unk ) {
		typedef player_t * ( __thiscall * o_fn )( void * );
		return ( *( o_fn ** ) c_unk ) [ 7 ] ( c_unk );
	};

	auto Entity = BaseEntity ( ClientUnknown ( info->renderable ) );

	if ( !Entity || !Entity->is_alive ( ))
		return list_leaves_in_box_original ( bsp, edx, mins, maxs, pList, listMax );

	if ( !(Entity->client_class ( )->class_id == cs_player || Entity->client_class ( )->class_id == cs_ragdoll))
		return list_leaves_in_box_original ( bsp, edx, mins, maxs, pList, listMax );

	*( uint16_t * ) ( uintptr_t ( info ) + 0x0016 ) &= ~0x100;
	*( uint16_t * ) ( uintptr_t ( info ) + 0x0018 ) |= 0xC0;

	static vec3_t map_min = vec3_t ( MIN_COORD_FLOAT, MIN_COORD_FLOAT, MIN_COORD_FLOAT );
	static vec3_t map_max = vec3_t ( MAX_COORD_FLOAT, MAX_COORD_FLOAT, MAX_COORD_FLOAT );
	auto count = list_leaves_in_box_original ( bsp, edx, map_min, map_max, pList, listMax );
	return count;
}



bool __fastcall hooks::sv_cheats::hook ( void * ecx, void *  ) {
	if ( !ecx )
		return false;
	
	if ( !interfaces::engine->is_in_game ( ) )
		return ( ( bool ( __thiscall * )( void * ) ) sv_cheats_original )( ecx );

	if ( local_pointer  ) {
		static auto cam_think = utilities::pattern_scan ( "client.dll", "85 C0 75 30 38 86" );

		if ( _ReturnAddress ( ) == cam_think )
			return true;
	}

	return ( ( bool ( __thiscall * )( void * ) ) sv_cheats_original )( ecx );
}

void __cdecl hooks::process_packet::hook ( void * packet, bool header ) {
	struct event_t {
	public:
		uint8_t uchPad0 [ 4 ];
		float delay;
		uint8_t uchPad2 [ 48 ];
		event_t * next;
	};


	if ( !interfaces::engine->get_net_channel ( ) )
		return process_packet_original ( packet, header );

	/* get this from CL_FireEvents string "Failed to execute event for classId" in engine.dll */
	auto event = *( event_t ** ) ( uintptr_t ( interfaces::clientstate ) + 0x4E6C );
	while ( event ) {
		event->delay = 0.0f;
		event = event->next;
	}

	/* game events are actually fired in OnRenderStart which is WAY later after they are received */
	/* effective delay by lerp time, now we call them right after theyre received (all receive proxies are invoked without delay). */
	interfaces::engine->fire_game_event ( );
}



void __fastcall  hooks::process_interpolated_list::hook ( ) {
	static auto allow_extrapolation = *( bool ** ) ( utilities::pattern_scan (  ( "client.dll" ),  ( "A2 ? ? ? ? 8B 45 E8" ) ) + 0x1 );

	if ( allow_extrapolation )
		*allow_extrapolation = false;

	return process_interpolated_list_original ( );
}
bool __fastcall hooks::should_skip_animation_frame::hook ( void * , void *  ) {
	return false;
}
vec3_t vecAimPunch, vecViewPunch;
vec3_t  pAimPunch = vec3_t();
vec3_t  pViewPunch = vec3_t ( );
void __fastcall hooks::frame_stage::hook ( void * ecx, void * edx, int stage ) {




	if ( !interfaces::engine->is_in_game ( ) )
		return frame_stage_original ( ecx, 0, stage );

	if ( !interfaces::engine->is_connected ( ) )
		return frame_stage_original ( ecx, 0, stage );



	switch ( stage ) {
	case FRAME_UNDEFINED:
		break;
	case FRAME_START:
		csgo::screen_matrix = interfaces::engine->world_to_screen_matrix ( );


		break;
	case FRAME_NET_UPDATE_START:
		shot_processor::manage_shots ( );
		break;
	case FRAME_NET_UPDATE_POSTDATAUPDATE_START:
	
		break;
	case FRAME_NET_UPDATE_POSTDATAUPDATE_END:
		player_manager::manage_bones ( );


		break;
	case FRAME_NET_UPDATE_END:

		player_manager::setup_players ( );
		//g_netdata.apply ( );
		break;
	case FRAME_RENDER_START:
 
		static auto blur = interfaces::material_system->find_material ( "dev/scope_bluroverlay", TEXTURE_GROUP_OTHER );
		
		  blur->set_material_var_flag ( material_var_no_draw, config.visuals_world_removals [ 4 ] );

		csgo::m_rate = ( int ) std::round ( 1.f / interfaces::globals->interval_per_tick );


		if ( config.ragebot_enabled ) {
			for ( int i = 1; i <= interfaces::globals->max_clients; i++ ) {
				player_t * entity = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( i ) );


				if ( !entity )
					continue;

				if ( entity == local_player::m_data.pointer )
					continue;

				if ( entity->dormant ( ) )
					continue;

				if ( entity->is_teammate ( ) )
					continue;

				if ( !entity->is_alive ( ) )
					continue;

				//*( int * ) ( ( uintptr_t ) entity + 0xA30 ) = interfaces::globals->frame_count; //we'll skip occlusion checks now
				//*( int * ) ( ( uintptr_t ) entity + 0xA28 ) = 0;//clear occlusion flags

				animations::update_player_animation ( entity );
			}
		}
		local_player::m_data.pointer = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( interfaces::engine->get_local_player ( ) ) );
		csgo::local_player = local_player::m_data.pointer;

		if ( local_player::available )
		   animations::update_animations_local ( );

	
		

		misc::removals::remove_flash ( );
		misc::removals::remove_smoke ( );

		
		break;
	case FRAME_RENDER_END:

		break;
	default:
		break;
	}


	frame_stage_original ( ecx, 0, stage );

}



void __fastcall hooks::override_view::hook ( void * edx, void * , view_setup_t * setup ) {
	/*if ( interfaces::engine->is_in_game( ) ) {
		if ( local_player::m_data.pointer && local_player::m_data.pointer->is_alive( ) && variables::visuals::fov > 0 )
			setup->fov = 90 + variables::visuals::fov;
	}*/

	

	misc::thirdperson::think (  );

	if ( config.visuals_world_removals [ 0 ] && local_player::m_data.pointer && local_player::m_data.pointer->is_alive() ) {
		vec3_t viewPunch = local_player::m_data.pointer->punch_angle ( );
		vec3_t aimPunch = local_player::m_data.pointer->aim_punch_angle ( );

		setup->angles [ 0 ] -= ( viewPunch [ 0 ] + ( aimPunch [ 0 ] * 2 * 0.4499999f ) );
		setup->angles [ 1 ] -= ( viewPunch [ 1 ] + ( aimPunch [ 1 ] * 2 * 0.4499999f ) );
		setup->angles [ 2 ] -= ( viewPunch [ 2 ] + ( aimPunch [ 2 ] * 2 * 0.4499999f ) );
	}
	override_view_original ( interfaces::clientmode, edx, setup );

	
}
bool __fastcall hooks::create_move::hook ( void * , void * , float input_sample_frametime, c_usercmd * cmd ) {

	csgo::in_create_move = false;
	csgo::send_packet = true;
	if ( !cmd || !cmd->command_number )
		return create_move_original ( input_sample_frametime, cmd );



	if ( !interfaces::engine->is_in_game ( ) )
		return  create_move_original ( input_sample_frametime, cmd );



	static c_usercmd * last_cmd = nullptr;

	uintptr_t * frame_pointer = 0x0;
	__asm mov frame_pointer, ebp;

	csgo::in_create_move = true;
	local_player::m_data.aimbot_working = false;

	local_player::begin_tick ( cmd );

	misc::movement::slide ( cmd );

	aimbot::autostop::m_data.slow_walk_this_tick = false;


	nade_pred.trace ( cmd );

	visuals::projectiles::grenades::proximity::think ( );
	aimbot::autostop::slow_walk ( cmd );
	misc::movement::bunny_hop ( cmd );
	misc::movement::fake_duck ( cmd );



	engine_prediction::initialize ( local_player::m_data.pointer, cmd );
	{

		player_manager::setup_records ( );

		fake_lag::create_move ( csgo::send_packet );

		anti_aim::on_create_move ( cmd, csgo::send_packet );


		aimbot::create_move ( cmd, csgo::send_packet );


		legit_bot::run ( cmd );


		engine_prediction::unload ( local_player::m_data.pointer );
	}



	misc::movement::fix_move ( cmd );



	cmd->viewangles.angle_normalize ( );
	cmd->viewangles.angle_clamp ( );


	local_player::end_tick ( cmd );
	

	*reinterpret_cast< bool * >( *frame_pointer - 0x1C ) = csgo::send_packet;
	
	if ( csgo::send_packet )
		csgo::real_origin = local_player::m_data.pointer->origin ( );

	if ( csgo::send_packet )
		csgo::real_angle = cmd->viewangles;
	else
		csgo::fake_angle = cmd->viewangles;
	
	bool did_shoot = cmd->buttons & cmd_buttons::in_attack || cmd->buttons & cmd_buttons::in_attack2;

	if ( config.antiaim_3way )
		did_shoot = false;

	if ( csgo::send_packet  && !did_shoot )
		localdata.antiaim_yaw = csgo::real_angle.y;


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


	static unsigned int panelID = 0, panelHudID = 0;

	if ( !panelHudID )
		if ( config.visuals_world_removals [ 4 ] && fnv::hash ( "HudZoom" ) == panel_to_draw ) {
			panelHudID = panel;
		}

	if ( panelHudID == panel && local_player::m_data.pointer && local_player::m_data.pointer->is_alive ( ) ) {
		if ( local_player::m_data.pointer->is_scoped ( ) )
			return;
	}

	paint_traverse_original ( interfaces::panel, panel, force_repaint, allow_force );
}

void __fastcall hooks::do_procedural_foot_plant::hook ( void * a1, void * , int a2, int a3, int a4, int a5 ) {

	if ( !config.do_procedural_foot_plant_target ) { 
		__asm {
			mov ecx, a1
			push a5
			push a4
			push a3
			push a2
			call do_procedural_foot_plant_original
		}

	}
	
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
bool __fastcall hooks::post_screen_space_fx::hook ( uintptr_t ecx, uintptr_t, const view_setup_t * setup ) {
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
	if ( !config.standard_blending_rules_target ) {
		standard_blending_rules_original ( player, hdr, pos, q, time, mask );
		return;
	}

	if (!local_player::m_data.pointer )
		return standard_blending_rules_original ( player, hdr, pos, q, time, mask );

	if ( !player )
		return standard_blending_rules_original ( player, hdr, pos, q, time, mask );

	if (!player->is_alive() )
		return standard_blending_rules_original ( player, hdr, pos, q, time, mask );

	if ( player->dormant() )
		return standard_blending_rules_original ( player, hdr, pos, q, time, mask );

	if ( player->is_teammate() )
		return standard_blending_rules_original ( player, hdr, pos, q, time, mask );

	if ( local_player::m_data.pointer && player ) {

		player->m_fEffects ( ) |= 0x008;
		standard_blending_rules_original ( player, hdr, pos, q, time, BONE_USED_BY_SERVER );
		player->m_fEffects ( ) &= ~0x008;
	}
}


void __fastcall hooks::update_client_side_animation::hook ( player_t * player, uint32_t ) {
	
	if (!config.update_client_side_animation )
		return update_client_side_animation_original ( player );


	if ( !player )
		return update_client_side_animation_original ( player );

	if ( !player->is_alive ( ) )
		return update_client_side_animation_original ( player );

	if (player->dormant() )
		return update_client_side_animation_original ( player );

	if ( player->is_teammate ( ) )
		return update_client_side_animation_original ( player );

	auto & anim_data = animations::player_data [ player->index ( ) ];

	if ( anim_data.update_anims ) 
		update_client_side_animation_original ( player );
	
}

static bool init = false;

bool __fastcall hooks::setup_bones::hook ( void * ecx, void * , matrix3x4_t * bone_to_world_out, int max_bones, int bone_mask, float curtime ) {
	if ( !config.setup_bones )
		return setup_bones_original ( ecx, bone_to_world_out, max_bones, bone_mask, curtime );

	auto result = true;

	auto e = reinterpret_cast < player_t * >( reinterpret_cast < uintptr_t > ( ecx ) - 0x4 );

	if ( e == local_player::m_data.pointer )
		return setup_bones_original ( ecx, bone_to_world_out, max_bones, bone_mask, curtime );

	if ( !e )
		return setup_bones_original ( ecx, bone_to_world_out, max_bones, bone_mask, curtime );

	if ( e->dormant() )
		return setup_bones_original ( ecx, bone_to_world_out, max_bones, bone_mask, curtime );

	if ( e->is_teammate ( ) )
		return setup_bones_original ( ecx, bone_to_world_out, max_bones, bone_mask, curtime );

	if ( !e->is_alive ( ) )
		return  setup_bones_original ( ecx, bone_to_world_out, max_bones, bone_mask, curtime );




	
	if ( e->index ( ) > 64 )
		return setup_bones_original ( ecx, bone_to_world_out, max_bones, bone_mask, curtime );

	    auto& anim = animations::player_data [ e->index ( ) ];
	

		
		 //if ( !e->m_CachedBoneData ( ).Count ( ) )
			// return  setup_bones_original ( ecx, bone_to_world_out, max_bones, bone_mask, curtime );
		 if ( anim.update_bones || !e->m_CachedBoneData ( ).Count ( ) )
		    result = setup_bones_original ( ecx, bone_to_world_out, max_bones, bone_mask, curtime );
		else if (bone_to_world_out && max_bones != -1 )
		   memcpy ( bone_to_world_out, e->m_CachedBoneData ( ).Base ( ), e->m_CachedBoneData ( ).Count ( ) * sizeof ( matrix3x4_t ) );
	

		return result;
}


void __fastcall hooks::build_transformations::hook ( void * ecx, void * edx, int a2, int a3, int a4, int a5, int a6, int a7 ) {
	if (!config.build_transformations_target )
		return build_transformations_original ( ecx, edx, a2, a3, a4, a5, a6, a7 );

	auto player = ( player_t * ) ( ecx );

	if ( !player )
		return build_transformations_original ( ecx, edx, a2, a3, a4, a5, a6, a7 );

	if ( player->dormant() )
		return build_transformations_original ( ecx, edx, a2, a3, a4, a5, a6, a7 );

	if ( !player->is_alive() )
		return build_transformations_original ( ecx, edx, a2, a3, a4, a5, a6, a7 );

	if ( player->is_teammate() )
		return build_transformations_original ( ecx, edx, a2, a3, a4, a5, a6, a7 );

	auto o_jiggle_bones_enabled = player->JiggleBones ( );

     player->JiggleBones ( ) = false;
	build_transformations_original ( ecx, edx, a2, a3, a4, a5, a6, a7 );
	player->JiggleBones ( ) = o_jiggle_bones_enabled;
}
bool __fastcall hooks::is_hltv::hook ( void * ecx, void *  ) {

//f ( !config.is_hltv_target ) {
	 	return is_hltv_original ( ecx );
	//

	
	static const auto accumulate_layers_call = reinterpret_cast< void * >(utilities::pattern_scan ( "client.dll", "84 C0 75 0D F6 87" ));
	static const auto setupvelocity_call = reinterpret_cast< void * >(utilities::pattern_scan ( "client.dll", "84 C0 75 38 8B 0D ? ? ? ? 8B 01 8B 80 ? ? ? ? FF D0" ));

	if ( _ReturnAddress ( ) == accumulate_layers_call || _ReturnAddress ( ) == setupvelocity_call )
		return true;



	return is_hltv_original ( ecx );
}
/*void WriteUserCmd ( void * buf, c_usercmd * incmd, c_usercmd * outcmd ) {
	using WriteUserCmd_t = void ( __fastcall * )( void *, c_usercmd *, c_usercmd * );
	static auto Fn = ( WriteUserCmd_t ) utilities::pattern_scan (  ( "client.dll" ),  ( "55 8B EC 83 E4 F8 51 53 56 8B D9" ) );

	__asm
	{
		mov     ecx, buf
		mov     edx, incmd
		push    outcmd
		call    Fn
		add     esp, 4
	}
}
bool __fastcall hooks::write_user_cmd::hook ( void * ecx, void * , int slot, bf_write * buf, int from, int to, bool is_new_command ) {
	return write_user_cmd_original ( ecx, slot, buf, from, to, is_new_command );
	if ( !local_player::m_data.tickbase_shift )
		return write_user_cmd_original ( ecx, slot, buf, from, to, is_new_command );

	if ( from != -1 )
		return true;

	auto final_from = -1;

	uintptr_t frame_ptr;
	__asm mov frame_ptr, ebp;

	auto backup_commands = reinterpret_cast < int * > ( frame_ptr + 0xFD8 );
	auto new_commands = reinterpret_cast < int * > ( frame_ptr + 0xFDC );

	auto newcmds = *new_commands;
	auto shift = tickbase_system::m_shift_data.shift_ticks;

	tickbase_system::m_shift_data.shift_ticks = 0;
	*backup_commands = 0;

	auto choked_modifier = newcmds + shift;

	if ( choked_modifier > 62 )
		choked_modifier = 62;

	*new_commands = choked_modifier;

	auto next_cmdnr = interfaces::clientstate->m_choked_commands + interfaces::clientstate->m_last_outgoing_command + 1;
	auto final_to = next_cmdnr - newcmds + 1;

	if ( final_to <= next_cmdnr ) {
		while ( write_user_cmd_original ( ecx, slot, buf, final_from, final_to, true ) ) {
			final_from = final_to++;

			if ( final_to > next_cmdnr )
				goto next_cmd;
		}

		return false;
	}
next_cmd:

	auto user_cmd = interfaces::input->get_user_cmd ( final_from );

	if ( !user_cmd )
		return true;

	c_usercmd to_cmd;
	c_usercmd from_cmd;

	from_cmd = *user_cmd;
	to_cmd = from_cmd;

	to_cmd.command_number++;
	to_cmd.tick_count += 200;

	if ( newcmds > choked_modifier )
		return true;

	for ( auto i = choked_modifier - newcmds + 1; i > 0; --i ) {
		WriteUserCmd ( buf, &to_cmd, &from_cmd );

		from_cmd = to_cmd;
		to_cmd.command_number++;
		to_cmd.tick_count++;
	}

	return true;
}
*/


void __fastcall  hooks::cl_move::hook ( void * ecx ) {

	cl_move_original ( ecx );
}
/*void _fastcall hooks::run_commmand::hook ( void * , void * , player_t * player, c_usercmd * cmd, player_move_helper * move_helper ) {
	
	local_player::m_data.pointer = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( interfaces::engine->get_local_player ( ) ) );

	if ( !player || player != local_player::m_data.pointer )
		return run_commmand_original ( interfaces::prediction, player, cmd, move_helper );

	if ( !cmd )
		return run_commmand_original ( interfaces::prediction, player, cmd, move_helper );

	if ( cmd->tick_count > interfaces::globals->tick_count + 72 ) //-V807
	{
		cmd->hasbeenpredicted = true;
		player->set_abs_origin ( player->origin ( ) );

		if ( interfaces::globals->frame_time > 0.0f && !interfaces::prediction->EnginePaused )
			++player->get_tick_base ( );

		return;
	}


	if ( config.ragebot_enabled && player->is_alive ( ) ) {
		auto weapon = player->active_weapon ( );

		if ( weapon ) {
			static int tickbase_records [ MULTIPLAYER_BACKUP ] = {};
			static bool in_attack [ MULTIPLAYER_BACKUP ] = {};
			static bool can_shoot [ MULTIPLAYER_BACKUP ] = {};

			tickbase_records [ cmd->command_number % MULTIPLAYER_BACKUP ] = player->get_tick_base ( );
			in_attack [ cmd->command_number % MULTIPLAYER_BACKUP ] = cmd->buttons & cmd_buttons::in_attack || cmd->buttons & cmd_buttons::in_attack2;
			can_shoot [ cmd->command_number % MULTIPLAYER_BACKUP ] = misc::can_fire (weapon, false );

			if ( weapon->item_definition_index ( ) == weapon_revolver ) {
				auto postpone_fire_ready_time = FLT_MAX;
				auto tickrate = ( int ) ( 1.0f / interfaces::globals->interval_per_tick );

				if ( tickrate >> 1 > 1 ) {
					auto command_number = cmd->command_number - 1;
					auto shoot_number = 0;

					for ( auto i = 1; i < tickrate >> 1; ++i ) {
						shoot_number = command_number;

						if ( !in_attack [ command_number % MULTIPLAYER_BACKUP ] || !can_shoot [ command_number % MULTIPLAYER_BACKUP ] )
							break;

						--command_number;
					}

					if ( shoot_number ) {
						auto tick = 1 - ( int ) ( -0.03348f / interfaces::globals->interval_per_tick );

						if ( cmd->command_number - shoot_number >= tick )
							postpone_fire_ready_time = math::ticks_to_time ( tickbase_records [ ( tick + shoot_number ) % MULTIPLAYER_BACKUP ] ) + 0.2f;
					}
				}

				weapon->get_postpone_fire_ready_time ( ) = postpone_fire_ready_time;
			}
		}

		auto backup_velocity_modifier = player->m_flVelocityModifier ( );

		player->m_flVelocityModifier ( ) = local_player::m_data.last_velocity_modifier;
		run_commmand_original ( interfaces::prediction, player, cmd, move_helper );

		if ( !csgo::in_create_move )
			player->m_flVelocityModifier ( ) = backup_velocity_modifier;
	}
	else
		return run_commmand_original ( interfaces::prediction, player, cmd, move_helper );

	//g_netdata.store ( );
}*/
void __fastcall hooks::packet_start::hook ( void * ecx, void * , int incoming, int outgoing ) {

	local_player::m_data.pointer = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( interfaces::engine->get_local_player ( ) ) );

	if ( !local_player::available ( ) )
		return packet_start_original ( ecx, incoming, outgoing );

	if ( !local_player::m_data.pointer->is_alive ( ) )
		return packet_start_original ( ecx, incoming, outgoing );

	if ( local_player::m_data.commands.empty ( ) )
		return packet_start_original ( ecx, incoming, outgoing );

	//if ( m_bIsValveDS )
		//return packet_start_original ( ecx, incoming, outgoing );

	for ( auto it = local_player::m_data.commands.rbegin ( ); it != local_player::m_data.commands.rend ( ); ++it ) {
		if ( !it->is_outgoing )
			continue;

		if ( it->command_number == outgoing || outgoing > it->command_number && ( !it->is_used || it->previous_command_number == outgoing ) ) {
			it->previous_command_number = outgoing;
			it->is_used = true;
			packet_start_original ( ecx, incoming, outgoing );
			break;
		}
	}

	auto result = false;

	for ( auto it = local_player::m_data.commands.begin ( ); it != local_player::m_data.commands.end ( );) {
		if ( outgoing == it->command_number || outgoing == it->previous_command_number )
			result = true;

		if ( outgoing > it->command_number && outgoing > it->previous_command_number )
			it = local_player::m_data.commands.erase ( it );
		else
			++it;
	}

	if ( !result )
		packet_start_original ( ecx, incoming, outgoing );
}

using PacketEnd_t = void ( __thiscall * )( void * );

void __fastcall hooks::packet_end::hook ( void * ecx, void *  ) {
	local_player::m_data.pointer = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( interfaces::engine->get_local_player ( ) ) );

	if ( !local_player::m_data.pointer->is_alive ( ) )  //-V807
	{
		local_player::m_data.data.clear ( );
		return packet_end_original ( ecx );
	}

	if ( *( int * ) ( ( uintptr_t ) ecx + 0x164 ) == *( int * ) ( ( uintptr_t ) ecx + 0x16C ) ) {
		auto ack_cmd = *( int * ) ( ( uintptr_t ) ecx + 0x4D2C );
		auto correct = std::find_if ( local_player::m_data.data.begin ( ), local_player::m_data.data.end ( ),
			[ &ack_cmd ] ( const local_player::correction_data & other_data ) {
			return other_data.command_number == ack_cmd;
		}
		);

		auto netchannel = interfaces::engine->get_net_channel_info ( );

		if ( netchannel && correct != local_player::m_data.data.end ( ) ) {
			if ( local_player::m_data.last_velocity_modifier > local_player::m_data.pointer->m_flVelocityModifier ( ) + 0.1f ) {
				auto weapon = local_player::m_data.pointer->active_weapon ( );

				if ( !weapon || weapon->item_definition_index ( ) != weapon_revolver && !weapon->is_grenade ( ) ) //-V648
				{
					for ( auto & number : local_player::m_data.choked_number ) {
						auto cmd = &interfaces::input->m_commands [ number % MULTIPLAYER_BACKUP ];
						auto verified = &interfaces::input->m_verified [ number % MULTIPLAYER_BACKUP ];

						if ( cmd->buttons & ( cmd_buttons::in_attack | cmd_buttons::in_attack2 ) ) {
							cmd->buttons &= ~cmd_buttons::in_attack;

							verified->m_cmd = *cmd;
							verified->m_crc = cmd->get_checksum ( );
						}
					}
				}
			}

			local_player::m_data.last_velocity_modifier = local_player::m_data.pointer->m_flVelocityModifier ( );
		}
	}

	return  packet_end_original ( ecx );
}

void __fastcall hooks::fire_event::hook ( void * ecx, void *  ) {
	if ( !config.ragebot_enabled )
		return fire_event_original ( ecx );
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

void __fastcall hooks::crc_server_check::hook ( void * , void *  ) {
	return;
}

/*void __stdcall hooks::render_smoke_overlay::hook ( void * ctx, bool a ) {
	render_smoke_overlay_original ( ctx, a );
}*/
/*void __fastcall hooks::trace_ray::hook ( void * thisptr, void *, const ray_t & ray, unsigned int fMask, trace_filter * pTraceFilter, trace_t * pTrace ) {

	if ( !csgo::in_trace )
		return trace_ray_original ( thisptr, ray, fMask, pTraceFilter, pTrace );

	trace_ray_original ( thisptr, ray, fMask, pTraceFilter, pTrace );

	pTrace->surface.flags |= SURF_SKY;
}*/
void __stdcall hooks::scene_end::hook ( ) {
	visuals::player::paint_traverse ( );

	visuals::local_esp_think ( );
	nade_pred.paint_traverse ( );
	visuals::projectiles::think ( );

	scene_end_original ( interfaces::render_view );
}

int __fastcall hooks::file_system::hook ( void * , void *  ) {
	static auto return_to_client_panorama = utilities::pattern_scan ( "client.dll", "83 F8 02 0F 85 ? ? ? ? 8B 3D ? ? ? ? FF D7" );
	static auto return_to_engine = utilities::pattern_scan ( "engine.dll", "83 F8 02 75 6C 68 ? ? ? ? FF 15 ? ? ? ? 8B 4C 24 28 83 C4 04" );

	if ( _ReturnAddress ( ) == return_to_client_panorama || _ReturnAddress ( ) == return_to_engine )
		return 0; //return 0 in some cases to not being kicked from game

	return 1; //return 1 to allow loading of chams materials
}

void __fastcall hooks::check_for_sequence_change::hook ( uintptr_t * ecx, uintptr_t * edx, uintptr_t * hdr, int cur_sequence, bool force_new_sequence, bool  ) {

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
