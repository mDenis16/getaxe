#pragma once
#include <d3dx9.h>
#include <any>


//framestage missing
#define draw_model_execute_original reinterpret_cast<void ( __fastcall * )( void *, void *, void *, const draw_model_state_t &, const model_render_info_t &, matrix3x4_t * ) >( hooks::list.at ( hooks::hook_index::draw_model_execute )->original )
#define crc_server_check_original reinterpret_cast<void ( __thiscall * )( void *, void * ) >( hooks::list.at ( hooks::hook_index::crc_server_check )->original )
#define sv_cheats_original reinterpret_cast< bool ( __thiscall * )( void * ) >( hooks::list.at ( hooks::hook_index::sv_cheats )->original )
#define list_leaves_in_box_original reinterpret_cast< int ( __fastcall * ) ( void *, void *, vec3_t &, vec3_t &, unsigned short *, int ) >( hooks::list.at ( hooks::hook_index::list_leaves_in_box )->original )
#define create_move_original reinterpret_cast< bool ( __stdcall * )( float, c_usercmd * ) >( hooks::list.at ( hooks::hook_index::create_move )->original )
#define process_packet_original reinterpret_cast< void ( __cdecl * )( void *, bool ) >( hooks::list.at ( hooks::hook_index::process_packet )->original )
#define paint_traverse_original reinterpret_cast< void(__thiscall*)(i_panel*, unsigned int, bool, bool) >( hooks::list.at ( hooks::hook_index::paint_traverse )->original )
#define is_hltv_original reinterpret_cast< bool ( __thiscall * )( void * ) >( hooks::list.at ( hooks::hook_index::is_hltv )->original )
#define post_screen_space_fx_original reinterpret_cast< bool( __thiscall* )( uintptr_t, const view_setup_t* ) >( hooks::list.at ( hooks::hook_index::post_screen_space_fx )->original )
#define setup_bones_original reinterpret_cast< bool( __thiscall*  )( void*, matrix3x4_t *, int, int, float ) >( hooks::list.at ( hooks::hook_index::setup_bones )->original )
#define accumulate_layers_original reinterpret_cast< void( __thiscall* )( uintptr_t* , uintptr_t , void* , vec3_t* , float , uintptr_t*  ) >( hooks::list.at ( hooks::hook_index::accumulate_layers )->original )
#define check_for_sequence_change_original reinterpret_cast< void ( __cdecl * )( uintptr_t * , uintptr_t * , uintptr_t * , int , bool , bool  ) >( hooks::list.at ( hooks::hook_index::check_for_sequence_change )->original )
#define build_transformations_original reinterpret_cast< void ( __fastcall * ) ( void *, void *, int, int, int, int, int, int ) >( hooks::list.at ( hooks::hook_index::build_transformations )->original )
#define modify_eye_position_original reinterpret_cast< void( __thiscall* )( void * , void * , vec3_t &  ) >( hooks::list.at ( hooks::hook_index::modify_eye_position )->original )
#define calculate_view_original reinterpret_cast< void ( __thiscall * )( void *, void * , vec3_t & , vec3_t & , float & , float & , float &  ) >( hooks::list.at ( hooks::hook_index::calculate_view )->original )
//#define do_procedural_foot_plant_original reinterpret_cast< void* >( hooks::list.at ( hooks::hook_index::do_procedural_foot_plant  )->original )
#define standard_blending_rules_original reinterpret_cast< void( __thiscall* )( player_t*, c_studio_hdr*, vec3_t*, quaternion_t*, float, int ) >( hooks::list.at ( hooks::hook_index::standard_blending_rules )->original )
#define write_user_cmd_original reinterpret_cast< bool ( __cdecl * )( void *, int, void *, int, int, bool ) >( hooks::list.at ( hooks::hook_index::write_user_cmd )->original )

#define update_client_side_animation_original reinterpret_cast< void( __thiscall* )( player_t* ) >( hooks::list.at ( hooks::hook_index::update_client_side_animation )->original )
#define fire_event_original reinterpret_cast< void( __thiscall* )( void* ) >( hooks::list.at ( hooks::hook_index::fire_event )->original )
#define run_command_original reinterpret_cast< void ( __fastcall * )( void *, void *, player_t *, c_usercmd *, void * ) >( hooks::list.at ( hooks::hook_index::run_command )->original )
#define packet_start_original reinterpret_cast<void ( __thiscall * )( void *, int, int ) >( hooks::list.at ( hooks::hook_index::packet_start )->original )
#define packet_end_original reinterpret_cast< void ( __thiscall * )( void * ) >( hooks::list.at ( hooks::hook_index::packet_end )->original )
#define override_view_original reinterpret_cast<void* ( __fastcall* )( i_client_mode*, void* , view_setup_t* setup ) >( hooks::list.at ( hooks::hook_index::override_view )->original )
#define cl_move_original reinterpret_cast<  void ( __cdecl * ) ( float, bool ) >( hooks::list.at ( hooks::hook_index::cl_move )->original )
#define clip_ray_collideable_original reinterpret_cast< void * ( __thiscall * )( void *, const ray_t &, uint32_t, collideable_t *, void * ) >( hooks::list.at ( hooks::hook_index::clip_ray_collideable )->original )
#define scene_end_original reinterpret_cast< void( __thiscall* )( void* ) >( hooks::list.at ( hooks::hook_index::scene_end )->original )
#define trace_ray_original reinterpret_cast< void( __thiscall* )( void*, const  ray_t&, unsigned int, trace_filter*, trace_t* ) >( hooks::list.at ( hooks::hook_index::trace_ray )->original )
#define present_original reinterpret_cast< long ( __stdcall * )( IDirect3DDevice9 *  ) >( hooks::list.at ( hooks::hook_index::present )->original )
#define reset_original reinterpret_cast< long( __stdcall* )( IDirect3DDevice9*, void* ) >( hooks::list.at ( hooks::hook_index::reset )->original )
#define do_procedural_foot_plant_original  reinterpret_cast< void( __thiscall* )( void *,  int , int , int , int  ) >( hooks::list.at ( hooks::hook_index::do_procedural_foot_plant )->original )
#define calc_view__original  reinterpret_cast< void( __thiscall* )( void * , void * , vec3_t & , vec3_t & , float & , float & , float &  ) >( hooks::list.at ( hooks::hook_index::calc_view_original )->original )

