

#include "../../dependencies/common_includes.hpp"
#include "bone_setup.h"

#include "c_bone_setup.h"
#include "ik_context.hpp"


void BoneSetup::build(player_t* base_animating, matrix3x4a_t* bone_to_world, int max_bones, int bone_mask)
{

    static auto ikAddress = 0x99B0; // utilities::pattern_scan("client.dll", "8B 8F ?? ?? ?? ?? 89 4C 24 1C");
    player_t* player = base_animating - 0x4;

    interfaces::modelcache->BeginLock();
    {
        auto studio_hdr = base_animating->get_model_ptr();

        if (studio_hdr) {
            alignas(16) vec3_t pos[MAXSTUDIOBONES];
            alignas(16) vec4_t q[MAXSTUDIOBONES];
            auto m_pIk = *(CIKContext**)((uintptr_t)base_animating  + ikAddress);

           
            // Allocate our own
            CIKContext* ik = new CIKContext;

            // Backup game ptr and set game to ours
            CIKContext* backup = m_pIk;

            m_pIk = ik;

            uint8_t bone_computed[32]{};
            RtlSecureZeroMemory(&bone_computed, sizeof bone_computed);
            vec3_t angles = player->abs_angles();
            qangle_t anglequat = qangle_t(angles.x, angles.y, angles.z);

            ik->Init(studio_hdr, anglequat, player->abs_origin(), player->simulation_time(), interfaces::globals->tick_count, bone_mask);
            get_skeleton(base_animating, studio_hdr, pos, q, bone_mask, ik);

            ik->UpdateTargets(pos, q, bone_to_world, &bone_computed);

           // base_animating->CalculateIKLocks(base_animating->simulation_time()); // Need to set game's ik ptr for this

            using oCalculateIKLocks = void(__thiscall*)(void*, float);
            call_virtual <oCalculateIKLocks>(base_animating, 193)(base_animating, player->simulation_time());

            ik->SolveDependencies(pos, q, bone_to_world, &bone_computed);

            // reset and cleanup
            m_pIk = backup;
            delete ik;

            // TODO: phage - Implement MoveParent handling
            // Useless in csgo, never seen a single instance of MoveParents
            build_matrices(base_animating, studio_hdr, pos, q, player->m_CachedBoneDataAligned().base(), bone_mask);
        }

        if (bone_to_world && max_bones >= player->m_CachedBoneDataAligned().count()) {
            memcpy(bone_to_world, player->m_CachedBoneDataAligned().base(), sizeof(matrix3x4a_t) * player->m_CachedBoneDataAligned().count());
        }
    }
    interfaces::modelcache->EndLock();
}

uintptr_t& GetBoneMerge(player_t* player)
{
    static auto bone_merge = utilities::pattern_scan(("client.dll"), ("89 86 ?? ?? ?? ?? E8 ?? ?? ?? ?? FF 75 08"));
    return *(uintptr_t*)((uintptr_t)player + *(uintptr_t*)(bone_merge + 0x2));
}

