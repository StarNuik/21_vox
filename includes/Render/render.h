#pragma once

#include "Types.h"
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

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
	uint32 _id, _vs, _fs;
};

class Texture {
public:
	Texture();
	~Texture();
	void Load(std::string path);
	void Use();
private:
	uint32 _id;
};

class Geometry {
public:
	Geometry();
	~Geometry();
	uint32 GetPolygonCount();
	void Use();
private:
	uint32 _polygonCount, _vbo, _vao;
};

class RenderModel {
public:
	RenderModel();
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
};

struct RenderEngineConfig {
	glm::ivec2 windowSize;
	glm::ivec2 windowPos;
	std::string windowName;
	bool windowResizeable;
	bool cursorEnabled;
	uint32 glVersionMajor;
	uint32 glVersionMinor;
	bool glForwardCompatibility;
	bool glDepthTest;
	bool glCullFace;
};

class IRenderer {
public:
	virtual ~IRenderer() {};
	virtual void RegisterModel() = 0;
	virtual void UnregisterModel() = 0;
	virtual void RenderFrame() = 0;
};

class NullRenderer : public IRenderer {
public:
	~NullRenderer() {};
	void RegisterModel() {};
	void UnregisterModel() {};
	void RenderFrame() {};
};

class GLRenderer : public IRenderer {
public:
	GLRenderer(RenderEngineConfig config);
	~GLRenderer();
	void RegisterModel();
	void UnregisterModel();
	void RegisterModel(RenderModel* model);
	void UnregisterModel(RenderModel* model);
	void RenderFrame();
private:
	bool _glfwOn;
	bool _imguiOn;
	uint32 _width;
	uint32 _height;
	GLFWwindow* _window;
	std::vector<Shader*> _shaders;
	std::vector<Texture*> _textures;
	std::vector<Geometry*> _geometries;
	std::vector<RenderModel*> _rendered;
};