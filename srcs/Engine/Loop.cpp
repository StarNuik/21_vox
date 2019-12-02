#include "Engine/Engine.h"
#include "Utilities/Time.h"

void Game::GameLoop() {
	while (!_finished)
	{
		long long start = LONG_TIME;
		InputUpdate();
		Update();
		PhysicsUpdate();
		RenderUpdate();
	}
};

void Game::InputUpdate() {
	std::cout << "Input Update\n";
};

void Game::Update() {
	std::cout << "Update\n";
};

void Game::PhysicsUpdate() {
	std::cout << "Physics Update\n";
};

void Game::RenderUpdate() {
	std::cout << "Render Update\n";
};