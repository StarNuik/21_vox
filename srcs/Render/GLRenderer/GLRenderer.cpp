#include <GL/glew.h>
#include <algorithm>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Render/GLRenderer.h"
#include "Utilities/Locator.hpp"

GLRenderer::GLRenderer(Game* game, RenderEngineConfig config) {
	_game = game;
	_glfwOn = false;
	_imguiOn = false;
	_window = nullptr;
	_width = std::max((int)config.windowSize.x, 1);
	_height = std::max((int)config.windowSize.y, 1);
	_cursorEnabled = true;

	if (!glfwInit()) {
		Locator::GetLogger()->LogError("[GLRenderer::GLRenderer]\nCouldn't init glfw.");
		//! Nice exit
		exit(1);
	}
	_glfwOn = true;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config.glVersionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config.glVersionMinor);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, config.glForwardCompatibility);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	_window = glfwCreateWindow(_width, _height, config.windowName.c_str(), nullptr, nullptr);
	if (!_window) {
		Locator::GetLogger()->LogError("[GLRenderer::GLRenderer]\nCouldn't initialize glfw window.");
		//! Nice exit
		exit(1);
	}
	glfwSetInputMode(_window, GLFW_CURSOR, config.cursorEnabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
	glfwSetWindowAttrib(_window, GLFW_RESIZABLE, config.windowResizeable ? GLFW_TRUE : GLFW_FALSE);
	if (glfwRawMouseMotionSupported() && config.cursorRaw)
	{
		glfwSetInputMode(_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}
	glfwSetWindowPos(_window, config.windowPos.x, config.windowPos.y);
	glfwMakeContextCurrent(_window);
	// glfwSetWindowUserPointer(_window, this); //! Change to Game later

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		Locator::GetLogger()->LogError("[GLRenderer::GLRenderer]\nCouldn't initialize glew.");
		//! Nice exit
		exit(1);
	}
	if (config.glDepthTest) {
		glEnable(GL_DEPTH_TEST);
	}
	if (config.glCullFace) {
		glEnable(GL_CULL_FACE);
		if (config.glCullCounterClockwise)
			glFrontFace(GL_CCW);
		else
			glFrontFace(GL_CW);
	}
	if (config.glSeamlessCubeMap) {
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(_window, true);
	ImGui_ImplOpenGL3_Init("#version 410 core");
	_imguiOn = true;

	Locator::GetLogger()->LogSuccess("[GLRenderer::GLRenderer]\nInitialized GLRenderer.");
	//! Turn on imgui here
};

GLRenderer::~GLRenderer() {
	if (_imguiOn) {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	if (_window) {
		glfwDestroyWindow(_window);
	}
	if (_glfwOn) {
		glfwTerminate();
	}
}