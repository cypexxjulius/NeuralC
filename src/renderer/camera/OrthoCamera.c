#include "OrthoCamera.h"

#include "src/platform/memory.h"
#include "src/core/error.h"

#include <string.h>

static inline void orthographicCameraRecalculate(n_Camera *this)
{
    ASSERT(this->camType == OrthographicCameraType, "Wrong camera type");   
    this->orthoCam.view_proj.view = glms_mat4_inv_fast(glms_translate(glms_mat4_identity(), (vec3s) {{ -this->orthoCam.position.x, -this->orthoCam.position.y, 0 }}));
}

extern n_Camera* newOrthographicCamera(float left, float right, float top, float bottom)
{
    n_Camera* this = nl_malloc(sizeof(n_Camera));
    memset(this, 0, sizeof(struct n_Camera));

    this->camType = OrthographicCameraType;

    this->orthoCam.view_proj.proj = glms_ortho(left, right, top, bottom, -100.0f, 100.0f);
    orthographicCameraRecalculate(this);
    return this;
} 

extern void orthographicCameraSetPosition(n_Camera* this, v2 pos)
{

    ASSERT(this->camType == OrthographicCameraType, "Wrong camera type");  
    if(pos.x == this->orthoCam.position.x && pos.y == this->orthoCam.position.y)
    {
        return 0;
    }
    
    this->orthoCam.position = pos;
    orthographicCameraRecalculate(this);
}

extern v2 orthographicCameraGetPosition(n_Camera* this)
{
    ASSERT(this->camType == OrthographicCameraType, "Wrong camera type");  
    return this->orthoCam.position;
}


extern mat4s orthographicCameraGetViewPosMat(n_Camera *this)
{

    ASSERT(this->camType == OrthographicCameraType, "Wrong camera type");
    return glms_mat4_mul(this->orthoCam.view_proj.view, this->orthoCam.view_proj.proj);
}

extern void deleteOrthographicCamera(n_Camera* this)
{
    ASSERT(this->camType == OrthographicCameraType, "Wrong camera type");  
    nl_free(this);
}
