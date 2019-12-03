#pragma once

#include "Types.h"
#include <string>
#include <vector>
#include <unordered_map>
#include "Render/Objects.h"
#include "Render/Camera.h"

struct RenderEngineConfig {
	glm::ivec2 windowSize;
	glm::ivec2 windowPos;
	std::string windowName;
	bool windowResizeable;
	bool cursorEnabled;
	bool cursorRaw;
	uint glVersionMajor;
	uint glVersionMinor;
	bool glForwardCompatibility;
	bool glDepthTest;
	bool glCullFace;
};

class GLRenderer {
public:
	GLRenderer(RenderEngineConfig config);
	~GLRenderer();
	GLFWwindow* GetWindow();

	// void AddShader(std::string, Shader*);
	// void RemoveShader(std::string);
	// Shader* GetShader(std::string);

	// void AddTexture(std::string, Texture*);
	// void RemoveTexture(std::string);
	// Texture* GetTexture(std::string);

	// void AddGeometry(std::string, Geometry*);
	// void RemoveGeometry(std::string);
	// Geometry* GetGeometry(std::string);

	void AddModel(RenderModel*);
	void RemoveModel(RenderModel*);
	void SetActiveCamera(Camera*);
	// RenderModel* GetModel(std::string);

	void RenderFrame();
	glm::ivec2 GetWindowSize();
private:
	bool _glfwOn;
	bool _imguiOn;
	uint _width;
	uint _height;
	GLFWwindow* _window;
	// std::unordered_map<std::string, Shader*> _shaders;
	// std::unordered_map<std::string, Texture*> _textures;
	// std::unordered_map<std::string, Geometry*> _geometries;
	std::vector<RenderModel*> _rendered;
	Camera* _activeCamera;
};