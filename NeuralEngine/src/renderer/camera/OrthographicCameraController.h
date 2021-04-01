#pragma once

#include "Camera.h"

CameraController* NewOrthographicCameraController(float aspectRatio);


void OrthographicCameraControllerOnUpdate(float timeStep);

