#pragma once
#include "imageformats.h"

enum material_var_flags_t {
	material_var_debug = (1 << 0),
	material_var_no_debug_override = (1 << 1),
	material_var_no_draw = (1 << 2),
	material_var_use_in_fillrate_mode = (1 << 3),
	material_var_vertexcolor = (1 << 4),
	material_var_vertexalpha = (1 << 5),
	material_var_selfillum = (1 << 6),
	material_var_additive = (1 << 7),
	material_var_alphatest = (1 << 8),
	//material_var_unused = (1 << 9),
	material_var_znearer = (1 << 10),
	material_var_model = (1 << 11),
	material_var_flat = (1 << 12),
	material_var_nocull = (1 << 13),
	material_var_nofog = (1 << 14),
	material_var_ignorez = (1 << 15),
	material_var_decal = (1 << 16),
	material_var_envmapsphere = (1 << 17), // obsolete
	material_var_unused = (1 << 18), // unused
	material_var_envmapcameraspace = (1 << 19), // obsolete
	material_var_basealphaenvmapmask = (1 << 20),
	material_var_translucent = (1 << 21),
	material_var_normalmapalphaenvmapmask = (1 << 22),
	material_var_needs_software_skinning = (1 << 23), // obsolete
	material_var_opaquetexture = (1 << 24),
	material_var_envmapmode = (1 << 25), // obsolete
	material_var_suppress_decals = (1 << 26),
	material_var_halflambert = (1 << 27),
	material_var_wireframe = (1 << 28),
	material_var_allowalphatocoverage = (1 << 29),
	material_var_alpha_modified_by_proxy = (1 << 30),
	material_var_vertexfog = (1 << 31),
};

enum preview_image_retval_t {
	material_preview_image_bad = 0,
	material_preview_image_ok,
	material_no_preview_image,
};

typedef int ImageFormat;
class IMaterialVar;
typedef int VertexFormat_t;
typedef int MaterialPropertyTypes_t;

struct model_t;
class i_material;



struct studiohwdata_t;
struct color_mesh_info_t;
struct draw_model_info_t;

class data_cache_handle_t;
class i_mat_render_context;
struct material_lighting_state_t;
typedef int vertex_format_t;
typedef void* light_cache_handle_t;
typedef void* studio_decal_handle_t;
typedef int material_property_types_t;
typedef unsigned short model_instance_handle_t;
using material_handle_t = unsigned short;

class i_client_renderable {
public:
	virtual void * get_client_unknown ( ) = 0;
	virtual vec3_t const & get_render_origin ( void ) = 0;
	virtual vec3_t const & get_render_angles ( void ) = 0;
	virtual bool should_draw ( void ) = 0;
	virtual uint32_t render_flags ( void ) = 0;
	virtual void unknown ( void ) const { }
	virtual unsigned short get_shadow_handle ( ) const = 0;
	virtual unsigned short & render_andle ( ) = 0;
	virtual model_t * get_model ( ) const = 0;
	virtual int draw_model ( int flags, const uint32_t & instance ) = 0;
	virtual int get_body ( ) = 0;
	virtual void get_color_modulation ( float * color ) = 0;
	virtual bool lod_test ( ) = 0;
	virtual bool setup_bones ( matrix3x4_t * bone_to_world_out, int max_bones, uint32_t mask, float curr_time ) = 0;
	virtual void setup_weights ( const matrix3x4_t * bone_to_world_out, int flex_weight_count, float * flex_weights, float * flex_delayed_weights ) = 0;
	virtual void do_animation_events ( void ) = 0;
	virtual void * get_pvs_notify_interface ( ) = 0;
	virtual void get_render_bounds ( vec3_t & mins, vec3_t & maxs ) = 0;
	virtual void get_render_bounds_world_space ( vec3_t & mins, vec3_t & maxs ) = 0;
	virtual void get_shadow_render_bounds ( vec3_t & mins, vec3_t & maxs, uint32_t shadow_type ) = 0;
	virtual bool should_receive_projected_textures ( int flags ) = 0;
	virtual bool get_shadow_cast_distance ( float * pDist, uint32_t shadow_type ) const = 0;
	virtual bool get_shadow_cast_direction ( vec3_t, uint32_t shadow_type ) const = 0;
	virtual bool is_shadow_dirty ( ) = 0;
	virtual void mark_shadow_dirty ( bool dirty ) = 0;
	virtual i_client_renderable * get_shadow_parent ( ) = 0;
	virtual i_client_renderable * first_shadow_child ( ) = 0;
	virtual i_client_renderable * next_shadow_peer ( ) = 0;
	virtual uint32_t shadow_cast_Type ( ) = 0;
	virtual void create_model_instance ( ) = 0;
	virtual unsigned short get_model_instance ( ) = 0;
	virtual const matrix3x4_t & renderable_to_world_transform ( ) = 0;
	virtual int lookup_attachment ( const char * attachment_name ) = 0;
	virtual bool get_attachment ( int number, vec3_t & origin, vec3_t & angles ) = 0;
	virtual bool get_attachment ( int number, matrix3x4_t & matrix ) = 0;
	virtual float * get_render_clip_plane ( void ) = 0;
	virtual int get_skin ( ) = 0;
	virtual void on_threaded_draw_setup ( ) = 0;
	virtual bool uses_flex_delayed_weights ( ) = 0;
	virtual void record_tool_message ( ) = 0;
	virtual bool should_draw_for_split_screen_user ( int slot ) = 0;
	virtual uint8_t override_alpha_modulation ( uint8_t alpha ) = 0;
	virtual uint8_t override_shadow_alpha_modulation ( uint8_t alpha ) = 0;
};
class i_material_var {


