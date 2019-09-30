#include "Light.h"

Light::Light()
{
	color = glm::vec3(1.0, 1.0, 1.0);
	ambientIntensity = 1;

	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	diffuseIntensity = 0.0f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dINtensity)
{
	color = glm::vec3(red, green, blue);
	ambientIntensity = aIntensity;

	direction = glm::vec3(xDir, yDir, zDir);
	diffuseIntensity = dINtensity;
}

void Light::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation, GLfloat diffuseIntensityLocation, GLfloat directionalLocation)
{
	glUniform3f(ambientColorLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform3f(directionalLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}


Light::~Light()
{
}
