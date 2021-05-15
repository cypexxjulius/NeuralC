#include "font.h"

#include <stb_truetype.h>

#include <glad/glad.h>
#include <stdio.h>
#include <math.h>
#include "src/utils/fileio.h"
#include "src/core/error.h"
#include "src/renderer/textures/Texture2D.h"


#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h" /* http://nothings.org/stb/stb_image_write.h */



Font* NewFontTexture(const char *filepath)
{   
   Font* this = CreateObject(Font);
   String fileContent = ReadStringFromFile(filepath);
    
   stbtt_fontinfo font;
   int errorCode = stbtt_InitFont(&font, (u8 *)fileContent.string, 0);

   Assert(!errorCode, "Failed to load font");   

   this->width = 512;
   this->height = 512;

   float fontHeight = 32;



   this->bitmap = Memory.Calloc(this->width * this->height, sizeof(byte));

   float scale = stbtt_ScaleForPixelHeight(&font, fontHeight);

   stbtt_pack_context PackContext;
   stbtt_packedchar charData[CHAR_COUNT];

   stbtt_PackBegin(&PackContext, this->bitmap, this->width, this->height, 0, 1, NULL);
   
   stbtt_PackSetOversampling(&PackContext, 2, 2);

   stbtt_PackFontRange(&PackContext, fileContent.string, 0, fontHeight, ' ', CHAR_COUNT, charData);
   stbtt_PackEnd(&PackContext);

   stbtt_aligned_quad quad;
   for(u8 i = 0; i < CHAR_COUNT; i++)
   {
      float xpos, ypos;
      stbtt_GetPackedQuad(charData, this->width, this->height, i, &xpos, &ypos, &quad, true);

      this->charData[i].x0 = quad.s0;
      this->charData[i].x1 = quad.s1;

      this->charData[i].y0 = quad.t0;
      this->charData[i].y1 = quad.t1;

      this->charData[i].width  = (quad.x1 - quad.x0) / (float)this->width;
      this->charData[i].height = (quad.y1 - quad.y0) / (float)this->height;
      
   }

   this->FontTexture = NewTexture2DEmpty(this->width, this->height, Image_TypeALPHA);
   Texture2DSetData(this->FontTexture, this->bitmap, this->width * this->height);

   return this;
}



void FontGetCharVertices(Font* this, char character, v2 outVertices[4], v2 *outSize)
{
   
   character -= ' ';
   outVertices[0] = V2(this->charData[character].x0, this->charData[character].y1);
   outVertices[1] = V2(this->charData[character].x1, this->charData[character].y1);
   outVertices[2] = V2(this->charData[character].x1, this->charData[character].y0);
   outVertices[3] = V2(this->charData[character].x0, this->charData[character].y0);   

   outSize->width = this->charData[character].width;
   outSize->height = this->charData[character].height;
}

void DeleteFont(Font *this)
{
   DeleteTexture2D(this->FontTexture);
   Memory.Free(this);
}

void FontBind(Font *this, u8 slot)
{
   Texture2DBind(this->FontTexture, slot);
}
