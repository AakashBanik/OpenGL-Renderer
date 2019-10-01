#pragma once
#include <GL/glew.h>
#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight();

	DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, 
					GLfloat aIntensity,  GLfloat dINtensity,
					GLfloat xDir, GLfloat yDir, GLfloat zDir);

	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation,
		GLfloat diffuseIntensityLocation, GLfloat directionalLocation);

	~DirectionalLight();

private:
	glm::vec3 direction;
};

