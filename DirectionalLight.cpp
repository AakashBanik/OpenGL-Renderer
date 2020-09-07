#include "DirectionalLight.h"
#include <iostream>

DirectionalLight::DirectionalLight()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, 
								   GLfloat aIntensity, GLfloat dINtensity,
								   GLfloat xDir, GLfloat yDir, GLfloat zDir) : Light(red, green, blue, aIntensity, dINtensity)
{
	std::cout << "[P9:GLCustom] Initializing Directional Light \n";
	direction = glm::vec3(xDir, yDir, zDir);
}

void DirectionalLight::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation, GLfloat diffuseIntensityLocation, GLfloat directionalLocation)
{
	glUniform3f(ambientColorLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform3f(directionalLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}
DirectionalLight::~DirectionalLight()
{
	
}