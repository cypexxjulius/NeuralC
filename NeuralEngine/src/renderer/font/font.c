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


   byte* bitmap = Memory.Calloc(this->width * this->height, sizeof(byte));

   float scale = stbtt_ScaleForPixelHeight(&font, fontHeight);

   stbtt_pack_context PackContext = { 0 };
   stbtt_packedchar charData[CHAR_COUNT] = { 0 };


   stbtt_PackBegin(&PackContext, bitmap, this->width, this->height, 0, 0, NULL);
   
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

   this->lineHeight = this->charData[CharToGlyph('[')].height - this->charData[CharToGlyph('[')].baseline;
   this->letterSpacing = this->charData[CharToGlyph('.')].height / 5.0f;


   this->FontTexture = NewTexture2DEmpty(this->width, this->height, Image_TypeALPHA);
   Texture2DSetData(this->FontTexture, bitmap, this->width * this->height);

   Memory.Free(bitmap);
   this->fontSize = fontHeight;

   return this;
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
