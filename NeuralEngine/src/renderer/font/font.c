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
#include "src/utils/Logger.h"


Font* NewFontTexture(const char *filepath)
{   
   Font* this = CreateObject(Font);
   String fileContent = ReadStringFromFile(filepath);
    
   stbtt_fontinfo font = { 0 };
   int errorCode = stbtt_InitFont(&font, (u8 *)fileContent.string, 0);

   Assert(errorCode != 1, "Failed to load font");   

   this->width = 1024;
   this->height = 2024;

   float fontHeight = 64;
   float fontScaleDownRatio = fontHeight * 10.0f;


   this->bitmap = Memory.Calloc(this->width * this->height, sizeof(byte));

   float scale = stbtt_ScaleForPixelHeight(&font, fontHeight);

   stbtt_pack_context PackContext = { 0 };
   stbtt_packedchar charData[CHAR_COUNT] = { 0 };


   stbtt_PackBegin(&PackContext, this->bitmap, this->width, this->height, 0, 0, NULL);
   
   stbtt_PackSetOversampling(&PackContext, 2, 2);

   stbtt_PackFontRange(&PackContext, fileContent.string, 0, fontHeight, ' ', CHAR_COUNT, charData);
   stbtt_PackEnd(&PackContext);

   stbtt_aligned_quad quad = { 0 };
   for(u8 i = 0; i < CHAR_COUNT; i++)
   {
      float xpos = 1, ypos = 1;
      stbtt_GetPackedQuad(charData, this->width, this->height, i, &xpos, &ypos, &quad, true);

      this->charData[i].x0 = quad.s0;
      this->charData[i].x1 = quad.s1;

      this->charData[i].y0 = quad.t0;
      this->charData[i].y1 = quad.t1;

      this->charData[i].width  = (quad.x1 - quad.x0) / fontScaleDownRatio;
      this->charData[i].height = (quad.y1 - quad.y0) / fontScaleDownRatio;
      
      this->charData[i].baseline = quad.y1 / fontScaleDownRatio;
   }

   this->lineHeight = this->charData['[' - ' '].height - this->charData['[' - ' '].baseline;
   this->letterSpacing = this->charData['.' - ' '].height / 5.0f;


   this->FontTexture = NewTexture2DEmpty(this->width, this->height, Image_TypeALPHA);
   Texture2DSetData(this->FontTexture, this->bitmap, this->width * this->height);

   return this;
}



void FontGetCharInfo(Font* this, char character, v2 outVertices[4], v2 *outSize, float *outbaseline)
{
   
   character -= ' ';

   if(outVertices != NULL)
   {
      outVertices[0] = V2(this->charData[character].x0, this->charData[character].y1);
      outVertices[1] = V2(this->charData[character].x1, this->charData[character].y1);
      outVertices[2] = V2(this->charData[character].x1, this->charData[character].y0);
      outVertices[3] = V2(this->charData[character].x0, this->charData[character].y0);   
   }

   if(outSize != NULL)
   {
      outSize->width = this->charData[character].width;
      outSize->height = this->charData[character].height;
   }

   if(outbaseline != NULL)
   {
      *outbaseline = this->charData[character].baseline;
   }
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
