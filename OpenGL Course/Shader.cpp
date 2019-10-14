#include "Shader.h"
#include <chrono>

Shader::Shader()
{
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
	spotlightCount = 0;
	pointLightCount = 0;
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
	auto start = std::chrono::steady_clock::now();
	std::cout<<"[P3:GLCustom] Loading Vertex Shaders from File(s): " <<vertexLocation <<"\n";
	std::cout<<"[P3.1:GLCustom] Loading Fragment Shaders from File(s): " <<fragmentLocation;
	std::string vertexString = ReadFile(vertexLocation); //read the file using ifstream 
	std::string fragmentString = ReadFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str(); //convert into char pointer string
	const char* fragmentCode = fragmentString.c_str();
	auto end = std::chrono::steady_clock::now();
	std::cout<<" Execution Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() <<" milliseconds \n";
	CompileShader(vertexCode, fragmentCode);
}

std::string Shader::ReadFile(const char* fileLocation)
{
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in); //file io var

	if (!fileStream.is_open()) {
		printf("[ERROR:GLCustom] Failed to read %s! File doesn't exist.", fileLocation);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n"); //write file contents to this variable (output = "#version 330      \n\"...)
	}

	fileStream.close();
	return content;
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
	auto start = std::chrono::steady_clock::now();
	std::cout<<"[P4:GLCustom] Started Compiling shaders from files\n";
	shaderID = glCreateProgram(); //create shader program executable in the GPU memory

	if (!shaderID) //if executable with appropriate shader id not found ERROR!
	{
		printf("[ERROR:GLCustom] Error creating shader program!\n");
		return;
	}

	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER); //create shaders from the shader file that was just imported (vertex shaders)
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER); //create shaders from the shader file that was just imported (fragment shaders)

	//for the debugging the OpenGL code as debugging on GPU is hard or is not possible
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result); //linker result stored to the result varibale reference
	if (!result) //if error, display it
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("[ERROR:GLCustom] Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result); //program validation status stored to the result varibale reference
	if (!result)//if error, display it
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("[ERROR:GLCustom] Error validating program: '%s'\n", eLog);
		return;
	}

	uniformProjection = glGetUniformLocation(shaderID, "projection"); //get the location of the "projection" variable from the shader files
	uniformModel = glGetUniformLocation(shaderID, "model"); //get the location of the "model" variable from the shader files
	uniformView = glGetUniformLocation(shaderID, "view");
	uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
	uniformDirectionalLight.uniformColor = glGetUniformLocation(shaderID, "directionalLight.base.color");
	uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
	uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
	unifromSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
	uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
	uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");

	uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");

	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.color", i);
		uniformPointLight[i].uniformColor = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
		uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
		uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
		uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
		uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
		uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", i);
		uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);
	}

	unifromSpotlightCount = glGetUniformLocation(shaderID, "spotLightCount");

	for (size_t i = 0; i < MAX_SPOTLIGHT; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.color", i);
		uniformSpotLight[i].uniformColor = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.ambientIntensity", i);
		uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.diffuseIntensity", i);
		uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.position", i);
		uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.constant", i);
		uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.linear", i);
		uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.exponent", i);
		uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", i);
		uniformSpotLight[i].unifromDirection = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", i);
		uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderID, locBuff);
	}

	std::cout <<"[P4.1:GLCustom] Finished compiling shaders from the files ";
	auto end = std::chrono::steady_clock::now();
	std::cout<<"Execution Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() <<" milliseconds \n";
}

void Shader::setDirectionalLight(DirectionalLight* dLight)
{
	dLight->UseLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColor,
					uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}

void Shader::setPointLights(PointLight* pLight, unsigned lightCount)
{
	if(lightCount > MAX_POINT_LIGHTS)
	{
		lightCount = MAX_POINT_LIGHTS;
	}

	glUniform1i(uniformPointLightCount, lightCount);

	for(size_t i = 0; i < lightCount; i++)
	{
		pLight[i].UseLight(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColor, 
							uniformPointLight[i].uniformDiffuseIntensity, uniformPointLight[i].uniformPosition,
							uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent);

	}
}

void Shader::setSpotLights(SpotLight* sLight, unsigned lightCount)
{
	
	if(lightCount > MAX_SPOTLIGHT)
	{
		lightCount = MAX_SPOTLIGHT;
	}

	glUniform1i(unifromSpotlightCount, lightCount);

	for(size_t i = 0; i < lightCount; i++)
	{
		sLight[i].UseLight(uniformSpotLight[i].uniformAmbientIntensity, uniformSpotLight[i].uniformColor, 
							uniformSpotLight[i].uniformDiffuseIntensity, uniformSpotLight[i].uniformPosition, uniformSpotLight[i].unifromDirection,
							uniformSpotLight[i].uniformConstant, uniformSpotLight[i].uniformLinear, uniformSpotLight[i].uniformExponent,
							uniformSpotLight[i].uniformEdge);

	}
}



GLuint Shader::GetProjectionLocation()
{
	return uniformProjection;
}
GLuint Shader::GetModelLocation()
{
	return uniformModel;
}

GLuint Shader::GetViewLocation()
{
	return uniformView;
}

GLuint Shader::getAmbientColorLocation()
{
	return uniformDirectionalLight.uniformColor;
}

GLuint Shader::getAmbientIntensityLocation()
{
	return uniformDirectionalLight.uniformAmbientIntensity;
}

GLuint Shader::getUnifromDirectionLocation()
{
	return uniformDirectionalLight.uniformDirection;
}

GLuint Shader::getunifromDiffuseIntensity()
{
	return uniformDirectionalLight.uniformDiffuseIntensity;
}

GLuint Shader::getShininessLocation()
{
	return uniformShininess;
}

GLuint Shader::getSpecularIntensityLocation()
{
	return unifromSpecularIntensity;
}


GLuint Shader::getEyePosition()
{
	return uniformEyePosition;
}






void Shader::UseShader()
{
	glUseProgram(shaderID);
}

void Shader::ClearShader()
{
	if (shaderID != 0)
	{
		glDeleteProgram(shaderID);
		shaderID = 0;
	}

	uniformModel = 0;
	uniformProjection = 0;
}


void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("[ERROR:GLCustom] Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
}

Shader::~Shader()
{
	ClearShader();
}
