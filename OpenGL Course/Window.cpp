#include "Window.h"



Window::Window()
{
	width = 800;
	height = 600;
}

Window::Window(GLint width, GLint height)
{
	width = width;
	height = height;
}

int Window::initialize()
{
	//initialize glfw
	if (!glfwInit())
	{
		std::cout << "GLFW Falied";
		glfwTerminate();
		return 1;
	}

	//glfw setup params and properties
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //use the latest glfw profile not the older ones, means no backward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //allow forward compatibility

	mainWindow = glfwCreateWindow(width, height, "Test Window", nullptr, nullptr); //create the main window with the height and width params

	if (!mainWindow)
	{
		std::cout << "GLFW Window Creation Failed";
		glfwTerminate();
		return 1;
	}

	//get buffer size info
	int bufferwidth, bufferheight;
	glfwGetFramebufferSize(mainWindow, &bufferwidth, &bufferheight);

	//set context for glew to use
	glfwMakeContextCurrent(mainWindow);

	//Allow modern extentions
	glewExperimental = GL_TRUE;

	//initialize glew
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW Failed";
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	//setup viewport size
	glViewport(0, 0, bufferwidth, bufferheight);
}

Window::~Window()
{
}
