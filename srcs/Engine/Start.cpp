#include "Engine/Engine.h"
#include "Engine/Locator.hpp"

RenderEngineConfig glConfig() {
	RenderEngineConfig config;
	config.windowSize = glm::ivec2(800, 600);
	config.windowPos = glm::ivec2(100, 100);
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
	Locator::provideRenderer(NULL);

	ILogger* log = new TerminalLogger();
	Locator::provideLogger(log);
	
	IRenderer* ren = new GLRenderer(glConfig());
	Locator::provideRenderer(ren);
};

Game::~Game() {
	IRenderer* ren = Locator::getRenderer();
	ILogger* log = Locator::getLogger();

	delete ren;
	delete log;
};