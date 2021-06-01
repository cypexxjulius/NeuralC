#include "Renderer2D.h"
#include "src/utils/types.h"
#include "src/renderer/font/font.h"

void Renderer2DText(Font* font, char* string, u16 textLength, float scale, v3 color, v2 position, float zIndex, float maxWidth, float maxHeight)
{
    float originalY = position.y;
    float letterSpacing = font->letterSpacing;
    
    if(textLength == 0 || string == NULL)
        return;

    float lineLength = 0.0f;
    
    position.y -= font->lineHeight * scale;

    float baseline;
    v2 textureCoords[4];
    v2 Size;

    for(u16 i = 0; i < textLength; i++)
    {
        char glyph = CharToGlyph(string[i]);

        if(string[i] == ' ')
        {
            glyph = CharToGlyph('!');
            Size = V2(font->charData[glyph].width, font->charData[glyph].height);

            baseline = font->charData[glyph].baseline;
            lineLength += Size.width * scale + letterSpacing;
            continue;
        }

        textureCoords[0] = V2(font->charData[glyph].x0, font->charData[glyph].y1);
        textureCoords[1] = V2(font->charData[glyph].x1, font->charData[glyph].y1);
        textureCoords[2] = V2(font->charData[glyph].x1, font->charData[glyph].y0);
        textureCoords[3] = V2(font->charData[glyph].x0, font->charData[glyph].y0);

        Size = V2(font->charData[glyph].width, font->charData[glyph].height);

        baseline = font->charData[glyph].baseline;


        float posx = position.x + lineLength;

        float posy = position.y - baseline  * scale;


        Renderer2DPushVertices(
            (v3 [4]){
                V3(posx,                        posy, zIndex),
                V3(posx + Size.width * scale,   posy, zIndex),
                V3(posx + Size.width * scale,   posy + Size.height * scale, zIndex),
                V3(posx,                        posy + Size.height * scale, zIndex),
            }, 
            V4(color.x, color.y, color.z, 1.0f), 
            textureCoords, 
            1, 
            1.0f
        );

        lineLength += Size.width * scale + font->letterSpacing * scale;

    }
}

void Renderer2DTextLength()
{

}