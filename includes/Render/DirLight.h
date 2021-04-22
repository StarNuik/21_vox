#pragma once

#include <glm/glm.hpp>
#include "Mathf/Mathf.h"

#include "Types.h"

class DirLight {
public:
	DirLight();
	~DirLight();
	void ApplySelf(Shader*, int);
	void ApplySelfLight(Shader*, int);
	void SetDirection(mathf::vec3);
	void SetDiffuse(mathf::vec3);
	void SetAmbient(mathf::vec3);
	mathf::vec3 GetDirection();
	mathf::vec3 GetDiffuse();
	mathf::vec3 GetAmbient();
private:
	mathf::vec3 _direction;
	mathf::vec3 _diffuse;
	mathf::vec3 _ambient;
};