// CBaseAnimatingOverlay::GetSkeleton
void BoneSetup::get_skeleton(player_t* base_animating, c_studio_hdr* studio_hdr, vec3_t* pos, vec4_t* q, int bone_mask, CIKContext* ik)
{
    CBoneSetup boneSetup(studio_hdr, bone_mask, base_animating->m_flPoseParameter().data());
    boneSetup.InitPose(pos, q);
    boneSetup.AccumulatePose(pos, q, base_animating->m_nSequence(), base_animating->get_cycle(), 1.f, base_animating->simulation_time(), ik);

    // sort the layers
    constexpr int MAX_LAYER_COUNT = 15;
    int layer[MAX_LAYER_COUNT]{};
    for (int i = 0; i < base_animating->m_AnimOverlay().Count(); i++) {
        layer[i] = MAX_LAYER_COUNT;
    }

    for (int i = 0; i < base_animating->m_AnimOverlay().Count(); i++) {
        auto pLayer = &base_animating->m_AnimOverlay()[i];
        if ((pLayer->m_flWeight > 0.f) && pLayer->m_nSequence != -1 && pLayer->m_nOrder >= 0 && pLayer->m_nOrder < base_animating->m_AnimOverlay().Count()) {
            layer[pLayer->m_nOrder] = i;
        }
    }

    weapon_t* weapon = nullptr;
    weapon_t* weapon_world_model = nullptr;
    C_BoneMergeCache* bone_merge_cache = nullptr;
    C_BoneMergeCache* bone_merge_backup = nullptr;

    bool do_weapon_setup = false;

    if (base_animating->client_class()->class_id == CCSPlayer) {
        player_t* player = (player_t*)base_animating;
        weapon = player->active_weapon();

        if (weapon) {
            weapon_world_model = weapon->model();

            // Currently just return true in HoldsPlayerAnimations, something fucked on knives and I can't be bothered
            if (weapon_world_model && weapon_world_model->model() && weapon_world_model->HoldsPlayerAnimations()) {
                bone_merge_backup = weapon_world_model->m_pBoneMergeCache();

                bone_merge_cache = new C_BoneMergeCache;
                weapon_world_model->m_pBoneMergeCache() = bone_merge_cache;
                weapon_world_model->m_pBoneMergeCache()->Init(weapon_world_model);

                if (weapon_world_model->m_pBoneMergeCache())
                    do_weapon_setup = true;
            }
        }
    }

    if (do_weapon_setup) {
        c_studio_hdr* weapon_studio_hdr = weapon_world_model->model();

        weapon_world_model->m_pBoneMergeCache()->MergeMatchingPoseParams();

        CIKContext* weaponIK = new CIKContext;
        weaponIK->Init(weapon_studio_hdr, base_animating->abs_angles(), base_animating->abs_origin(), base_animating->simulation_time(), 0, BONE_USED_BY_BONE_MERGE);

        CBoneSetup weaponSetup(weapon_studio_hdr, BONE_USED_BY_BONE_MERGE, weapon_world_model->m_flPoseParameter().data());
        vec3_t weaponPos[256];
        vec4_t weaponQ[256];

        weaponSetup.InitPose(weaponPos, weaponQ);

        for (int i = 0; i < 13; i++) {
            auto* pLayer = &base_animating->anim_layers()[i];
            if (pLayer->m_sequence <= 1 || pLayer->m_weight <= 0.f)
                continue;

            // TODO: phage - This should be moved to the end of animation update,
            // will break shit if we happen to call setupbones more than once per
            // tick on an entity
            // Alternatively, we ensure that the above condition never happens
            base_animating->UpdateDispatchLayer(pLayer, weapon_studio_hdr, pLayer->m_nSequence);

            if (pLayer->m_nDispatchedDst <= 0 || pLayer->m_nDispatchedDst >= studio_hdr->GetNumSeq()) {
                boneSetup.AccumulatePose(pos, q, pLayer->m_nSequence, pLayer->m_flCycle, pLayer->m_flWeight, base_animating->m_flSimulationTime(), ik);
            }
            else {
                weapon_world_model->m_pBoneMergeCache()->CopyFromFollow(pos, q, BONE_USED_BY_BONE_MERGE, weaponPos, weaponQ);

                mstudioseqdesc_t& seqdesc = studio_hdr->GetSeqdesc(pLayer->m_nSequence);
                ik->AddDependencies(seqdesc, pLayer->m_nSequence, pLayer->m_flCycle, base_animating->m_flPoseParameter().data(), pLayer->m_flWeight);

                weaponSetup.AccumulatePose(weaponPos, weaponQ, pLayer->m_nDispatchedDst, pLayer->m_flCycle, pLayer->m_flWeight, base_animating->m_flSimulationTime(), weaponIK);

                weapon_world_model->m_pBoneMergeCache()->CopyToFollow(weaponPos, weaponQ, BONE_USED_BY_BONE_MERGE, pos, q);

                weaponIK->CopyTo(ik, weapon_world_model->m_pBoneMergeCache()->m_iRawIndexMapping);
            }
        }

        delete weaponIK;
    }
    else {
        for (int i = 0; i < base_animating->m_AnimOverlay().Count(); i++) {
            if (layer[i] >= 0 && layer[i] < base_animating->m_AnimOverlay().Count()) {
                CAnimationLayer pLayer = base_animating->m_AnimOverlay()[layer[i]];
                boneSetup.AccumulatePose(pos, q, pLayer.m_nSequence, pLayer.m_flCycle, pLayer.m_flWeight, base_animating->simulation_time(), ik);
            }
        }
    }

    CIKContext* auto_ik = new CIKContext;
    auto_ik->Init(studio_hdr, base_animating->abs_angles(), base_animating->abs_origin(), base_animating->simulation_time(), 0, bone_mask);
    boneSetup.CalcAutoplaySequences(pos, q, base_animating->simulation_time(), auto_ik);
    delete auto_ik;

    if (studio_hdr->m_pStudioHdr->bone_controllers_count > 0) {
        boneSetup.CalcBoneAdj(pos, q, base_animating->m_flEncodedController());
    }

    if (weapon_world_model && bone_merge_cache) {
        weapon_world_model->m_pBoneMergeCache() = bone_merge_backup;
        delete bone_merge_cache;
    }
}

