#ifndef __ORTHO_CAMERA_H_
#define __ORTHO_CAMERA_H_

#include <cglm/types.h>
#include "../../n_types.h"


#include <cglm/cglm.h>
#include <cglm/struct.h>

struct ViewProjection {
    mat4s view, proj;
};

typedef struct OrthographicCamera {
    struct ViewProjection view_proj;
    vec2s position;
} OrthographicCamera;


extern OrthographicCamera* newOrthographicCamera(float left, float right, float top, float bottom);

extern void orthographicCameraSetPosition(OrthographicCamera* this, vec2s pos);

extern vec2s orthographicCameraGetPosition(OrthographicCamera* this);

extern void deleteOrthographicCamera(OrthographicCamera* this);

extern mat4s orthographicCameraGetViewPosMat(OrthographicCamera *this);

#endif // __ORTHO_CAMERA_H_