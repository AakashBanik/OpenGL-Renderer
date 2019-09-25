#pragma once
#include <GL/glew.h>

class mesh
{
public:
	mesh();

	void createMesh(GLfloat *vertices, unsigned int *indices, unsigned int numofVertices, unsigned int numofIndices);
	void renderMesh();
	void clearMesh();
	~mesh();

private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount; //holds the number of points to draw
};

