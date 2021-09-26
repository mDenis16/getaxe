#pragma once
#include "../../dependencies/math/math.hpp"
#include <array>
#include "collideable.hpp"
#include "client_class.hpp"
#include "../structs/animstate.hpp"

#include "../../dependencies/utilities/netvars/netvars.hpp"
#include "../../core/features/inventory_changer/CEconItem.h"

namespace csgo {
	extern player_t * local_player;

	extern  bool in_
		[ 65 ];
}
enum data_update_type_t {
	DATA_UPDATE_CREATED = 0,
	DATA_UPDATE_DATATABLE_CHANGED,
};

enum cs_weapon_type {
	WEAPONTYPE_KNIFE = 0,
	WEAPONTYPE_PISTOL,
	WEAPONTYPE_SUBMACHINEGUN,
	WEAPONTYPE_RIFLE,
	WEAPONTYPE_SHOTGUN,
	WEAPONTYPE_SNIPER_RIFLE,
	WEAPONTYPE_MACHINEGUN,
	WEAPONTYPE_C4,
	WEAPONTYPE_PLACEHOLDER,
	WEAPONTYPE_GRENADE,
	WEAPONTYPE_UNKNOWN
};

enum client_frame_stage_t {
	FRAME_UNDEFINED = -1,			// (haven't run any frames yet)
	FRAME_START,

	// A network packet is being recieved
	FRAME_NET_UPDATE_START,
	// Data has been received and we're going to start calling PostDataUpdate
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	// Data has been received and we've called PostDataUpdate on all data recipients
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	// We've received all packets, we can now do interpolation, prediction, etc..
	FRAME_NET_UPDATE_END,

	// We're about to start rendering the scene
	FRAME_RENDER_START,
	// We've finished rendering the scene.
	FRAME_RENDER_END
};
enum InvalidatePhysicsBits_t : int {
	POSITION_CHANGED = 0x1,
	ANGLES_CHANGED = 0x2,
	VELOCITY_CHANGED = 0x4,
	ANIMATION_CHANGED = 0x8,
	BOUNDS_CHANGED = 0x10,
	SEQUENCE_CHANGED = 0x20
};

enum move_type {
	movetype_none = 0,
	movetype_isometric,
	movetype_walk,
	movetype_step,
	movetype_fly,
	movetype_flygravity,
	movetype_vphysics,
	movetype_push,
	movetype_noclip,
	movetype_ladder,
	movetype_observer,
	movetype_custom,
	movetype_last = movetype_custom,
	movetype_max_bits = 4,
	max_movetype
};

enum entity_flags {
	fl_onground = ( 1 << 0 ),
	fl_ducking = ( 1 << 1 ),
	fl_waterjump = ( 1 << 2 ),
	fl_ontrain = ( 1 << 3 ),
	fl_inrain = ( 1 << 4 ),
	fl_frozen = ( 1 << 5 ),
	fl_atcontrols = ( 1 << 6 ),
	fl_client = ( 1 << 7 ),
	fl_fakeclient = ( 1 << 8 ),
	fl_inwater = ( 1 << 9 ),
	fl_fly = ( 1 << 10 ),
	fl_swim = ( 1 << 11 ),
	fl_conveyor = ( 1 << 12 ),
	fl_npc = ( 1 << 13 ),
	fl_godmode = ( 1 << 14 ),
	fl_notarget = ( 1 << 15 ),
	fl_aimtarget = ( 1 << 16 ),
	fl_partialground = ( 1 << 17 ),
	fl_staticprop = ( 1 << 18 ),
	fl_graphed = ( 1 << 19 ),
	fl_grenade = ( 1 << 20 ),
	fl_stepmovement = ( 1 << 21 ),
	fl_donttouch = ( 1 << 22 ),
	fl_basevelocity = ( 1 << 23 ),
	fl_worldbrush = ( 1 << 24 ),
	fl_object = ( 1 << 25 ),
	fl_killme = ( 1 << 26 ),
	fl_onfire = ( 1 << 27 ),
	fl_dissolving = ( 1 << 28 ),
	fl_transragdoll = ( 1 << 29 ),
	fl_unblockable_by_player = ( 1 << 30 )
};

enum item_definition_indexes {
	weapon_deagle = 1,
	weapon_elite = 2,
	weapon_fiveseven = 3,
	weapon_glock = 4,
	weapon_ak47 = 7,
	weapon_aug = 8,
	weapon_awp = 9,
	weapon_famas = 10,
	weapon_g3sg1 = 11,
	weapon_galilar = 13,
	weapon_m249 = 14,
	weapon_m4a1 = 16,
	weapon_mac10 = 17,
	weapon_p90 = 19,
	weapon_mp5sd = 23,
	weapon_ump45 = 24,
	weapon_xm1014 = 25,
	weapon_bizon = 26,
	weapon_mag7 = 27,
	weapon_negev = 28,
	weapon_sawedoff = 29,
	weapon_tec9 = 30,
	weapon_taser = 31,
	weapon_hkp2000 = 32,
	weapon_mp7 = 33,
	weapon_mp9 = 34,
	weapon_nova = 35,
	weapon_p250 = 36,
	weapon_scar20 = 38,
	weapon_sg556 = 39,
	weapon_ssg08 = 40,
	weapon_knifegg = 41,
	weapon_knife = 42,
	weapon_flashbang = 43,
	weapon_hegrenade = 44,
	weapon_smokegrenade = 45,
	weapon_molotov = 46,
	weapon_decoy = 47,
	weapon_incgrenade = 48,
	weapon_c4 = 49,
	weapon_knife_t = 59,
	weapon_m4a1_silencer = 60,
	weapon_usp_silencer = 61,
	weapon_cz75a = 63,
	weapon_revolver = 64,
	weapon_knife_bayonet = 500,
	weapon_knife_flip = 505,
	weapon_knife_gut = 506,
	weapon_knife_karambit = 507,
	weapon_knife_m9_bayonet = 508,
	weapon_knife_tactical = 509,
	weapon_knife_falchion = 512,
	weapon_knife_bowie = 514,
	weapon_knife_butterfly = 515,
	weapon_knife_shadowdaggers = 516,
	weapon_knife_navaja = 520,
	weapon_knife_stiletto = 522,
	weapon_knife_ursus = 519,
	weapon_knife_widowmaker = 523
};

struct bbox_t {
	int x, y, w, h;
	bbox_t ( ) = default;
	bbox_t ( int x, int y, int w, int h ) {
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}
};
struct RenderableInstance_t {
	uint8_t m_alpha;
	__forceinline RenderableInstance_t ( ) : m_alpha { 255ui8 } { }
};



class entity_t {
public:
	void * animating ( ) {
		return reinterpret_cast< void * >( uintptr_t ( this ) + 0x4 );
	}
	i_client_renderable * get_renderable_virtual ( void ) {
		return ( i_client_renderable * ) ( ( uintptr_t ) this + 0x4 );
	}

	bool IsStandable ( )  {
		if ( this->collideable()->GetSolidFlags ( ) & FSOLID_NOT_STANDABLE )
			return false;

		if ( this->collideable ( )->GetSolid ( ) == SOLID_BSP || this->collideable ( )->GetSolid ( ) == SOLID_VPHYSICS || this->collideable ( )->GetSolid ( ) == SOLID_BBOX )
			return true;

		return IsBSPModel ( );
	}

