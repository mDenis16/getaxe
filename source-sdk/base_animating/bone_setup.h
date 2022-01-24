#pragma once
class CIKContext;

class BoneSetup {
public:
    void build(player_t* base_animating, matrix3x4a_t* bone_to_world, int max_bones, int bone_mask);

private:
    void get_skeleton(player_t* base_animating, c_studio_hdr* studio_hdr, vec3_t* pos, vec4_t* q, int bone_mask, CIKContext*);
    void build_matrices(player_t* base_animating, c_studio_hdr* studio_hdr, vec3_t* pos, vec4_t* q, matrix3x4a_t* bone_to_world, int bone_mask);
    void concat_transforms(const matrix3x4a_t& m0, const matrix3x4a_t& m1, matrix3x4a_t& out);
};
