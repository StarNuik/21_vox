#pragma once

#include <string>
#include <vector>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Types.h"

class GLRenderer {
public:
	class RenderEngineConfig;

	GLRenderer(Game*, RenderEngineConfig config);
	GLRenderer();
	~GLRenderer();
	void Init(Game*, RenderEngineConfig config);
	void InitChildren();

	GLFWwindow* GetWindow();
	void SetCursor(bool enabled);
	void SetActiveCamera(Camera*);
	Camera* GetActiveCamera();
	glm::ivec2 GetWindowSize();
	
	void AddModel(RenderModel*);
	void RemoveModel(RenderModel*);
	void RenderFrame();
private:
	void PrepareData();
	void RenderSkybox();
	void RenderShadows();
	void RenderBlocks();
	void RenderPostprocess();
	void RenderUI();

	class StaticData {
	public:
		bool glfwOn;
		bool imguiOn;
		bool ready;
		glm::ivec2 windowSize;
		bool cursorEnabled;

		GLFWwindow* window;
		Game* game;
		ResourceLoader* rs;
		UIController* ui;

		std::vector<RenderModel*> rendered;
		Camera* activeCamera;
		Skybox* skybox;
		ShadowRenderer* shadows;
		Framebuffer* framebuffer;
		Framebuffer* shadowFbo;
	};
	class FrameData {
	public:
		glm::mat4 view;
		glm::mat4 projection;
		glm::vec3 cameraPos;
		float dayTime;
		float sunAngle;
	};
	StaticData _static;
	FrameData _frame;
};

class GLRenderer::RenderEngineConfig {
public:
	glm::ivec2 windowSize;
	glm::ivec2 windowPos;
	std::string windowName;
	bool windowResizeable;
	bool cursorEnabled;
	bool cursorRaw;
	bool glCullFace;
	bool glCullCounterClockwise;
};