	bool IsBSPModel ( )  {
		if ( this->collideable ( )->GetSolid ( ) == SOLID_BSP )
			return true;

	
		if ( this->collideable ( )->GetSolid ( ) == SOLID_VPHYSICS && interfaces::model_info->get_model_type ( this->model() ) == mod_brush )
			return true;

		return false;
	}


	matrix3x4_t & coord_frame ( ) {
		const static auto m_CollisionGroup = netvar_manager::get_net_var ( fnv::hash ( "DT_BaseEntity" ), fnv::hash ( "m_CollisionGroup" ) );

		auto m_rgflCoordinateFrame = m_CollisionGroup - 0x30;

		return *reinterpret_cast< matrix3x4_t * >( reinterpret_cast< uintptr_t >( this ) + m_rgflCoordinateFrame );
	}

	NETVAR ( "DT_BaseCSGrenadeProjectile", "m_vInitialVelocity", m_vInitialVelocity, vec3_t );
	NETVAR ( "DT_BaseEntity", "m_iHealth", m_iHealth, int );
	NETVAR ( "DT_BaseEntity", "m_vecMins", mins, vec3_t );
	NETVAR ( "DT_BaseEntity", "m_vecMaxs", maxs, vec3_t );
	NETVAR ( "DT_BaseEntity", "m_vecBaseVelocity", m_vecBaseVelocity, vec3_t );
	
	NETVAR ( "DT_SmokeGrenadeProjectile", "m_bDidSmokeEffect", m_bDidSmokeEffect, bool );
	NETVAR ( "DT_CSRagdoll", "m_hPlayer", m_hPlayer, void* );
	NETVAR ( "DT_SmokeGrenadeProjectile", "m_nSmokeEffectTickBegin", m_nSmokeEffectTickBegin, int );
	NETVAR ( "DT_SmokeGrenadeProjectile", "m_nExplodeEffectTickBegin", m_nExplodeEffectTickBegin, int );
	
	NETVAR (  "DT_Inferno", "m_fireXDelta", fireXDelta, int* );
	NETVAR (  "DT_Inferno", "m_fireYDelta", fireYDelta, int * );
	NETVAR (  "DT_Inferno", "m_fireZDelta", fireZDelta, int * );
	NETVAR (  "DT_Inferno", "m_bFireIsBurning", fireIsBurning, bool* );
	NETVAR (  "DT_Inferno", "m_fireCount", fireCount, int );

	int get_sequence_act ( int sequence ) {
		const auto model = this->model ( );
		if ( !model )
			return -1;
		static auto get_sequence_act_pattern = utilities::pattern_scan ( "client.dll", "55 8B EC 53 8B 5D 08 56 8B F1 83" );
		const auto hdr = interfaces::model_info->get_studio_model ( model );
		if ( !hdr )
			return -1;

		static auto get_sequence_act = reinterpret_cast< int ( __fastcall * )( void *, studio_hdr_t *, int ) >( get_sequence_act_pattern );
		return get_sequence_act ( this, hdr, sequence );
	}
	void * networkable ( ) {
		return reinterpret_cast< void * >( uintptr_t ( this ) + 0x8 );
	}
	collideable_t * collideable ( ) {
		using original_fn = collideable_t * ( __thiscall * )( void * );
		return ( *( original_fn ** ) this ) [ 3 ] ( this );
	}
	c_client_class * client_class ( ) {
		using original_fn = c_client_class * ( __thiscall * )( void * );
		return ( *( original_fn ** ) networkable ( ) ) [ 2 ] ( networkable ( ) );
	}

	int index ( ) {
		using original_fn = int ( __thiscall * )( void * );
		return ( *( original_fn ** ) networkable ( ) ) [ 10 ] ( networkable ( ) );
	}
	inline const char * get_classname ( void ) {
		return ( ( const char * ( __thiscall * )( entity_t * ) ) * ( uintptr_t * ) ( *( uintptr_t * ) this + 556 ) )( this );
	}

	bool is_player ( ) {
		using original_fn = bool ( __thiscall * )( entity_t * );
		return ( *( original_fn ** ) this ) [158] ( this );
	}
	bool is_weapon ( ) {
		using original_fn = bool ( __thiscall * )( entity_t * );
		return ( *( original_fn ** ) this ) [166] ( this );
	}
	vec3_t get_absolute_origin ( ) {
		__asm {
			MOV ECX, this
			MOV EAX, DWORD PTR DS : [ECX]
			CALL DWORD PTR DS : [EAX + 0x28]
		}
	}
	vec3_t  get_render_origin ( ) {
		using original_fn = vec3_t  ( __thiscall * )( void * );
		return ( *( original_fn ** ) animating ( ) ) [ 1 ] ( animating ( ) );
	}

	
	bool setup_bones_alternative ( BoneArray * out, int max_bones, int mask, float time ) {
		if ( !this )
			return false;

		using original_fn = bool ( __thiscall * )( void *, BoneArray *, int, int, float );
		return ( *( original_fn ** ) animating ( ) ) [ 13 ] ( animating ( ), out, max_bones, mask, time );
	}


	/*__forceinline IClientUnknown * GetIClientUnknown ( ) {
		return utilities::get_virtual_function< IClientUnknown * ( __thiscall * )( void * ) > ( networkable ( ), 0 )( networkable ( ) );
	}
	__forceinline unsigned long GetRefEHandle ( ) {
		if ( !this->GetIClientUnknown ( ) )
			return NULL;

		return this->GetIClientUnknown ( )->GetRefEHandle ( );
	}*/
	model_t * model ( ) {
		using original_fn = model_t * ( __thiscall * )( void * );
		return ( *( original_fn ** ) animating ( ) ) [ 8 ] ( animating ( ) );
	}
	void update ( ) {
		using original_fn = void ( __thiscall * )( entity_t * );
		( *( original_fn ** ) this ) [ 218 ] ( this );
	}
	int draw_model ( int flags, uint8_t alpha ) {
		using original_fn = int ( __thiscall * )( void *, int, uint8_t );
		return ( *( original_fn ** ) animating ( ) ) [ 9 ] ( animating ( ), flags, alpha );
	}
	void set_angles ( vec3_t angles ) {
		using original_fn = void ( __thiscall * )( void *, const vec3_t & );
		static original_fn set_angles_fn = ( original_fn ) ( ( DWORD ) utilities::pattern_scan ( "client.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1" ) );
		set_angles_fn ( this, angles );
	}
	void set_position ( vec3_t position ) {
		using original_fn = void ( __thiscall * )( void *, const vec3_t & );
		static original_fn set_position_fn = ( original_fn ) ( ( DWORD ) utilities::pattern_scan ( "client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8" ) );
		set_position_fn ( this, position );
	}



	void set_model_index ( int index ) {
		using original_fn = void ( __thiscall * )( void *, int );
		return ( *( original_fn ** ) this ) [ 75 ] ( this, index );
	}

	void net_pre_data_update ( int update_type ) {
		using original_fn = void ( __thiscall * )( void *, int );
		return ( *( original_fn ** ) networkable ( ) ) [ 6 ] ( networkable ( ), update_type );
	}

