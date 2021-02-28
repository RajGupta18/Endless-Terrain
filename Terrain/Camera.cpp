#include "Camera.h"
Camera::Camera() {}

Camera::Camera(glm::vec3 startPos, glm::vec3 wUP, GLfloat startYaw, GLfloat startPitch, GLfloat turn, GLfloat move) {
	position = startPos;
	worldUP = wUP;
	Yaw = startYaw;
	Pitch = startPitch;
	turnSpeed = turn;
	moveSpeed = move;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	Update();
}

void Camera::SetCameraPosition(glm::vec3 newPos) {
	position = newPos;
}

glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(position, position + front, worldUP);
}

void Camera::Update() {
	front.y = sin(glm::radians(Pitch));
	front.x = cos(glm::radians(Pitch))*cos(glm::radians(Yaw));
	front.z = cos(glm::radians(Pitch))*sin(glm::radians(Yaw));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUP));
}

void Camera::KeyControls(bool* keys, GLfloat deltaTime) {
	GLfloat velocity = moveSpeed * deltaTime;
	if (keys[GLFW_KEY_LEFT_SHIFT]) velocity *= 10.0f;
	if (keys[GLFW_KEY_W]) position += front * velocity;
	if (keys[GLFW_KEY_S]) position -= front * velocity;
	if (keys[GLFW_KEY_A]) position -= right * velocity;
	if (keys[GLFW_KEY_D]) position += right * velocity;
}

void Camera::MouseControls(GLfloat xChange, GLfloat yChange, GLfloat deltaTime) {
	xChange *= turnSpeed * deltaTime;
	yChange *= turnSpeed * deltaTime;

	Yaw += xChange;
	Pitch += yChange;

	if (Pitch > 89.0f) Pitch = 89.0f;
	if (Pitch < -89.0f) Pitch= -89.0f;

	Update();
}

Camera::~Camera() {}