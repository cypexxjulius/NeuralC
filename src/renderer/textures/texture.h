#ifndef __TEXTURE_H_
#define __TEXTURE_H_

#include "../../utils/types.h"

typedef struct n_Texture
{
    unsigned int id;
    int width, height, bpp;
} n_Texture;

extern n_Texture* newTexture(char *filepath);

extern void textureBind(n_Texture *this, byte slot);

extern void textureUnbind();

extern void deleteTexture(n_Texture* this);


#endif // !__TEXTURES_H_
