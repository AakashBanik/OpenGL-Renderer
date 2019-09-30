#include "Camera.h"



Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp,  GLfloat startYaw, GLfloat startPitch, GLfloat startmoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;

	front = glm::vec3(0.0f, 0.0f, -1.0f);
	moveSpeed = startmoveSpeed;
	turnSpeed = startTurnSpeed;

	update();
}

Camera::Camera()
{
	
}



Camera::~Camera()
{
}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{

	if(keys[GLFW_KEY_W])
	{
		position += front * moveSpeed * deltaTime;
	}

	if (keys[GLFW_KEY_S])
	{
		position -= front * moveSpeed * deltaTime;
	}

	if (keys[GLFW_KEY_A])
	{
		position -= right * moveSpeed * deltaTime;
	}

	if (keys[GLFW_KEY_D])
	{
		position += right * moveSpeed * deltaTime;
	}
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if(pitch > 89.0)
	{
		pitch = 89.0;
	}

	if(pitch < -89.0)
	{
		pitch = -89.0;
	}

	update();
}


glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::getCameraPosition()
{
	return position;
}


