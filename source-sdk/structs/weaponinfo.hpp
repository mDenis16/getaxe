#pragma once

class weapon_info_t {
public:
	int8_t pad0 [ 20 ];
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
	int8_t pad11 [ 32 ];
};