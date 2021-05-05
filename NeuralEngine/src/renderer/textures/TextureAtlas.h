#ifndef __TEXTUREATLAS_H_
#define __TEXTUREATLAS_H_

#include "src/utils/types.h"
#include "Texture2D.h"

typedef struct TextureAtlas
{
    float rows, columns;
    float unitx, unity;
    Texture2D * texture;
} TextureAtlas;


extern TextureAtlas* NewTextureAtlas(const char * string, float rows, float columns);

extern void TextureAtlasGetTextCoords(TextureAtlas* this, float row, float column, float width, float height, v2 outVertices[4]);

extern void DeleteTextureAtlas(TextureAtlas* this);


#endif // __TEXTUREATLAS_H_


