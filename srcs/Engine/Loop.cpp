#include "Engine/Engine.h"
#include "Utilities/Time.h"
#include "Engine/Locator.hpp"
#include "Types.h"
#include <unistd.h>
#include <string>

void Game::Update() {
	if (glfwWindowShouldClose(_renderer->GetWindow()) || _input->KeyPressed(GLFW_KEY_ESCAPE)) {
		_finished = true;
	}
	for (Entity* entity : _entities) {
		entity->Update();
	}
};

void Game::GameLoop() {
	Locator::getLogger()->LogSuccess("[Game::GameLoop]\nGame loop started.");
	while (!_finished)
	{
		uint64 start = LONG_TIME;
		_input->Update(_renderer->GetWindow());
		Update();
		// PhysicsUpdate();
		_renderer->RenderFrame();
		uint64 end = LONG_TIME;

		if ((end - start) - FRAME_MS > 0)
			usleep(((end - start) - FRAME_MS) * 1000);
	}
};
