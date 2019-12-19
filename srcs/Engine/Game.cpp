#include "Engine/Game.h"

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

float Game::GetDayNightVal() {
	const float dawnTime = SECONDS_IN_A_DAY / 12;
	const float duskTime = SECONDS_IN_A_DAY / 12;
	const float nightTime = SECONDS_IN_A_DAY / 6;
	const float dayTime = SECONDS_IN_A_DAY - dawnTime - duskTime - nightTime;
	float currentTime = (int)_runtime % SECONDS_IN_A_DAY;
	if (currentTime <= dawnTime) {
		return currentTime / dawnTime;
	} else if (currentTime <= dawnTime + dayTime) {
		return 1.f;
	} else if (currentTime <= dawnTime + dayTime + duskTime) {
		return (currentTime - (dawnTime + dayTime)) / duskTime;
	} else {
		return 0.f;
	}
};

GLRenderer* Game::GetRenderer() {return _renderer;};
Input* Game::GetInput() {return _input;};
ResourceLoader* Game::GetResources() {return _resources;};
UIController* Game::GetUI() {return _ui;};
World* Game::GetWorld() {return _world;};
MapGeneration* Game::GetGeneration() {return _mpGen;};
float Game::GetRuntime() {return _runtime;};
// uint Game::GetLastFrameTime() {return _lastFrameTime;};