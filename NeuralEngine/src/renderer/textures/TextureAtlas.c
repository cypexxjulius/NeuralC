#include "TextureAtlas.h"
#include "src/platform/memory.h"


TextureAtlas* NewTextureAtlas(const char * string, float rows, float columns)
{
    TextureAtlas* this = CreateObject(TextureAtlas);

    this->texture = NewTexture2D(string);

    this->columns   = columns;
    this->rows      = rows;
    this->unitx     = columns   / (float)this->texture->width;
    this->unity     = rows      / (float)this->texture->height;

    return this;
}


void TextureAtlasGetTextCoords(TextureAtlas* this, float row, float column, float width, float height, v2 outVertices[4])
{
    outVertices[0] = v2(this->unitx * (column - 0.0f),  1.0f - this->unity * (row + height));
    outVertices[1] = v2(this->unitx * (column + width), 1.0f - this->unity * (row + height));
    outVertices[2] = v2(this->unitx * (column + width), 1.0f - this->unity * (row - 0.0f));
    outVertices[3] = v2(this->unitx * (column - 0.0f),  1.0f - this->unity * (row - 0.0f));
}



void DeleteTextureAtlas(TextureAtlas* this)
{
    DeleteTexture2D(this->texture);
    Memory.Free(this);
}