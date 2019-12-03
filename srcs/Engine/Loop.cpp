#include "Engine/Engine.h"
#include "Utilities/Time.h"
#include "Engine/Locator.hpp"
#include "Types.h"

void Game::Update() {
	if (glfwWindowShouldClose(_renderer->GetWindow()) || _input->KeyPressed(GLFW_KEY_ESCAPE)) {
		_finished = true;
	}
	for (Entity* entity : _entities) {
		entity->Update();
	}
};

void Game::PhysicsUpdate() {
	// std::cout << "Physics Update\n";
};

// void Game::Render() {
// 	_renderer->RenderFrame();
// };

void Game::GameLoop() {
	Locator::getLogger()->LogSuccess("[Game::GameLoop]\nGame loop started.");
	while (!_finished)
	{
		uint64 start = LONG_TIME;
		_input->Update(_renderer->GetWindow());
		Update();
		PhysicsUpdate();
		_renderer->RenderFrame();
	}
};

void Game::AddEntity(Entity* entity) {
	_entities.push_back(entity);
};

void Game::RemoveEntity(Entity* entity) {
	std::vector<Entity*>::iterator pos;
	pos = std::find(_entities.begin(), _entities.end(), entity);
	if (pos != _entities.end())
	{
		_entities.erase(pos);
	}
};

GLRenderer* Game::GetRenderer() {return _renderer;};
Input* Game::GetInput() {return _input;};
ResourceLoader* Game::GetResources() {return _resources;};