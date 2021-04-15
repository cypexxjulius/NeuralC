#ifndef __RENDERER2D_H_
#define __RENDERER2D_H_

#include "camera/Camera.h"
#include "textures/Texture2D.h"
#include "src/utils/types.h"

typedef struct Renderer2DQuadInitializer
{
    v3 position;

    v2 scale;
    v4 color;
    float tilling;
    float rotation;
    Texture2D* texture;

} Quad2D;



void Renderer2DInit();

void Renderer2DShutdown();


void Renderer2DBeginScene(Camera* camera);

void Renderer2DDrawQuad(Quad2D initializer);

void Renderer2DEndScene();


#endif // __RENDERER2D_H_