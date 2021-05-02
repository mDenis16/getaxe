#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "../features/features.hpp"
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

#include <d3dx9.h>
#include "../../dependencies/imgui/impl/imgui_impl_dx9.h"
#include "../../dependencies/imgui/impl/imgui_impl_win32.h"




namespace hooks {

	bool unloading = false;

	void CREATE_HOOK ( void * _target,  void* _detour ) {
		list.push_back(new hook (  _target, _detour ));
	}
	

	inline void* get_virtual ( void * _class, unsigned int index ) { return reinterpret_cast< void * >( ( *reinterpret_cast< int ** >( _class ) ) [ index ] ); }

	bool create_hooks ( ) {
		if ( MH_Initialize ( ) != MH_OK ) {
			throw std::runtime_error ( "failed to initialize MH_Initialize." );
		}
		int index = 0;
		for ( auto & hook : hooks::list ) {
			if ( !hook->target )
				continue;

			if ( MH_CreateHook ( hook->target, hook->detour, reinterpret_cast< void ** >(&hook->original)) != MH_OK ) {
 				throw std::runtime_error ( std::to_string( index).c_str());
			}

			if ( MH_EnableHook ( MH_ALL_HOOKS ) != MH_OK ) {
				throw std::runtime_error ( "failed to enable hooks." );
			}

			printf ( "hooked address %i \n", std::addressof( hook->detour ) );
			index++;

		}


		return true;
	}
	/*void __stdcall frame_stage::hook ( client_frame_stage_t stage ) {
		printf ( "hello from framestage" );

		reinterpret_cast< void ( __stdcall * )( client_frame_stage_t ) >( list.at ( 0 )->original )(stage);
	}*/
	long __stdcall  reset_hook ( IDirect3DDevice9 * thisptr, void * params ) {

		long hr = reset_original ( thisptr, params );

		return hr;
	}
	void initialize ( ) {

		static const auto c_cs_player_table = reinterpret_cast< uint32_t >( utilities::pattern_scan ( "client.dll",
			"55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 89 7C 24 0C" ) ) + 0x47;


		void * d3d_device = **( void *** ) ( ( uintptr_t ) utilities::pattern_scan ( "shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C" ) + 0x1 );

		void * pt = reinterpret_cast< void * >( c_cs_player_table );

		printf ( "c_cs_player_table ptr = %p \n", pt );
		static const auto _sbf = utilities::pattern_scan ( "client.dll", ( "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 89 7C 24 0C" ) ) + 0x4E;
		void * sbf = reinterpret_cast< void * > ( _sbf );

		CREATE_HOOK ( get_virtual ( interfaces::client, 37 ), &callback::frame_stage_notify );
		CREATE_HOOK ( get_virtual ( interfaces::model_render, 21 ), &callback::draw_model_exec );
		CREATE_HOOK ( utilities::pattern_scan ( "engine.dll", "55 8B EC 81 EC ? ? ? ? 53 8B D9 89 5D F8 80" ), &callback::crc_server_check );
		CREATE_HOOK ( get_virtual ( interfaces::console->get_convar ( "sv_cheats" ), 13 ), &callback::sv_cheats );
		CREATE_HOOK ( get_virtual ( interfaces::engine->get_bsp_tree_query ( ), 6 ), &callback::list_leaves_in_box );
		CREATE_HOOK ( get_virtual ( interfaces::clientmode, 24 ), &callback::create_move );
		CREATE_HOOK ( /*utilities::pattern_scan ( "engine.dll", "55 8B EC 83 E4 C0 81 EC ? ? ? ? 53 56" )*/ nullptr, &callback::process_packet );
		CREATE_HOOK ( get_virtual ( interfaces::panel, 41 ), &callback::paint_traverse );
		CREATE_HOOK ( nullptr/* get_virtual ( interfaces::engine, 93 )*/, &callback::is_hltv );
		CREATE_HOOK ( get_virtual ( interfaces::clientmode, 44 ), &callback::post_screen_space_fx );
		CREATE_HOOK ( get_virtual ( sbf, 13 ), &callback::setup_bones );
		CREATE_HOOK ( nullptr, &callback::accumulate_layers );
		CREATE_HOOK ( nullptr /* utilities::pattern_scan ( "client.dll", "55 8B EC 51 53 8B 5D 08 56 8B F1 57 85" )*/, &callback::check_for_sequence_change );
		CREATE_HOOK ( utilities::pattern_scan ( "client.dll", "55 8B EC 83 E4 F0 81 EC ? ? ? ? 56 57 8B F9 8B 0D ? ? ? ? 89 7C 24 1C" ), &callback::build_transformations );
		CREATE_HOOK ( utilities::pattern_scan ( "client.dll", "57 8B F9 8B 07 8B 80 ? ? ? ? FF D0 84 C0 75 02 5F C3" ), &callback::should_skip_animation_frame );
		CREATE_HOOK ( nullptr, &callback::calculate_view );//aici e cretane
		CREATE_HOOK ( nullptr /* utilities::pattern_scan ( "client.dll", "55 8B EC 83 E4 F0 83 EC 78 56 8B F1 57 8B 56 60" )*/, &callback::do_procedural_foot_plant );
		CREATE_HOOK ( get_virtual ( pt, 205 ), &callback::standard_blending_rules );
		CREATE_HOOK ( nullptr /* get_virtual ( interfaces::client, 24 )*/, &callback::write_user_cmd );
		CREATE_HOOK ( get_virtual ( pt, 223 ), &callback::update_client_side_animations );
		CREATE_HOOK ( nullptr /*  get_virtual ( interfaces::engine, 59 )*/, &callback::fire_event );
		CREATE_HOOK ( get_virtual ( interfaces::prediction, 19 ), &callback::run_command );
		CREATE_HOOK ( nullptr /*get_virtual ( ( i_client_state * ) ( uint32_t ( interfaces::clientstate ) + 0x8 ), 5 )*/, &callback::packet_start );
		CREATE_HOOK ( nullptr /*get_virtual ( ( i_client_state * ) ( uint32_t ( interfaces::clientstate ) + 0x8 ), 6 )*/, &callback::packet_end );
		CREATE_HOOK ( get_virtual ( interfaces::clientmode, 18 ), &callback::override_view );
		CREATE_HOOK ( nullptr /* utilities::pattern_scan ( "engine.dll", "55 8B EC 81 EC ? ? ? ? 53 56 57 8B 3D ? ? ? ? 8A" )*/, &callback::cl_move );
		CREATE_HOOK ( nullptr /*  get_virtual ( interfaces::trace_ray, 4 )*/, &callback::clip_ray_collideable );
		CREATE_HOOK ( get_virtual ( interfaces::render_view, 9 ), &callback::scene_end );
		CREATE_HOOK ( nullptr, &callback::trace_ray );
		CREATE_HOOK ( get_virtual ( interfaces::file_system, 128 ), &callback::file_system );
		CREATE_HOOK ( get_virtual ( d3d_device, 42 ), &callback::present_hook );
		CREATE_HOOK ( get_virtual ( d3d_device, 16 ), &reset_hook );



		wndproc_original = ( WNDPROC ) SetWindowLongPtrA ( csgo::window, GWL_WNDPROC, ( LONG ) callback::wnd_proc );
	}

	void release ( ) {

		//ImGui_ImplDX9_Shutdown ( );
		//ImGui_ImplWin32_Shutdown ( );
		//ImGui::DestroyContext ( );

		for ( auto & hook : hooks::list ) {
			if ( !hook->target )
				continue;

			MH_DisableHook ( hook->target );
			MH_RemoveHook ( hook->target );
		}


		MH_Uninitialize ( );
		SetWindowLongA ( csgo::window, GWLP_WNDPROC, long ( wndproc_original ) );
	}
}


