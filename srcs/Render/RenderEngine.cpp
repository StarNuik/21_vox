#include "Render/Render.h"

RenderEngine::RenderEngine(RenderEngineConfig config) {
	_glfwOn = false;
	_imguiOn = false;
	_window = nullptr;
	_width = std::max(config.windowSize.x, 1);
	_height = std::max(config.windowSize.y, 1);

	if (!glfwInit()) {
		//! Log and nice exit
		exit(1);
	}
	_glfwOn = true;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config.glVersionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config.glVersionMinor);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, config.glForwardCompatibility);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	_window = glfwCreateWindow(_width, _height, config.windowName.c_str(), nullptr, nullptr);
	if (!_window) {
		//! Log and nice exit
		exit(1);
	}
	glfwSetWindowAttrib(_window, GLFW_RESIZABLE, config.windowResizeable ? GLFW_TRUE : GLFW_FALSE);
	glfwSetInputMode(_window, GLFW_CURSOR, config.cursorEnabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
	glfwSetWindowPos(_window, config.windowPos.x, config.windowPos.y);
	glfwMakeContextCurrent(_window);
	glfwSetWindowUserPointer(_window, this); //! Change to Game later

	glewExperimental = GL_TRUE;
	glewInit();
	if (config.glDepthTest)
		glEnable(GL_DEPTH_TEST);
	if (config.glCullFace)
		glEnable(GL_CULL_FACE);
	
	//! Turn on imgui here
	//! Log renderer success
};