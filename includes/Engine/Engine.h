#pragma once
#include <vector>

class Game {
	public:
	void InitSystems();
	void GameLoop();
	private:
	void InputUpdate();
	void Update();
	void PhysicsUpdate();
	void RenderUpdate();
	private:
	bool _finished;
	std::vector<Entity*> _entities;
};

class Entity {
	public:
	virtual ~Entity();
	virtual void Update();
};