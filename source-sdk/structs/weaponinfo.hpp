#pragma once

class weapon_info_t {
public:
	/*int8_t pad0 [ 20 ];
	int32_t iMaxClip1;
	int8_t pad1 [ 12 ];
	int32_t iMaxReservedAmmo;
	int8_t pad2 [ 96 ];
	char* szHudName;
	char* szWeaponName;
	int8_t pad3 [ 56 ];
	int32_t iWeaponType;
	int8_t pad4 [ 4 ];
	int32_t iWeaponPrice;
	int32_t iKillAward;
	int8_t pad5 [ 20 ];
	uint8_t bFullAuto;
	int8_t pad6 [ 3 ];
	int32_t iDamage;
	float_t flArmorRatio;
	int32_t iBullets;
	float_t flPenetration;
	int8_t pad7 [ 8 ];
	float_t flRange;
	float_t flRangeModifier;
	int8_t pad8 [ 16 ];
	uint8_t bHasSilencer;
	char		pad_27 [ 0xb ];			// 0x121
	char* szBulletType;			// 0x12c
	float_t		flMaxSpeed;				// 0x130
	float_t		flMaxSpeedAlt;			// 0x134
	float_t flSpread;
	float_t flSpreadAlt;
	int8_t pad10 [ 76 ];
	int32_t iRecoilSeed;
	int8_t pad11 [ 32 ];*/
	virtual ~weapon_info_t ( ) = default;

	char padBase[4];				// 0x0000
	char* consoleName;				// 0x0004
	char pad0[12];					// 0x0008
	int iMaxClip1;					// 0x0014
	int iMaxClip2;					// 0x0018
	int iDefaultClip1;				// 0x001C
	int iDefaultClip2;				// 0x0020
	int iPrimaryMaxReserveAmmo;		// 0x0024
	int iSecondaryMaxReserveAmmo;	// 0x0028
	const char* szWorldModel;		// 0x002C
	const char* szViewModel;		// 0x0030
	const char* szDroppedModel;		// 0x0034
	char pad1[0x50];				// 0x0038
	const char* szHudName;			// 0x0088
	const char* szWeaponName;		// 0x008C
	char pad2[0x2];					// 0x0090
	bool bIsMeleeWeapon;			// 0x0092
	char pad3[0x9];					// 0x0093
	float flWeaponWeight;			// 0x009C
	char pad4[0x4];					// 0x00A0
	int iSlot;						// 0x00A4
	int iPosition;					// 0x00A8
	char pad5[0x1C];				// 0x00AC
	int32_t iWeaponType;			// 0x00C8
	char pad6[0x4];					// 0x00CC
	int iWeaponPrice;				// 0x00D0
	int iKillAward;					// 0x00D4
	const char* szAnimationPrefix;	// 0x00D8
	float flCycleTime;				// 0x00DC
	float flCycleTimeAlt;			// 0x00E0
	char pad8[0x8];					// 0x00E4
	bool bFullAuto;					// 0x00EC
	char pad9[0x3];					// 0x00ED
	int iDamage;					// 0x00F0
	float flArmorRatio;				// 0x00F4
	int iBullets;					// 0x00F8
	float flPenetration;			// 0x00FC
	char pad10[0xC];				// 0x0100
	float flRange;					// 0x010C
	float flRangeModifier;			// 0x0110
	float flThrowVelocity;			// 0x0114
	char pad11[0xC];				// 0x0118

	bool bHasSilencer;				// 0x0124
	char pad12[0xF];				// 0x0125
	float flMaxSpeed[2];			// 0x0134
	char pad13[0x4];				// 0x013C
	float flSpread[2];				// 0x0140
	float flInaccuracyCrouch[2];	// 0x0148
	float flInaccuracyStand[2];		// 0x0150
	char pad14[0x8];				// 0x0158
	float flInaccuracyJump[2];		// 0x0160
	float flInaccuracyLand[2];		// 0x0168
	float flInaccuracyLadder[2];	// 0x0170
	float flInaccuracyFire[2];		// 0x0178
	float flInaccuracyMove[2];		// 0x0180
	float flInaccuracyReload;		// 0x0188
	int iRecoilSeed;				// 0x018C
	float flRecoilAngle[2];			// 0x0190
	float flRecoilAngleVariance[2];	// 0x0198
	float flRecoilMagnitude[2];		// 0x01A0
	float flRecoilMagnitudeVariance[2]; // 0x01A8
	int iSpreadSeed;				// 0x01B0
};