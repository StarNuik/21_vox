#include "Engine/Engine.h"
#include "Engine/Locator.hpp"
#include "Player/Player.h"

RenderEngineConfig glConfig() {
	RenderEngineConfig config;
	config.windowSize = glm::ivec2(1920, 1080);
	config.windowPos = glm::ivec2(100, 100);
	config.windowName = std::string("Test");
	config.windowResizeable = false;
	config.cursorEnabled = true;
	config.cursorRaw = true;
	config.glVersionMajor = 4;
	config.glVersionMinor = 1;
	config.glForwardCompatibility = true;
	config.glDepthTest = true;
	config.glCullFace = false;
	return config;
}

Game::Game() {
	_finished = false;
	Locator::provideLogger(NULL);

	ILogger* log = new TerminalLogger();
	Locator::provideLogger(log);
	
	_renderer = new GLRenderer(glConfig());
	_input = new Input();
	_resources = new ResourceLoader();

	Entity* player = new Player(this);
	AddEntity(player);
	// RenderModel* test = new RenderModel(_resources->GetShader("Base"), _resources->GetTexture("Stone"), _resources->GetGeometry("Cube"));
	// _renderer->AddModel(test);
	// Camera* camera = new Camera(_renderer, 90.f, 1.f, 100.f);
	// camera->SetPosition(glm::vec3(0.f, 2.f, 3.f));
	// _renderer->SetActiveCamera(camera);
};

Game::~Game() {
	delete _input;
	delete _renderer;

	ILogger* log = Locator::getLogger();
	delete log;
};