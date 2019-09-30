#pragma once
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL\glew.h>

class Shader
{
public:
	Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);

	std::string ReadFile(const char* fileLocation);

	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	GLuint GetViewLocation();

	void UseShader();
	void ClearShader();

	GLuint getAmbientIntensityLocation();
	GLuint getAmbientColorLocation();
	GLuint getUnifromDirectionLocation();
	GLuint getunifromDiffuseIntensity();
	GLuint getSpecularIntensityLocation();
	GLuint getShininessLocation();
	GLuint getEyePosition();


	~Shader();

private:
	GLuint shaderID, uniformProjection, uniformModel, uniformView, 
	unifromAmbientIntensity, uniformAmbientColor, unifromDiffuseIntensity, unifromDirection,
	uniformEyePosition, unifromSpecularIntensity, uniformShininess;

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};
