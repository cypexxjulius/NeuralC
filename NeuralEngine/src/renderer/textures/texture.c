#include "texture.h"
#include "src/platform/memory.h"
#include <string.h>

#include <stb_image.h>

extern Texture* NewTexture(char *filepath)
{
    Texture *this = CreateObject(Texture);

    stbi_set_flip_vertically_on_load(1);
    byte *buffer = stbi_load(filepath, &this->width, &this->height, &this->channels, 0);
    Assert(buffer == NULL, "Failed to read texture");

    GLenum internalFormat = 0, dataFormat = 0;
	if (this->channels == 4)
	{
		internalFormat = GL_RGBA8;
		dataFormat = GL_RGBA;
	}
	else if (this->channels == 3)
	{
		internalFormat = GL_RGB8;
		dataFormat = GL_RGB;
	}
    Assert(!(internalFormat & dataFormat), "Format not supported!");

    glCreateTextures(GL_TEXTURE_2D, 1, &this->id);
	glTextureStorage2D(this->id, 1, internalFormat, this->width, this->height);

	glTextureParameteri(this->id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(this->id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureSubImage2D(this->id, 0, 0, 0, this->width, this->height, dataFormat, GL_UNSIGNED_BYTE, buffer);


    if(buffer)
    {
        stbi_image_free(buffer);
    }
    return this;
}