#include "DirectionalLight.h"
DirectionalLight::DirectionalLight() : Light() {
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::DirectionalLight(glm::vec3 col, GLfloat aIntensity, GLfloat dIntensity, glm::vec3 dir) : Light(col, aIntensity, dIntensity) {
	direction = dir;
}

void DirectionalLight::UseLight(Shader *shader) {
	shader->SetFloat1("directionlight.base.intensity", ambientIntensity);
	shader->SetFloat1("directionlight.base.diffuseIntensity", diffuseIntensity);
	shader->SetFloat3("directionlight.base.color", color.x, color.y, color.z);
	shader->SetFloat3("directionlight.direction", direction.x, direction.y, direction.z);
}

void DirectionalLight::UpdateDirection(glm::vec3 dir) {
	direction = dir;
}

DirectionalLight::~DirectionalLight() {}