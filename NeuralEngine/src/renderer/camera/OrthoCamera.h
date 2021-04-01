#pragma once

#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/struct.h>

#include "Camera.h"

extern Camera* NewOrthographicCamera(float left, float right, float top, float bottom);

extern void orthographicCameraSetPosition(Camera* this, v2 pos);

extern v2 orthographicCameraGetPosition(Camera* this);

extern void DeleteOrthographicCamera(Camera* this);

extern mat4s orthographicCameraGetViewPosMat(Camera *this);
