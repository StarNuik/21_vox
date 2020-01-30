#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Types.h"
#include "Engine/Entity.h"
#include "World/World.h"
#include "World/Block.h"

#define SPEED 10.f
#define MOUSE_SENSITIVITY 0.75f

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
		glm::vec3 hitRayPos;
		glm::vec3 lastRayStep;
		Block hitBlock;
	};

	void PlayerCollision(glm::vec3& _position, glm::vec3& forward, glm::vec3& rigjt);

	void DestroyBlock(glm::vec3& _position, glm::vec3& forward);
	void PutBlock(glm::vec3& _position, glm::vec3& forward, BlockType blockType);

	void ChangePostShader();

	float RayCastDist(const glm::vec3 _position, glm::vec3 direction, const float rayLength, float rayStep); // return the nearest distance to the block in the direction (limited by the max ray length). Else return inf
	RayCastHitInfo RayCast(const glm::vec3 _position, glm::vec3 direction, const float rayLength, float rayStep);
};

