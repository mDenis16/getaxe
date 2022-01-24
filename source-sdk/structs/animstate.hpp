#pragma once
struct animstate_pose_param_cache_t {
	std::uint8_t pad_0x0 [ 0x4 ]; //0x0
	std::uint32_t m_idx; //0x4 
	char * m_name; //0x8

	void set_value ( void * e, float val );
};
struct anim_state {
   /* OFFSET_VFUNC(void(__thiscall*)(decltype(this)), reset(), g_ctx->offsets().m_anim_state.m_reset);

    OFFSET_VFUNC(void(__vectorcall*)(decltype(this), void*, float, float, float, void*),
        update(float pitch, float yaw), g_ctx->offsets().m_anim_state.m_update, nullptr, 0.f, yaw, pitch, nullptr);*/

    std::uint8_t            pad0[4u]{};
    bool                    m_first_update{};
    std::uint8_t            pad1[91u]{};
    player_t* m_player{};
    void* m_weapon{},
        * m_last_weapon{};
    float                   m_last_update_time{};
    int                     m_last_update_frame{};
    float                   m_last_update_increment{},
        m_eye_yaw{},
        m_eye_pitch{},
        m_foot_yaw{},
        m_last_foot_yaw{},
        m_move_yaw{},
        m_move_yaw_ideal{},
        m_move_yaw_cur_to_ideal{},
        m_time_to_align_lower_body{},
        m_primary_cycle{},
        m_move_weight{},
        m_move_weight_smoothed{},
        m_duck_amount{},
        m_duck_additional{},
        m_recrouch_weight{};
    vec3_t             m_origin{},
        m_last_origin{},
        m_velocity{},
        m_velocity_normalized{},
        m_velocity_normalized_non_zero{};
    float                   m_speed_2d{},
        m_up_speed{},
        m_speed_as_portion_of_run_speed{},
        m_speed_as_portion_of_walk_speed{},
        m_speed_as_portion_of_crouch_speed{},
        m_time_since_started_moving{},
        m_time_since_stopped_moving{};
    bool                    m_on_ground{},
        m_landing{};
    std::uint8_t            pad6[6u]{};
    float                   m_time_since_in_air{},
        m_left_ground_height{},
        m_land_anim_multiplier{},
        m_walk_to_run_transition{};
    std::uint8_t            pad7[4u]{};
    float                   m_in_air_smooth_value{};
    bool                    m_on_ladder{};
    std::uint8_t            pad8[47u]{};
    float                   m_velocity_test_time{};
    vec3_t             m_last_velocity{},
        m_dst_acceleration{},
        m_acceleration{};
    float                   m_acceleration_weight{};
    std::uint8_t            pad9[12u]{};
    float                   m_strafe_weight{};
    std::uint8_t            pad10[4u]{};
    float                   m_strafe_cycle{};
    int                     m_strafe_sequence{};
    std::uint8_t            pad11[388u]{};
    float                   m_camera_shooth_height{};
    bool                    m_smooth_height_valid{};
    std::uint8_t            pad12[11u]{};
    float                   m_aim_yaw_min{},
        m_aim_yaw_max{},
        m_aim_pitch_min{},
        m_aim_pitch_max{};
    int                     m_ver{};
};
class animationlayer {
public:
    float			flAnimationTime;		//0x00
    float			flFadeOut;				//0x04
    void* pStudioHdr;				//0x08
    int				nDispatchedSrc;			//0x0C
    int				nDispatchedDst;			//0x10
    int				iOrder;					//0x14
    std::uintptr_t  nSequence;				//0x18
    float			flPrevCycle;			//0x1C
    float			flWeight;				//0x20
    float			flWeightDeltaRate;		//0x24
    float			flPlaybackRate;			//0x28
    float			flCycle;				//0x2C
    void* pOwner;					//0x30
    int				nInvalidatePhysicsBits;	//0x34
};