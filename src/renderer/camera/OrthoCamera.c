#include "OrthoCamera.h"

#include "../../platform/memory.h"
#include <cglm/mat4.h>
#include <cglm/cam.h>

#include <string.h>

static inline void OrthographicCameraRecalculate(OrthographicCamera *this)
{
    this->view_proj.view = glms_mat4_inv_fast(glms_translate(glms_mat4_identity(), (vec3s) {{ -this->position.x, -this->position.y, 0 }}));
}

extern OrthographicCamera* newOrthographicCamera(float left, float right, float top, float bottom)
{
    OrthographicCamera* this = nl_malloc(sizeof(OrthographicCamera));

    memset(this, 0, sizeof(struct OrthographicCamera));
    this->view_proj.proj = glms_ortho(left, right, top, bottom, -100.0f, 100.0f);
    OrthographicCameraRecalculate(this);
    return this;
} 

extern void orthographicCameraSetPosition(OrthographicCamera* this, vec2s pos)
{
    this->position = pos;
    OrthographicCameraRecalculate(this);
}

extern vec2s orthographicCameraGetPosition(OrthographicCamera* this)
{
    return this->position;
}

extern mat4s orthographicCameraGetViewPosMat(OrthographicCamera *this)
{
    return glms_mat4_mul(this->view_proj.view, this->view_proj.proj);
}

extern void deleteOrthographicCamera(OrthographicCamera* this)
{
    nl_free(this);
}
