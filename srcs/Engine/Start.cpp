#include "Engine/Engine.h"
#include "Engine/Locator.hpp"
#include "Player/Player.h"
#include "World/World.h"

RenderEngineConfig glConfig() {
	RenderEngineConfig config;
	config.windowSize = glm::ivec2(1500, 900);
	config.windowPos = glm::ivec2(900, 100);
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
	_renderer = nullptr;
	_input = nullptr;
	_resources = nullptr;
	_world = nullptr;
	_ui = nullptr;
};

void Game::InitSystems() {
	ILogger* log = new TerminalLogger();
	Locator::provideLogger(log);
	
	_renderer = new GLRenderer(this, glConfig());
	_input = new Input();
	_resources = new ResourceLoader();
	_ui = new UIController(this);
	_world = new World(this);

	Entity* player = new Player(this);
	AddEntity(player);
};

void Game::InitWorld() {};

Game::~Game() {
	delete _input;
	delete _renderer;

	ILogger* log = Locator::getLogger();
	delete log;
};