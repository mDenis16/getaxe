#pragma once
class CIKContext
{
	// Not sure of the correct size, also don't care
	uint8_t pad[0x1070];

public:
	void* operator new(size_t size);
	void  operator delete(void* ptr);

	static void Construct(CIKContext* ik);
	void Init(const c_studio_hdr* hdr, const qangle_t& local_angles, const vec3_t& local_origin, float current_time, int frame_count, int bone_mask);
	void UpdateTargets(vec3_t* pos, vec4_t* q, matrix3x4_t* bone_cache, void* computed);
	void SolveDependencies(vec3_t* pos, vec4_t* q, matrix3x4_t* bone_cache, void* computed);
	void ClearTargets();
	void AddDependencies(mstudioseqdesc_t& seqdesc, int iSequence, float flCycle, const float poseParameters[], float flWeight);
	void CopyTo(CIKContext* other, const unsigned short* iRemapping);
};