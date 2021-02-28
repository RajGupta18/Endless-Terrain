#include "Light.h"
Light::Light() {
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;
	diffuseIntensity = 1.0f;
}

Light::Light(glm::vec3 col, GLfloat aIntensity, GLfloat dIntensity) {
	color = col;
	ambientIntensity = aIntensity;
	diffuseIntensity = dIntensity;
}

Light::~Light() {}