// Left commented out, you can choose to rebuild this or sig the client's, they're identical 
void BoneSetup::update_dispatch_layer(CAnimationLayer* layer, c_studio_hdr* studio_hdr)
{
    //if( !studio_hdr || !layer )
    //{
    //  if( layer )
    //  {
    //    layer->m_nDispatchedDst = ACT_INVALID;
    //  }
    //
    //  return;
    //}
    //
    //if( layer->m_pDispatchedStudioHdr != studio_hdr || layer->m_nDispatchedSrc != layer->m_nSequence || layer->m_nDispatchedDst >= studio_hdr->GetNumSeq() )
    //{
    //  layer->m_pDispatchedStudioHdr = studio_hdr;
    //  layer->m_nDispatchedSrc = layer->m_nSequence;
    //
    //  const char* pszLayerName = GetSequenceName( layer->m_nSequence );
    //  layer->m_nDispatchedDst = studio_hdr->LookupSequence( pszLayerNAme );
    //}
}

void BoneSetup::build_matrices(player_t* base_animating, c_studio_hdr* studio_hdr, vec3_t* pos, vec4_t* q, matrix3x4a_t* bone_to_world, int bone_mask)
{
    int i = 0, j = 0;
    int chain[MAXSTUDIOBONES] = {};
    int chain_length = studio_hdr->m_pStudioHdr->numbones;

    for (i = 0; i < studio_hdr->m_pStudioHdr->numbones; i++) {
        chain[chain_length - i - 1] = i;
    }

    matrix3x4a_t bone_matrix = {};
    matrix3x4a_t rotation_matrix = {};
    AngleMatrix(base_animating->GetAbsAngles(), base_animating->GetAbsOrigin(), rotation_matrix);

    for (j = chain_length - 1; j >= 0; j--) {
        i = chain[j];

        auto bone = studio_hdr->m_pStudioHdr->GetBone(i);
        if (bone->flags & bone_mask) {
            QuaternionMatrix(q[i], pos[i], bone_matrix);

            int bone_parent = bone->parent;
            if (bone_parent == -1) {
                concat_transforms(rotation_matrix, bone_matrix, bone_to_world[i]);
            }
            else {
                concat_transforms(bone_to_world[bone_parent], bone_matrix, bone_to_world[i]);
            }
        }
    }
}

void BoneSetup::concat_transforms(const matrix3x4a_t& m0, const matrix3x4a_t& m1, matrix3x4a_t& out)
{
    for (int i = 0; i < 3; i++) {
        // Normally, you can't just multiply 2 3x4 matrices together, so translation is done separately
        out[i][3] = m1[0][3] * m0[i][0] + m1[1][3] * m0[i][1] + m1[2][3] * m0[i][2] + m0[i][3]; // translation

        for (int j = 0; j < 3; j++) // rotation/scale
        {
            out[i][j] = m0[i][0] * m1[0][j] + m0[i][1] * m1[1][j] + m0[i][2] * m1[2][j];
        }
    }
}