#ifndef __FONT_H_
#define __FONT_H_

#include "src/utils/types.h"git 
#include "src/renderer/textures/Texture2D.h"

#define CHAR_COUNT 96

typedef struct FontCharData
{
    float x0, x1, y0, y1;
    float width, height;
} FontCharData;

typedef struct font_struct
{
    u16 width, height;
    byte* bitmap;
    Texture2D *FontTexture;
    FontCharData charData[CHAR_COUNT];
} Font;


typedef struct font_struct Font;

extern Font* NewFontTexture(const char *filepath);

extern void FontGetCharVertices(Font* this, char character, v2 outVertices[4], v2 *outSize);

extern void DeleteFont(Font *this);

extern void FontBind(Font* this, u8 slot);


#endif// __FONT_H_  