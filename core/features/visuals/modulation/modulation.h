
namespace modulation {
	struct custom_material {
		i_material * mat_address;
		i_material_var * envmaptint;
		bool  found_env_tint;
	};
	enum class shader_type_t {
		VertexLitGeneric = 0,
		UnlitGeneric,
		Modulate
	};

	enum mat_type {
		regular,
		flat,
		reflective,
		glow
	};

	 void run ( void * ecx, void * edx, void * ctx, const draw_model_state_t & state, const model_render_info_t & info, matrix3x4_t * custom_bone_to_world, bool & skip_return );
	 void run_player ( void * ecx, void * edx, void * ctx, const draw_model_state_t & state, const model_render_info_t & info, matrix3x4_t * custom_bone_to_world, bool & skip_return, player_t * player );
	 void run_player_chams ( void * ecx, void * edx, void * ctx, const draw_model_state_t & state, const model_render_info_t & info, matrix3x4_t * custom_bone_to_world, bool & skip_return, player_t * player, chams_visual & chams_data );
	 void run_player_attachments ( void * ecx, void * edx, void * ctx, const draw_model_state_t & state, const model_render_info_t & info, matrix3x4_t * custom_bone_to_world, bool & skip_return, player_t * player );
	extern 	std::array<custom_material, 4> materials;

	extern void release_mem ( );
	extern void create_materials ( );
}