namespace hooks {


	extern bool unloading;

	void initialize ( );
	bool create_hooks ( );
	void release ( );


	enum hook_index {
		frame_stage_notify,
		draw_model_execute,
		crc_server_check,
		sv_cheats,
		list_leaves_in_box,
		create_move,
		process_packet,
		paint_traverse,
		is_hltv,
		post_screen_space_fx,
		setup_bones,
		accumulate_layers,
		check_for_sequence_change,
		build_transformations,
		should_skip_animation_frame, /*no need to call original*/
		calculate_view,
		do_procedural_foot_plant,
		standard_blending_rules,
		write_user_cmd,
		update_client_side_animation,
		fire_event,
		run_command,
		packet_start,
		packet_end,
		override_view,
		cl_move,
		clip_ray_collideable,
		scene_end,
		trace_ray,
		file_system,
		present,
		reset
		/*no need to call original*/
	};

	inline WNDPROC wndproc_original;
	inline HWND window;
	
	namespace callback {
		extern void __stdcall frame_stage_notify ( client_frame_stage_t stage );
		extern void __fastcall crc_server_check ( void *, void * );
		extern bool __fastcall sv_cheats ( void * ecx, void * );
		extern int __fastcall list_leaves_in_box ( void * bsp, void * edx, vec3_t & mins, vec3_t & maxs, unsigned short * pList, int listMax );
		extern bool __fastcall create_move ( void * ecx, void * edx, int input_sample_frametime, c_usercmd * cmd );
		extern void __cdecl process_packet ( void * packet, bool header );
		extern void __stdcall paint_traverse ( unsigned int panel, bool force_repaint, bool allow_force );
		extern bool __fastcall is_hltv ( void * ecx, void * );
		extern bool __fastcall post_screen_space_fx ( uintptr_t ecx, uintptr_t, const view_setup_t * setup );
		extern bool __fastcall setup_bones ( void * ecx, void *, matrix3x4_t * bone_to_world_out, int max_bones, int bone_mask, float curtime );
		extern void __fastcall accumulate_layers ( uintptr_t * player, uintptr_t edx, void * setup, vec3_t * pos, float time, uintptr_t * q );
		extern void __fastcall check_for_sequence_change ( uintptr_t * ecx, uintptr_t * edx, uintptr_t * hdr, int cur_sequence, bool force_new_sequence, bool );
		extern void __fastcall build_transformations ( void * ecx, void * edx, int a2, int a3, int a4, int a5, int a6, int a7 );
		extern bool __fastcall should_skip_animation_frame ( void *, void * );
   		//extern void __fastcall modify_eye_position ( void * ecx, void * edx, vec3_t & input_eye_position );
		extern void __fastcall calculate_view ( void * ecx, void * edx, vec3_t & eye_origin, vec3_t & eye_angles, float & z_near, float & z_far, float & fov );
		extern bool __fastcall in_prediction ( void * ecx, void * edx );
		extern void __fastcall do_procedural_foot_plant ( void * a1, void *, int a2, int a3, int a4, int a5 );
		extern void _fastcall standard_blending_rules ( player_t * player, uint32_t, c_studio_hdr * hdr, vec3_t * pos, quaternion_t * q, const float time, int mask );
		extern bool __fastcall write_user_cmd ( void * ecx, void *, int m_nSlot, bf_write * m_pBuffer, int m_nFrom, int m_nTo, bool m_bNewCmd );
		extern void _fastcall update_client_side_animations ( player_t * player, uint32_t );
		extern void _fastcall fire_event ( void * ecx, void * );
		extern void __fastcall draw_model_exec ( void * ecx, void * edx, void * ctx, const draw_model_state_t & state, const model_render_info_t & info, matrix3x4_t * custom_bone_to_world = nullptr );
		extern long __stdcall  present_hook ( IDirect3DDevice9 * device );
		extern long __stdcall  reset_hook ( IDirect3DDevice9 * thisptr, void * params );
		extern LRESULT __stdcall  wnd_proc ( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam );
		extern void __fastcall packet_start ( void * ecx, void *, int incoming, int outgoing );
		extern  void __fastcall packet_end ( void * ecx, void * );
		extern void __fastcall override_view ( void * ecx, void *, view_setup_t * setup );
		extern void __cdecl cl_move ( float accumulated_extra_samples, bool final_tick );
		extern void __fastcall clip_ray_collideable ( void * ecx, void * edx, const ray_t & ray, uint32_t fMask, collideable_t * pCollide, void * pTrace );
		extern void __stdcall scene_end ( );
		extern void __fastcall trace_ray ( void * thisptr, void *, const ray_t & ray, unsigned int fMask, trace_filter * pTraceFilter, trace_t * pTrace );
		extern int __fastcall file_system ( void *, void * );
		extern void __fastcall run_command ( void * ecx, void * edx, player_t * player, c_usercmd * cmd, player_move_helper * move_helper );
	}



	class hook {
	public:

		void* target;
		void* detour;
		void* original;

		hook ( void* _target, void* _detour ) {
			this->target = _target;
			this->detour = _detour;
		}

	};
	inline std::vector<hook *>  list;

	
}
