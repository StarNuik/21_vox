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
	void SetPostShader(std::string);
	
	void AddModel(RenderModel*);
	void RemoveModel(RenderModel*);
	void RenderFrame();
private:
	void PrepareData();
	void RenderBlocks();
	void RenderPostprocess();

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
		Framebuffer* screenFbo;
		Shader* postShader;
		Geometry* postQuad;
	};
	class FrameData {
	public:
		glm::mat4 view;
		glm::mat4 projection;
		glm::mat4 vp;
		glm::vec3 cameraPos;
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
