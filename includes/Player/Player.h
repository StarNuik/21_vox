#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Types.h"
#include "Engine/Entity.h"
#include "World/World.h"


#define SPEED 10.f

class Player : public Entity {
public:
	Player(Game* game);
	virtual ~Player();
	void Update(float delta);
	glm::vec3 GetPosition();
	glm::vec3 GetDirection();
	struct MovementProperty
	{
		int godMode = 1;
		const float objectHeight = 2.5f;
		const float avoidBlockDistance = 0.1f;
		glm::vec3 oldPlayerPos = glm::vec3(0);
		glm::vec3 vecUp = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 vecRight = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 vecForward = glm::vec3(0.0f, 0.0f, -1.0f);
	};

private:
	Game* _game;
	Camera* _camera;
	World* _world;
	MovementProperty _movementPropety;
	glm::vec3 _position;
	glm::quat _rotation;
	bool _rotateCamera;
	float _camAngleX;
	float _camAngleY;

	void PlayerCollision(glm::vec3& _position);
	float RayCastDist(const glm::vec3 _position, glm::vec3 direction, const float rayLength, float rayStep); // return the nearest distance to the block in the direction (limited by the max ray length). Else return inf
};

