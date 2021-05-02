#ifndef __RENDERER2D_H_
#define __RENDERER2D_H_

#include "camera/Camera.h"
#include "textures/Texture2D.h"
#include "src/utils/types.h"

typedef struct Renderer2DQuadInitializer
{
    v2 position;
    v2 scale;
    v4 color;
    float zIndex;
    float tiling;
    float rotation;
    Texture2D* texture;
} Quad2D;

extern void Renderer2DInit();

extern void Renderer2DShutdown();

extern void Renderer2DRenderTest(char* character);

extern void Renderer2DBeginScene(Camera* camera);

extern void Renderer2DDrawQuad(Quad2D initializer);

extern void Renderer2DEndScene();



#endif // __RENDERER2D_H_