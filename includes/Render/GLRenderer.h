#pragma once

// #include "Types.h"
// #include <string>
// #include <vector>
// #include <unordered_map>
// #include "Render/Objects.h"
// #include "Render/Camera.h"
// #include "Engine/Engine.h"

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <GLFW/glfw3.h>

#include "Types.h"

// class Game;

class GLRenderer {
public:
	class RenderEngineConfig;

	GLRenderer(Game*, RenderEngineConfig config);
	~GLRenderer();
	GLFWwindow* GetWindow();

	void AddModel(RenderModel*);
	void RemoveModel(RenderModel*);
	void SetActiveCamera(Camera*);
	Camera* GetActiveCamera();
	glm::ivec2 GetWindowSize();
	void SetCursor(bool enabled);
	
	void RenderFrame();
private:
	void SetDirLights();
	bool _glfwOn;
	bool _imguiOn;
	uint _width;
	uint _height;
	bool _cursorEnabled;
	GLFWwindow* _window;
	std::vector<RenderModel*> _rendered;
	Camera* _activeCamera;
	Game* _game;
};

class GLRenderer::RenderEngineConfig {
public:
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
	bool glCullCounterClockwise;
	bool glSeamlessCubeMap;
};
