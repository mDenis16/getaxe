#pragma once
#include "..\classes\entities.hpp"

class CSetupBones
{
public:
	void setup();
	void get_skeleton();
	void studio_build_matrices(c_studio_hdr* hdr, const matrix3x4_t& worldTransform, vec3_t* pos, Quaternion* q, int boneMask, matrix3x4_t* out, uint32_t* boneComputed);
	void attachment_helper();
	void fix_bones_rotations();

	matrix3x4_t* m_boneMatrix = nullptr;
	vec3_t m_vecOrigin = vec3_t(0,0,0);
	vec3_t m_angAngles = vec3_t(0, 0, 0);
	c_studio_hdr* m_pHdr = nullptr;
	vec3_t* m_vecBones = nullptr;
	Quaternion* m_quatBones = nullptr;
	bool m_bShouldDoIK = false;
	bool m_bShouldAttachment = true;
	bool m_bShouldDispatch = true;
	int m_boneMask = 0;
	float m_flPoseParameters[24];
	float m_flWorldPoses[24];
	int m_nAnimOverlayCount = 0;
	animationlayer* m_animLayers = nullptr;
	float m_flCurtime = 0.0f;
	player_t* m_animating = nullptr;
};