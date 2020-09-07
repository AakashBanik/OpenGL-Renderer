#include "Light.h"

Light::Light()
{
	color = glm::vec3(1.0, 1.0, 1.0);
	ambientIntensity = 0.4;
	diffuseIntensity = 0.6f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,GLfloat dINtensity)
{
	color = glm::vec3(red, green, blue);
	ambientIntensity = aIntensity;

	diffuseIntensity = dINtensity;
}



Light::~Light()
{
}
