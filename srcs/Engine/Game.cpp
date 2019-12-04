// #include "Engine/Engine.h"
#include "Engine/Engine.h"

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
UIController* Game::GetUI() {return _ui;};