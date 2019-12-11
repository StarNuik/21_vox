#pragma once
#include <vector>
#include <iostream>
#include "Types.h"
#include "Render/Render.h"
#include "Input/Input.h"
#include "World/Resources.h"
#include "UI/UI.h"
#include "Generation/Map.h"

#define FIXED_DELTA (1.f / 60.f)
#define FRAME_MS (int)(FIXED_DELTA * 1000)

class ResourceLoader;
class UIController;
class MapGeneration;

class Entity {
public:
	virtual ~Entity() {};
	virtual void Update() = 0;
};

class Game {
public:
	Game();
	~Game();
	void InitSystems();
	void InitWorld();
	void GameLoop();
	void AddEntity(Entity*);
	void RemoveEntity(Entity*);
	GLRenderer* GetRenderer();
	Input* GetInput();
	ResourceLoader* GetResources();
	UIController* GetUI();
	World* GetWorld();
	uint GetLastFrameTime();
	MapGeneration* GetGeneration();
	MapGeneration* _mpGen;
private:
	void Update();

	bool _finished;
	GLRenderer* _renderer;
	Input* _input;
	std::vector<Entity*> _entities;
	ResourceLoader* _resources;
	World* _world;
	UIController* _ui;
	uint _lastFrameTime;
};
