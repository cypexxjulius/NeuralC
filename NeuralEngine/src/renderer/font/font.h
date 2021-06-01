#ifndef __FONT_H_
#define __FONT_H_

#include "src/utils/types.h" 
#include "src/renderer/textures/Texture2D.h"


#define CharToGlyph(character) ((character) - ' ')

#define CHAR_COUNT CharToGlyph(255) + 1

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
    float fontSize;
    Texture2D *FontTexture;
    FontCharData charData[CHAR_COUNT];
} Font;


extern Font* NewFontTexture(const char *filepath);

extern void DeleteFont(Font *this);

extern void FontBind(Font* this, u8 slot);

#endif// __FONT_H_  