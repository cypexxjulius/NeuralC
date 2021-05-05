#ifndef __TEXTURE2D_H_
#define __TEXTURE2D_H_

typedef struct Texture2D
{   
    unsigned int id, slot;
    int width, height, channels;
    unsigned int format, internalFormat;
} Texture2D;


Texture2D* NewTexture2D(const char *filepath);

Texture2D* NewTexture2DEmpty(unsigned int width, unsigned int height);

void Texture2DBind(Texture2D* this, unsigned int slot);

void Texture2DSetData(Texture2D *this, void *data, unsigned int size);

void DeleteTexture2D(Texture2D *this);


#endif // __TEXTURE2D_H_