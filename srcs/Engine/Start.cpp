#include "Engine/Engine.h"
#include "Engine/Locator.hpp"
#include "Player/Player.h"
#include "Generation/Map.h"
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

	ILogger* log = new TerminalLogger();
	Locator::provideLogger(log);
	
	_renderer = new GLRenderer(this, glConfig());
	_input = new Input();
	_resources = new ResourceLoader();

	Entity* player = new Player(this);
	AddEntity(player);
	std::unordered_map<glm::ivec2, StoredMapData*> umap;
	MapGeneration* mp = new MapGeneration;
	mp->Generation(16.f, 16.f, umap);
	mp->SpawnObject(this, umap);
	// RenderModel* test = new RenderModel(_resources->GetShader("Base"), _resources->GetTexture("Stone"), _resources->GetGeometry("Cube"));
	// _renderer->AddModel(test);
	// Camera* camera = new Camera(_renderer, 90.f, 1.f, 100.f);
	// camera->SetPosition(glm::vec3(0.f, 2.f, 3.f));
	// _renderer->SetActiveCamera(camera);

	_ui = new UIController(this);

	// _world = new World(this);
	Shard* testShard = new Shard(this, glm::ivec3(0));
};

Game::~Game() {
	delete _input;
	delete _renderer;

	ILogger* log = Locator::getLogger();
	delete log;
};