#pragma once
#include "src/utils/types.h"

#include <glad/glad.h>

typedef struct Texture
{
    unsigned int id;
    int width, height, bpp;
} Texture;

extern Texture* NewTexture(char *filepath);

inline void textureUnbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

inline void textureBind(Texture *this, byte slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, this->id);   
}
inline void DeleteTexture(Texture * this)
{
    glDeleteTextures(1, &this->id);
    MemFree(this);
}

