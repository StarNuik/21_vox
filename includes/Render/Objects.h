#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Types.h"
#include "Render/Camera.h"

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
private:
	uint _id, _vs, _fs;
};

class Texture {
public:
	Texture(std::string);
	~Texture();
	void Use();
private:
	uint _id;
};

class Geometry {
public:
	Geometry(std::string);
	Geometry(std::vector<float>);
	~Geometry();
	uint GetPolygonCount();
	void Use();
	static std::vector<float> ReadGeometry(std::string);
private:
	void Init(std::vector<float>);
	uint _polygonCount, _vbo, _vao;
};

class RenderModel {
public:
	RenderModel(Shader*, Texture*, Geometry*);
	RenderModel(Shader*, Texture*, Texture*, Geometry*);
	RenderModel(GLRenderer*, Shader*, Texture*, Geometry*);
	RenderModel(GLRenderer*, Shader*, Texture*, Texture*, Geometry*);
	~RenderModel();
	Shader* Use(Camera* camera);
	void SetPosition(glm::vec3);
	void SetRotation(glm::quat);
	void SetScale(glm::vec3);
	glm::vec3 GetPosition();
	glm::quat GetRotation();
	glm::vec3 GetScale();
	glm::mat4 GetAphineMatrix();
	uint GetPolygonCount();
	Geometry* GetGeometry();
private:
	void Init(Shader*, Texture*, Texture*, Geometry*);
	Shader* _shader;
	Texture* _diffuseTexture;
	Texture* _normalTexture;
	Geometry* _geometry;
	glm::vec3 _position, _scale;
	glm::quat _rotation;
	GLRenderer* _renderer;
};

class VertexBuffers {
public:
	enum BufferType {
		Top = 0,
		Bottom,
		Right,
		Left,
		Front,
		Back,
		MultiTop,
		MultiBottom,
		MultiRight,
		MultiLeft,
		MultiFront,
		MultiBack,
		Flower,
		First = Top,
		Last = Flower,
		Size = Last + 1
	};
	static void Init();
	static void Destroy();
	static float* GetBuffer(BufferType);
private:
	static float* _buffers[Size];
};