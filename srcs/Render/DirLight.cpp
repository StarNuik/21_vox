#include <string>

#include "Render/DirLight.h"
#include "Render/Shader.h"

DirLight::DirLight() {
	_direction = glm::vec3(0.f);
	_color = glm::vec3(0.f);
};

DirLight::~DirLight() {};

void DirLight::ApplySelf(Shader* shader, int num) {
	shader->SetFloat3("dirLight[" + std::to_string(num) + "].direction", _direction);
	shader->SetFloat3("dirLight[" + std::to_string(num) + "].color", _color);
};

void DirLight::SetColor(glm::vec3 color) {_color = color;};
void DirLight::SetDirection(glm::vec3 direction) {_direction = direction;};
