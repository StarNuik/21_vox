#pragma once

#include <string>
#include <vector>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Mathf/Mathf.h"

#include "Mathf/Mathf.h"


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
	mathf::ivec2 GetWindowSize();
	void SetPostShader(std::string);
	float GetBloomCutoff();
	void SetBloomCutoff(float);
	
	void AddModel(RenderModel*);
	void RemoveModel(RenderModel*);
	void RenderFrame();
private:
	void PrepareData();
	void RenderBlocks();
	void RenderBloom();
	void RenderPostprocess();

	class StaticData {
	public:
		bool glfwOn;
		bool imguiOn;
		bool ready;
		mathf::ivec2 windowSize;
		bool cursorEnabled;
		float bloomCutoff;

		GLFWwindow* window;
		Game* game;
		ResourceLoader* rs;
		UIController* ui;

		std::vector<RenderModel*> rendered;
		Camera* activeCamera;
		Skybox* skybox;
		ShadowRenderer* shadows;
		Framebuffer* screenFbo;
		Framebuffer* bloomFbo;
		Shader* bloomShader;
		Shader* postShader;
		Geometry* postQuad;
	};
	class FrameData {
	public:
		mathf::mat4x4 view;
		mathf::mat4x4 projection;
		mathf::mat4x4 vp;
		mathf::vec3 cameraPos;
	};
	StaticData _static;
	FrameData _frame;
};

class GLRenderer::RenderEngineConfig {
public:
	mathf::ivec2 windowSize;
	mathf::ivec2 windowPos;
	std::string windowName;
	bool windowResizeable;
	bool cursorEnabled;
	bool cursorRaw;
	bool glCullFace;
	bool glCullCounterClockwise;
};