	void net_release ( ) {
		using original_fn = void ( __thiscall * )( void * );
		return ( *( original_fn ** ) networkable ( ) ) [ 1 ] ( networkable ( ) );
	}
	void set_local_viewangles ( vec3_t & angle ) {
		( ( void ( __thiscall * )( void *, vec3_t & ) ) get_vfunc ( this, 372 ) )( this, angle );
	}
	int net_set_destroyed_on_recreate_entities ( ) {
		using original_fn = int ( __thiscall * )( void * );
		return ( *( original_fn ** ) networkable ( ) ) [ 13 ] ( networkable ( ) );
	}
	float & get_old_simulation_time ( void ) {
		static auto	offset = netvar_manager::get_net_var ( fnv::hash ( "DT_BaseEntity" ), fnv::hash ( "m_flSimulationTime" ) );

		return *( float * ) ( ( uintptr_t ) this + offset + 0x4 );
	}

	OFFSET ( float, get_spawn_time, 0x20 );

	NETVAR ( "DT_CSPlayer", "m_fFlags", flags, int );
	OFFSET ( bool, dormant, 0xED );
	OFFSET ( int, get_take_damage, 0x27C )
	OFFSET ( vec3_t, old_origin, 0x3A8 );
	NETVAR ( "DT_BaseEntity", "m_hOwnerEntity", owner_handle, unsigned long );
	NETVAR ( "DT_BaseEntity", "m_hOwner", original_owner_handle, unsigned long );
	NETVAR ( "DT_BaseEntity", "m_flSimulationTime", simulation_time, float );
	OFFSET ( float, m_flVelocityModifier, 0xA38C );
	NETVAR ( "DT_BaseEntity", "m_vecOrigin", origin, vec3_t );

	NETVAR ( "DT_BaseEntity", "m_vecVelocity[0]", vec_velocity, vec3_t );

	NETVAR ( "DT_BasePlayer", "m_vecViewOffset[0]", view_offset, vec3_t );
	NETVAR ( "DT_CSPlayer", "m_iTeamNum", team, int );
	NETVAR ( "DT_BaseEntity", "m_bSpotted", spotted, bool );
	NETVAR ( "DT_CSPlayer", "m_nSurvivalTeam", survival_team, int );

	NETVAR ( "DT_CSPlayer", "m_flHealthShotBoostExpirationTime", health_boost_time, float );



};

class econ_view_item_t {
public:
	NETVAR ( "DT_ScriptCreatedItem", "m_bInitialized", is_initialized, bool );
	NETVAR ( "DT_ScriptCreatedItem", "m_iEntityLevel", entity_level, int );
	NETVAR ( "DT_ScriptCreatedItem", "m_iAccountID", account_id, int );
	NETVAR ( "DT_ScriptCreatedItem", "m_iItemIDLow", item_id_low, int );
};

class IRefCounted {
private:
	volatile long refCount;

public:
	virtual void destructor ( char bDelete ) = 0;
	virtual bool OnFinalRelease ( ) = 0;

	void unreference ( ) {
		if ( InterlockedDecrement ( &refCount ) == 0 && OnFinalRelease ( ) ) {
			destructor ( 1 );
		}
	}
};
class C_EconItemDefinition {
public:
	/*char pad_0x0000 [ 0x8 ]; ///0x0000
	int32_t m_iItemDefinitionIndex; ///0x0008 
	char pad_0x000C [ 0x40 ]; ///0x000C
	char * m_szHudName; ///0x004C 
	char pad_0x0050 [ 0x4 ]; ///0x0050
	char * m_szWeaponType; ///0x0054 
	char pad_0x0058 [ 0x4 ]; ///0x0058
	char * m_szWeaponDescription; ///0x005C 
	char pad_0x0060 [ 0x34 ]; ///0x0060
	char * m_szViewModel; ///0x0094 
	char pad_0x0098 [ 0x4 ]; ///0x0098
	char * m_szWorldModel; ///0x009C 
	char * m_szWorldModelDropped; ///0x00A0 
	char pad_0x00A4 [ 0x110 ]; ///0x00A4
	char * m_szWeaponName1; ///0x01B4 
	char pad_0x01B8 [ 0x8 ]; ///0x01B8
	char * m_szWeaponName; ///0x01C0 
	char pad_0x01C4 [ 0x27C ]; ///0x01C4*/

	int GetEquippedPosition ( ) {
		return *reinterpret_cast< int * >( ( uintptr_t ) this + 0x28C );
	}
}; ///Size=0x0440
class C_EconItemView {
private:
	using str_32 = char [ 32 ];
public:
	NETVAR2 ( int32_t, m_bInitialized, "DT_BaseAttributableItem", "m_bInitialized" );
	NETVAR2 ( int32_t, m_iItemDefinitionIndex, "DT_BaseAttributableItem", "m_iItemDefinitionIndex" );
	NETVAR2 ( int32_t, m_iEntityLevel, "DT_BaseAttributableItem", "m_iEntityLevel" );
	NETVAR2 ( int32_t, m_iAccountID, "DT_BaseAttributableItem", "m_iAccountID" );
	NETVAR2 ( int32_t, m_iItemIDLow, "DT_BaseAttributableItem", "m_iItemIDLow" );
	NETVAR2 ( int32_t, m_iItemIDHigh, "DT_BaseAttributableItem", "m_iItemIDHigh" );
	NETVAR2 ( int32_t, m_iEntityQuality, "DT_BaseAttributableItem", "m_iEntityQuality" );
	NETVAR2 ( str_32, m_iCustomName, "DT_BaseAttributableItem", "m_szCustomName" );

	c_utl_vector<IRefCounted *> & m_CustomMaterials ( );
	c_utl_vector<IRefCounted *> & m_VisualsDataProcessors ( );


	

	C_EconItemDefinition * GetStaticData ( ) {
		static auto fnGetStaticData
			= reinterpret_cast< C_EconItemDefinition * ( __thiscall * )( void * ) >(
				utilities::pattern_scan (  ( "client.dll" ), "55 8B EC 51 53 8B D9 8B ? ? ? ? ? 56 57 8B ? ? ? ? ? 85 FF 74 16" )
				);
		return fnGetStaticData ( this );
	}
	template <typename T>
	static constexpr auto relativeToAbsolute ( uintptr_t address ) noexcept {
		return ( T ) ( address + 4 + *reinterpret_cast< std::int32_t * >( address ) );
	}
	CEconItem * GetSOCData ( ) {
		if ( !this )return nullptr;
		static bool no_init = false;
		static CEconItem * ( __thiscall * getSOCData )( C_EconItemView * itemView );
		if ( !no_init ) {
			
			getSOCData = relativeToAbsolute<decltype( getSOCData )> ( utilities::code_style_pattern ( "client.dll", "\xE8????\x32\xC9" ) + 1 );
			no_init = true;
		}

		return getSOCData ( this );
	}
};

class base_view_model_t : public entity_t {
public:
	NETVAR ( "DT_BaseViewModel", "m_nModelIndex", model_index, int );
	NETVAR ( "DT_BaseViewModel", "m_nViewModelIndex", view_model_index, int );
	NETVAR ( "DT_BaseViewModel", "m_hWeapon", m_hweapon, int );
	NETVAR ( "DT_BaseViewModel", "m_hOwner", m_howner, int );
};

class weapon_t : public entity_t {
public:
	NETVAR ( "DT_BaseCombatWeapon", "m_flNextPrimaryAttack", next_primary_attack, float );
	NETVAR ( "DT_BaseCombatWeapon", "m_flNextSecondaryAttack", next_secondary_attack, float );
	NETVAR ( "DT_BaseCombatWeapon", "m_iClip1", clip1_count, int );
	NETVAR ( "DT_BaseCombatWeapon", "m_iClip2", clip2_count, int );