	void SetVectorInternal ( const float x, const float y ) {
		utilities::get_virtual_function<void ( __thiscall * )( void *, float, float )> ( this, 10 )( this, x, y );
	}

	void SetVectorInternal ( const float x, const float y, const float z ) {
		utilities::get_virtual_function<void ( __thiscall * )( void *, float, float, float )> ( this, 11 )( this, x, y, z );
	}

public:
	void SetFloat ( const float val ) {
		utilities::get_virtual_function<void ( __thiscall * )( void *, float )> ( this, 4 )( this, val );
	}

	void SetInt ( const int val ) {
		utilities::get_virtual_function<void ( __thiscall * )( void *, int )> ( this, 5 )( this, val );
	}

	void SetString ( char const * val ) {
		utilities::get_virtual_function<void ( __thiscall * )( void *, char const * )> ( this, 6 )( this, val );
	}

	void SetMatrix ( matrix3x4_t & matrix ) {
		utilities::get_virtual_function<void ( __thiscall * )( void *, matrix3x4_t & )> ( this, 6 )( this, matrix );
	}

	void SetVectorComponent ( const float val, const int comp ) {
		utilities::get_virtual_function<void ( __thiscall * )( void *, float, int )> ( this, 26 )( this, val, comp );
	}

	void SetVector ( const vec2_t vector ) {
		SetVectorInternal ( vector.x, vector.y );
	}

	void SetVector ( const vec3_t vector ) {
		SetVectorInternal ( vector.x, vector.y, vector.z );
	}
};

class c_key_values {
public:
	~c_key_values( )
	{
		free( this );
	}

	void init( const char* key_name )
	{
		using InitKeyValues_t = void( __thiscall* )( void*, const char* );
		static InitKeyValues_t InitKeyValuesEx = nullptr;

		if ( !InitKeyValuesEx )
			InitKeyValuesEx = *reinterpret_cast< InitKeyValues_t >( utilities::pattern_scan( "client.dll", "55 8B EC 51 33 C0 C7 45" ) );

		InitKeyValuesEx( this, key_name );
	}

	void load_from_buffer( const char* resource, const char* buf, void* file = nullptr, const char* id = nullptr, void* eval_proc = nullptr, void* unk = nullptr )
	{
		using LoadFromBuffer_t = void( __thiscall* )( void*, const char*, const char*, void*, const char*, void*, void* );
		static LoadFromBuffer_t LoadFromBufferEx = nullptr;

		if ( !LoadFromBufferEx )
			LoadFromBufferEx = *reinterpret_cast< LoadFromBuffer_t >( utilities::pattern_scan( "client.dll", "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89" ) );

		LoadFromBufferEx( this, resource, buf, file, id, eval_proc, unk );
	}

	c_key_values* find_key( const char* name, bool create )
	{
		using findkey_t = c_key_values * ( __thiscall* )( c_key_values*, const char*, bool );
		static auto FindKey = *reinterpret_cast< findkey_t >( utilities::pattern_scan( "client.dll", "55 8B EC 83 EC 1C 53 8B D9 85 DB" ) );
		return FindKey( this, name, create );
	}

	void set_int( const char* key_name, std::int32_t val )
	{
		auto key_int = find_key( key_name, true );
		if ( key_int ) {
			*reinterpret_cast< std::int32_t* >( reinterpret_cast< std::uint32_t >( key_int ) + 0xC ) = val;
			*reinterpret_cast< char* >( reinterpret_cast< std::uint32_t >( key_int ) + 0x10 ) = 2;
		}
	}

