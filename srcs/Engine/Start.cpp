// #include "Engine/Engine.h"
// #include "Engine/Log.h"
// #include "Player/Player.h"
// #include "Generation/Map.h"
// #include "World/World.h"

#include <thread>

#include "Render/GLRenderer.h"
#include "Engine/Game.h"
#include "Engine/Physics.h"
#include "Input/Input.h"
#include "World/ResourceLoader.h"
#include "UI/UIController.h"
#include "World/World.h"
#include "Generation/MapGeneration.h"
#include "Player/Player.h"
#include "Utilities/Log.h"
#include "Utilities/Profiler.h"
#include "World/WorldCreator.h"

GLRenderer::RenderEngineConfig glConfig() {
	GLRenderer::RenderEngineConfig config;
	config.windowSize = glm::ivec2(3840, 2160);
	config.windowPos = glm::ivec2(0, 0);
	config.windowName = std::string("V O X");
	config.windowResizeable = false;
	config.cursorEnabled = false;
	config.cursorRaw = true;
	return config;
}

Game::Game() {
	_finished = false;
	_renderer = nullptr;
	_input = nullptr;
	_resources = nullptr;
	_world = nullptr;
	_ui = nullptr;
	_lastFrame = 0.f;
	_runtime = 0.f;
};

void Game::InitSystems() {
		
	_renderer = new GLRenderer();
	_renderer->Init(this, glConfig());
	_input = new Input();
	_resources = new ResourceLoader(this);
	_ui = new UIController(this);
	_worldCreator = new WorldCreator(this);
	_world = new World(this);
	_mpGen = new MapGeneration();
	_physics = new Physics(this);
	_player = new Player(this);
	AddEntity(_player);

	_renderer->InitChildren();
};

void Game::InitWorld() {
	_wcThread = std::thread(&WorldCreator::Start, _worldCreator);
};

void Game::DestroyWorld() {
	_world->DestroyWorld();
}

Game::~Game() {
	delete _mpGen;
	delete _world;
	delete _ui;
	delete _resources;
	delete _input;
	delete _renderer;
	delete _physics;
};