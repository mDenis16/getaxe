#pragma once
#include <cstdint>

using quaternion = float[4];
using rad_euler = float[3];

#define MAX_QPATH  260

#define BONE_CALCULATE_MASK             0x1F
#define BONE_PHYSICALLY_SIMULATED       0x01    // bone is physically simulated when physics are active
#define BONE_PHYSICS_PROCEDURAL         0x02    // procedural when physics is active
#define BONE_ALWAYS_PROCEDURAL          0x04    // bone is always procedurally animated
#define BONE_SCREEN_ALIGN_SPHERE        0x08    // bone aligns to the screen, not constrained in motion.
#define BONE_SCREEN_ALIGN_CYLINDER      0x10    // bone aligns to the screen, constrained by it's own axis.

#define BONE_USED_MASK                  0x0007FF00
#define BONE_USED_BY_ANYTHING           0x0007FF00
#define BONE_USED_BY_HITBOX             0x00000100    // bone (or child) is used by a hit box
#define BONE_USED_BY_ATTACHMENT         0x00000200    // bone (or child) is used by an attachment point
#define BONE_USED_BY_VERTEX_MASK        0x0003FC00
#define BONE_USED_BY_VERTEX_LOD0        0x00000400    // bone (or child) is used by the toplevel model via skinned vertex
#define BONE_USED_BY_VERTEX_LOD1        0x00000800
#define BONE_USED_BY_VERTEX_LOD2        0x00001000
#define BONE_USED_BY_VERTEX_LOD3        0x00002000
#define BONE_USED_BY_VERTEX_LOD4        0x00004000
#define BONE_USED_BY_VERTEX_LOD5        0x00008000
#define BONE_USED_BY_VERTEX_LOD6        0x00010000
#define BONE_USED_BY_VERTEX_LOD7        0x00020000
#define BONE_USED_BY_BONE_MERGE         0x00040000    // bone is available for bone merge to occur against it

#define BONE_USED_BY_VERTEX_AT_LOD(lod) ( BONE_USED_BY_VERTEX_LOD0 << (lod) )
#define BONE_USED_BY_ANYTHING_AT_LOD(lod) ( ( BONE_USED_BY_ANYTHING & ~BONE_USED_BY_VERTEX_MASK ) | BONE_USED_BY_VERTEX_AT_LOD(lod) )

#define MAX_NUM_LODS 8
#define MAXSTUDIOBONES		128		// total bones actually used

#define BONE_TYPE_MASK                  0x00F00000
#define BONE_FIXED_ALIGNMENT            0x00100000    // bone can't spin 360 degrees, all interpolation is normalized around a fixed orientation

#define BONE_HAS_SAVEFRAME_POS          0x00200000    // Vector48
#define BONE_HAS_SAVEFRAME_ROT64        0x00400000    // Quaternion64
#define BONE_HAS_SAVEFRAME_ROT32        0x00800000    // Quaternion32
template <typename T>
struct bit_flag_t {
	bit_flag_t() = default;
	bit_flag_t(const T& value) { m_value = value; }

	__forceinline bool has(const T& value) const { return m_value & value; }

	__forceinline void add(const T& value) { m_value |= value; }

	__forceinline void remove(const T& value) { m_value &= ~value; }

	__forceinline void clear() { m_value = {}; }

	__forceinline bool empty() const { return m_value == std::numeric_limits<T>::quiet_NaN(); }

	__forceinline operator T() const { return m_value; }

	__forceinline bit_flag_t<T>& operator=(const bit_flag_t<T>& value) {
		m_value = value.m_value;

		return *this;
	}

	__forceinline T& operator=(const T& value) {
		m_value = value;

		return m_value;
	}