	NETVAR ( "DT_BaseAttributableItem", "m_OriginalOwnerXuidHigh", m_OriginalOwnerXuidHigh, int );
	NETVAR ( "DT_BaseAttributableItem", "m_OriginalOwnerXuidLow", m_OriginalOwnerXuidLow, int );

	NETVAR ( "DT_BaseCombatWeapon", "m_iPrimaryReserveAmmoCount", primary_reserve_ammo_acount, int );
	NETVAR ( "DT_WeaponCSBase", "m_flRecoilIndex", recoil_index, float );
	NETVAR ( "DT_WeaponCSBaseGun", "m_zoomLevel", zoom_level, float );
	NETVAR ( "DT_WeaponCSBase", "m_fLastShotTime", m_fLastShotTime, float );
	NETVAR ( "DT_BaseAttributableItem", "m_iItemDefinitionIndex", item_definition_index, short );
	NETVAR ( "DT_BaseCombatWeapon", "m_iEntityQuality", entity_quality, int );
	NETVAR ( "CWeaponCSBase", "m_flPostponeFireReadyTime", get_postpone_fire_ready_time, float );
	NETVAR ( "CWeaponCSBase", "m_fLastShotTime", last_shot_time, float );
	NETVAR ( "DT_BaseCSGrenade", "m_fThrowTime", throw_time, float );
	NETVAR ( "DT_BaseCSGrenade", "m_bPinPulled", pin_pulled, bool );
	NETVAR ( "CBaseEntity", "m_hOwnerEntity", h_prev_owner, uintptr_t );

	bool is_non_aim ( ) {
		if ( !this ) //-V704
			return true;

		auto idx = item_definition_index ( );

		if ( idx == weapon_c4  )
			return true;

		if ( is_knife ( ) )
			return true;

		if ( is_grenade ( ) )
			return true;

		return false;
	}
	bool is_empty ( ) {
		if ( !this ) //-V704
			return true;

		return clip1_count ( ) <= 0;
	}



	bool can_double_tap ( ) {
		if ( !this ) //-V704
			return false;

		if ( is_non_aim ( ) )
			return false;

		auto idx = item_definition_index ( );

		if ( idx == weapon_taser || idx == weapon_revolver || idx == weapon_awp || idx == weapon_xm1014 || idx == weapon_nova || idx == weapon_sawedoff || idx == weapon_mag7 )
			return false;

		return true;
	}

	int get_max_tickbase_shift ( ) {
		if ( !this )
			return 0;
		if ( !can_double_tap ( ) )
			return 16;// m_gamerules ( )->m_bIsValveDS ( ) ? 6 : 16;

		auto idx = item_definition_index ( );
		auto max_tickbase_shift = 0;

		switch ( idx ) {
		case weapon_m249:
		case weapon_mac10:
		case weapon_p90:
		case weapon_mp5sd:
		case weapon_negev:
		case weapon_mp9:
			max_tickbase_shift = 5;
			break;
		case weapon_elite:
		case weapon_ump45:
		case weapon_bizon:
		case weapon_tec9:
		case weapon_mp7:
			max_tickbase_shift = 6;
			break;
		case weapon_ak47:
		case weapon_aug:
		case weapon_famas:
		case weapon_galilar:
		case weapon_m4a1:
		case weapon_m4a1_silencer:
		case weapon_cz75a:
			max_tickbase_shift = 7;
			break;
		case weapon_fiveseven:
		case weapon_glock:
		case weapon_p250:
		case weapon_sg556:
			max_tickbase_shift = 8;
			break;
		case weapon_hkp2000:
		case weapon_usp_silencer:
			max_tickbase_shift = 9;
			break;
		case weapon_deagle:
			max_tickbase_shift = 13;
			break;
		case weapon_g3sg1:
		case weapon_scar20:
			max_tickbase_shift = 12;
			break;
		}

		//if ( m_gamerules ( )->m_bIsValveDS ( ) )
			//max_tickbase_shift = min ( max_tickbase_shift, 6 );

		return max_tickbase_shift;
	}


	inline bool is_grenade ( void ) {
		if ( !this )
			return false;
		switch ( item_definition_index ( ) ) {
		case ( short ) item_definition_indexes::weapon_hegrenade:
		case ( short ) item_definition_indexes::weapon_molotov:
		case ( short ) item_definition_indexes::weapon_smokegrenade:
		case ( short ) item_definition_indexes::weapon_decoy:
		case ( short ) item_definition_indexes::weapon_incgrenade:
		case ( short ) item_definition_indexes::weapon_flashbang:
			return true;
		default:
			return false;
		}
	}
	bool can_fire_post_pone ( ) {
		if ( !this )
			return false;
		float rdyTime = get_postpone_fire_ready_time ( );

		if ( rdyTime > 0 && rdyTime < interfaces::globals->cur_time )
			return true;

		return false;
	}


	inline bool is_c4 ( void ) {
		if ( !this )
			return false;
		return get_weapon_data ( )->iWeaponType == ( int ) WEAPONTYPE_C4;
	}
	bool is_reloading ( void ) {
		if ( !this )
			return false;

		static auto in_reload = *( uintptr_t * ) ( ( uintptr_t ) utilities::pattern_scan ( "client.dll", "C6 87 ? ? ? ? ? 8B 06 8B CE FF 90" ) + 0x2 );
		return *( bool * ) ( ( uintptr_t ) this + in_reload );
	}
	inline bool is_knife ( void ) {
		if ( !this )
			return false;
		switch ( item_definition_index ( ) ) {
		case ( short ) item_definition_indexes::weapon_knife:
		case ( short ) item_definition_indexes::weapon_knifegg:
		case ( short ) item_definition_indexes::weapon_knife_bayonet:
		case ( short ) item_definition_indexes::weapon_knife_bowie:
		case ( short ) item_definition_indexes::weapon_knife_butterfly:
		case ( short ) item_definition_indexes::weapon_knife_falchion:
		case ( short ) item_definition_indexes::weapon_knife_flip:
		case ( short ) item_definition_indexes::weapon_knife_gut:
		case ( short ) item_definition_indexes::weapon_knife_karambit:
		case ( short ) item_definition_indexes::weapon_knife_m9_bayonet:
		case ( short ) item_definition_indexes::weapon_knife_navaja:
		case ( short ) item_definition_indexes::weapon_knife_shadowdaggers:
		case ( short ) item_definition_indexes::weapon_knife_stiletto:
		case ( short ) item_definition_indexes::weapon_knife_tactical:
		case ( short ) item_definition_indexes::weapon_knife_ursus:
		case ( short ) item_definition_indexes::weapon_knife_widowmaker:
		case ( short ) item_definition_indexes::weapon_knife_t:
			return true;
		default:
			return false;
		}
	}

	bool is_pistol ( void ) {
		return get_weapon_data ( )->iWeaponType == ( int ) cs_weapon_type::WEAPONTYPE_PISTOL;
	}

	inline bool is_revolver ( void ) {
		if ( !this )
			return false;

		return item_definition_index ( ) == ( int ) item_definition_indexes::weapon_revolver;
	}

	inline bool is_planted_c4 ( void ) {
		if ( !this )
			return false;

		return client_class ( )->class_id == 108;
	}

	inline bool is_defuse_kit ( void ) {
		if ( !this )
			return false;

		return client_class ( )->class_id == 2;
	}

