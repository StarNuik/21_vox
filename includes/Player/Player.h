#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Types.h"
#include "Engine/Entity.h"
#include "World/World.h"
#include "World/Block.h"

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
		const float objectHeight = 1.80f;
		const float avoidBlockDistance = 0.30f;
		const float g = 9.83f;
		const float jumpForce = 400.f;
		bool isAir = false;
		bool isJump = false;
		glm::vec3 velocity = glm::vec3(0);
		glm::vec3 oldObjectPos = glm::vec3(0);
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

	RenderModel* _monkey;

	struct RayCastHitInfo
	{
		bool hit;
		float distance;
		glm::vec3 hitRayPos;
		glm::vec3 lastRayStep;
		Block hitBlock;
	};

	void PlayerCollision(glm::vec3& _position, glm::vec3& forward, glm::vec3& right);
	void PlayerMovement(Input* input, glm::vec3& forward, glm::vec3& right, glm::vec3& up, const float delta);
	void GodMovement(Input* input, glm::vec3& forward, glm::vec3& right, glm::vec3& up);

	void DestroyBlock(glm::vec3& _position, glm::vec3& forward);
	void PutBlock(glm::vec3& _position, glm::vec3& forward, BlockType blockType);

	float RayCastDist(const glm::vec3 _position, glm::vec3 direction, const float rayLength, float rayStep); // return the nearest distance to the block in the direction (limited by the max ray length). Else return inf
	RayCastHitInfo RayCast(const glm::vec3 _position, glm::vec3 direction, const float rayLength, float rayStep);
};

