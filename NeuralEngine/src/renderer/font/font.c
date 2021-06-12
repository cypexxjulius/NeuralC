#include "font.h"

#include <stb_truetype.h>

#include <glad/glad.h>
#include <stdio.h>
#include <math.h>
#include "src/utils/fileio.h"
#include "src/core/error.h"
#include "src/renderer/textures/Texture2D.h"
#include <glad/glad.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#define STBRP_STATIC
#define STB_RECT_PACK_IMPLEMENTATION
#include <stb_rect_pack.h>

#define STBTT_STATIC
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#include "src/utils/Logger.h"


Font* NewFontTexture(const char *filepath)
{   
   Font* this = CreateObject(Font);
   String fileContent = ReadStringFromFile(filepath);
    
   stbtt_fontinfo font = { 0 };
   int errorCode = stbtt_InitFont(&font, (u8 *)fileContent.string, 0);

   Assert(errorCode != 1, "Failed to load font");   

   this->width = 1024;
   this->height = 1024;

   float fontHeight = 64;


   byte* bitmap = Memory.Calloc(this->width * this->height, sizeof(byte));

   float scale = stbtt_ScaleForPixelHeight(&font, fontHeight);

   stbtt_pack_context packContext = { 0 };
   stbtt_packedchar charData[CHAR_COUNT] = { 0 };

   int ascent, descent, linegap;
   stbtt_GetFontVMetrics(&font, &ascent, &descent, &linegap);

   // Packing the font into the bitmap
   stbtt_PackBegin(&packContext, bitmap, this->width, this->height, this->width, 0, NULL);
   
   stbtt_PackSetOversampling(&packContext, 2, 2);
   stbtt_PackFontRange(&packContext, (u8 *)fileContent.string, 0, fontHeight, FIRST_CHAR_IN_FONT, CHAR_COUNT, charData);

   stbtt_PackEnd(&packContext);

   // Converting the stbtt_aligned_quad data to my own struct for each glyp
   for(u16 i = 0; i < CHAR_COUNT; i++)
   {
      stbtt_aligned_quad quad = { 0 };
      float unused_xpos = 0, unused_ypos = 0;
      stbtt_GetPackedQuad(charData, this->width, this->height, i, &unused_xpos, &unused_ypos, &quad, false);

      // Texture coordinates
      this->charData[i].x0 = quad.s0;
      this->charData[i].x1 = quad.s1;

      this->charData[i].y0 = quad.t1;
      this->charData[i].y1 = quad.t0;

      
      float width = this->charData[i].width  = (quad.x1 - quad.x0) * scale;
      float height = this->charData[i].height = (quad.y1 - quad.y0) * scale;
      
      LOG("%c : { %f %f }", i + FIRST_CHAR_IN_FONT, width, height);

      
      this->charData[i].baseline = quad.y1 * scale;
   }

   // There is probably a better way to do this ... 
   this->lineHeight = this->charData[CharToGlyph('[')].height - this->charData[CharToGlyph('[')].baseline;
   this->letterSpacing = this->charData[CharToGlyph('.')].width;

   // Creating the Texture 
   this->FontTexture = NewTexture2DEmpty(this->width, this->height, Image_TypeALPHA);
   Texture2DSetData(this->FontTexture, bitmap, this->width * this->height);
   this->fontSize = fontHeight;

   Memory.Free(bitmap);

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