	void set_string( const char* name, const char* a2 )
	{
		auto key = find_key( name, true );
		using setstring_t = void( __thiscall* )( void*, const char* );
		if ( key ) {
			static auto SetString = *reinterpret_cast< setstring_t >( utilities::pattern_scan( "client.dll" , "55 8B EC A1 ? ? ? ? 53 56 57 8B F9 8B 08 8B 01" ) );
			SetString( key, a2 );
		}
	}
};
class i_material {
public:
	virtual const char* get_name( ) const = 0;
	virtual const char* GetTextureGroupName( ) const = 0;
	virtual preview_image_retval_t get_preview_image_properties( int* width, int* height, image_format* imageFormat, bool* isTranslucent ) const = 0;
	virtual preview_image_retval_t get_preview_image( unsigned char* data, int width, int height, image_format imageFormat ) const = 0;
	virtual int get_mapping_width( ) = 0;
	virtual int get_mapping_height( ) = 0;
	virtual int get_num_animation_frames( ) = 0;
	virtual bool in_material_page( void ) = 0;
	virtual void get_material_offset( float* pOffset ) = 0;
	virtual void get_material_scale( float* pScale ) = 0;
	virtual i_material* get_material_page( void ) = 0;
	virtual i_material_var* find_var( const char* varName, bool* found, bool complain = true ) = 0;
	virtual void increment_reference_count( void ) = 0;
	virtual void decrement_reference_count( void ) = 0;
	inline void add_ref( ) { increment_reference_count( ); }
	inline void release( ) { decrement_reference_count( ); }
	virtual int get_enumeration_id( void ) const = 0;
	virtual void get_low_res_color_sample( float s, float t, float* color ) const = 0;
	virtual void recompute_state_snapshots( ) = 0;
	virtual bool is_translucent( ) = 0;
	virtual bool is_alpha_tested( ) = 0;
	virtual bool is_vertex_lit( ) = 0;
	virtual vertex_format_t get_vertex_format( ) const = 0;
	virtual bool has_proxy( void ) const = 0;
	virtual bool uses_env_cubemap( void ) = 0;
	virtual bool needs_tangent_space( void ) = 0;
	virtual bool needs_power_of_two_frame_buffer_texture( bool bCheckSpecificToThisFrame = true ) = 0;
	virtual bool needs_full_frame_buffer_texture( bool bCheckSpecificToThisFrame = true ) = 0;
	virtual bool needs_software_skinning( void ) = 0;
	virtual void alpha_modulate( float alpha ) = 0;
	virtual void color_modulate( float r, float g, float b ) = 0;
	virtual void set_material_var_flag( material_var_flags_t flag, bool on ) = 0;
	virtual bool get_material_var_flag( material_var_flags_t flag ) const = 0;
	virtual void get_reflectivity( vec3_t& reflect ) = 0;
	virtual bool get_property_flag( material_property_types_t  type ) = 0;
	virtual bool is_two_sided( ) = 0;
	virtual void set_shader( const char* pShaderName ) = 0;
	virtual int get_num_passes( void ) = 0;
	virtual int get_texture_memory_bytes( void ) = 0;
	virtual void refresh( ) = 0;
	virtual bool needs_lightmap_blend_alpha( void ) = 0;
	virtual bool needs_software_lighting( void ) = 0;
	virtual int shader_param_count( ) const = 0;
	virtual i_material_var** get_shader_params( void ) = 0;
	virtual bool is_error_material( ) const = 0;
	virtual void unused( ) = 0;
	virtual float get_alpha_modulation( ) = 0;
	virtual void get_color_modulation( float* r, float* g, float* b ) = 0;
	virtual bool is_translucent_under_modulation( float fAlphaModulation = 1.0f ) const = 0;
	virtual i_material_var* find_var_fast( char const* pVarName, unsigned int* pToken ) = 0;
	virtual void set_shader_and_params( c_key_values* pKeyValues ) = 0;
	virtual const char* get_shader_name( ) const = 0;
	virtual void delete_if_unreferenced( ) = 0;
	virtual bool is_sprite_card( ) = 0;
	virtual void call_bind_proxy( void* proxyData ) = 0;
	virtual void refresh_preserving_material_vars( ) = 0;
	virtual bool was_reloaded_from_whitelist( ) = 0;
	virtual bool set_temp_excluded( bool bSet, int nExcludedDimensionLimit ) = 0;
	virtual int get_reference_count( ) const = 0;
};