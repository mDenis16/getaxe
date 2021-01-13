#pragma once
#include "../math/vector3d.hpp"

struct model_t {
	void* handle;
	char name[260];
	int loadFlags;
	int serverCount;
	int type;
	int flags;
	vec3_t  vecMins;
	vec3_t  vecMaxs;
	float   radius;
};

struct model_render_info_t {
    vec3_t                  origin;
    vec3_t                  angles;
    char pad [ 0x4 ];
    void * pRenderable;
    const model_t * pModel;
    const matrix3x4_t * pModelToWorld;
    const matrix3x4_t * pLightingOffset;
    const vec3_t * pLightingOrigin;
    int                     flags;
    int                     entity_index;
    int                     skin;
    int                     body;
    int                     hitboxset;
    void*   instance;

    model_render_info_t ( ) //-V730
    {
        pModelToWorld = NULL;
        pLightingOffset = NULL;
        pLightingOrigin = NULL;
    }
};