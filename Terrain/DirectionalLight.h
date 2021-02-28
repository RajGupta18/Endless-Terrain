#ifndef __TGEN_DIRECTLIGHT_H__
#define __TGEN_DIRECTLIGHT_H__

#include "Shader.h"
#include "Light.h"

class DirectionalLight :
	public Light
{
public:
	DirectionalLight();
	DirectionalLight(glm::vec3 col, GLfloat aIntensity, GLfloat dIntensity, glm::vec3 dir);
	void UseLight(Shader *shader);
	void UpdateDirection(glm::vec3 dir);
	~DirectionalLight();
private:
	glm::vec3 direction;
};
#endif