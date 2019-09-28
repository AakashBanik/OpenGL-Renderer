#pragma once

#include <GL/glew.h>
#include "stb_image.h"

class Texture
{
public:
	Texture();
	Texture(const char *fileloc);

	void loadTexture();
	void useTexture();
	void clearTexture();
	~Texture();

private:
	GLuint textureID;
	int widht, height, bitDepth;

	const char* fileLocation;
};

