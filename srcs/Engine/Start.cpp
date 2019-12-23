// #include "Engine/Engine.h"
// #include "Engine/Log.h"
// #include "Player/Player.h"
// #include "Generation/Map.h"
// #include "World/World.h"

#include "Render/GLRenderer.h"
#include "Engine/Game.h"
#include "Input/Input.h"
#include "World/ResourceLoader.h"
#include "UI/UIController.h"
#include "World/World.h"
#include "Generation/MapGeneration.h"
#include "Player/Player.h"
#include "Utilities/Log.h"
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
	_renderer = nullptr;
	_input = nullptr;
	_resources = nullptr;
	_world = nullptr;
	_ui = nullptr;
	_lastFrame = 0.f;
	_runtime = 0.f;
};

void Game::InitSystems() {
	
	_renderer = new GLRenderer(this, glConfig());
	_input = new Input();
	_resources = new ResourceLoader();
	_ui = new UIController(this);
	_world = new World(this);
	_mpGen = new MapGeneration();

	Entity* player = new Player(this);
	AddEntity(player);
};

#define WORLD_RADIUS 3

void Game::InitWorld() {
	Profiler::Prepare("Generation");
	Profiler::Prepare("Model");
	Profiler::Prepare("Model|Gen");
	Profiler::Prepare("Model|Block");
	Profiler::Prepare("Model|Get");
	Profiler::Prepare("Model|Pos");
	Profiler::Prepare("Model|Pos2");
	const int border = WORLD_RADIUS;
	for (int x = -border; x <= border; x++)
		for (int z = -border; z <= border; z++) {
			Profiler::Start("Generation");
			_world->GenerateChunk(glm::ivec2(x, z));
			Profiler::Add("Generation");
		}
	for (int x = -border; x <= border; x++)
		for (int z = -border; z <= border; z++) {
			Profiler::Start("Model");
			_world->ActivateChunk(glm::ivec2(x, z));
			Profiler::Add("Model");
		}
	Log::Basic("Generation total: " + std::to_string(Profiler::GetTotalS("Generation")) + "s");
	Log::Basic("Single chunk generation, on average: " + std::to_string(Profiler::GetAverageMs("Generation")) + "ms");
	Log::Warning("|       Name       |   Total (s)  | Average (ms)|");
	Log::Basic  ("|      Models      |   " + std::to_string(Profiler::GetTotalS("Model")) + "   |  " + std::to_string(Profiler::GetAverageMs("Model")) + "  |");
	// Log::Basic  ("|  Generate model  |   " + std::to_string(Profiler::GetTotalS("Model|Gen")) + "   |  " + std::to_string(Profiler::GetAverageMs("Model|Gen")) + "  |");
	// Log::Basic  ("|   Single block   |   " + std::to_string(Profiler::GetTotalS("Model|Block")) + "   |  " + std::to_string(Profiler::GetAverageMs("Model|Block")) + "  |");
	// Log::Basic  ("|    Pos floorf    |   " + std::to_string(Profiler::GetTotalS("Model|Pos")) + "   |  " + std::to_string(Profiler::GetAverageMs("Model|Pos")) + "  |");
	// Log::Basic  ("|      Pos mod     |   " + std::to_string(Profiler::GetTotalS("Model|Pos2")) + "   |  " + std::to_string(Profiler::GetAverageMs("Model|Pos2")) + "  |");
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
};