#ifndef __RENDERER2D_H_
#define __RENDERER2D_H_

#include "camera/Camera.h"
#include "textures/Texture2D.h"
#include "src/utils/types.h"
#include "src/renderer/shader/shader.h"
#include "src/events/eventStructs.h"
#include "src/renderer/font/font.h"

typedef struct TextQuadElement 
{   
    char *string; 
    u16 maxLength;
    float fontSize;
    v3 color;
} TextElement;

typedef struct Renderer2DQuadInitializer
{
    v4 color;
    v2 position;
    float width;
    float tiling;
    float height;
    float zIndex;
    float rotation;
    Texture2D* texture;
} Quad2D;

extern void Renderer2DInit();

extern void Renderer2DShutdown();

extern void Renderer2DBeginScene(Camera* camera, Shader* shader, Font* font);

extern void Renderer2DDrawQuad(Quad2D* initializer);

extern void Renderer2DEndScene();

extern void Renderer2DEndSceneCallback();

extern void Renderer2DStartSceneCallback();

extern void Renderer2DPushVertices(v3 ipositions[4], v4 icolor, v2 itextureCoords[4], float iTextureID, float itiling);

extern float Renderer2DText(Font* font, char* string, float scale, v3 color, v2 position, float zIndex, float maxWidth, float maxHeight);

extern float Renderer2DTextLength(Font* font, char* string, float scale, float maxWidth);

#endif // __RENDERER2D_H_