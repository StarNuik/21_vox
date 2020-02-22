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

#define WORLD_RADIUS 10

void Game::InitWorld() {
	// _worldCreator->Start();
	_wcThread = std::thread(&WorldCreator::Start, _worldCreator);
	// _wcThread.detach();
	// std::thread a(&WorldCreator::Start, _worldCreator);

	Profiler::Prepare("Generation");
	Profiler::Prepare("Geometry");
	Profiler::Prepare("GL Geometry");
	const int border = WORLD_RADIUS;
	Profiler::Start("Generation");
	for (int x = -border; x <= border; x++)
		for (int z = -border; z <= border; z++) {
			_world->GenerateChunk(glm::ivec2(x, z));
			Profiler::Add("Generation");
		}
	Profiler::Start("Geometry");
	for (int x = -border; x <= border; x++)
		for (int z = -border; z <= border; z++) {
			_world->ActivateChunk(glm::ivec2(x, z));
			Profiler::Add("Geometry");
		}
	Log::Important("[Chunk count : ", (WORLD_RADIUS * 2 + 1) * (WORLD_RADIUS * 2 + 1), "]");
	Log::Important("[Generation T: ", Profiler::GetS("Generation"), " s, A: ", Profiler::GetAverageMs("Generation"), " ms ]");
	Log::Important("[Geometry   T: ", Profiler::GetS("Geometry"), " s,  A: ", Profiler::GetAverageMs("Geometry"), " ms ]");
	Log::Important("[GL Context T: ", Profiler::GetS("GL Geometry"), "s, A: ", Profiler::GetAverageMs("GL Geometry"), " ms ]");
	Log::Important("[Total       : ", Profiler::GetS("Generation") + Profiler::GetS("Geometry"), " s ]");
};

void Game::DestroyWorld() {
	const int border = WORLD_RADIUS;
	for (int x = -border; x <= border; x++)
		for (int z = -border; z <= border; z++)
			_world->DestroyChunk(glm::ivec2(x, z));
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