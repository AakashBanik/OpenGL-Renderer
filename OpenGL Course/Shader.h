#pragma once
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include "DirectionalLight.h"
#include "PointLight.h"
#include <GL\glew.h>
#include "common.h"
#include "SpotLight.h"

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

	void setDirectionalLight(DirectionalLight *dLight);
	void setPointLights(PointLight *pLight, unsigned int lightCount);
	void setSpotLights(SpotLight *sLight, unsigned int lightCount);


	~Shader();

private:
	int pointLightCount, spotlightCount;

	GLuint shaderID, uniformProjection, uniformModel, uniformView,
	uniformEyePosition, unifromSpecularIntensity, uniformShininess;

	struct
	{
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformDirection;
	}uniformDirectionalLight;

	GLuint uniformPointLightCount, unifromSpotlightCount;

	struct
	{
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;

		GLuint unifromDirection;
		GLuint uniformEdge;
	}uniformSpotLight[MAX_SPOTLIGHT];

	struct
	{
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	}uniformPointLight[MAX_POINT_LIGHTS];

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};
