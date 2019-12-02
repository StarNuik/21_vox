#pragma once

#include "Base.h"

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
	Texture();
	~Texture();
	void Load(std::string path);
	void Use();
private:
	uint _id;
};

class Geometry {
public:
	Geometry();
	~Geometry();
	uint GetPolygonCount();
	void Use();
private:
	uint _polygonCount, _vbo, _vao;
};

struct RenderEngineConfig {
	glm::ivec2 windowSize;
	glm::ivec2 windowPos;
	uint glVersionMajor;
	uint glVersionMinor;
	bool glForwardCompatibility;
	std::string windowName;
	bool windowResizeable;
	bool cursorEnabled;
	bool glDepthTest;
	bool glCullFace;
};

class RenderEngine {
public:
	RenderEngine(RenderEngineConfig config);
	~RenderEngine();
	void RegisterGeometry();
	void UnregisterGeometry();
private:
	bool _glfwOn;
	bool _imguiOn;
	uint _width;
	uint _height;
	GLFWwindow* _window;
	std::vector<Geometry*> _rendered;
};