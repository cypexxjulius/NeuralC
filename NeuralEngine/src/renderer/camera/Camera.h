#ifndef __CAMERA_H_
#define __CAMERA_H_


#include "src/utils/types.h"

struct ViewProjection {
    mat4s view, proj;
};

struct OrthographicCamera {
    struct ViewProjection view_proj;
    v2 position;
    float width;
    float height;
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

#include "OrthoCamera.h"


#endif // !__CAMERA_H_