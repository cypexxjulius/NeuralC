#ifndef __RENDERER2D_H_
#define __RENDERER2D_H_

#include "camera/Camera.h"
#include "textures/Texture2D.h"
#include "src/utils/types.h"

typedef struct TextQuadElement 
{   
    char *string; 
    u16 maxLength;
    float fontSize;
    v3 color;
} TextElement;

typedef struct Renderer2DQuadInitializer
{
    v2 position;
    float width;
    float height;
    v4 color;
    float zIndex;
    float tiling;
    float rotation;
    TextElement* text;
    Texture2D* texture;
} Quad2D;

extern void Renderer2DInit();

extern void Renderer2DShutdown();

extern void Renderer2DBeginScene(Camera* camera);

extern void Renderer2DDrawQuad(Quad2D initializer);

extern void Renderer2DEndScene();

extern void Renderer2DEndSceneCallback();

extern void Renderer2DStartSceneCallback();



#endif // __RENDERER2D_H_