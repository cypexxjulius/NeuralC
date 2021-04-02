#ifndef __ORTHOGRAPHIC_CAMERA_CONTROLLER_H_
#define __ORTHOGRAPHIC_CAMERA_CONTROLLER_H_

#include "Camera.h"

CameraController* NewOrthographicCameraController(float aspectRatio);


void OrthographicCameraControllerOnUpdate(float timeStep);

#endif //__ORTHOGRAPHIC_CAMERA_CONTROLLER_H_