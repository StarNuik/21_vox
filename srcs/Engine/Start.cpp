#include "Engine/Engine.h"
#include "Engine/Locator.hpp"

RenderEngineConfig glConfig() {
	RenderEngineConfig config;
	config.windowSize = glm::ivec2(800, 600);
	config.windowPos = glm::ivec2(500, 500);
	config.windowName = std::string("Test");
	config.windowResizeable = false;
	config.cursorEnabled = true;
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
};

Game::~Game() {
	ILogger* log = Locator::getLogger();
	delete log;

	delete _renderer;
};