	T m_value = {};
};
enum bone_flags {
	bone_calculate_mask = 0x1f,
	bone_physically_simulated = 0x01,
	bone_physics_procedural = 0x02,
	bone_always_procedural = 0x04,
	bone_screen_align_sphere = 0x08,
	bone_screen_align_cylinder = 0x10,
	bone_used_mask = 0x0007ff00,
	bone_used_by_anything = 0x0007ff00,
	bone_used_by_hitbox = 0x00000100,
	bone_used_by_attachment = 0x00000200,
	bone_used_by_vertex_mask = 0x0003fc00,
	bone_used_by_vertex_lod0 = 0x00000400,
	bone_used_by_vertex_lod1 = 0x00000800,
	bone_used_by_vertex_lod2 = 0x00001000,
	bone_used_by_vertex_lod3 = 0x00002000,
	bone_used_by_vertex_lod4 = 0x00004000,
	bone_used_by_vertex_lod5 = 0x00008000,
	bone_used_by_vertex_lod6 = 0x00010000,
	bone_used_by_vertex_lod7 = 0x00020000,
	bone_used_by_bone_merge = 0x00040000,
	bone_type_mask = 0x00f00000,
	bone_fixed_alignment = 0x00100000,
	bone_has_saveframe_pos = 0x00200000,
	bone_has_saveframe_rot = 0x00400000
};

enum hitgroups {
	hitgroup_generic = 0,
	hitgroup_head = 1,
	hitgroup_chest = 2,
	hitgroup_stomach = 3,
	hitgroup_leftarm = 4,
	hitgroup_rightarm = 5,
	hitgroup_leftleg = 6,
	hitgroup_rightleg = 7,
	hitgroup_neck = 8,
	hitgroup_gear = 10
};

enum modtypes {
	mod_bad = 0,
	mod_brush,
	mod_sprite,
	mod_studio
};

enum hitboxes {
	hitbox_head,
	hitbox_neck,
	hitbox_pelvis,
	hitbox_body,
	hitbox_thorax,
	hitbox_chest,
	hitbox_upper_chest,
	hitbox_right_thigh,
	hitbox_left_thigh,
	hitbox_right_calf,
	hitbox_left_calf,
	hitbox_right_foot,
	hitbox_left_foot,
	hitbox_right_hand,
	hitbox_left_hand,
	hitbox_right_upper_arm,
	hitbox_right_forearm,
	hitbox_left_upper_arm,
	hitbox_left_forearm,
	hitbox_max
};

struct studio_bone_t {
	int name_index;
	inline char* const name(void) const {
		return ((char*)this) + name_index;
	}
	int parent;
	int bone_controller[6];

	vec3_t pos;
	vec4_t quat;
	qangle_t rotation;

	vec3_t pos_scale;
	vec3_t rot_scale;

	matrix3x4_t pose_to_bone;
	quaternion quat_alignement;
	int flags;
	int proc_type;
	int proc_index;
	mutable int physics_bone;

	inline void* procedure() const {
		if (proc_index == 0) return NULL;
		else return(void*)(((unsigned char*)this) + proc_index);
	};

	int surface_prop_idx;
	inline char* const surface_prop(void) const {
		return ((char*)this) + surface_prop_idx;
	}
	inline int get_surface_prop(void) const {
		return surf_prop_lookup;
	}

	int contents;
	int surf_prop_lookup;
	int unused[7];
};

struct studio_box_t {
	int bone;
	int group;
	vec3_t mins;
	vec3_t maxs;
	int name_index;
	vec3_t	rotation;
	float radius;
	int pad02[4];
};

struct studio_hitbox_set_t {
	int name_index;
	int hitbox_count;
	int hitbox_index;

	inline char* const name(void) const {
		return ((char*)this) + name_index;
	}
	inline studio_box_t* hitbox(int i) const {
		return (studio_box_t*)(((unsigned char*)this) + hitbox_index) + i;
	}
};
struct mstudioposeparamdesc_t {
	int						m_name_index;
	bit_flag_t<uint32_t>	m_flags;
	float					m_start;
	float					m_end;
	float					m_loop;

	__forceinline const char* get_name() const { return reinterpret_cast<const char*>(this) + m_name_index; }
};

struct mstudioactivitymodifier_t {
	int					m_name_index;

	__forceinline const char* get_name() const { return m_name_index ? reinterpret_cast<const char*>(reinterpret_cast<const uint8_t*>(this) + m_name_index) : nullptr; }
};
struct anim_tag_t {
	int		m_index;
	float	m_first_cycle;
	int		m_tag_string_offset_from_this;

