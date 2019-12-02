#pragma once

#include "Types.h"
#include <string>
#include <vector>
#include <unordered_map>
#include "Render/Objects.h"

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

class GLRenderer {
public:
	GLRenderer(RenderEngineConfig config);
	~GLRenderer();

	void AddShader(std::string, Shader*);
	void RemoveShader(std::string);
	Shader* GetShader(std::string);

	void AddTexture(std::string, Texture*);
	void RemoveTexture(std::string);
	Texture* GetTexture(std::string);

	void AddGeometry(std::string, Geometry*);
	void RemoveGeometry(std::string);
	Geometry* GetGeometry(std::string);

	void AddModel(RenderModel*);
	void RemoveModel(RenderModel*);
	// RenderModel* GetModel(std::string);

	void RenderFrame();
private:
	bool _glfwOn;
	bool _imguiOn;
	uint32 _width;
	uint32 _height;
	GLFWwindow* _window;
	std::unordered_map<std::string, Shader*> _shaders;
	std::unordered_map<std::string, Texture*> _textures;
	std::unordered_map<std::string, Geometry*> _geometries;
	std::vector<RenderModel*> _rendered;
};