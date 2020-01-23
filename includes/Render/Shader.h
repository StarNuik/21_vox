#pragma once

#include <string>
#include <glm/glm.hpp>

#include "Types.h"

class Shader {
public:
	Shader(std::string vertexPath, std::string fragmentPath);
	~Shader();
	void Use();
	void SetInt(std::string varName, int x);
	void SetFloat(std::string varName, float x);
	void SetFloat2(std::string varName, float x, float y);
	void SetFloat3(std::string varName, float x, float y, float z);
	void SetFloat4(std::string varName, float x, float y, float z, float w);
	void SetFloat2(std::string varName, glm::vec2 vector);
	void SetFloat3(std::string varName, glm::vec3 vector);
	void SetFloat4(std::string varName, glm::vec4 vector);
	void SetMatrix4(std::string varName, glm::mat4 matrix);
	uint GetId();
private:
	uint _id, _vs, _fs;
};
