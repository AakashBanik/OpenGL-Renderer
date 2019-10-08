#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startmoveSpeed, GLfloat startTurnSpeed);
	~Camera();
	Camera();
	void keyControl(bool* keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange);
	glm::mat4 calculateViewMatrix();

	glm::vec3 getCameraPosition();
	glm::vec3 getCameraDirection();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw; //looking left-right
	GLfloat pitch; //looking up-down

	GLfloat moveSpeed;
	GLfloat turnSpeed;

	void update();
};

