#ifndef __FONT_H_
#define __FONT_H_

#include "src/utils/types.h" 
#include "src/renderer/textures/Texture2D.h"

#define CHAR_COUNT 255 - ' ' + 1

typedef struct FontCharData
{
    float x0, x1, y0, y1;
    float width, height;
    float baseline;
} FontCharData;

typedef struct Font
{
    u16 width, height;
    float lineHeight;
    float letterSpacing;
    byte* bitmap;
    Texture2D *FontTexture;
    FontCharData charData[CHAR_COUNT];
} Font;

extern Font* NewFontTexture(const char *filepath);

extern void FontGetCharInfo(Font* this, char character, v2 outVertices[4], v2 *outSize, float *outbaseline);

extern void DeleteFont(Font *this);

extern void FontBind(Font* this, u8 slot);

#endif// __FONT_H_  