#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Mathf/Mathf.h"

#include "Types.h"
#include "Engine/Entity.h"
#include "World/Block.h"

#define SPEED 10.f
#define FLY_SPEED 5.f
#define CROUCHING_SPEED 2.f
#define MOUSE_SENSITIVITY 7.5f

class Player : public Entity {
public:
	Player(Game* game);
	virtual ~Player();
	void Update(float delta);
	mathf::vec3 GetPosition();
	mathf::vec3 GetDirection();

private:
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
        mathf::vec3 velocity = mathf::vec3(0);
    };

	Game* _game;
	Camera* _camera;
	World* _world;
	Physics* _physics;
	MovementProperty _movementPropety;
	mathf::vec3 _position;
	glm::quat _rotation;
	bool _rotateCamera;
	float _camAngleX;
	float _camAngleY;
	float _delta;

	Block _selectedBlock;

	RenderModel* _monkey;


	inline void Move(const mathf::vec3 vel, const float& speed)
	{
		_position += vel * _delta * speed;
	}

	void GodMovement(Input* input, mathf::vec3& forward, mathf::vec3& right, mathf::vec3& up);
};

