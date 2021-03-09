#ifndef __ORTHO_CAMERA_H_
#define __ORTHO_CAMERA_H_

#include "../../libinclude/cglm.h"

#include "Camera.h"

extern n_Camera* newOrthographicCamera(float left, float right, float top, float bottom);

extern void orthographicCameraSetPosition(n_Camera* this, v2 pos);

extern v2 orthographicCameraGetPosition(n_Camera* this);

extern void deleteOrthographicCamera(n_Camera* this);

extern mat4s orthographicCameraGetViewPosMat(n_Camera *this);

#endif // __ORTHO_CAMERA_H_