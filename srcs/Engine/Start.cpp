// #include "Engine/Engine.h"
// #include "Engine/Locator.hpp"
// #include "Player/Player.h"
// #include "Generation/Map.h"
// #include "World/World.h"

#include "Render/GLRenderer.h"
#include "Engine/Game.h"
#include "Utilities/Locator.hpp"
#include "Input/Input.h"
#include "World/ResourceLoader.h"
#include "UI/UIController.h"
#include "World/World.h"
#include "Generation/MapGeneration.h"
#include "Player/Player.h"
#include "Utilities/Profiler.h"


GLRenderer::RenderEngineConfig glConfig() {
	GLRenderer::RenderEngineConfig config;
	config.windowSize = glm::ivec2(1500, 900);
	config.windowPos = glm::ivec2(900, 100);
	config.windowName = std::string("Test");
	config.windowResizeable = false;
	config.cursorEnabled = false;
	config.cursorRaw = true;
	config.glVersionMajor = 4;
	config.glVersionMinor = 1;
	config.glForwardCompatibility = true;
	config.glDepthTest = true;
	config.glCullFace = false;
	config.glCullCounterClockwise = false;
	config.glSeamlessCubeMap = true;
	return config;
}

Game::Game() {
	_finished = false;
	Locator::ProvideLogger(NULL);
	_renderer = nullptr;
	_input = nullptr;
	_resources = nullptr;
	_world = nullptr;
	_ui = nullptr;
	_lastFrame = 0.f;
	_runtime = 0.f;
};

void Game::InitSystems() {
	ILogger* log = new TerminalLogger();
	Locator::ProvideLogger(log);

	Profiler::SetResolution(Profiler::Nanoseconds);
	Profiler::Prepare("FrameFull");
	Profiler::Prepare("Input");
	Profiler::Prepare("Update");
	Profiler::Prepare("RenderFull");
	
	_renderer = new GLRenderer(this, glConfig());
	_input = new Input();
	_resources = new ResourceLoader();
	_ui = new UIController(this);
	_world = new World(this);
	_mpGen = new MapGeneration();

	Entity* player = new Player(this);
	AddEntity(player);
};

#define WORLD_RADIUS 4

void Game::InitWorld() {
	const int border = WORLD_RADIUS;
	Profiler::Start("Generation");
	for (int x = -border; x <= border; x++)
		for (int z = -border; z <= border; z++)
			_world->GenerateChunk(glm::ivec2(x, z));
	Profiler::End("Generation");
	Profiler::Start("Models");
	for (int x = -border; x <= border; x++)
		for (int z = -border; z <= border; z++)
			_world->ActivateChunk(glm::ivec2(x, z));
	Profiler::End("Models");
	Locator::GetLogger()->LogWarning("Generation took " + std::to_string(Profiler::Getf("Generation") * 0.001f) + "s");
	Locator::GetLogger()->LogWarning("Model creation took " + std::to_string(Profiler::Getf("Models") * 0.001f) + "s");
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

	ILogger* log = Locator::GetLogger();
	delete log;
};