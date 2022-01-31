#pragma once
struct animstate_pose_param_cache_t {
	std::uint8_t pad_0x0 [ 0x4 ]; //0x0
	std::uint32_t m_idx; //0x4 
	char * m_name; //0x8

	void set_value ( void * e, float val );
};
struct anim_state {
    char pad[3];
    char bUnknown; //0x4
    char pad2[87];
    void* m_pLastBoneSetupWeapon; //0x5C
    player_t* m_pBaseEntity; //0x60
    void* m_pActiveWeapon; //0x64
    void* m_pLastActiveWeapon; //0x68
    float m_flLastClientSideAnimationUpdateTime; //0x6C
    int m_iLastClientSideAnimationUpdateFramecount; //0x70
    float m_flUpdateTimeDelta; //0x74
    float m_flEyeYaw; //0x78
    float m_flPitch; //0x7C
    float m_flGoalFeetYaw; //0x80
    float m_flCurrentFeetYaw; //0x84
    float m_flCurrentTorsoYaw; //0x88
    float m_flUnknownVelocityLean; //0x8C //changes when moving/jumping/hitting ground
    float m_flLeanAmount; //0x90
    char pad4[4]; //NaN
    float m_flFeetCycle; //0x98 0 to 1
    float m_flFeetYawRate; //0x9C 0 to 1
    float m_fUnknown2;
    float m_fDuckAmount; //0xA4
    float m_fLandingDuckAdditiveSomething; //0xA8
    float m_fUnknown3; //0xAC
    vec3_t m_vOrigin; //0xB0, 0xB4, 0xB8
    vec3_t m_vLastOrigin; //0xBC, 0xC0, 0xC4
    float m_vVelocityX; //0xC8
    float m_vVelocityY; //0xCC
    char pad5[4];
    float m_flUnknownFloat1; //0xD4 Affected by movement and direction
    char pad6[8];
    float m_flUnknownFloat2; //0xE0 //from -1 to 1 when moving and affected by direction
    float m_flUnknownFloat3; //0xE4 //from -1 to 1 when moving and affected by direction
    float m_unknown; //0xE8
    float m_velocity; //0xEC
    float flUpVelocity; //0xF0
    float m_flSpeedNormalized; //0xF4 //from 0 to 1
    float m_flFeetSpeedForwardsOrSideWays; //0xF8 //from 0 to 2. something  is 1 when walking, 2.something when running, 0.653 when crouch walking
    float m_flFeetSpeedUnknownForwardOrSideways; //0xFC //from 0 to 3. something
    float m_flTimeSinceStartedMoving; //0x100
    float m_flTimeSinceStoppedMoving; //0x104
    bool m_bOnGround; //0x108
    bool m_bInHitGroundAnimation; //0x109
    char pad7[10];
    float m_flLastOriginZ; //0x114
    float m_flHeadHeightOrOffsetFromHittingGroundAnimation; //0x118 from 0 to 1, is 1 when standing
    float m_flStopToFullRunningFraction; //0x11C from 0 to 1, doesnt change when walking or crouching, only running
    char pad8[4]; //NaN
    float m_flMovingFraction; //0x124 affected while jumping and running, or when just jumping, 0 to 1
    char pad9[4]; //NaN
    float m_flUnknown3;
    char pad10[528];

    float& time_since_in_air()
    {
        return *(float*)((uintptr_t)this + 0x110);
    }

    float& yaw_desync_adjustment()
    {
        return *(float*)((uintptr_t)this + 0x334); //fixed shonax
    }
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