#pragma once

#include <thread>
#include <mutex>
#include <queue>

#include "Types.h"

#define WORLD_CHECK_BIAS 2

using lockguard = std::lock_guard<std::mutex>;

class WorldCreator {
public:
	WorldCreator(Game*);
	~WorldCreator();
	void Start();
	// std::mutex worldMutex;
private:
	Chunk* GetChunkFromQueue();
	void AddChunkToQueue(Chunk*);
	void LoopTest();
	void Loop();
	bool _active;
	Game* _game;
	std::mutex _inMutex;
	std::queue<Chunk*> _inQueue;
	std::mutex _outMutex;
	std::queue<Chunk*> _outQueue;
};