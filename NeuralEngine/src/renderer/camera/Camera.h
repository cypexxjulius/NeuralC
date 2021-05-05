#ifndef __CAMERA_H_
#define __CAMERA_H_

#include "src/utils/types.h"

struct ViewProjection {
    mat4s view, proj;
};

struct OrthographicCamera {
    struct ViewProjection view_proj;
    float width;
    float height;

    v2 position;
    float rotation;
};


enum CamType
{ 
    OrthographicCameraType = 2,
    PerspectiveCameraType = 3
};

typedef struct Camera
{
    enum CamType camType;

    union 
    {
        struct OrthographicCamera orthoCam;
    };
} Camera;


typedef enum CameraControllerType
{
    CameraNoControls = 0,
    CameraKeyboardController = 0b01,
    CameraMouseDragController = 0b10,
    CameraMouseScrollSensitive = 0b100,
    CameraAllControls = 0b1111111

} CameraControllerType;

struct OrthographicCameraControllerAssets
{
    float aspectRatio;
    float zoomLevel;

    CameraControllerType controllerType;

    v2 mouseMovedDelta;

    float cameraTranslationSpeed;
    float cameraRotationSpeed;
};

typedef struct CameraController 
{   
    Camera *camera;
    union 
    {
        struct OrthographicCameraControllerAssets OrthoCamAssets;
    };
} CameraController;

#include "OrthoCamera.h"
#endif // __CAMERA_H_