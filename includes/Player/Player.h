#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Types.h"
#include "Engine/Entity.h"
#include "World/World.h"
#include "World/Block.h"

#define SPEED 10.f
#define CROUCHING_SPEED 2.f

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
		float currObjectHeight = 1.80f;
		const float maxObjectHeight = 1.80f;
		const float avoidBlockDistance = 0.12f;
		const float g = 9.83f;
		const float jumpForce = 2.1f;
		bool isAir = false;
		bool isJump = false;
		bool isCrouch = false;
		glm::vec3 velocity = glm::vec3(0);
	};

private:
	Game* _game;
	Camera* _camera;
	World* _world;
	Physics* _physics;
	MovementProperty _movementPropety;
	glm::vec3 _position;
	glm::quat _rotation;
	glm::vec3 _upperBody;
	glm::vec3 _middleBody;
	glm::vec3 _lowerBody;
	bool _rotateCamera;
	float _camAngleX;
	float _camAngleY;
	float _delta;

	RenderModel* _monkey;

	inline void Move(const glm::vec3 vel, const float& speed)
	{
		_position += vel * _delta * speed;
	}

	inline glm::vec3 GetLowerBody()
	{
		return glm::vec3(_position.x, _position.y - (_movementPropety.currObjectHeight * 0.95f), _position.z);
	}

	inline glm::vec3 GetMiddleBody()
	{
		return glm::vec3(_position.x, _position.y - (_movementPropety.currObjectHeight * 0.50f), _position.z);
	}

	inline glm::vec3 GetUpperBody()
	{
		return _position;
	}

	void PlayerHorizontalMovement(Input* input, glm::vec3& forward, glm::vec3& right);
	void PlayerVerticalMovement(Input* input);
	void GodMovement(Input* input, glm::vec3& forward, glm::vec3& right, glm::vec3& up);

	void DestroyBlock(glm::vec3& _position, glm::vec3& forward);
	void PutBlock(glm::vec3& _position, glm::vec3& forward, BlockType blockType);
};

