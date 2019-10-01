//Aakash Banik
//OpenGL

#define STB_IMAGE_IMPLEMENTATION

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
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "common.h"
#include "PointLight.h"

using namespace std;

Window mainWindow;
DirectionalLight mainLight;
vector<mesh*> meshList; //vector of type mesh pointers to store the different mesh objects created
vector<Shader> shaderList; //vector of type shader pointers to store the different shader objects created
Camera camera;

Material shinyMaterial;
Material dullMaterial;
PointLight pointLight[MAX_POINT_LIGHTS];

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

Texture brickTexture;
Texture dirtTexture;
Texture floorTexture;


//vertex Shader written in GLSL, can be written in a separate file
static const char* vShader = "Shaders/shader.vert"; //vertex shaders file location

static const char* fShader = "Shaders/shader.frag"; //fragment shader file location
//end shaders

void calcAverageNormal(unsigned int * indices, unsigned int indiceCount, GLfloat *vertices, 
	unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i+=3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i+1] * vLength;
		unsigned int in2 = indices[i+2] * vLength;

		glm::vec3 v1(vertices [in1] - vertices[in0], vertices [in1+1] - vertices[in0+1], vertices [in1+2] - vertices[in0+2]);
		glm::vec3 v2(vertices [in2] - vertices[in0], vertices [in2+1] - vertices[in0+1], vertices [in2+2] - vertices[in0+2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset;	in1+= normalOffset;		in2 += normalOffset;

		vertices[in0] += normal.x;	vertices[in0 + 1] += normal.y;	vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x;	vertices[in1 + 1] += normal.y;	vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x;	vertices[in2 + 1] += normal.y;	vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i<verticeCount/vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset+1], vertices[nOffset+2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x;	vertices[nOffset+1] = vec.y;	vertices[nOffset+2] = vec.z;
	}
}

//create objects i.e. VA/VB/IB O for the shapes to be rendered
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
	//   x		y		z		u		 v			normals
		-1.0f, -1.0f, -0.6f,    0.0f,	0.0f,	0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f,	   0.5f,	0.0f,	0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -0.6f,	   1.0f,	0.0f,	0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,	   0.5f,	1.0f,	0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = { 
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0, 0.0, -10.0,		0.0f, 0.0,		0.0, -1.0, 0.0,
		10.0f, 0.0, -10.0,		10.0, 0.0,		0.0, -1.0, 0.0,
		-10.0, 0.0, 10.0,		0.0, 10.0,		0.0, -1.0, 0.0,
		10.0, 0.0, 10.0,		10.0, 10.0,		0.0, -1.0, 0.0	
	};

	calcAverageNormal(Indices, 12, vertices, 32, 8, 5);

	//mesh objects for the VAO, VBO and the IBO for the given object to be rendered
	mesh *objs = new mesh();
	objs->createMesh(vertices, Indices, 32, 12);
	meshList.push_back(objs); //stores into a vertex array for a later use in the program

	//object for a second triangle
	mesh *objs2 = new mesh();
	objs2->createMesh(vertices, Indices, 32, 12);
	meshList.push_back(objs2);

	mesh *obj3 = new mesh();
	obj3->createMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);
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
	mainWindow = Window(1366, 768); //create a window, default const value (800x600), overloaded const for variable width and height
	mainWindow.initialize(); //initializes the window with proper attributes, check Window.cpp for further details
	
	glEnable(GL_DEPTH_TEST); //enable glew expertimental feature i.e depth 

	//calling the above functions to create the triangle and add and compile shaders
	create_objects(); //create/ initialize the different props to create the shapes
	createShaders(); //create shaders

	camera = Camera(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0), -90.0, 0.0, 2.0, 0.1);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.loadTexture();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.loadTexture();
	floorTexture = Texture("Textures/plain.png");
	floorTexture.loadTexture();

	shinyMaterial = Material(1.0f, 32);
	dullMaterial = Material(0.3f, 4);

	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 
								0.1f, 0.3f,
								0.0f, 0.0f, -1.0f);

	unsigned int pointLightCount = 0;

	pointLight[0] = PointLight(0.0, 0.0, 1.0,
								0.1, 0.4, 
								4.0, 0.0, 0.0, 
								0.3, 0.2, 0.1);
	pointLightCount++;

	pointLight[1] = PointLight(0.0, 1.0, 0.0,
								0.1, 1.0, 
								-4.0, 2.0, 0.0, 
								0.3, 0.1, 0.1);
	pointLightCount++;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, 
		uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0; //variables to take in the position of the "projection","model" and "view" variables from the shaders
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f); //creates a perspective model for viewing the shapes

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		//calculating the deltatime for uniformity across all computers
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		// Get + Handle User Input
		glfwPollEvents();

		//camera controls for keyboard and mouse (WASD)
		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getxChange(), mainWindow.getyChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear both the depth and the color buffer



		shaderList[0].UseShader(); //use shader that was previously created in the createShader function to render the obejcts

		uniformModel = shaderList[0].GetModelLocation(); //get the "model" var "location"
		uniformProjection = shaderList[0].GetProjectionLocation(); //get the "projection" var "location"
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].getEyePosition();
		uniformSpecularIntensity = shaderList[0].getSpecularIntensityLocation();
		uniformShininess = shaderList[0].getShininessLocation();

		shaderList[0].setDirectionalLight(&mainLight);
		shaderList[0].setPointLights(pointLight, pointLightCount);

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); //"projection" and projection linked
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix())); //"view" and the model linked
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		glm::mat4 model; //identity matrix

		//for the first model
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f)); //translate function applied to the model genrated
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f)); //scale function applied to the object
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); //"model" and the model linked
		brickTexture.useTexture();
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[0]->renderMesh(); //render the appropriate shape from the above vars and initializations (check mesh.h and mesh.cpp for further details)


		//for the second model
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		dirtTexture.useTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->renderMesh();

		//floor
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		floorTexture.useTexture();
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->renderMesh();

		glUseProgram(0); //unbind the program that was used

		mainWindow.swapBuffers();
	}

	return 0;
}