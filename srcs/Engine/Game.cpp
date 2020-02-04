#include "Engine/Game.h"
#include <cmath>

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

float Game::GetSunVal() {
	const float dawnTime = SECONDS_IN_A_DAY / 12.f;
	const float duskTime = SECONDS_IN_A_DAY / 12.f;
	const float nightTime = SECONDS_IN_A_DAY / 2.f;
	const float dayTime = SECONDS_IN_A_DAY - dawnTime - duskTime - nightTime;
	float currentTime = std::fmod(_runtime, SECONDS_IN_A_DAY);//(int)_runtime % SECONDS_IN_A_DAY;
	if (currentTime <= dawnTime) {
		return currentTime / dawnTime;
	} else if (currentTime <= dawnTime + dayTime) {
		return 1.f;
	} else if (currentTime <= dawnTime + dayTime + duskTime) {
		return 1.f - (currentTime - (dawnTime + dayTime)) / duskTime;
	} else {
		return 0.f;
	}
};

float Game::GetMoonVal() {
	const float dawnTime = SECONDS_IN_A_MOON_DAY / 12.f;
	const float duskTime = SECONDS_IN_A_MOON_DAY / 12.f;
	const float dayTime = SECONDS_IN_A_MOON_DAY / 2.f;
	const float nightTime = SECONDS_IN_A_MOON_DAY - dawnTime - duskTime - dayTime;
	float currentTime = std::fmod(_runtime, SECONDS_IN_A_MOON_DAY);//(int)_runtime % SECONDS_IN_A_DAY;
	if (currentTime <= dawnTime) {
		return currentTime / dawnTime;
	} else if (currentTime <= dawnTime + dayTime) {
		return 1.f;
	} else if (currentTime <= dawnTime + dayTime + duskTime) {
		return 1.f - (currentTime - (dawnTime + dayTime)) / duskTime;
	} else {
		return 0.f;
	}
};

float Game::GetSunAngle() {
	float currentTime = std::fmod(_runtime, SECONDS_IN_A_DAY);
	return currentTime / SECONDS_IN_A_DAY * 360.f;
}

float Game::GetMoonAngle() {
	float currentTime = std::fmod(_runtime, SECONDS_IN_A_MOON_DAY);
	return currentTime / SECONDS_IN_A_MOON_DAY * 360.f;
}

GLRenderer* Game::GetRenderer() {return _renderer;};
Input* Game::GetInput() {return _input;};
ResourceLoader* Game::GetResources() {return _resources;};
UIController* Game::GetUI() {return _ui;};
World* Game::GetWorld() {return _world;};
MapGeneration* Game::GetGeneration() {return _mpGen;};
Physics* Game::GetPhysics() {return _physics;};
float Game::GetRuntime() {return _runtime;};
Player* Game::GetPlayer() {return _player;};
// uint Game::GetLastFrameTime() {return _lastFrameTime;};