	__forceinline const char* get_name() { return reinterpret_cast<const char*>(this) + m_tag_string_offset_from_this; }
};
struct mstudioseqdesc_t {
	char					pad0[4];
	int						m_label_index;
	int						m_activity_name_index;
	bit_flag_t<uint32_t>	m_flags;
	int						m_activity;
	int						m_activity_weight;
	int						m_events_count;
	int						m_event_index;
	vec3_t					m_obb_min;
	vec3_t					m_obb_max;
	int						m_blends_count;
	int						m_anim_index;
	int						m_movement_index;
	int						m_group_size[2];
	int						m_param_index[2];
	float					m_param_start[2];
	float					m_param_end[2];
	int						m_param_parent;
	float					m_fade_in_time;
	float					m_fade_out_time;
	int						m_local_entry_node;
	int						m_local_exit_node;
	bit_flag_t<uint32_t>	m_node_flags;
	float					m_entry_phase;
	float					m_exit_phase;
	float					m_last_frame;
	int						m_next_seq;
	int						m_pose;
	int						m_ik_rules_count;
	int						m_auto_layers_count;
	int						m_auto_layer_index;
	int						m_weight_list_index;
	int						m_pose_key_index;
	int						m_ik_locks_count;
	int						m_ik_lock_index;
	int						m_key_value_index;
	int						m_key_value_size;
	int						m_cycle_pose_index;
	int						m_activity_modifier_index;
	int						m_activity_modifiers_count;
	int						m_anim_tag_index;
	int						m_anim_tags_count;
	char					pad1[12];

	__forceinline const anim_tag_t* get_anim_tag(int i) const { return reinterpret_cast<const anim_tag_t*>(reinterpret_cast<const uint8_t*>(this) + m_anim_tag_index) + i; }

	__forceinline const mstudioactivitymodifier_t* get_activity_modifier(int i) const {
		return m_activity_modifier_index != 0 ? reinterpret_cast<const mstudioactivitymodifier_t*>(reinterpret_cast<const uint8_t*>(this) + m_activity_modifier_index) + i : nullptr;
	}
};
class studio_hdr_t {
public:
	int id;
	int version;
	long checksum;
	char name_char_array[64];
	int length;
	vec3_t eye_pos;
	vec3_t illium_pos;
	vec3_t hull_mins;
	vec3_t hull_maxs;
	vec3_t mins;
	vec3_t maxs;
	int flags;
	int bones_count;
	int bone_index;
	int bone_controllers_count;
	int bone_controller_index;
	int hitbox_sets_count;
	int hitbox_set_index;
	int local_anim_count;
	int local_anim_index;
	int local_seq_count;
	int local_seq_index;
	int activity_list_version;
	int events_indexed;
	int textures_count;
	int texture_index;

	studio_hitbox_set_t* hitbox_set(int i) {
		if (i > hitbox_sets_count) return nullptr;
		return (studio_hitbox_set_t*)((uint8_t*)this + hitbox_set_index) + i;
	}
	studio_bone_t* bone(int i) {
		if (i > bones_count) return nullptr;
		return (studio_bone_t*)((uint8_t*)this + bone_index) + i;
	}
};

class c_studio_hdr {
public:
	class mstudioposeparamdesc_t {
	public:
		int					sznameindex;
		__forceinline char * const name ( void ) const { return ( ( char * ) this ) + sznameindex; }
		int					flags;	// ????
		float				start;	// starting value
		float				end;	// ending value
		float				loop;	// looping range, 0 for no looping, 360 for rotations, etc.
	};

	studio_hdr_t * m_pStudioHdr;
	void * m_pVModel;
};
class c_bone_accessor {
public:
	void * m_pAnimating;
	BoneArray * m_pBones;
	int        m_ReadableBones;
	int        m_WritableBones;
};

class c_bone_cache {
public:
	BoneArray * m_pCachedBones;
	char pad [ 8 ];
	int        m_CachedBoneCount;
};