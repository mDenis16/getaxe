#include "../../dependencies/common_includes.hpp"
#include "c_bone_setup.h"

CBoneSetup::CBoneSetup(const c_studio_hdr* studio_hdr, int bone_mask, float* pose_parameters)
    : m_pStudioHdr(studio_hdr)
    , m_boneMask(bone_mask)
    , m_flPoseParameter(pose_parameters)
    , m_pPoseDebugger(nullptr)
{
}

void CBoneSetup::InitPose(vec3_t pos[], vec4_t q[])
{
    auto hdr = m_pStudioHdr->m_pStudioHdr;

    for (int i = 0; i < hdr->bones_count; i++) {
        auto bone = hdr->bone(i);

        if (bone->flags & m_boneMask) {
            pos[i] = bone->pos;
            q[i] = bone->quat;
        }
    }
}

void CBoneSetup::AccumulatePose(vec3_t pos[], vec4_t q[], int sequence, float cycle, float weight, float time, void* IKContext)
{
    using AccumulatePoseFn = void(__thiscall*)(CBoneSetup*, vec3_t*, vec4_t*, int, float, float, float, void*);
    AccumulatePoseFn AccumulatePose = reinterpret_cast<AccumulatePoseFn>(utilities::pattern_scan("client.dll", "55 8B EC 83 E4 F0 B8 ? ? ? ? E8 ? ? ? ? A1"));
	// client.dll - \x55\x8B\xEC\x83\xE4\xF0\xB8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xA1

    AccumulatePose(this, pos, q, sequence, cycle, weight, time, IKContext);
}

void CBoneSetup::CalcAutoplaySequences(vec3_t pos[], vec4_t q[], float real_time, void* IKContext)
{
    using CalcAutoplaySequencesFn = void(__thiscall*)(CBoneSetup*, vec3_t*, vec4_t*, float, void*);
    CalcAutoplaySequencesFn CalcAutoplaySequences = reinterpret_cast<CalcAutoplaySequencesFn>(utilities::pattern_scan("client.dll", "55 8B EC 83 EC 10 53 56 57 8B 7D 10"));

	// client.dll - \x55\x8B\xEC\x83\xEC\x10\x53\x56\x57\x8B\x7D\x10

    // Thanks clang!
    __asm
    {
        mov ecx, this
        movss xmm0, real_time
        push IKContext
        push q
        push pos
        call CalcAutoplaySequences
    }
}

void CBoneSetup::CalcBoneAdj(vec3_t pos[], vec4_t q[], const float controllers[])
{
    using CalcBoneAdjFn = void(__thiscall*)(CBoneSetup*, const c_studio_hdr*, vec3_t*, vec4_t*, const float*, int);
    CalcBoneAdjFn CalcBoneAdj = reinterpret_cast<CalcBoneAdjFn>(utilities::pattern_scan("client.dll", "55 8B EC 83 E4 F8 81 EC ? ? ? ? 8B C1 89"));
      //  offsets->boneSetup.m_calcBoneAdj; // client.dll - \x55\x8B\xEC\x83\xE4\xF8\x81\xEC\x00\x00\x00\x00\x8B\xC1\x89

    // Thanks clang!
    __asm
    {
        mov ecx, m_pStudioHdr
        mov edx, this
        push m_boneMask
        push controllers
        push q
        push pos
        call CalcBoneAdj
    }
}