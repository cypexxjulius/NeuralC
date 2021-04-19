#include "OrthoCamera.h"

#include "src/platform/memory.h"
#include "src/core/error.h"

#include <string.h>

static inline void orthographicCameraRecalculate(Camera *this)
{
    Assert(this->camType != OrthographicCameraType, "Wrong camera type"); 
    this->orthoCam.view_proj.view = glms_mat4_inv_fast(glms_translate(glms_mat4_identity(), (vec3s) {{ -this->orthoCam.position.x, -this->orthoCam.position.y, 0 }}));
}

extern Camera* NewOrthographicCamera(float left, float right, float bottom, float top)
{
    Camera* this = CreateObject(Camera);

    this->camType = OrthographicCameraType;

    this->orthoCam.view_proj.proj = glms_ortho(left, right, bottom, top, -1.0f, 1.0f);
    orthographicCameraRecalculate(this);

    this->orthoCam.height = GetUnsignedFloat(top) + GetUnsignedFloat(bottom);
    this->orthoCam.width  = GetUnsignedFloat(left) + GetUnsignedFloat(right);

    return this;
} 

extern void orthographicCameraSetProjection(Camera* this, float left, float right, float bottom, float top)
{
    Assert(this->camType != OrthographicCameraType, "Wrong camera type");
    this->orthoCam.view_proj.proj = glms_ortho(left, right, bottom, top, -1.0f, 1.0f);
}


extern mat4s orthographicCameraGetViewProjMat(Camera *this)
{
    Assert(this->camType != OrthographicCameraType, "Wrong camera type");
    return glms_mat4_mul(this->orthoCam.view_proj.proj, this->orthoCam.view_proj.view);
}


extern void orthgraphicCameraAddVector(Camera* this, v2 vector)
{
    Assert(this->camType != OrthographicCameraType, "Wrong camera type");
    this->orthoCam.position.y += vector.y;
    this->orthoCam.position.x += vector.x;
    orthographicCameraRecalculate(this);
}
extern void orthographicCameraSetPosition(Camera* this, v2 newPos)
{
    Assert(this->camType != OrthographicCameraType, "Wrong camera type");
    this->orthoCam.position = newPos;
    orthographicCameraRecalculate(this);
}
extern void DeleteOrthographicCamera(Camera* this)
{
    Assert(this->camType != OrthographicCameraType, "Wrong camera type");  
    Memory.Free(this);
}
