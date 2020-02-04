#pragma once

#include <vector>
#include <iostream>

#include "Types.h"

#define SECONDS_IN_A_DAY (360.f)
#define SECONDS_IN_A_MOON_DAY (SECONDS_IN_A_DAY * 10.f)
// #define SECONDS_IN_A_MOON_DAY (10.f)

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
	Physics* GetPhysics();
	float GetRuntime();
	float GetSunVal();
	float GetSunAngle();
	float GetMoonVal();
	float GetMoonAngle();
	MapGeneration* _mpGen;
	Physics* _physics;
	Player* GetPlayer();
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
	Player* _player;
};
