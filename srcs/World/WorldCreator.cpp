#include <algorithm>
#include <functional>
#include <glm/glm.hpp>

#include "World/World.h"
#include "World/WorldCreator.h"
#include "Engine/Game.h"
#include "Player/Player.h"
#include "Utilities/Log.h"
#include "Utilities/Profiler.h"


WorldCreator::WorldCreator(Game* game) {
	_game = game;
	_active = false;
	_worldRadius = 10;
};

WorldCreator::~WorldCreator() {};

void WorldCreator::Start() {
	_active = true;
	Loop();
};

void WorldCreator::Test() {
	// Player* p = _game->GetPlayer();
	// World* w = _game->GetWorld();
	// glm::vec3 playerPos = p->GetPosition();
	// const int testRadius = _worldRadius + WORLD_CHECK_BIAS;
	// int active = 0;
	// int inactive = 0;
	// for (int x = -testRadius; x < testRadius; x++)
	// 	for (int z = -testRadius; z < testRadius; z++) {
	// 		glm::vec3 offset(x * 16, 0, z * 16);
	// 		if (w->ChunkPresent(playerPos + offset)) {
	// 			active++;
	// 		} else {
	// 			inactive++;
	// 		}
	// 	}
	// Log::Basic("[WorldCreator::Test]\n[ Radius: ", testRadius, ", A: ", active, ", I: ", inactive, " ]");
};

void WorldCreator::Loop() {
	__timepoint start = std::chrono::high_resolution_clock::now();;
	float runtime = 0;
	int lastPrint = 0;
	while (_active) {
		__timepoint now = std::chrono::high_resolution_clock::now();

		float runtime = std::chrono::duration_cast<std::chrono::nanoseconds>(now - start).count() / __S_DIV;
		int currentSecond = (int)std::floorf(runtime);
		if (lastPrint + 1 <= currentSecond) {
			lastPrint = currentSecond;
			Log::Basic(currentSecond, " seconds have passed!");
			// Test();
		}
	}
}

void WorldCreator::SetWorldRadius(uint radius) {_worldRadius = glm::clamp(radius, 1u, 64u);};
