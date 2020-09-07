#pragma once
#include "Light.h"

class PointLight : public Light
{
public:
	PointLight();

	PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dINtensity,
				GLfloat xPos, GLfloat yPos, GLfloat zPos,
				GLfloat con, GLfloat lin, GLfloat exp);

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
		GLuint diffuseIntensityLocation, GLuint positionalLocation,
				GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);

	~PointLight();

protected:
	glm::vec3 position;

	GLfloat constant, linear, exponent; //ax^2+bx+c

};

