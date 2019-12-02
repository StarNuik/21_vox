#pragma once
#include <vector>
#include <iostream>
#include "Render/Render.h"

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
};
