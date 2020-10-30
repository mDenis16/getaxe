#pragma once
#include <vector>

class convar;
using fn_change_callback_t = void(*)(convar* var, const char* old_value, float f_old_value);

template <typename T>
class utl_vector {
public:
	constexpr T& operator[](int i) { return memory[i]; };

	T* memory;
	int allocation_count;
	int grow_size;
	int size;
	T* elements;
};

enum cvar_flags {
	fcvar_none = 0,
	fcvar_unregistered = (1 << 0),
	fcvar_developmentonly = (1 << 1),
	fcvar_gamedll = (1 << 2),
	fcvar_clientdll = (1 << 3),
	fcvar_hidden = (1 << 4),
	fcvar_protected = (1 << 5),
	fcvar_sponly = (1 << 6),
	fcvar_archive = (1 << 7),
	fcvar_notify = (1 << 8),
	fcvar_userinfo = (1 << 9),
	fcvar_printableonly = (1 << 10),
	fcvar_unlogged = (1 << 11),
	fcvar_never_as_string = (1 << 12),
	fcvar_replicated = (1 << 13),
	fcvar_cheat = (1 << 14),
	fcvar_ss = (1 << 15),
	fcvar_demo = (1 << 16),
	fcvar_dontrecord = (1 << 17),
	fcvar_ss_added = (1 << 18),
	fcvar_release = (1 << 19),
	fcvar_reload_materials = (1 << 20),
	fcvar_reload_textures = (1 << 21),
	fcvar_not_connected = (1 << 22),
	fcvar_material_system_thread = (1 << 23),
	fcvar_archive_xbox = (1 << 24),
	fcvar_accessible_from_threads = (1 << 25),
	fcvar_server_can_execute = (1 << 28),
	fcvar_server_cannot_query = (1 << 29),
	fcvar_clientcmd_can_execute = (1 << 30),
	fcvar_meme_dll = (1 << 31),
	fcvar_material_thread_mask = (fcvar_reload_materials | fcvar_reload_textures | fcvar_material_system_thread)
};
inline unsigned int get_vfunc( void* pp_class, unsigned int index ) {
	return ( unsigned int ) ( *( int** ) pp_class ) [ index ];
}
class convar {
public:
	__forceinline float get_float( void ) {
		int xord = ( int ) ( *( int* ) ( &f_value ) ^ ( uintptr_t ) this );
		return *( float* ) &xord;
	}

	__forceinline int get_int( void ) {
		int xord = ( int ) ( *( int* ) ( &n_value ) ^ ( uintptr_t ) this );
		return *( int* ) &xord;
	}

	bool get_bool( void ) {
		return !!get_int( );
	}

	void set_value( const char* value ) {
		return ( ( void( __thiscall* )( void*, const char* ) ) get_vfunc( this, 14 ) )( this, value );
	}

	void set_value( float value ) {
		return ( ( void( __thiscall* )( void*, float ) ) get_vfunc( this, 15 ) )( this, value );
	}

	void set_value( int value ) {
		return ( ( void( __thiscall* )( void*, int ) ) get_vfunc( this, 16 ) )( this, value );
	}


	void null_callback( void ) {
		*( int* ) ( ( uintptr_t ) &change_callback_fn + 0xC ) = 0;
	}

	byte		pad_0x0 [ 0x4 ];
	convar* next;
	int			registered;
	char* name;
	char* help_string;
	int			flags;
	byte		pad_0x18 [ 0x4 ];
	convar* parent;
	char* default_value;
	char* string;
	int			string_len;
	float		f_value;
	int			n_value;
	int			has_min;
	float		min_val;
	int			has_max;
	float		max_val;
	void* change_callback_fn;
};