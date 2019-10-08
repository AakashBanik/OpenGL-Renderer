#pragma once

#include <GL/glew.h>
#include "stb_image.h"

class Texture
{
public:
	Texture();
	Texture(const char *fileloc);

	bool loadTexture();
	bool loadTextureA(); //for textures with alpha channel
	void useTexture();
	void clearTexture();
	~Texture();

private:
	GLuint textureID;
	int widht, height, bitDepth;

	const char* fileLocation;
};

