#include "mesh.h"

mesh::mesh()
{
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
}

void mesh::createMesh(GLfloat *vertices, unsigned int *indices, unsigned int numofVertices, unsigned int numofIndices)
{
	indexCount = numofIndices;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0])*numofIndices, indices, GL_STATIC_DRAW);

	//binding the vertex buffer objects to objects
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0])*numofVertices, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr); //can put zero instead of nullptr
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0); //unbind the vertex array consisting of vbo to gpu
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind the IBO
}

void mesh::renderMesh()
{
	//can put a if statement for 0 indexCount, not top draw then
	glBindVertexArray(VAO); //using the VAO for triangles 

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0); //unbind the values
}

void mesh::clearMesh()
{
	if (IBO != 0)
	{
		glDeleteBuffers(1, &IBO); //delete data buffers in  gpu mem
		IBO = 0; 
	}

	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO); //delete data buffers in  gpu mem
		VBO = 0;
	}

	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VBO); //delete data buffers in  gpu mem
		VAO = 0;
	}
}


mesh::~mesh()
{
	clearMesh();
}
