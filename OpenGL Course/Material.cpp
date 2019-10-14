#include "Material.h"
#include <iostream>


Material::Material()
{
	specularIntensity = 0.0;
	shininess = 0.0;
}

Material::Material(GLfloat sIntensity, GLfloat shine)
{
	std::cout << "[P8:GLCustom] Setting material types for textures \n";
	specularIntensity = sIntensity;
	shininess = shine;
}

void Material::UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation)
{
	glUniform1f(specularIntensityLocation, specularIntensity);
	glUniform1f(shininessLocation, shininess);
}



Material::~Material()
{
}
