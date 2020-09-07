#include "SpotLight.h"



SpotLight::SpotLight() :PointLight()
{
	direction = glm::vec3 (0.0f, -1.0f, 0.0f);
	edge = 0.0;
	procEdge = cosf(glm::radians(edge));
}

SpotLight::SpotLight(GLfloat red, GLfloat green, GLfloat blue, 
					GLfloat aIntensity, GLfloat dINtensity, 
					GLfloat xPos, GLfloat yPos, GLfloat zPos, 
					GLfloat xDir, GLfloat yDir, GLfloat zDir, 
					GLfloat con, GLfloat lin, GLfloat exp, GLfloat edg) :PointLight(red, green, blue, aIntensity, dINtensity, xPos, yPos, zPos, con, lin, exp)
{
	direction = glm::normalize(glm::vec3(xDir, yDir, zDir));
	edge = edg;
	procEdge = cosf(glm::radians(edge));
}

void SpotLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, 
						GLuint diffuseIntensityLocation, GLuint positionalLocation,
						GLuint directionLoc, GLuint constantLocation, GLuint linearLocation, 
						GLuint exponentLocation, GLuint edgeLocation)
{
	glUniform3f(ambientColorLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	glUniform3f(positionalLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);

	glUniform3f(directionLoc, direction.x, direction.y, direction.z);
	glUniform1f(edgeLocation, procEdge);
}

void SpotLight::SetFlash(glm::vec3 pos, glm::vec3 dir)
{
	position = pos;
	direction = dir;
}



SpotLight::~SpotLight()
{
}
