#pragma once
#include <vector>
#include <iostream>
#include "Render/Render.h"
#include "Input/Input.h"

class Entity {
public:
	virtual ~Entity();
	virtual void Update();
};

class Game {
public:
	Game();
	~Game();
	void GameLoop();
private:
	void GetInput();
	void Update();
	void PhysicsUpdate();
	void Render();
private:
	bool _finished;
	GLRenderer* _renderer;
	Input* _input;
	std::vector<Entity*> _entities;
};
