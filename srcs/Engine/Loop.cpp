#include "Engine/Engine.h"
#include "Utilities/Time.h"
#include "Engine/Locator.hpp"

void Game::GetInput() {
	// std::cout << "Input Update\n";
};

void Game::Update() {
	// std::cout << "Update\n";
};

void Game::PhysicsUpdate() {
	// std::cout << "Physics Update\n";
};

void Game::Render() {
	_renderer->RenderFrame();
};

void Game::GameLoop() {
	Locator::getLogger()->LogSuccess("Game loop started.");
	while (!_finished)
	{
		long long start = LONG_TIME;
		_input->Update(_renderer->GetWindow());
		Update();
		PhysicsUpdate();
		_renderer->RenderFrame();
	}
};
