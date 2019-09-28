#include "Window.h"



Window::Window()
{
	width = 800;
	height = 600;
	for (int i=0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

Window::Window(GLint width, GLint height)
{
	width = width;
	height = height;
	for (int i = 0; i < 1024; i++)
	{
		keys[i] = 0; //initialize all keys to false
	}
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

	//handle key and mouse input
	createCallBacks();

	//lock mouse and disable cursor to the viewport
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode) //handle key press events, params should match the callback function params to access the data
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window)); //get the pointer to the current window for the static function access

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE); //if ESC is pressed, close the window, 
	}

	if(key >= 0 && key < 1024)
	{
		if(action == GLFW_PRESS)
		{
			theWindow->keys[key] = true; //set the key pressed to true
			//std::cout << "Pressed: " << key <<"\n";
		}
		else if(action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//std::cout << "Released: " << key << "\n";
		}
	}
	
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos) //handle mouse movements and press events, params should match the callback function params to access the data
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window)); //get the pointer to the current window for the static function access

	if(theWindow->mouseFirstMoved) //if mouse is moved the first time, put the last value as the current value
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	//theWindow->xChange = xPos - theWindow->lastX; //change in value is the current minus the last value
	theWindow->xChange = theWindow->lastX - xPos; //for inverted controls
	theWindow->yChange = yPos - theWindow->lastY;//for inverted controls
	//theWindow->yChange = theWindow->lastY - yPos;//for normal controls

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;

	//std::cout << "x: " << double(theWindow->xChange) << " y: " << double(theWindow->yChange) << "\n";
	//printf("x: %.6f, y: %.6f\n", theWindow->xChange, theWindow->yChange);
}


void Window::createCallBacks() //callbacks for the mouse pointer values and key pressed events and action, also returns the current window pointer
{
	glfwSetKeyCallback(mainWindow, handleKeys);
	glfwSetCursorPosCallback(mainWindow, handleMouse);
}

GLfloat Window::getxChange()
{
	GLfloat theChange = xChange; //get the current change value and then initialize it to zero
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getyChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

Window::~Window()
{
}
