#pragma once
struct CBoneSetup {
    CBoneSetup(const c_studio_hdr* studio_hdr, int bone_mask, float* pose_parameters);

    void InitPose(vec3_t pos[], vec4_t q[]);
    void AccumulatePose(vec3_t pos[], vec4_t q[], int sequence, float cycle, float weight, float time, void* IKContext);
    void CalcAutoplaySequences(vec3_t pos[], vec4_t q[], float real_time, void* IKContext);
    void CalcBoneAdj(vec3_t pos[], vec4_t q[], const float controllers[]);

    const c_studio_hdr* m_pStudioHdr;
    int m_boneMask;
    float* m_flPoseParameter;
    void* m_pPoseDebugger;
};

struct IBoneSetup {
    CBoneSetup* bone_setup;
};