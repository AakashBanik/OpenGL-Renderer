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
	{return bufferwidht;}

	GLint getBufferHeight()
	{return bufferheight;}

	bool getShouldClose()
	{return glfwWindowShouldClose(mainWindow);}

	void swapBuffers() { glfwSwapBuffers(mainWindow); }

	bool* getKeys() { return keys; }

	GLfloat getxChange();
	GLfloat getyChange();
	
	~Window();

private:
	GLFWwindow* mainWindow;
	GLint width, height;
	GLint bufferwidht, bufferheight;
	bool keys[1024]; //covers all ascii range 0-1024
	GLfloat lastX, lastY, xChange, yChange;
	bool mouseFirstMoved;

	static void handleKeys(GLFWwindow* windows, int key, int code, int action, int mode);
	void createCallBacks();
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};

