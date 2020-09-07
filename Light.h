#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dINtensity);
	~Light();

protected:
	glm::vec3 color;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;
};