	bool is_sniper ( void ) {
		switch ( item_definition_index ( ) ) {
		case ( short ) item_definition_indexes::weapon_awp:
		case ( short ) item_definition_indexes::weapon_g3sg1:
		case ( short ) item_definition_indexes::weapon_ssg08:
		case ( short ) item_definition_indexes::weapon_scar20:
			return true;
		default:
			return false;
		}
	}

	inline bool is_rifle ( void ) {
		switch ( get_weapon_data ( )->iWeaponType ) {
		case ( int ) cs_weapon_type::WEAPONTYPE_RIFLE:
		case ( int ) cs_weapon_type::WEAPONTYPE_SHOTGUN:
			return true;
		default:
			return false;
		}
	}

	float inaccuracy ( ) {
		using original_fn = float ( __thiscall * )( void * );
		return ( *( original_fn ** ) this ) [ 483 ] ( this );
	}

	float get_spread ( ) {
		using original_fn = float ( __thiscall * )( void * );
		return ( *( original_fn ** ) this ) [ 453 ] ( this );
	}

	void update_accuracy_penalty ( ) {
		using original_fn = void ( __thiscall * )( void * );
					( *( original_fn ** ) this ) [ 484 ] ( this );
	}

	weapon_info_t * get_weapon_data ( ) {
		return interfaces::weapon_system->get_weapon_data ( this->item_definition_index ( ) );
	}
	//NETVAR ( "DT_BaseAttributableItem", "m_Item", GetEconItemView, C_EconItemView * );

	C_EconItemView * GetEconItemView ( ) {
		using GetEconItemView_t = C_EconItemView * ( __thiscall * )( weapon_t * );

		static GetEconItemView_t        GetEconItemViewS = reinterpret_cast< GetEconItemView_t >(utilities::pattern_scan ( "client.dll", ( "8B 81 ? ? ? ? 81 C1 ? ? ? ? FF 50 04 83 C0 40 C3" ) ));

		return GetEconItemViewS ( this );
	}
	//GetEconItemView                 = pattern::find( m_client_dll, XOR( "8B 81 ? ? ? ? 81 C1 ? ? ? ? FF 50 04 83 C0 40 C3" ) ).as< GetEconItemView_t >( );
	
};

class grenade_t : public entity_t {
public:
	NETVAR ( "DT_BaseGrenade", "m_hThrower", thrower, int );
	NETVAR ( "DT_BaseGrenade", "m_vecVelocity", velocity, vec3_t );
	NETVAR ( "DT_BaseEntity", "m_flElasticity", elasticty, float );
	
};

class player_t : public entity_t {
private:
	

public:
	template <typename T>
	T & read ( uintptr_t offset ) {
		return *reinterpret_cast< T * >( reinterpret_cast< uintptr_t >( this ) + offset );
	}

	template <typename T>
	void write ( uintptr_t offset, T data ) {
		*reinterpret_cast< T * >( reinterpret_cast< uintptr_t >( this ) + offset ) = data;
	}
	OFFSET ( bool, JiggleBones, 0x2930);
	NETVAR ( "DT_BasePlayer", "m_hViewModel[0]", view_model, int );
	NETVAR ( "DT_CSPlayer", "m_bHasDefuser", has_defuser, bool );
	NETVAR ( "DT_CSPlayer", "m_bGunGameImmunity", has_gun_game_immunity, bool );
	NETVAR ( "DT_CSPlayer", "m_iShotsFired", shots_fired, int );
	NETVAR ( "DT_CSPlayer", "m_angEyeAngles", eye_angles, vec3_t );
	 
	NETVAR ( "DT_BaseAnimating", "m_bClientSideAnimation", m_bClientSideAnimation, bool );
	

	NETVAR_PTR("DT_CSPlayer", "m_hMyWearables", get_wearables, UINT);

	/*UINT* get_wearables() {
		return (UINT*)((uintptr_t)this + (netvar_manager::get_net_var(fnv::hash("DT_CSPlayer"), fnv::hash("m_hMyWearables"))));
	}*/
	NETVAR_PTR("DT_CSPlayer", "m_hMyWeapons", get_weapons, UINT);

	/*UINT* get_weapons() {
		return (UINT*)((uintptr_t)this + (netvar_manager::get_net_var(fnv::hash("DT_CSPlayer"), fnv::hash("m_hMyWeapons"))));
	}*/



	NETVAR ( "DT_CSPlayer", "m_ArmorValue", armor, int );
	NETVAR ( "DT_CSPlayer", "m_bHasHelmet", has_helmet, bool );
	NETVAR ( "DT_CSPlayer", "m_bIsScoped", is_scoped, bool );
	NETVAR ( "DT_CSPlayer", "m_bIsDefusing", is_defusing, bool );
	NETVAR ( "DT_CSPlayer", "m_iAccount", money, int );
	NETVAR ( "DT_CSPlayer", "m_flLowerBodyYawTarget", lower_body_yaw, float );
	NETVAR ( "DT_CSPlayer", "m_flNextAttack", next_attack, float );
	NETVAR ( "DT_CSPlayer", "m_flFlashDuration", flash_duration, float );
	NETVAR ( "DT_CSPlayer", "m_flFlashMaxAlpha", flash_alpha, float );
	NETVAR ( "DT_CSPlayer", "m_bHasNightVision", m_bHasNightVision, float );
	NETVAR ( "DT_CSPlayer", "m_bNightVisionOn", m_bNightVisionOn, float );
	NETVAR ( "DT_CSPlayer", "m_iHealth", health, int );
	NETVAR ( "DT_CSPlayer", "m_lifeState", life_state, int );
	NETVAR ( "DT_BasePlayer", "m_fFlags", flags, int );
	NETVAR ( "DT_BasePlayer", "m_viewPunchAngle", punch_angle, vec3_t );
	NETVAR ( "DT_BasePlayer", "m_aimPunchAngle", aim_punch_angle, vec3_t );
	NETVAR ( "DT_BasePlayer", "m_aimPunchAngleVel", aim_punch_angle_vel, vec3_t );

	NETVAR ( "DT_BasePlayer", "m_vecVelocity[0]", velocity, vec3_t );
	NETVAR ( "DT_BasePlayer", "m_flMaxspeed", max_speed, float );
	NETVAR ( "DT_BaseEntity", "m_flShadowCastDistance", m_flFOVTime, float );
	NETVAR ( "DT_BasePlayer", "m_hObserverTarget", observer_target, unsigned long );
	NETVAR ( "DT_BasePlayer", "m_nHitboxSet", hitbox_set, int );
	NETVAR ( "DT_CSPlayer", "m_flDuckAmount", duck_amount, float );
	NETVAR ( "DT_CSPlayer", "m_bHasHeavyArmor", has_heavy_armor, bool );
	NETVAR ( "DT_SmokeGrenadeProjectile", "m_nSmokeEffectTickBegin", smoke_grenade_tick_begin, int );
	
	NETVAR ( "DT_CSPlayer", "m_nTickBase", get_tick_base, int);
	NETVAR ( "DT_CSPlayer", "m_nNextThinkTick", m_nNextThinkTick, int )
	NETVAR ( "DT_BaseEntity", "m_fEffects", m_fEffects, int );
	
