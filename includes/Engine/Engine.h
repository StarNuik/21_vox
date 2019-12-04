#pragma once
#include <vector>
#include <iostream>
#include "Render/Render.h"
#include "Input/Input.h"
#include "World/Resources.h"

#define FIXED_DELTA (1.f / 60.f)
#define FRAME_MS (int)(FIXED_DELTA * 1000)

class ResourceLoader;

class Entity {
public:
	virtual ~Entity() {};
	virtual void Update() = 0;
};

class Game {
public:
	Game();
	~Game();
	void GameLoop();
	void AddEntity(Entity*);
	void RemoveEntity(Entity*);
	GLRenderer* GetRenderer();
	Input* GetInput();
	ResourceLoader* GetResources();
private:
	void Update();
private:
	bool _finished;
	GLRenderer* _renderer;
	Input* _input;
	std::vector<Entity*> _entities;
	ResourceLoader* _resources;
};
