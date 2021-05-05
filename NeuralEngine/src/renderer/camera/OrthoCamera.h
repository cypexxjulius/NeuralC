#ifndef __ORTHO_CAMERA_H_
#define __ORTHO_CAMERA_H_

#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/struct.h>

#include "Camera.h"

extern Camera* NewOrthographicCamera(float left, float right, float top, float bottom);

extern void DeleteOrthographicCamera(Camera* this);

extern mat4s OrthographicCameraGetViewProjMat(Camera *this);

extern void OrthographicCameraSetProjection(Camera* this, float left, float right, float top, float bottom);

extern void OrthographicCameraSetPosition(Camera* this, v2 newPos);

extern void OrthgraphicCameraAddVector(Camera* this, v2 vector);


#endif // __ORTHO_CAMERA_H_