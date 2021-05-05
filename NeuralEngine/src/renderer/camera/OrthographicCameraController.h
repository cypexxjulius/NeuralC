#ifndef __ORTHOGRAPHIC_CAMERA_CONTROLLER_H_
#define __ORTHOGRAPHIC_CAMERA_CONTROLLER_H_

#include "Camera.h"
#include "src/events/event.h"


CameraController* NewOrthographicCameraController(CameraControllerType controlltype);

void CameraControllerOnUpdate(CameraController* this, float deltaTime);

void CameraControllerOnEvent(CameraController* this, const Event* event);


void DeleteOrthographicCameraController(CameraController* this);

#endif //__ORTHOGRAPHIC_CAMERA_CONTROLLER_H_