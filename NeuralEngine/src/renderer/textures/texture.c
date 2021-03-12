#include "texture.h"
#include "src/platform/memory.h"
#include <glad/glad.h>
#include <string.h>

#include <stb_image.h>

extern n_Texture* newTexture(char *filepath)
{
    n_Texture *this = nl_malloc(sizeof(n_Texture));

    stbi_set_flip_vertically_on_load(0);
    byte *buffer = stbi_load(filepath, &this->width, &this->height, &this->bpp, 4);

    glGenTextures(1, &this->id);
    glBindTexture(GL_TEXTURE_2D, this->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    glBindTexture(GL_TEXTURE_2D, 0);

    if(buffer)
    {
        stbi_image_free(buffer);
    }
    return this;
}

extern void textureBind(n_Texture *this, byte slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, this->id);   
}

extern void textureUnbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
extern void deleteTexture(n_Texture * this)
{
    glDeleteTextures(1, &this->id);
    nl_free(this);
}