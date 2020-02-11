#pragma once

#include <glm/glm.hpp>

#include "Types.h"
#include "World/Block.h"
#include "World/World.h"

class Physics {
public: 
	Physics(Game* game);
	~Physics() {};
	struct RayCastHitInfo
	{
		bool hit;
		float distance;
		glm::vec3 hitRayPos;
		glm::vec3 lastRayStep;
		Block hitBlock;
	};
	
	struct CollisionInfo
	{
		bool isCollision;
		float distX;
		float distZ;
		glm::vec3 sideNormal;
	};

	RayCastHitInfo RayCast(const glm::vec3 _position, glm::vec3 direction, const float rayLength, float rayStep);
	RayCastHitInfo RayCastWalkthrough(const glm::vec3 _position, glm::vec3 direction, const float rayLength, float rayStep);
	CollisionInfo CheckCollision(const glm::vec3& direction, const glm::vec3& upperBody, const glm::vec3& middleBody,
									const glm::vec3& lowerBody, const glm::vec3& _position, const float& avoidBlockDistance);
private:
	World* _world;
	Game* _game;
};
