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

#include "World/Shard.h"
void Game::InitWorld() {
	// for (int x = -1; x <= 1; x++)
	// 	for (int z = -1; z <= 1; z++)
	// 		_world->GenerateChunk(glm::ivec2(x, z));
	// for (int x = -1; x <= 1; x++)
	// 	for (int z = -1; z <= 1; z++)
	// 		_world->ActivateChunk(glm::ivec2(x, z));

	_world->GenerateChunk(glm::ivec2(0));
	_world->ActivateChunk(glm::ivec2(0));

	// Shard* s = new Shard(this, glm::ivec3(0));
	// s->SetActive(true);
};

Game::~Game() {
	delete _input;
	delete _renderer;

	ILogger* log = Locator::getLogger();
	delete log;
};