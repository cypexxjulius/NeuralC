#include "Renderer2D.h"
#include "src/utils/types.h"
#include "src/renderer/font/font.h"
#include "src/utils/Logger.h"
#include "src/core/error.h"

float Renderer2DText(Font* font, char* string, float scale, v3 color, v2 position, float zIndex, float maxWidth, float maxHeight)
{
   
    if(string == NULL)
        return 0.0f;

    float lineLength = 0.0f;
    
    position.y -= font->lineHeight * scale;

    float baseline = 0.0f;
    v2 textureCoords[4] = {0};
    v2 Size = {0};

    for(u16 i = 0; string[i] != '\0'; i++)
    {
        char glyph = CharToGlyph(string[i]);


        Size = V2(
            font->charData[glyph].width * scale, 
            font->charData[glyph].height * scale
        );

        
        if(maxWidth <= lineLength + Size.width + font->letterSpacing * scale)
            return lineLength;
        
        
        baseline = font->charData[glyph].baseline;

        if(string[i] == ' ')
        {
            lineLength += (font->charData[CharToGlyph('a')].width + font->letterSpacing) * scale;
            continue;
        }

        textureCoords[0] = V2(font->charData[glyph].x0, font->charData[glyph].y0);
        textureCoords[1] = V2(font->charData[glyph].x1, font->charData[glyph].y0);
        textureCoords[2] = V2(font->charData[glyph].x1, font->charData[glyph].y1);
        textureCoords[3] = V2(font->charData[glyph].x0, font->charData[glyph].y1);


        float posx = position.x + lineLength;
        float posy = position.y - baseline  * scale;


        Renderer2DPushVertices(
            (v3 [4]){
                V3(posx,                posy,               zIndex),
                V3(posx + Size.width,   posy,               zIndex),
                V3(posx + Size.width,   posy + Size.height, zIndex),
                V3(posx,                posy + Size.height, zIndex),
            }, 
            V4(color.x, color.y, color.z, 1.0f), 
            textureCoords, 
            1, 
            1.0f
        );

        lineLength += Size.width + font->letterSpacing * scale;

    }

    return lineLength;
}

float Renderer2DTextLength(Font* font, char* string, float scale, float maxWidth)
{
    
    if(string == NULL)
        return 0;


    float lineLength = 0.0f;
    for(u16 i = 0; string[i] != '\0'; i++)
    {
        char glyph = CharToGlyph(string[i]);

        float width = (string[i] == ' ') ? font->charData[CharToGlyph('a')].width : font->charData[glyph].width;  
        
        if(maxWidth <= lineLength + width + font->letterSpacing * scale && maxWidth != 0)
            return lineLength - font->letterSpacing * scale; 

        lineLength += (width + font->letterSpacing) * scale;

    }

    return( lineLength - font->letterSpacing * scale );
}