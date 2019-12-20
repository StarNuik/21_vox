#pragma once

#include <glm/glm.hpp>

#include "Types.h"

class DirLight {
public:
	DirLight();
	~DirLight();
	void ApplySelf(Shader*, int);
	void SetColor(glm::vec3);
	void SetDirection(glm::vec3);
private:
	glm::vec3 _direction;
	glm::vec3 _color;
};