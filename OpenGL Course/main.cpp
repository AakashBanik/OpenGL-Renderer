#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <GLM/mat4x4.hpp>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

//window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
GLuint VAO, VBO, IBO, shader, uniformModel, uniformProjection;
const float toRadians = 3.14f / 180.0f;

float curAngle = 0.0f;

bool direction = true;

bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.005f;

//vertex shader written in GLSL, can be written in a separate file
static const char* vShader = "						            \n\
#version 330													\n\
																\n\
layout (location = 0) in vec3 pos;								\n\
																\n\
out vec4 vCol;													\n\
																\n\
uniform	mat4 model;												\n\
																\n\
uniform mat4 projection;										\n\
																\n\
void main()														\n\
{																\n\
	gl_Position = projection * model * vec4(pos, 1.0);			\n\
	vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);					\n\
}";

static const char* fShader = "						            \n\
#version 330													\n\
																\n\
out vec4 color;													\n\
in vec4 vCol;													\n\
void main()														\n\
{																\n\
	color = vCol;												\n\
}";
//end shaders

void create_triangle()
{	
	unsigned int Indices[] = { 
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2 
	};


	//vertices of the triangle min = 0 max = 1.0f (normalized scale)
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	//binding the vertex array objects to ids
	glGenVertexArrays(1 ,&VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
		
		//binding the vertex buffer objects to objects
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr); //can put zero instead of nullptr
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		

	glBindVertexArray(0); //unbind the vertex array consisting of vbo to gpu
	

}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLint theShader = glCreateShader(shaderType); //create shader

	//storing the shader code the "theCode" variable
	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codelength[1];
	codelength[0] = strlen(shaderCode);

	
	glShaderSource(theShader, 1, theCode, codelength); //assisgning the shader code to the given shader type
	glCompileShader(theShader); //compiling the shader

	//debugging the shader code as debugging in gpu is not possible
	GLint result = 0; //storing the result of the compilation
	GLchar eLog[1024] = { 0 };  //logging the error is present


	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result); //gets the compile status for the shader

	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		cout << "Error Compiling the " <<shaderType <<" shader : " << eLog;
	}

	glAttachShader(theProgram, theShader); //attaches the shader to the executable on the gpu

}

void compile_shaders()
{
	shader = glCreateProgram(); //creates the shader executable (or program executable) on the gpu for linking 

	if (!shader)
	{
		cout << "Error creating shader program";
		return;
	}

	AddShader(shader, vShader, GL_VERTEX_SHADER); //links the shader to the program, in this case, Vertex Shader
	AddShader(shader, fShader, GL_FRAGMENT_SHADER); // here it is fragment shader

	
	//debugging purposes, explained above
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader); //link the program and generate a executable
	glGetProgramiv(shader, GL_LINK_STATUS, &result);

	if(!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		cout << "Error Linking Program: " << eLog;
	}

	glValidateProgram(shader); 
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);

	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		cout << "Error Validating Program: " << eLog;
	}

	uniformModel = glGetUniformLocation(shader, "model"); //get the location of the shader var model and assign to uniformModel
	uniformProjection = glGetUniformLocation(shader, "projection");
}

int main()
{
	//initialize glfw
	if(!glfwInit())
	{
		cout << "GLFW Falied";
		glfwTerminate();
		return 1;
	}

	//glfw setup params and properties
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //use the latest glfw profile not the older ones, means no backward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //allow forward compatibility

	GLFWwindow *mainwindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", nullptr, nullptr); //create the main window with the height and width params

	if(!mainwindow)
	{
		cout << "GLFW Window Creation Failed";
		glfwTerminate();
		return 1;
	}

	//get buffer size info
	int bufferwidth, bufferheight;
	glfwGetFramebufferSize(mainwindow, &bufferwidth, &bufferheight);

	//set context for glew to use
	glfwMakeContextCurrent(mainwindow);

	//Allow modern extentions
	glewExperimental = GL_TRUE;

	//initialize glew
	if(glewInit() != GLEW_OK)
	{
		cout << "GLEW Failed";
		glfwDestroyWindow(mainwindow);
		glfwTerminate();
		return 1;
	}

	//setup viewport size
	glViewport(0, 0, bufferwidth, bufferheight);
	glEnable(GL_DEPTH_TEST);

	//calling the above functions to create the triangle and add and compile shaders
	create_triangle();
	compile_shaders();

	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)bufferwidth / (GLfloat)bufferheight, 0.1f, 100.0f);

	//Loop Until window closes
	while(!glfwWindowShouldClose(mainwindow))
	{
		//get and handle user input event
		glfwPollEvents();

		if(direction)
		{
			triOffset += triIncrement;
		} 
		else
		{
			triOffset -= triIncrement;
		}

		if(abs(triOffset) >= triMaxOffset)
		{
			direction = !direction;
		}

		curAngle += 0.5f;
		if (curAngle >= 360)
		{
			curAngle -= 360;
		}

		if(sizeDirection)
		{
			curSize += 0.001f;
		}
		else
		{
			curSize -= 0.001f;
		}

		if(curSize >= maxSize || curSize <= minSize)
		{
			sizeDirection = !sizeDirection;
		}

		//Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader); //using the executable with id "shader" in the gpu previously created

		glm::mat4 model; //construct identity matrix of 4x4 and assign to model
		model = translate(model, glm::vec3(0.0f, 0.0f, -3.5f)); //translate the model matrix i.e. change only the x and y value in this case for the triangle transformation
		model = rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = scale(model, glm::vec3(0.4f, 0.4f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model)); //bind the uniformModel to this new model 
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, value_ptr(projection));
		glBindVertexArray(VAO); //using the VAO for triangles 

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
			glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindVertexArray(0); //unbind the values
		
		glUseProgram(0); //unassign the program executable(shader)
		glfwSwapBuffers(mainwindow);
	}

	return 0;
}