#pragma once
#include <vector>
#include <iostream>
#include "Render/Render.h"
#include "Input/Input.h"
#include "World/Resources.h"

#define MS_PER_UPDATE 0.05f

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
	void PhysicsUpdate();
private:
	bool _finished;
	GLRenderer* _renderer;
	Input* _input;
	std::vector<Entity*> _entities;
	ResourceLoader* _resources;
};
