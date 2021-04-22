#include <string>

#include "Render/DirLight.h"
#include "Render/Shader.h"

DirLight::DirLight() {
	_direction = mathf::vec3(0.f);
	_diffuse = mathf::vec3(0.f);
	_ambient = mathf::vec3(0.f);
};

DirLight::~DirLight() {};

void DirLight::ApplySelf(Shader* shader, int num) {
	shader->SetFloat3("dirLight[" + std::to_string(num) + "].direction", _direction);
	shader->SetFloat3("dirLight[" + std::to_string(num) + "].diffuse", _diffuse);
	shader->SetFloat3("dirLight[" + std::to_string(num) + "].ambient", _ambient);
};

void DirLight::ApplySelfLight(Shader* shader, int num) {
	shader->SetFloat3("dirLight[" + std::to_string(num) + "].direction", _direction);
};

void DirLight::SetDirection(mathf::vec3 direction) {_direction = direction;};
void DirLight::SetDiffuse(mathf::vec3 diffuse) {_diffuse = diffuse;};
void DirLight::SetAmbient(mathf::vec3 ambient) {_ambient = ambient;};
mathf::vec3 DirLight::GetDirection() {return _direction;};
mathf::vec3 DirLight::GetDiffuse() {return _diffuse;};
mathf::vec3 DirLight::GetAmbient() {return _ambient;};

