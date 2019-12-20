#include <string>

#include "Render/DirLight.h"
#include "Render/Shader.h"

DirLight::DirLight() {
	_direction = glm::vec3(0.f);
	_diffuse = glm::vec3(0.f);
	_ambient = glm::vec3(0.f);
};

DirLight::~DirLight() {};

void DirLight::ApplySelf(Shader* shader, int num) {
	shader->SetFloat3("dirLight[" + std::to_string(num) + "].direction", _direction);
	shader->SetFloat3("dirLight[" + std::to_string(num) + "].diffuse", _diffuse);
	shader->SetFloat3("dirLight[" + std::to_string(num) + "].ambient", _ambient);
};

void DirLight::SetDirection(glm::vec3 direction) {_direction = direction;};
void DirLight::SetDiffuse(glm::vec3 diffuse) {_diffuse = diffuse;};
void DirLight::SetAmbient(glm::vec3 ambient) {_ambient = ambient;};
glm::vec3 DirLight::GetDirection() {return _direction;};
glm::vec3 DirLight::GetDiffuse() {return _diffuse;};
glm::vec3 DirLight::GetAmbient() {return _ambient;};

