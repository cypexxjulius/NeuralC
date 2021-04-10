#ifndef __TEXTURE_H_
#define __TEXTURE_H_


#include "src/utils/types.h"

#include <glad/glad.h>

typedef struct Texture
{
    unsigned int id;
    int width, height, channels;
} Texture;

extern Texture* NewTexture(char *filepath);

static inline void textureUnbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

static inline void textureBind(Texture *this, byte slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, this->id);   
}
static inline void DeleteTexture(Texture * this)
{
    glDeleteTextures(1, &this->id);
    MemFree(this);
}

#endif // __TEXTURE_H_