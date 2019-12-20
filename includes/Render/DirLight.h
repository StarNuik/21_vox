#pragma once

#include <glm/glm.hpp>

#include "Types.h"

class DirLight {
public:
	DirLight();
	~DirLight();
	void ApplySelf(Shader*, int);
	void SetDirection(glm::vec3);
	void SetDiffuse(glm::vec3);
	void SetAmbient(glm::vec3);
	glm::vec3 GetDirection();
	glm::vec3 GetDiffuse();
	glm::vec3 GetAmbient();
private:
	glm::vec3 _direction;
	glm::vec3 _diffuse;
	glm::vec3 _ambient;
};