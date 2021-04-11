#include "Texture2D.h"
#include "src/platform/memory.h"
#include <stb_image.h>
#include <glad/glad.h>

Texture2D* NewTexture2D(char * filepath)
{
    Texture2D* this = CreateObject(Texture2D);
    stbi_set_flip_vertically_on_load(1);
    stbi_uc* data = stbi_load(filepath, &this->width, &this->height, &this->channels, 0);

    if(!data)
    {
        char message[300];

        snprintf(message, 300, "Failed to read image : %s", filepath);
        Assert(0, message);
        return NULL;
    }

    GLenum internalFormat = 0, dataFormat = 0;
    if(this->channels == 4)
    {
        internalFormat = GL_RGBA8;
        dataFormat = GL_RGBA;
    } 
    else if(this->channels == 3)
    {
        internalFormat = GL_RGB8;
        dataFormat = GL_RGB;
    }

    Assert(!(internalFormat & dataFormat), "Image Format not supported");

    this->format = (unsigned int)dataFormat;
    this->internalFormat = (unsigned int)internalFormat;

    glCreateTextures(GL_TEXTURE_2D, 1, &this->id);
	glTextureStorage2D(this->id, 1, internalFormat, this->width, this->height);

	glTextureParameteri(this->id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(this->id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureParameteri(this->id, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(this->id, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTextureSubImage2D(this->id, 0, 0, 0, this->width, this->height, dataFormat, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);


    return this;
}

Texture2D* NewTexture2DEmpty(unsigned int width, unsigned int height)
{
    Texture2D* this = CreateObject(Texture2D);
    this->width = width;
    this->height = height;
    
    GLenum internalFormat = GL_RGBA8, dataFormat = GL_RGBA;

    this->format = (unsigned int)dataFormat;
    this->internalFormat = (unsigned int)internalFormat;

    glCreateTextures(GL_TEXTURE_2D, 1, &this->id);
	glTextureStorage2D(this->id, 1, internalFormat, this->width, this->height);

	glTextureParameteri(this->id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(this->id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureParameteri(this->id, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(this->id, GL_TEXTURE_WRAP_T, GL_REPEAT);


    return this;
}

void Texture2DSetData(Texture2D *this, void *data, unsigned int size)
{
    unsigned int bpp = GL_RGBA ? 4 : 3;
    Assert(!(size == this->width * this->height * bpp), "Size must be the entire Texture");
	glTextureSubImage2D(this->id, 0, 0, 0, this->width, this->height, this->format, GL_UNSIGNED_BYTE, data);
}

void DeleteTexture2D(Texture2D *this)
{
    glDeleteTextures(1, &this->id);
    MemFree(this);
}

void Texture2DBind(Texture2D* this, unsigned int slot)
{
    this->slot = slot;
    glBindTextureUnit(slot, this->id);
}