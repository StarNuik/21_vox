#include <GLFW/glfw3.h>
#include <unistd.h>

#include "Engine/Game.h"
#include "Render/GLRenderer.h"
#include "Input/Input.h"
#include "Engine/Entity.h"
#include "Utilities/Profiler.h"
#include "Utilities/Log.h"

void Game::Update(float delta) {
	if (glfwWindowShouldClose(_renderer->GetWindow()) || _input->KeyPressed(GLFW_KEY_ESCAPE)) {
		_finished = true;
	}
	for (Entity* entity : _entities) {
		entity->Update(delta);
	}
};

void Game::GameLoop() {
	Profiler::Prepare("FrameFull");
	Profiler::Prepare("Input");
	Profiler::Prepare("Update");
	Profiler::Prepare("RenderFull");

	Log::Success("[Game::GameLoop]\nGame loop started.");
	while (!_finished)
	{
		float delta = Profiler::GetLastS("FrameFull");
		_runtime += delta;
		Profiler::Start("FrameFull");
		Profiler::Start("Input");

		_input->Update(_renderer->GetWindow());

		Profiler::Add("Input");
		Profiler::Start("Update");

		Update(delta);

		Profiler::Add("Update");
		// PhysicsUpdate();
		Profiler::Start("RenderFull");

		_renderer->RenderFrame();

		Profiler::Add("RenderFull");
		Profiler::Add("FrameFull");
	}
};
