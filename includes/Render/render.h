#pragma once

#include "Types.h"
#include <string>
#include <vector>
#include <unordered_map>
#include "Render/Objects.h"
#include "Render/Camera.h"
#include "Engine/Engine.h"

class Game;

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
	GLRenderer(Game*, RenderEngineConfig config);
	~GLRenderer();
	GLFWwindow* GetWindow();

	void AddModel(RenderModel*);
	void RemoveModel(RenderModel*);
	void SetActiveCamera(Camera*);
	Camera* GetActiveCamera();
	glm::ivec2 GetWindowSize();
	
	void RenderFrame();
private:
	bool _glfwOn;
	bool _imguiOn;
	uint _width;
	uint _height;
	GLFWwindow* _window;
	std::vector<RenderModel*> _rendered;
	Camera* _activeCamera;
	Game* _game;
};