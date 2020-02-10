#include <GL/glew.h>
#include <algorithm>

#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

#include "Render/GLRenderer.h"
#include "Render/Framebuffer.h"
#include "Render/Skybox.h"
#include "Render/ShadowRenderer.h"
#include "Utilities/Log.h"
#include "Engine/Game.h"
#include "World/ResourceLoader.h"

GLRenderer::GLRenderer() {
	// _static.glfwOn = false;
	// _static.imguiOn = false;
	// _static.ready = false;
	// _static.width = 1;
	// _static.height = 1;
	// _static.cursorEnabled = true;
	// _static.window = nullptr;
	// _static.rendered = std::vector<RenderModel*>();
	// _static.activeCamera = nullptr;
	// _static.game = nullptr;
	// _static.framebuffer = nullptr;
	_static = {0};
	_static.screenFbo = new Framebuffer();
	_static.bloomFbo = new Framebuffer();
	_static.skybox = new Skybox();
	_static.shadows = new ShadowRenderer();
	_static.rendered = std::vector<RenderModel*>();
	_frame.view = glm::mat4(1.f);
	_frame.projection = glm::mat4(1.f);
	_frame.cameraPos = glm::vec3(0.f);
}

GLRenderer::~GLRenderer() {
	if (_static.imguiOn) {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	if (_static.window) {
		glfwDestroyWindow(_static.window);
	}
	if (_static.glfwOn) {
		glfwTerminate();
	}
	delete _static.screenFbo;
}

namespace {
GLFWwindow* InitGLFW(GLRenderer::RenderEngineConfig& config) {
	GLFWwindow* window;

	if (!glfwInit()) {
		Log::Error("[GLRenderer::GLRenderer]\nCouldn't init glfw.");
		//! Nice exit
		exit(1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifdef __APPLE__
		window = glfwCreateWindow(config.windowSize.x / 2, config.windowSize.y / 2, config.windowName.c_str(), nullptr, nullptr);
	#else
		window = glfwCreateWindow(config.windowSize.x, config.windowSize.y, config.windowName.c_str(), nullptr, nullptr);
	#endif
	if (!window) {
		Log::Error("[GLRenderer::GLRenderer]\nCouldn't initialize glfw window.");
		//! Nice exit
		exit(2);
	}
	glfwSetInputMode(window, GLFW_CURSOR, config.cursorEnabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
	glfwSetWindowAttrib(window, GLFW_RESIZABLE, config.windowResizeable ? GLFW_TRUE : GLFW_FALSE);
	if (glfwRawMouseMotionSupported() && config.cursorRaw) {
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}
	glfwSetWindowPos(window, config.windowPos.x, config.windowPos.y);
	glfwMakeContextCurrent(window);
	// glfwSetWindowUserPointer(window, _static.game);
	return window;
}

void InitGL(GLRenderer::RenderEngineConfig& config) {
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		Log::Error("[GLRenderer::GLRenderer]\nCouldn't initialize glew.");
		//! Nice exit
		exit(3);
	}
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glDepthMask(GL_TRUE);
};

void InitImgui(GLFWwindow* window) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 410 core");
}
}

void GLRenderer::Init(Game* game, RenderEngineConfig config) {
	_static.game = game;
	_static.windowSize = config.windowSize;

	_static.window = InitGLFW(config);
	_static.glfwOn = true;
	InitGL(config);
	InitImgui(_static.window);
	_static.imguiOn = true;
	_static.ready = true;

	Log::Success("[GLRenderer::GLRenderer]\nInitialized GLRenderer.");
};

void GLRenderer::InitChildren() {
	_static.screenFbo->NewColor(_static.windowSize);
	_static.bloomFbo->NewBloom(_static.windowSize);
	_static.skybox->Init(_static.game);
	_static.shadows->Init(_static.game);
	_static.rs = _static.game->GetResources();
	_static.ui = _static.game->GetUI();
	_static.postQuad = _static.rs->GetGeometry("Screen Quad");
	SetPostShader("Post Main: Bloom & HDR Aces approximate & Gamma");
	_static.bloomShader = _static.rs->GetShader("Post Gaussian Blur 5x5");
	_static.bloomCutoff = 1.0f;
};
