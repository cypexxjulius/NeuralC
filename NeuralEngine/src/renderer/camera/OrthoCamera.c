#include "OrthoCamera.h"

#include "src/platform/memory.h"
#include "src/core/error.h"

#include <string.h>

static inline void orthographicCameraRecalculate(Camera *this)
{
    Assert(this->camType != OrthographicCameraType, "Wrong camera type"); 
    this->orthoCam.view_proj.view = glms_mat4_inv_fast(glms_translate(glms_mat4_identity(), (vec3s) {{ -this->orthoCam.position.x, -this->orthoCam.position.y, 0 }}));
}

extern Camera* newOrthographicCamera(float left, float right, float top, float bottom)
{
    Camera* this = MemCalloc(1, sizeof(Camera));

    this->camType = OrthographicCameraType;

    this->orthoCam.view_proj.proj = glms_ortho(left, right, top, bottom, -100.0f, 100.0f);
    orthographicCameraRecalculate(this);

    this->orthoCam.height = GetUnsignedFloat(top) + GetUnsignedFloat(bottom);
    this->orthoCam.width  = GetUnsignedFloat(left) + GetUnsignedFloat(right);

    return this;
} 

extern void orthographicCameraSetPosition(Camera* this, v2 pos)
{

    Assert(this->camType != OrthographicCameraType, "Wrong camera type");  
    if(pos.x == this->orthoCam.position.x && pos.y == this->orthoCam.position.y)
    {
        return;
    }
    
    this->orthoCam.position = pos;
    orthographicCameraRecalculate(this);
}

extern v2 orthographicCameraGetPosition(Camera* this)
{
    Assert(this->camType != OrthographicCameraType, "Wrong camera type");  
    return this->orthoCam.position;
}


extern mat4s orthographicCameraGetViewPosMat(Camera *this)
{

    Assert(this->camType != OrthographicCameraType, "Wrong camera type");
    return glms_mat4_mul(this->orthoCam.view_proj.proj, this->orthoCam.view_proj.view);
}

extern void deleteOrthographicCamera(Camera* this)
{
    Assert(this->camType != OrthographicCameraType, "Wrong camera type");  
    MemFree(this);
}
