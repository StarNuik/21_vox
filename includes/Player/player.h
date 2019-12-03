#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Engine/Engine.h"
#include "Render/Camera.h"

class Command {

};

class Player : public Entity{
public:
	Player(Game* game);
	virtual ~Player();
	void Update();
private:
	Game* _game;
	Camera* _camera;
	glm::vec3 _position;
	glm::quat _rotation;
};

