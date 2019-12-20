// #include "Engine/Engine.h"
// #include "Utilities/Time.h"
// #include "Engine/Locator.hpp"
// #include "Types.h"
// #include <unistd.h>
// #include <string>
#include <GLFW/glfw3.h>
#include <unistd.h>

#include "Engine/Game.h"
#include "Utilities/Locator.hpp"
#include "Render/GLRenderer.h"
#include "Input/Input.h"
#include "Engine/Entity.h"
#include "Utilities/Time.h"
#include "Utilities/Profiler.h"

void Game::Update(float delta) {
	if (glfwWindowShouldClose(_renderer->GetWindow()) || _input->KeyPressed(GLFW_KEY_ESCAPE)) {
		_finished = true;
	}
	for (Entity* entity : _entities) {
		entity->Update(delta);
	}
};

void Game::GameLoop() {
	Locator::GetLogger()->LogSuccess("[Game::GameLoop]\nGame loop started.");
	while (!_finished)
	{
		float delta = Profiler::Getf("FrameFull") * 0.001;
		_runtime += delta;
		Profiler::Start("FrameFull");
		Profiler::Start("Input");

		_input->Update(_renderer->GetWindow());

		Profiler::End("Input");
		Profiler::Start("Update");

		Update(delta);

		Profiler::End("Update");
		// PhysicsUpdate();
		Profiler::Start("RenderFull");

		_renderer->RenderFrame();

		Profiler::End("RenderFull");
		Profiler::End("FrameFull");
	}
};