	c_utl_vector <matrix3x4_t> & m_CachedBoneData ( ) {
		static auto m_CachedBoneData = *( DWORD * ) ( utilities::pattern_scan (  ( "client.dll" ),  ( "FF B7 ?? ?? ?? ?? 52" ) ) + 0x2 ) + 0x4;
		return *( c_utl_vector <matrix3x4_t> * )( uintptr_t ( this ) + m_CachedBoneData );
	}
	POFFSET ( get_bone_accessor, CBoneAccessor, 0x26A8 )
	

	OFFSET ( vec3_t, get_abs_velocity, 0x94 )
	OFFSET ( int, Effects, 0xF0 );
	OFFSET ( int, get_ieflags, 0xE8 );

	OFFSET ( int, m_iOcclusionFlags, 0xA28 );
	OFFSET ( int, m_iOcclusionFramecount, 0xA30 );

	NETVAR ( "DT_BaseAnimating", "m_flCycle",  get_cycle, float );
	//NETVAR ( "DT_BaseAnimating", "m_flPoseParameter", m_flPoseParameter, float * );
	 std::array<float, 24> & m_flPoseParameter ( ) {
		 static int _m_flPoseParameter = netvar_manager::get_net_var ( fnv::hash ( "DT_BaseAnimating" ), fnv::hash ( "m_flPoseParameter" ) );
		 return *( std::array<float, 24> * )( uintptr_t ( this ) + _m_flPoseParameter );
	 }
	 bool & get_clientside_animation ( ) {
		 static auto offset = netvar_manager::get_net_var ( fnv::hash ( "DT_BaseAnimating" ), fnv::hash ( "m_bClientSideAnimation" ) );
		 return *reinterpret_cast< bool * >( uintptr_t ( this ) + offset );
	 }

	
	 std::array<animationlayer, 13> anim_layers ( ) {
		return  *( std::array<animationlayer, 13> * )( uintptr_t ( this ) + 0x2980 );
	 }
	animationlayer * get_animoverlays ( ) {
		return *( animationlayer ** ) ( ( uintptr_t ) this + 0x2980 );
	}
	float * posparams ( ) {
		static int _m_flPoseParameter = netvar_manager::get_net_var ( fnv::hash ( "DT_BaseAnimating" ), fnv::hash ( "m_flPoseParameter" ) );
		return *( float ** ) ( ( uintptr_t ) this + _m_flPoseParameter );
	}
	int animlayer_count ( ) {
		if ( !this ) //-V704
			return 0;

		return *( int * ) ( ( DWORD ) this + 0x298C );
	}

	void select_item ( const char * string, int sub_type = 0 ) {
		static auto select_item_fn = reinterpret_cast< void ( __thiscall * )( void *, const char *, int ) >( utilities::pattern_scan ( "client.dll", "55 8B EC 56 8B F1 ? ? ? 85 C9 74 71 8B 06" ) );
		select_item_fn ( this, string, sub_type );
	}




	weapon_t * active_weapon ( ) {
		static auto constexpr table_hash = fnv::hash("DT_CSPlayer");
		static auto constexpr prop_hash = fnv::hash("m_hActiveWeapon");


		static auto offset = netvar_manager::get_net_var(table_hash, prop_hash);


		auto active_weapon = read<uintptr_t> (offset) & 0xFFF;
		return reinterpret_cast< weapon_t * >( interfaces::entity_list->get_client_entity ( active_weapon ) );
	}


	c_utl_vector<matrix3x4_t> & GetBoneCache ( ) {
		static auto m_CachedBoneData = *( DWORD * ) ( utilities::pattern_scan ("client.dll",
			"FF B7 ?? ?? ?? ?? 52" ) + 0x2 ) + 0x4;
		return *( c_utl_vector<matrix3x4_t> * )( uintptr_t ( this ) + m_CachedBoneData );
	}
	void SetBoneCache ( matrix3x4_t * m ) {
		auto& cache = GetBoneCache ( );
		memcpy ( cache.base ( ), m, sizeof ( matrix3x4_t ) * cache.count ( ) );
	}

	void modify_eye_pos ( anim_state * animstate, vec3_t * pos ) {


		static auto lookup_bone_fn = ( int ( __thiscall * )( void *, const char * ) ) utilities::pattern_scan ( "client.dll", "555 8B EC 53 56 8B F1 57 83 BE ?? ?? ?? ?? ?? 75 14" );
		if ( animstate->m_hit_ground || animstate->m_duck_amount != 0.f ) {
			auto base_entity = animstate->m_entity;

			auto bone_pos = reinterpret_cast< player_t * >( base_entity )->get_bone_position ( 8 );

			bone_pos.z += 1.7f;

			if ( ( *pos ).z > bone_pos.z ) {
				float some_factor = 0.f;

				float delta = ( *pos ).z - bone_pos.z;

				float some_offset = ( delta - 4.f ) / 6.f;
				if ( some_offset >= 0.f )
					some_factor = std::fminf ( some_offset, 1.f );

				( *pos ).z += ( ( bone_pos.z - ( *pos ).z ) * ( ( ( some_factor * some_factor ) * 3.f ) - ( ( ( some_factor * some_factor ) * 2.f ) * some_factor ) ) );
			}
		}
	}

	vec3_t eye_pos ( ) {

		static auto Weapon_ShootPosition = reinterpret_cast< float * ( __thiscall * )( void *, vec3_t * ) >(
			utilities::pattern_scan ( "client.dll", "55 8B EC 56 8B 75 08 57 8B F9 56 8B 07 FF 90 ? ? ? ?" )
			);

		vec3_t pos;
		Weapon_ShootPosition ( this, &pos );

		return pos;
	}
	bool setup_bones ( matrix3x4_t * out, int max_bones, int mask, float time ) {
		if ( !this )
			return false;

		this->get_bone_accessor ( )->m_ReadableBones = this->get_bone_accessor ( )->m_WritableBones = 0;
		this->invalidate_bone_cache ( );

		using original_fn = bool ( __thiscall * )( void *, matrix3x4_t *, int, int, float );
		return ( *( original_fn ** ) animating ( ) ) [ 13 ] ( animating ( ), out, max_bones, mask, time );
	}

	vec3_t get_eye_pos ( ) {

	
		vec3_t pos = this->origin ( ) + this->view_offset ( );

		if ( interfaces::inputsystem->is_button_down(KEY_C) && this->index ( ) ==  interfaces::engine->get_local_player ( ) ) {
			vec3_t _origin = origin ( );

			vec3_t vDuckHullMin = interfaces::game_movement->GetPlayerMins ( true );
			vec3_t vStandHullMin = interfaces::game_movement->GetPlayerMins ( false );

			float fMore = ( vDuckHullMin.z - vStandHullMin.z );

			vec3_t vecDuckViewOffset = interfaces::game_movement->GetPlayerViewOffset ( true );
			vec3_t vecStandViewOffset = interfaces::game_movement->GetPlayerViewOffset ( false );
			float duckFraction = duck_amount ( );

			float tempz = ( ( vecDuckViewOffset.z - fMore ) * duckFraction ) +
				( vecStandViewOffset.z * ( 1 - duckFraction ) );

			_origin.z += tempz;

			return _origin;
		}

		auto anim_state = this->get_anim_state ( );

		if ( anim_state )
			modify_eye_pos ( anim_state, &pos );


		return pos;
	}


