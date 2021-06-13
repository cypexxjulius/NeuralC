#include "GUI.h"
#include "src/core/Input.h"

void GUItoRenderSpace(v2 *Position)
{
    v2 WindowSize = GetWindowSize();
    float aspectRatio = WindowSize.width / WindowSize.height;

    Position->x *= aspectRatio * 2;
    Position->y *= -2;

    Position->x -= aspectRatio;
    Position->y += 1.0f;
}

void WindowToGUISpace(v2 *Position)
{
    v2 WindowSize = GetWindowSize();
    float aspectRatio = WindowSize.width / WindowSize.height;


    Position->x /= WindowSize.width;
    Position->y /= WindowSize.height;

    GUItoRenderSpace(Position);
}