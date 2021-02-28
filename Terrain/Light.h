#ifndef __TGEN_LIGHT_H__
#define __TGEN_LIGHT_H__

#include<GL/glew.h>
#include<glm/glm.hpp>

class Light
{
public:
	Light();
	Light(glm::vec3 col, GLfloat aIntensity, GLfloat dIntensity);
	~Light();
protected:
	glm::vec3 color;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;
};
#endif