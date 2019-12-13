#pragma once

#include <vector>
#include <iostream>

#include "Types.h"

#define FIXED_DELTA (1.f / 60.f)
#define FRAME_MS (int)(FIXED_DELTA * 1000)

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
	// uint GetLastFrameTi me();
	MapGeneration* GetGeneration();
	MapGeneration* _mpGen;
	class ProfilerInfo;
private:
	void Update();

	bool _finished;
	GLRenderer* _renderer;
	Input* _input;
	std::vector<Entity*> _entities;
	ResourceLoader* _resources;
	World* _world;
	UIController* _ui;
	// uint _lastFrameTime;
};
