#include "OrthographicCameraController.h"
#include "Camera.h"
#include "src/events/keycode.h"
#include "src/core/Application.h"
#include "src/utils/types.h"

const Window* localWindow;

CameraController* NewOrthographicCameraController(float aspectRatio, CameraControllerType controllertype)
{
    CameraController* this = CreateObject(CameraController);

    localWindow = ApplicationGetWindow();

    this->OrthoCamAssets.zoomLevel = 1;
    this->camera = NewOrthographicCamera(   -aspectRatio * this->OrthoCamAssets.zoomLevel, 
                                            aspectRatio * this->OrthoCamAssets.zoomLevel, 
                                            -this->OrthoCamAssets.zoomLevel, 
                                            this->OrthoCamAssets.zoomLevel);
    this->OrthoCamAssets.cameraRotationSpeed = 1.0f;
    this->OrthoCamAssets.cameraTranslationSpeed = 5.0f;
    this->OrthoCamAssets.controllerType = controllertype;
    this->OrthoCamAssets.aspectRatio = aspectRatio;

    orthographicCameraSetPosition(this->camera,  v2(0.0f, 0.0f));
    return this;
}

void CameraControllerOnUpdate(CameraController* this, float deltaTime)
{
    if(this->camera->camType == OrthographicCameraType)
    {
        if(this->OrthoCamAssets.controllerType & KeyboardController)
        {
            float speed = this->OrthoCamAssets.cameraTranslationSpeed * this->OrthoCamAssets.zoomLevel;
            v2 pos = {0};

            if(InputIsButtonPressed(NL_KEY_W))
                pos.y -= speed * deltaTime;
            
            
            else if(InputIsButtonPressed(NL_KEY_S))
                pos.y += speed * deltaTime;
            
            
            if(InputIsButtonPressed(NL_KEY_A))
                pos.x += speed * deltaTime;
            
            else if(InputIsButtonPressed(NL_KEY_D))
                pos.x -= speed * deltaTime;


            orthgraphicCameraAddVector(this->camera,  pos);
        }

        if(this->OrthoCamAssets.controllerType & MouseDragController)
        {
            float xRatio = this->OrthoCamAssets.zoomLevel * this->camera->orthoCam.height / localWindow->state.height;
            float yRatio = this->OrthoCamAssets.zoomLevel * this->camera->orthoCam.width  / localWindow->state.width;           
            orthgraphicCameraAddVector(this->camera, v2(this->OrthoCamAssets.mouseMovedDelta.x * xRatio, this->OrthoCamAssets.mouseMovedDelta.y * yRatio));
            this->OrthoCamAssets.mouseMovedDelta = v2(0.0f, 0.0f);
        }
    }
}

void CameraControllerOnEvent(CameraController* this, const Event* event)
{
    switch (event->type)
    {
        case ScrolledEventType :
        {
            float zoomIntensity = 2.0f;
            if(event->PosEvent.pos.y == 0)
                return;

            if(event->PosEvent.pos.y < 0)
                this->OrthoCamAssets.zoomLevel *=  GetUnsignedFloat(event->PosEvent.pos.y) * zoomIntensity;
            else 
                this->OrthoCamAssets.zoomLevel /= event->PosEvent.pos.y * zoomIntensity; 

            this->OrthoCamAssets.zoomLevel = C_Max(this->OrthoCamAssets.zoomLevel, 0.01f);

            orthographicCameraSetProjection(this->camera,   -this->OrthoCamAssets.aspectRatio * this->OrthoCamAssets.zoomLevel, 
                                                            this->OrthoCamAssets.aspectRatio * this->OrthoCamAssets.zoomLevel, 
                                                            -this->OrthoCamAssets.zoomLevel, 
                                                            this->OrthoCamAssets.zoomLevel);
        } break;

        case WindowResizeEventType :
        {
            this->OrthoCamAssets.aspectRatio = (float)event->WindowResizeEvent.width / (float)event->WindowResizeEvent.height;
            orthographicCameraSetProjection(this->camera,   -this->OrthoCamAssets.aspectRatio * this->OrthoCamAssets.zoomLevel, 
                                                            this->OrthoCamAssets.aspectRatio * this->OrthoCamAssets.zoomLevel, 
                                                            -this->OrthoCamAssets.zoomLevel, 
                                                            this->OrthoCamAssets.zoomLevel);
        } break;

        case MouseMovedEventType :
        {
            if(this->OrthoCamAssets.controllerType & MouseDragController)
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
    MemFree(this);
}