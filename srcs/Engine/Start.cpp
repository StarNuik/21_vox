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

	_mpGen = new MapGeneration();
	// this->_mpGen->umap = new std::unordered_map<glm::ivec2, StoredMapData*>();
	// _mpGen->Generation(16.f, 16.f);
	// _mpGen->SpawnObject(this);
	std::cout << _mpGen << std::endl;
	std::cout << _mpGen->umap << std::endl;

	_ui = new UIController(this);

	Entity* player = new Player(this);
	AddEntity(player);
	// RenderModel* test = new RenderModel(_resources->GetShader("Base"), _resources->GetTexture("Stone"), _resources->GetGeometry("Cube"));
	// _renderer->AddModel(test);
	// Camera* camera = new Camera(_renderer, 90.f, 1.f, 100.f);
	// camera->SetPosition(glm::vec3(0.f, 2.f, 3.f));
	// _renderer->SetActiveCamera(camera);

	// _world = new World(this);
	// RenderModel* monkey = new RenderModel(_renderer, _resources->GetShader("Base"), _resources->GetTexture("Planks"), _resources->GetGeometry("Monkey"));
	// Shard* testShard = new Shard(this, glm::ivec3(0));
	// _world = new World(this);
	Shard* testShard = new Shard(this, glm::ivec3(0));
//	std::cout << _mpGen->a << std::endl;
	// std::cout << this->GetGeneration() << std::endl;
};

Game::~Game() {
	delete _input;
	delete _renderer;

	ILogger* log = Locator::getLogger();
	delete log;
};