	bool & use_new_animation_state ( void ) {
		static auto use_new_animation_state_fn = *( uintptr_t * ) ( ( uintptr_t ) utilities::pattern_scan ( "client.dll", "88 87 ? ? ? ? 75" ) + 0x2 );
		return *( bool * ) ( ( uintptr_t ) this + use_new_animation_state_fn );
	}


	inline bool is_enemy ( void ) {
		if ( !csgo::local_player )
			csgo::local_player = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( interfaces::engine->get_local_player ( ) ) );
		return csgo::local_player->team ( ) != team ( );
	}

	inline bool is_teammate ( void ) {

		if ( !csgo::local_player )
			csgo::local_player = reinterpret_cast< player_t * >(interfaces::entity_list->get_client_entity ( interfaces::engine->get_local_player ( ) ));

		return csgo::local_player->team ( ) == team ( ) && index ( ) != csgo::local_player->index ( );
	}
	const vec3_t world_space_center ( ) {
		vec3_t vec_origin = origin ( );

		vec3_t min = this->collideable ( )->mins ( ) + vec_origin;
		vec3_t max = this->collideable ( )->maxs ( ) + vec_origin;

		vec3_t size = max - min;
		size /= 2.f;
		size += min;

		return size;
	}

	bool is_valid_player ( void ) {

		if ( !this )
			return false;

		if ( this->dormant ( ) || !this->is_alive ( ) )
			return false;

		if ( !this->is_enemy ( ) )
			return false;

		if ( this->client_class ( )->class_id != class_ids::CCSPlayer )
			return false;

		if ( this == csgo::local_player )
			return false;

		if ( this->has_gun_game_immunity ( ) )
			return false;


		return true;

	}
	anim_state * get_anim_state ( ) {
		return *reinterpret_cast< anim_state ** >( this + 0x3914 );
	}
	void update_state ( anim_state * state, vec3_t ang ) {
		if ( !state )
			return;
		using fn = void ( __vectorcall * )( void *, void *, float, float, float, void * );
		static auto memory_address = reinterpret_cast< fn >( utilities::pattern_scan ( ( "client.dll" ), "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24" ) );

		if ( !memory_address )
			return;

		__asm {
			push 0
			mov ecx, state
			movss xmm1, dword ptr [ ang + 4 ]
			movss xmm2, dword ptr [ ang ]

			call memory_address
		}
	}
	void update_animation_state ( anim_state * state, vec3_t angle ) {
		static auto UpdateAnimState = utilities::pattern_scan (
			"client.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24" );

		if ( !UpdateAnimState )
			return;

		__asm {
			push 0
		}

		__asm
		{
			mov ecx, state

			movss xmm1, dword ptr [ angle + 4 ]
			movss xmm2, dword ptr [ angle ]

			call UpdateAnimState
		}
	}
	player_info_t get_info ( ) {
		player_info_t info;
		interfaces::engine->get_player_info ( index ( ), &info );
		return info;
	}

	bool can_see_player_pos ( player_t * ent, const vec3_t & start,  const vec3_t & end ) {
		trace_t tr;
		ray_t ray;
		trace_filter filter;
		filter.skip = this;

		auto dir = ( end - start ).normalized ( );

		ray.initialize ( start, end );
		interfaces::trace_ray->trace_ray ( ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr );

		return tr.entity == ent || tr.flFraction > 0.97f;
	}

	void * get_renderable ( void ) {
		return ( void * ) ( ( uintptr_t ) this + 0x4 );
	}

	/*BoneAccessor = pattern::find( m_client_dll, XOR( "8D 81 ? ? ? ? 50 8D 84 24" ) ).add( 2 ).to< size_t >( );*/


	matrix3x4_t & coord_frame ( ) {
		const static auto m_CollisionGroup = netvar_manager::get_net_var ( fnv::hash("DT_BaseEntity"), fnv::hash("m_CollisionGroup") );

		auto m_rgflCoordinateFrame = m_CollisionGroup - 0x30;

		return *reinterpret_cast< matrix3x4_t * >( reinterpret_cast< uintptr_t >( this ) + m_rgflCoordinateFrame );
	}

	c_studio_hdr * get_model_ptr ( ) {
		return *( c_studio_hdr ** ) ( ( uintptr_t ) this + 0x294C );
	}

	vec3_t get_bone_position ( int bone, matrix3x4_t bone_matrix [ MAXSTUDIOBONES ] = nullptr ) {
		if ( !is_alive ( ) )
			return vec3_t ( );

		if ( bone_matrix == nullptr )
			bone_matrix = this->m_CachedBoneData ( ).base ( );

		if ( !bone_matrix )
			return vec3_t ( );

		return vec3_t { bone_matrix [ bone ][ 0 ][ 3 ],  bone_matrix [ bone ][ 1 ][ 3 ],  bone_matrix [ bone ][ 2 ][ 3 ] };

	}

	vec3_t get_hitbox_position ( int hitbox_id, matrix3x4_t bone_matrix [ MAXSTUDIOBONES ] = nullptr ) {

		if ( !this )
			return vec3_t ( );

		if ( !is_alive ( ) )
			return vec3_t ( );

		if ( bone_matrix == nullptr ) {
			if ( !this->m_CachedBoneData ( ).count() )
				return vec3_t ( );
			bone_matrix = new matrix3x4_t [ 128 ];
			memcpy ( bone_matrix, this->m_CachedBoneData ( ).base ( ), this->m_CachedBoneData ( ).count ( ) * sizeof ( matrix3x4_t ) );
		}

		if ( !bone_matrix )
			return vec3_t ( );

		auto studio_model = interfaces::model_info->get_studio_model ( model ( ) );

		if ( studio_model ) {
			auto hitbox = studio_model->hitbox_set ( 0 )->hitbox ( hitbox_id );

			if ( hitbox ) {
			
				const auto mod = hitbox->radius != -1.f ? hitbox->radius : 0.f;
				auto max = math::vector_transform ( hitbox->maxs + mod, bone_matrix [ hitbox->bone ] );
				auto  min = math::vector_transform ( hitbox->mins - mod, bone_matrix [ hitbox->bone ] );

				return vec3_t ( ( min.x + max.x ) * 0.5f, ( min.y + max.y ) * 0.5f, ( min.z + max.z ) * 0.5f );
			}
		}
		delete[] bone_matrix;
		return vec3_t {};
	}
	vec3_t get_hitbox_position ( matrix3x4_t bone_matrix [ MAXSTUDIOBONES ], studio_box_t * hitbox ) {
		if ( !hitbox )
			return vec3_t ( );

		if ( !this )
			return vec3_t ( );

		if ( !is_alive ( ) )
			return vec3_t ( );

		if ( !bone_matrix ) {
			memcpy ( bone_matrix, this->m_CachedBoneData ( ).base ( ), this->m_CachedBoneData ( ).count ( ) * sizeof ( matrix3x4_t ) );
		}

		if ( !bone_matrix )
			return vec3_t ( );

		if ( hitbox ) {

			const auto mod = hitbox->radius != -1.f ? hitbox->radius : 0.f;
			auto max = math::vector_transform ( hitbox->maxs + mod, bone_matrix [ hitbox->bone ] );
			auto min = math::vector_transform ( hitbox->mins - mod, bone_matrix [ hitbox->bone ] );

			return vec3_t ( ( min.x + max.x ) * 0.5f, ( min.y + max.y ) * 0.5f, ( min.z + max.z ) * 0.5f );
		}


		return vec3_t {};
	}
	bool is_alive ( ) {
		if ( !this )
			return false;

		return health() > 0;
	}
	void create_anim_state ( anim_state * state ) {
		using create_anim_state_t = void ( __thiscall * ) ( anim_state *, player_t * );
		static auto create_anim_state = reinterpret_cast< create_anim_state_t > ( utilities::pattern_scan ( "client.dll", "55 8B EC 56 8B F1 B9 ? ? ? ? C7 46" ) );

		if ( !create_anim_state )
			return;

		if ( !state )
			return;

		create_anim_state ( state, this );
	}

	bool is_moving ( ) {
		if ( this->velocity ( ).length ( ) > 0.1f )
			return true;

		return false;
	}

	bool is_in_air ( ) {
		if ( this->flags ( ) & fl_onground )
			return false;

		return true;
	}

	bool is_flashed ( ) {
		if ( this->flash_duration ( ) > 0.0f )
			return true;

		return false;
	}

	void update_client_side_animations ( ) {
		using original_fn = void ( __thiscall * )( void * );
		( *( original_fn ** ) this ) [ 224 ] ( this );
	}
	void set_abs_origin ( const vec3_t & value ) {
		if ( !this )
			return;

		static auto _set_abs_origin = ( void ( __thiscall * )( void *, const vec3_t & ) )( utilities::pattern_scan ( "client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8" ) );

		_set_abs_origin ( this, value );
	}
	void invalidate_bone_cache ( void ) {
		static auto invalidate_bone_bache_fn = utilities::pattern_scan ( "client.dll", "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81" );
		unsigned long g_iModelBoneCounter = **( unsigned long ** ) ( invalidate_bone_bache_fn + 10 );
		*( unsigned int * ) ( ( DWORD ) this + 0x2924 ) = 0xFF7FFFFF; // m_flLastBoneSetupTime = -FLT_MAX;
		*( unsigned int * ) ( ( DWORD ) this + 0x2690 ) = ( g_iModelBoneCounter - 1 ); // m_iMostRecentModelBoneCounter = g_iModelBoneCounter - 1;

		//m_flLastBoneSetupTime ( ) = -FLT_MAX;
		//m_iMostRecentModelBoneCounter ( ) = UINT_MAX;
	}
	uint32_t & m_iMostRecentModelBoneCounter ( ) {
		static auto invalidate_bone_cache = utilities::pattern_scan (  ( "client.dll" ),  ( "80 3D ?? ?? ?? ?? ?? 74 16 A1 ?? ?? ?? ?? 48 C7 81" ) );
		static auto most_recent_model_bone_counter = *( uintptr_t * ) ( invalidate_bone_cache + 0x1B );

		return *( uint32_t * ) ( ( uintptr_t ) this + most_recent_model_bone_counter );
	}
	float & m_flLastBoneSetupTime ( ) {
		static auto invalidate_bone_cache = utilities::pattern_scan (  ( "client.dll" ),  ( "80 3D ?? ?? ?? ?? ?? 74 16 A1 ?? ?? ?? ?? 48 C7 81" ) );
		static auto last_bone_setup_time = *( uintptr_t * ) ( invalidate_bone_cache + 0x11 );

		return *( float * ) ( ( uintptr_t ) this + last_bone_setup_time );
	}

	void set_abs_angles ( vec3_t angles ) {
		if ( !this )
			return;

		

		static auto _set_abs_angles = ( void ( __thiscall * )( void *, const vec3_t & ) )utilities::pattern_scan ( "client.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1" );

		_set_abs_angles ( this, angles );
	}
	bool can_shoot ( bool double_tap ) {
		if ( double_tap )
			return csgo::local_player->can_shoot_time ( math::ticks_to_time ( csgo::local_player->get_tick_base ( ) ) );

		auto weapon = active_weapon ( );

		if ( !weapon || weapon->is_knife ( ) || ( weapon->is_reloading ( ) || !weapon->clip1_count ( ) ) )
			return false;


		if ( weapon->is_revolver ( ) ) {
			auto nci = interfaces::engine->get_net_channel ( );

			if ( !nci )
				return false;

			auto postpone_fire_ready_time = weapon->get_postpone_fire_ready_time ( );

			if ( !postpone_fire_ready_time || postpone_fire_ready_time >= interfaces::globals->cur_time + interfaces::globals->interval_per_tick * nci->choked_packets )
				return false;

			return true;
		}
		else
			return weapon->next_primary_attack ( ) <= get_tick_base ( ) * interfaces::globals->interval_per_tick;

	}
	bool is_firing ( c_usercmd * cmd, float curtime ) {
		const auto weapon = this->active_weapon ( );
		if ( !weapon )
			return false;

		return cmd->buttons & ( in_attack ) && can_shoot_time ( curtime );
	}
	bool can_shoot_time ( float curtime ) {

		auto weapon = active_weapon ( );

		if ( !weapon )
			return false;


		if ( weapon->is_revolver ( ) ) {
			auto nci = interfaces::engine->get_net_channel ( );

			if ( !nci )
				return false;

			auto postpone_fire_ready_time = weapon->get_postpone_fire_ready_time ( );

			if ( !postpone_fire_ready_time || postpone_fire_ready_time >= curtime )
				return false;

			return true;
		}
		else
			return curtime >= weapon->next_primary_attack ( );


	}
	virtual_fn ( pre_think ( void ), 318, void ( __thiscall * )( void * ) ); // 55 8B EC 83 E4 F8 51 56 8B F1 8B 06
	virtual_fn ( think ( void ), 139, void ( __thiscall * )( void * ) ); // 8B C1 8B 50 40
	bool physics_run_think ( int index ) {
		static auto physics_run_think_fn = reinterpret_cast< bool ( __thiscall * )( void *, int ) >( utilities::pattern_scan ( "client.dll", "E8 ? ? ? ? 84 C0 74 0A 8B 06 8B" ) );
		return physics_run_think_fn ( this, index );
	}


	vec3_t & abs_origin ( ) {
		using original_fn = vec3_t & ( __thiscall * )( void * );
		return ( *( original_fn ** ) this ) [ 10 ] ( this );
	}
	vec3_t & abs_angles ( ) {
		using original_fn = vec3_t & ( __thiscall * )( void * );
		return ( *( original_fn ** ) this ) [ 11 ] ( this );;
	}

	void BuildTransformations ( c_studio_hdr * hdr, vec3_t * pos, quaternion_t * q, const matrix3x4_t & transform, int mask, uint8_t * computed ) {
		using BuildTransformations_t = void ( __thiscall * )( decltype( this ), c_studio_hdr *, vec3_t *, quaternion_t *, matrix3x4_t const &, int, uint8_t * );

		return utilities::get_virtual_function< BuildTransformations_t > ( this, 189 )( this, hdr, pos, q, transform, mask, computed );
	}

	void StandardBlendingRules ( c_studio_hdr * hdr, vec3_t * pos, quaternion_t * q, float time, int mask ) {
		using StandardBlendingRules_t = void ( __thiscall * )( decltype( this ), c_studio_hdr *, vec3_t *, quaternion_t *, float, int );

		return utilities::get_virtual_function< StandardBlendingRules_t > ( this, 205 )( this, hdr, pos, q, time, mask );
	}

	int move_type ( ) {
		static int type = netvar_manager::get_net_var ( fnv::hash ( "DT_BaseEntity" ), fnv::hash ( "m_nRenderMode" ) ) + 1;
		return read<int> ( type );
	}

};
