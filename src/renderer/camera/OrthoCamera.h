#ifndef __ORTHO_CAMERA_H_
#define __ORTHO_CAMERA_H_

#include <cglm/types.h>
#include "../../n_types.h"


#include <cglm/cglm.h>
#include <cglm/struct.h>

#include "Camera.h"

extern n_Camera* newOrthographicCamera(float left, float right, float top, float bottom);

extern void orthographicCameraSetPosition(n_Camera* this, vec2s pos);

extern vec2s orthographicCameraGetPosition(n_Camera* this);

extern void deleteOrthographicCamera(n_Camera* this);

extern mat4s orthographicCameraGetViewPosMat(n_Camera *this);

#endif // __ORTHO_CAMERA_H_