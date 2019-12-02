#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Types.h"
#include <string>

class Shader {
public:
	Shader(std::string vertexPath, std::string fragmentPath);
	~Shader();
	void Use();
	void SetFloat(std::string varName, float x);
	void SetFloat2(std::string varName, float x, float y);
	void SetFloat3(std::string varName, float x, float y, float z);
	void SetFloat4(std::string varName, float x, float y, float z, float w);
	void SetFloat2(std::string varName, glm::vec2 vector);
	void SetFloat3(std::string varName, glm::vec3 vector);
	void SetFloat4(std::string varName, glm::vec4 vector);
	void SetMatrix4(std::string varName, glm::mat4 matrix);
private:
	uint _id, _vs, _fs;
};

class Texture {
public:
	Texture(std::string path);
	~Texture();
	void Use();
private:
	uint _id;
	std::string _name;
};

class Geometry {
public:
	Geometry();
	~Geometry();
	uint GetPolygonCount();
	void Use();
private:
	uint _polygonCount, _vbo, _vao;
	std::string _name;
};

class RenderModel {
public:
	RenderModel(Shader*, Texture*, Geometry*);
	RenderModel(Shader*, Texture*, Texture*, Geometry*);
	~RenderModel();
	void Use();
	void SetPosition(glm::vec3 position);
	void SetEuler(glm::vec3 eulerAngles);
	void SetRotation(glm::quat rotation);
	void SetScale(glm::vec3 scale);
	glm::vec3 GetPosition();
	glm::quat GetRotation();
	glm::vec3 GetScale();
private:
	Shader* shader;
	Texture* diffuseTexture;
	Texture* normalTexture;
	Geometry* geometry;
	glm::vec3 _position, _scale;
	glm::quat _rotation;
	std::string _name;
};