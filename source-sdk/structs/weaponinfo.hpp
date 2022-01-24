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
	void* vtable;		//0x0000
	char* szConsoleName; //0x0004
	char pad_0008[8]; //0x0008
	void* pBaseItemDefinition; //0x0010
	int32_t iMaxClip1; //0x0014
	int32_t iMaxClip2; //0x0018
	int32_t iDefaultClip1; //0x001C
	int32_t iDefaultClip2; //0x0020
	int32_t iMaxReservedAmmo; //0x0024
	char pad_0028[4]; //0x0028
	char* szWorldModel; //0x002C
	char* szViewModel; //0x0030
	char* szDroppedModel; //0x0034
	char* szEmptySound; //0x0038
	char* szShotSound; //0x003C
	char pad_0040[20]; //0x0040
	char* szReloadSound; //0x0054
	char pad_0058[16]; //0x0058
	char* szShotSoundSpecial; //0x0068
	char pad_006C[12]; //0x006C
	char* szNearlyEmptySound; //0x0078
	char pad_007C[4]; //0x007C
	char* szBulletType; //0x0080
	char pad_0084[4]; //0x0084
	char* szHudName; //0x0088
	char* szWeaponName; //0x008C
	char pad_0090[12]; //0x0090
	int32_t iWeaponWeight; //0x009C
	int32_t iRumbleEffect; //0x00A0
	char pad_00A4[32]; //0x00A4
	void* pItemDefinition; //0x00C4
	int32_t iWeaponType; //0x00C8
	int32_t WeaponGroup; //0x00CC
	int32_t iWeaponPrice; //0x00D0
	int32_t iKillAward; //0x00D4
	char* szPlayerAnimationExtension; //0x00D8
	float flCycleTime; //0x00DC
	float flCycleTimeAlt; //0x00E0
	float flTimeToIdle; //0x00E4
	float flIdleInterval; //0x00E8
	bool bFullAuto; //0x00EC
	char pad_00ED[3]; //0x00ED
	int32_t iDamage; //0x00F0
	float fHeadshotMultiplier; //0x00F4
	float flArmorRatio; //0x00F8
	int32_t iBullets; //0x00FC
	float flPenetration; //0x0100
	float flFlinchVelocityModifierLarge; //0x0104
	float flFlinchVelocityModifierSmall; //0x0108
	float flRange; //0x010C
	float flRangeModifier; //0x0110
	float flThrowVelocity; //0x0114
	char pad_0118[12]; //0x0118
	bool bHasSilencer; //0x0124
	char pad_0125[3]; //0x0125
	char* szSilencerModel; //0x0128
	int32_t iCrosshairMinDistance; //0x012C
	int32_t iCrosshairDeltaDistance; //0x0130
	float flMaxPlayerSpeed; //0x0134
	float flMaxPlayerSpeedAlt; //0x0138
	int32_t flAttackMoveSpeedFactor; //0x013C
	float flSpread; //0x0140
	float flSpreadAlt; //0x0144
	float flInaccuracyCrouch; //0x0148
	float flInaccuracyCrouchAlt; //0x014C
	float flInaccuracyStand; //0x0150
	float flInaccuracyStandAlt; //0x0154
	float flInaccuracyJumpInitial; //0x0158
	char pad_015C[4]; //0x015C
	float flInaccuracyJump; //0x0160
	float flInaccuracyJumpAlt; //0x0164
	float flInaccuracyLand; //0x0168
	float flInaccuracyLandAlt; //0x016C
	float flInaccuracyLadder; //0x0170
	float flInaccuracyLadderAlt; //0x0174
	float flInaccuracyFire; //0x0178
	float flInaccuracyFireAlt; //0x017C
	float flInaccuracyMove; //0x0180
	float flInaccuracyMoveAlt; //0x0184
	float flInaccuracyReload; //0x0188
	int32_t iRecoilSeed; //0x018C
	float flRecoilAngle; //0x0190
	float flRecoilAngleAlt; //0x0194
	float flRecoilAngleVariance; //0x0198
	float flRecoilAngleVarianceAlt; //0x019C
	float flRecoilMagnitude; //0x01A0
	float flRecoilMagnitudeAlt; //0x01A4
	float flRecoilMagnitudeVariance; //0x01A8
	float flRecoilMagnitudeVarianceAlt; //0x01AC
	int32_t iSpreadSeed; //0x01B0
	float flRecoveryTimeCrouch; //0x01B4
	float flRecoveryTimeStand; //0x01B8
	float flRecoveryTimeCrouchFinal; //0x01BC
	float flRecoveryTimeStandFinal; //0x01C0
	int32_t iRecoveryTransitionStartBullet; //0x01C4
	int32_t iRecoveryTransitionEndBullet; //0x01C8
	bool bUnzoomAfterShot; //0x01CC
	bool bHideViewModelZoomed; //0x01CD
	char pad_01CE[2]; //0x01CE
	int32_t iZoomLevels; //0x01D0
	int32_t iZoomFOVs[2]; //0x01D4
	float flZoomTimes[3]; //0x01DC
	char* szAddonLocation; //0x01E8
	char pad_01EC[4]; //0x01EC
	float flAddonScale; //0x01F0
	char* szEjectBrassEffect; //0x01F4
	char* szTracerEffect; //0x01F8
	int32_t iTracerFrequency; //0x01FC
	int32_t iTracerFrequencyAlt; //0x0200
	char* szFlashEffectFirstPerson; //0x0204
	char pad_0208[4]; //0x0208
	char* szFlashEffectThirdPerson; //0x020C
	char pad_0210[4]; //0x0210
	char* szHeatEffect; //0x0214
	float flHeatPerShot; //0x0218
	char* szZoomInSound; //0x021C
	char* szZoomOutSound; //0x0220
	float flInaccuracyPitchShift; //0x0224
	float flInaccuracySoundThreshold; //0x0228
	float flBotAudibleRange; //0x022C
	void* pPaintData; //0x0230
	char pad_0234[4]; //0x0234
	char* szWrongTeamMsg; //0x0238
	bool bHasBurstMode; //0x023C
	bool bIsRevolver; //0x023D
	bool bUnknown01; //0x023E
	bool bUnknown02; //0x023F
};