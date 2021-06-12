#include "OrthographicCameraController.h"
#include "Camera.h"
#include "src/events/keycode.h"
#include "src/core/Application.h"
#include "src/utils/types.h"
#include "src/core/Input.h"
#include "src/utils/Logger.h"

#define ZoomIntensity 2.0f


CameraController* NewOrthographicCameraController(CameraControllerType controllertype)
{
    CameraController* this = CreateObject(CameraController);


    v2 WindowSize = GetWindowSize();
    float aspectRatio = WindowSize.width / WindowSize.height;
    

    this->OrthoCamAssets.zoomLevel = 1.0f;
    this->camera = NewOrthographicCamera(   
        -aspectRatio * this->OrthoCamAssets.zoomLevel, 
        aspectRatio * this->OrthoCamAssets.zoomLevel, 
        -this->OrthoCamAssets.zoomLevel, 
        this->OrthoCamAssets.zoomLevel
    );


    this->OrthoCamAssets.cameraRotationSpeed = 1.0f;
    this->OrthoCamAssets.cameraTranslationSpeed = 5.0f;
    this->OrthoCamAssets.controllerType = controllertype;
    this->OrthoCamAssets.aspectRatio = aspectRatio;

    OrthographicCameraSetPosition(this->camera,  V2(0.0f, 0.0f));
    return this;
}

void CameraControllerOnUpdate(CameraController* this, float deltaTime)
{
    if(this->camera->camType == OrthographicCameraType)
    {
        if(this->OrthoCamAssets.controllerType & CameraKeyboardController)
        {
            float speed = this->OrthoCamAssets.cameraTranslationSpeed * this->OrthoCamAssets.zoomLevel;
            v2 pos = {0};

            u8 isPressed = InputIsMouseButtonPressed(NL_MOUSE_BUTTON_LEFT);
            
            if(InputIsButtonPressed(NL_KEY_W))
                pos.y -= speed * deltaTime * !isPressed;
            
            
            else if(InputIsButtonPressed(NL_KEY_S))
                pos.y += speed * deltaTime * !isPressed;
            
            
            if(InputIsButtonPressed(NL_KEY_A))
                pos.x += speed * deltaTime * !isPressed;
            
            else if(InputIsButtonPressed(NL_KEY_D))
                pos.x -= speed * deltaTime * !isPressed;


            OrthgraphicCameraAddVector(this->camera,  pos);
        }

        if(this->OrthoCamAssets.controllerType & CameraMouseDragController)
        {
            v2 WindowSize = GetWindowSize();

            float xRatio = this->camera->orthoCam.height / WindowSize.height; 
            float yRatio = this->camera->orthoCam.width  / WindowSize.width;
            
            OrthgraphicCameraAddVector(
                this->camera, 
                V2(this->OrthoCamAssets.mouseMovedDelta.x * xRatio, this->OrthoCamAssets.mouseMovedDelta.y * yRatio)
            );
            
            this->OrthoCamAssets.mouseMovedDelta = V2(0.0f, 0.0f);
        }
    }
}

void CameraControllerOnEvent(CameraController* this, const Event* event)
{
    switch (event->type)
    {
        case ScrolledEventType :
        {
            float scrollDistance = event->PosEvent.pos.y;

            if((this->OrthoCamAssets.controllerType & CameraMouseScrollSensitive) == 0 || scrollDistance == 0)
                return;


            if(scrollDistance < 0)
                this->OrthoCamAssets.zoomLevel *=  (-scrollDistance) * ZoomIntensity;
            else 
                this->OrthoCamAssets.zoomLevel /= scrollDistance * ZoomIntensity; 


            this->OrthoCamAssets.zoomLevel = max(this->OrthoCamAssets.zoomLevel, 0.01f);

            OrthographicCameraSetProjection(this->camera,   -this->OrthoCamAssets.aspectRatio * this->OrthoCamAssets.zoomLevel, 
                                                            this->OrthoCamAssets.aspectRatio * this->OrthoCamAssets.zoomLevel, 
                                                            -this->OrthoCamAssets.zoomLevel, 
                                                            this->OrthoCamAssets.zoomLevel);
        } break;

        case WindowResizeEventType :
        {
            this->OrthoCamAssets.aspectRatio = (float)event->WindowResizeEvent.width / (float)event->WindowResizeEvent.height;
            OrthographicCameraSetProjection(this->camera,   -this->OrthoCamAssets.aspectRatio * this->OrthoCamAssets.zoomLevel, 
                                                            this->OrthoCamAssets.aspectRatio * this->OrthoCamAssets.zoomLevel, 
                                                            -this->OrthoCamAssets.zoomLevel, 
                                                            this->OrthoCamAssets.zoomLevel);
        } break;

        case MouseMovedEventType :
        {
            if(this->OrthoCamAssets.controllerType & CameraMouseDragController)
            {
                unsigned int isPressed = InputIsMouseButtonPressed(NL_MOUSE_BUTTON_LEFT);
                this->OrthoCamAssets.mouseMovedDelta.x += event->PosEvent.delta.x * isPressed;
                this->OrthoCamAssets.mouseMovedDelta.y -= event->PosEvent.delta.y * isPressed;
            }
        }break;
    }

}

void DeleteOrthographicCameraController(CameraController* this)
{
    DeleteOrthographicCamera(this->camera);
    Memory.Free(this);
}