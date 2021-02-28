#ifndef __TGEN_CAMERA_H__
#define __TGEN_CAMERA_H__

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPos, glm::vec3 wUP, GLfloat startYaw, GLfloat startPitch , GLfloat turn, GLfloat move);
	glm::vec3 GetCameraPosition() { return position; }
	glm::mat4 GetViewMatrix();

	void MouseControls(GLfloat xChange, GLfloat yChange, GLfloat deltaTime);
	void KeyControls(bool* keys, GLfloat deltaTime);

	void SetCameraPosition(glm::vec3 newPos);

	~Camera();
private:
	glm::vec3 position, worldUP;
	glm::vec3 front, right;

	GLfloat Yaw, Pitch;

	GLfloat turnSpeed, moveSpeed;
	void Update();
};
#endif