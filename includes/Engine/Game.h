#pragma once

#include <vector>
#include <iostream>

#include "Types.h"

class Game {
public:
	Game();
	~Game();
	void InitSystems();
	void InitWorld();
	void DestroyWorld();
	void GameLoop();
	void AddEntity(Entity*);
	void RemoveEntity(Entity*);
	GLRenderer* GetRenderer();
	Input* GetInput();
	ResourceLoader* GetResources();
	UIController* GetUI();
	World* GetWorld();
	MapGeneration* GetGeneration();
	float GetRuntime();
	float GetDayNightVal();
	MapGeneration* _mpGen;
	class ProfilerInfo;
private:
	void Update(float delta);

	bool _finished;
	GLRenderer* _renderer;
	Input* _input;
	std::vector<Entity*> _entities;
	ResourceLoader* _resources;
	World* _world;
	UIController* _ui;
	float _lastFrame;
	float _runtime;
};
