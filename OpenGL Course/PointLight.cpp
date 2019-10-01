#include "PointLight.h"



PointLight::PointLight()
{
	position = glm::vec3(0, 0, 0);
	constant = 1.0;
	linear = 0.0;
	exponent = 0.0;
}

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue,
						GLfloat aIntensity, GLfloat dINtensity, 
						GLfloat xPos, GLfloat yPos, GLfloat zPos, 
						GLfloat con, GLfloat lin, GLfloat exp) : Light(red, green, blue, aIntensity, dINtensity)
{
	position = glm::vec3(xPos, yPos, zPos);
	constant = con;
	linear = lin;
	exponent = exp;
}

void PointLight::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation, GLfloat diffuseIntensityLocation, GLfloat positionalLocation, GLfloat constantLocation, GLfloat linearLocation, GLfloat exponentLocation)
{
	glUniform3f(ambientColorLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	glUniform3f(positionalLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);
}




PointLight::~PointLight()
{
}
