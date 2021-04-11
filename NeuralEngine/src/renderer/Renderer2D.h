#ifndef __RENDERER2D_H_
#define __RENDERER2D_H_

#include "camera/Camera.h"

void Renderer2DInit();

void Renderer2DShutdown();


void Renderer2DBeginScene(Camera* camera);

void Renderer2DDrawColoredQuad(v3 position, v2 scale, v4 color);

void Renderer2DDrawTexturedQuad(v3 position, v2 scale, Texture* texture);

void Renderer2DEndScene();


#endif // __RENDERER2D_H_