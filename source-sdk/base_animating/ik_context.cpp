#include "../../dependencies/common_includes.hpp"
#include "ik_context.hpp"

void* CIKContext::operator new(size_t size)
{
    CIKContext* ptr = (CIKContext*)interfaces::memalloc->Alloc(size);
    Construct(ptr);

    return ptr;
}

void CIKContext::operator delete(void* ptr)
{
    interfaces::memalloc->Free(ptr);
}

void CIKContext::Construct(CIKContext* ik)
{
    using ConstructFn = CIKContext * (__fastcall*)(CIKContext*);
    static ConstructFn Construct = reinterpret_cast<ConstructFn>(utilities::pattern_scan("client.dll", "53 8B D9 F6 C3 03 74 0B FF 15 ? ? ? ? 84 C0 74 01 CC C7 83 ? ? ? ? ? ? ? ? 8B CB"));
    // e valida // 53 8B D9 F6 C3 03 74 0B FF 15 ?? ?? ?? ?? 84 C0 74 01 CC C7 83 ?? ?? ?? ?? ?? ?? ?? ?? 8B CB

    Construct(ik);
}

void CIKContext::Init(const c_studio_hdr* hdr, const qangle_t& local_angles, const vec3_t& local_origin, float current_time, int frame_count, int bone_mask)
{
    using InitFn = void(__thiscall*)(CIKContext*, const c_studio_hdr*, const qangle_t&, const vec3_t&, float, int, int);
    static InitFn Init = reinterpret_cast<InitFn>(utilities::pattern_scan("client.dll", "55 8B EC 83 EC 08 8B 45 08 56 57 8B F9 8D"));
    // e valida

    Init(this, hdr, local_angles, local_origin, current_time, frame_count, bone_mask);
}

void CIKContext::UpdateTargets(vec3_t* pos, vec4_t* q, matrix3x4_t* bone_cache, void* computed)
{
    using UpdateTargetsFn = void(__thiscall*)(CIKContext*, vec3_t*, vec4_t*, matrix3x4_t*, void*);
    static UpdateTargetsFn UpdateTargets = reinterpret_cast<UpdateTargetsFn>(utilities::pattern_scan("client.dll", "55 8B EC 83 E4 F0 81 EC ? ? ? ? 33 D2 89 4C 24 34"));
    // client.dll - \x55\x8B\xEC\x83\xE4\xF0\x81\xEC\x00\x00\x00\x00\x33\xD2

    UpdateTargets(this, pos, q, bone_cache, computed);
}

void CIKContext::SolveDependencies(vec3_t* pos, vec4_t* q, matrix3x4_t* bone_cache, void* computed)
{
    using SolveDependenciesFn = void(__thiscall*)(CIKContext*, vec3_t*, vec4_t*, matrix3x4_t*, void*);
    static SolveDependenciesFn SolveDependencies = reinterpret_cast<SolveDependenciesFn>(utilities::pattern_scan("client.dll", "55 8B EC 83 E4 F0 81 EC ? ? ? ? 8B 81"));

    // e buna 


    SolveDependencies(this, pos, q, bone_cache, computed);
}

struct CIKTarget {
    int m_iFrameCount;

private:
    char pad_00004[0x51];
};

void CIKContext::ClearTargets()
{
    int m_iTargetCount = *(int*)((uintptr_t)this + 0xFF0);
    auto m_pIkTarget = (CIKTarget*)((uintptr_t)this + 0xD0);
    for (int i = 0; i < m_iTargetCount; i++) {
        m_pIkTarget->m_iFrameCount = -9999;
        m_pIkTarget++;
    }
}

void CIKContext::AddDependencies(mstudioseqdesc_t& seqdesc, int iSequence, float flCycle, const float poseParameters[], float flWeight)
{
    using AddDependenciesFn = void(__thiscall*)(CIKContext*, mstudioseqdesc_t&, int, float, const float[], float);
    AddDependenciesFn AddDependencies = reinterpret_cast<AddDependenciesFn>(utilities::pattern_scan("client.dll", "55 8B EC 81 EC ? ? ? ? 53 56 57 8B F9 0F 28 CB F3 0F 11 4D ?"));
    // e buna

    AddDependencies(this, seqdesc, iSequence, flCycle, poseParameters, flWeight);
}

void CIKContext::CopyTo(CIKContext* other, const unsigned short* iRemapping)
{
    using CopyToFn = void(__thiscall*)(CIKContext*, CIKContext*, const unsigned short*);
    CopyToFn CopyTo = reinterpret_cast<CopyToFn>(utilities::pattern_scan("client.dll", "55 8B EC 83 EC 24 8B 45 08 57 8B F9 89 7D F4 85 C0"));
    // server.dll - \x55\x8B\xEC\x83\xEC\x24\x8B\x45\x08\x57\x8B\xF9\x89\x7D\xF4\x85\xC0

    CopyTo(this, other, iRemapping);
}