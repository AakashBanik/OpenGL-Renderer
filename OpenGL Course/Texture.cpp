#include "Texture.h"
#include <iostream>


Texture::Texture()
{
	textureID = 0;
	widht = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = "";
}

Texture::Texture(const char *fileloc)
{
	textureID = 0;
	widht = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = fileloc;
}

void Texture::loadTexture()
{
	unsigned char *texData = stbi_load(fileLocation, &widht, &height, &bitDepth, 0);

	if(!texData)
	{
		std::cout << "No Texture data in: " << fileLocation;
	} else
	{
		std::cout << fileLocation;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widht, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(texData);
}

void Texture::clearTexture()
{
	glDeleteTextures(1, &textureID);
	textureID = 0;
	widht = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = "";
}

void Texture::useTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

Texture::~Texture()
{
}