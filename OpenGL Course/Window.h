#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


class Window
{
public:
	Window();
	Window(GLint width, GLint height);

	int initialize();

	GLint getBufferWidth()
	{
		return bufferwidht;
	}

	GLint getBufferHeight()
	{
		return bufferheight;
	}

	bool getShouldClose()
	{
		return glfwWindowShouldClose(mainWindow);
	}

	void swapBuffers() { glfwSwapBuffers(mainWindow); }
	~Window();

private:
	GLFWwindow* mainWindow;
	GLint width, height;
	GLint bufferwidht, bufferheight;
};

