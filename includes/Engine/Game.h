#pragma once

#include <vector>
#include <iostream>
#include <thread>

#include "Types.h"

#define SECONDS_IN_A_DAY (360.f)
#define SECONDS_IN_A_MOON_DAY (SECONDS_IN_A_DAY * 10.f)

#define TARGET_FPS (60)
#define TARGET_S (1.0f / (float)(TARGET_FPS))
#define TARGET_MS (1000.f / (float)(TARGET_FPS))
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
	WorldCreator* GetWorldCreator();
	float GetRuntime();
	float GetSunVal();
	float GetSunAngle();
	float GetMoonVal();
	float GetMoonAngle();
	MapGeneration* _mpGen;
	Physics* _physics;
	Player* GetPlayer();
	//* The World
	float GetTheWorldRadius();
	void TheWorldOn();
private:
	void Update(float delta);

	bool _finished;
	GLRenderer* _renderer;
	Input* _input;
	std::vector<Entity*> _entities;
	ResourceLoader* _resources;
	World* _world;
	WorldCreator* _worldCreator;
	std::thread _wcThread;
	UIController* _ui;
	float _lastFrame;
	float _runtime;
	Player* _player;
	//* The World
	bool _theworldOn;
	float _theworldStart;
};

#define THE_WORLD_UNSAFE_DIST (270.f)
#define THE_WORLD_BLOCKS_DIST (176.f)
#define THE_WORLD_RADIUS (THE_WORLD_UNSAFE_DIST + THE_WORLD_BLOCKS_DIST + 50.f)
#define THE_WORLD_OFFSET (2.2f)
#define THE_WORLD_ANIM_TIME (4.0f)
#define THE_WORLD_COLLAPSE_TIME (2.0f)
#define THE_WORLD_DURATION (9.f)