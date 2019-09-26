//Aakash Banik
//OpenGL

#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <GLM/mat4x4.hpp>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh.h"
#include "Shader.h"
#include "Window.h"

using namespace std;

Window mainWindow;
//window dimensions

vector<mesh*> meshList; //vector of type mesh pointers to store the different mesh objects created
vector<Shader> shaderList; //vector of type shader pointers to store the different shader objects created

//vertex Shader written in GLSL, can be written in a separate file
static const char* vShader = "Shaders/shader.vert"; //vertex shaders file location

static const char* fShader = "Shaders/shader.frag"; //fragment shader file location
//end shaders

//create objects i.e. VA/B/IBO for the shapes to be rendered
void create_objects()
{	
	//indices to plot out the pyramids, 0 = -1.0f, -1.0f, 0.0f etc...
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

	//mesh objects for the VAO, VBO and the IBO for the given object to be rendered
	mesh *objs = new mesh();
	objs->createMesh(vertices, Indices, 12, 12);
	meshList.push_back(objs); //stores into a vertex array for a later use in the program

	//object for a second triangle
	mesh *objs2 = new mesh();
	objs2->createMesh(vertices, Indices, 12, 12);
	meshList.push_back(objs2);
}

//function to load in shaders and create them to render the object
void createShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{
	mainWindow = Window(); //create a window, default const value (800x600), overloaded const for variable width and height
	mainWindow.initialize(); //initializes the window with proper attributes, check Window.cpp for further details
	
	glEnable(GL_DEPTH_TEST); //enable glew expertimental feature i.e depth 

	//calling the above functions to create the triangle and add and compile shaders
	create_objects(); //create/ initialize the different props to create the shapes
	createShaders(); //create shaders

	GLuint uniformProjection = 0, uniformModel = 0; //variables to take in the position of the "projection" and "model" variables from the shaders
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f); //creates a perspective model for viewing the shapes

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		// Get + Handle User Input
		glfwPollEvents();

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear both the depth and the color buffer

		shaderList[0].UseShader(); //use shader that was previously created in the createShader function to render the obejcts
		uniformModel = shaderList[0].GetModelLocation(); //get the "model" var "location"
		uniformProjection = shaderList[0].GetProjectionLocation(); //get the "projection" var "location"

		glm::mat4 model; //identity matrix

		//for the first model
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f)); //translate function applied to the model genrated
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f)); //scale function applied to the object
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); //"model" and the model linked
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); //"projection" and projection linked
		meshList[0]->renderMesh(); //render the appropriate shape from the above vars and initializations (check mesh.h and mesh.cpp for further details)

		//for the second model
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, 0.9f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->renderMesh();

		glUseProgram(0); //unbind the program that was used

		mainWindow.swapBuffers();
	}

	return 0;
}