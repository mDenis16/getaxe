#pragma once

namespace hooks {
	bool initialize();
	void release();

	

	

	inline unsigned int get_virtual(void* _class, unsigned int index) { return static_cast<unsigned int>((*reinterpret_cast<int**>(_class))[index]); }
	typedef float quaternion_t [ 4 ];
	namespace frame_stage {
		using fn = void( __thiscall* )( i_base_client_dll*, int frame_stage );
		static void __stdcall hook( int frame_stage );
	}
	namespace crc_server_check {
		using fn = void( __thiscall* )( void*, void* );
		void __fastcall hook( void*, void* );
	}
	namespace create_move {
		using fn = bool(__stdcall*)(float, c_usercmd*);
		static bool __fastcall hook(void* ecx, void* edx, int input_sample_frametime, c_usercmd* cmd);
	};

	namespace paint_traverse {
		using fn = void(__thiscall*)(i_panel*, unsigned int, bool, bool);
		static void __stdcall hook(unsigned int panel, bool force_repaint, bool allow_force);
	}

	namespace post_screen_space_fx {
		using fn = bool( __thiscall* )( uintptr_t, const view_setup_t* );
		static bool __fastcall hook( uintptr_t ecx, uintptr_t edx, const view_setup_t* setup );
	}
	namespace setup_bones {
		using fn = bool( __thiscall*  )( void*, matrix_t*, int, int, float );
		static bool __fastcall hook( void* ecx, void* edx, matrix_t* bone_to_world_out, int max_bones, int bone_mask, float curtime );
		
	}
	namespace accumulate_layers {
		using fn = void( __thiscall* )( uintptr_t* , uintptr_t , void* , vec3_t* , float , quaternion_t*  );
		static void __fastcall hook( uintptr_t* player, uintptr_t edx, void* setup, vec3_t* pos, float time, quaternion_t* q );

	}
	namespace do_extra_bone_processing {
		using fn = void( __thiscall* )( void*, void*, vec3_t*, void*,  void*, uint8_t*, void* );
		
		static void _fastcall hook( void* ecx, uint32_t, void* hdr, vec3_t* pos, void* q, void* matrix, uint8_t* bone_computed, void* context );
	}
	namespace do_procedural_foot_plant {
	
		static	void __fastcall hook( void* a1, void* _edx, int a2, int a3, int a4, int a5 );
	}
	namespace standard_blending_rules {
		using fn = void( __thiscall* )( player_t*, c_studio_hdr*, vec3_t*, quaternion_t*, float, int );
		static void _fastcall hook( player_t* player, uint32_t, c_studio_hdr* hdr, vec3_t* pos, quaternion_t* q, const float time, int mask );
	}
	namespace write_user_cmd {
		using fn = bool( __thiscall* )( void*, int, void*, int, int, bool );
		static bool __fastcall hook( void* ecx, void* edx, int m_nSlot, bf_write* m_pBuffer, int m_nFrom, int m_nTo, bool m_bNewCmd );
	}
	namespace update_client_side_animation {
		using fn = void( __thiscall* )( player_t* );
		static void _fastcall hook( player_t* player, uint32_t );
	}
	namespace fire_event {
		using fn = void( __thiscall* )( void* );
		static void _fastcall hook( void* ecx, void* edx );
	}
	
	namespace draw_model_execute {
		using fn = void( __thiscall* )( iv_model_render*, i_mat_render_context*, const draw_model_state_t&, const model_render_info_t&, matrix_t* );
		static void __stdcall hook( i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* bone_to_world );
	}

	namespace run_commmand {
		using fn = void( __thiscall* )( void*, player_t* , c_usercmd*, player_move_helper*  );
		static void _fastcall hook( void* prediction, void* edx, player_t* player, c_usercmd* cmd, player_move_helper* move_helper );
	}
	namespace render_smoke_overlay {
		using fn = void( __thiscall* )( void*, bool );
		static void __stdcall hook( void* ctx, bool a );
	}
	namespace eye_angles {
		using fn = vec3_t * ( __thiscall* )( void* );
		static vec3_t* __fastcall hook( void* ecx, void* edx );
	}
	namespace override_view {
		using fn = void* ( __fastcall* )( i_client_mode*, void* _this, view_setup_t* setup );
		static void __fastcall hook( void* _this, void* _edx, view_setup_t* setup );
	}
	namespace cl_move {
		using fn = void* ( __thiscall* )( void*  );
		static void __fastcall  hook( void* ecx );
	}
	namespace scene_end {
		using fn = void( __thiscall* )( void* );
		static void __stdcall hook( );
	}
	namespace trace_ray {
		using fn = void( __thiscall* )( void*, const  ray_t&, unsigned int, trace_filter*, trace_t* );
		
		static void __fastcall hook( void* thisptr, void*, const ray_t& ray, unsigned int fMask, trace_filter* pTraceFilter, trace_t* pTrace );
	}
	namespace file_system {
		int __fastcall hook( void* ecx, void* edx );
	}
}
