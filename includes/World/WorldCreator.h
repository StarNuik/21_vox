#pragma once

#include <mutex>

#include "Types.h"

#define WORLD_CHECK_BIAS 2

class WorldCreator {
public:
	WorldCreator(Game*);
	~WorldCreator();
	void Start();
	void SetWorldRadius(uint);
	std::mutex worldMutex;
private:
	void Loop();
	void Test();
	bool _active;
	Game* _game;
	uint _worldRadius;
};