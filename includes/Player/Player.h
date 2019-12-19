#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Types.h"
#include "Engine/Entity.h"


#define SPEED 10.f

class Player : public Entity {
public:
	Player(Game* game);
	virtual ~Player();
	void Update(float delta);
	glm::vec3 GetPosition();
	glm::vec3 GetDirection();
private:
	Game* _game;
	Camera* _camera;
	glm::vec3 _position;
	glm::quat _rotation;
	bool _rotateCamera;
	float _camAngleX;
	float _camAngleY;
};

