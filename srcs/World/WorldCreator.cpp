#include <algorithm>
#include <functional>
#include <glm/glm.hpp>

#include "World/WorldCreator.h"
#include "World/World.h"
#include "World/Chunk.h"
#include "Engine/Game.h"
#include "Player/Player.h"
#include "Utilities/Log.h"
#include "Utilities/Profiler.h"

WorldCreator::WorldCreator(Game* game) {
	_game = game;
	_active = false;
};

WorldCreator::~WorldCreator() {};

void WorldCreator::Start() {
	_active = true;
	LoopTest();
};

void WorldCreator::Loop() {
	Chunk* chunk = GetChunkFromQueue();
	if (!chunk)
		return;
	if (chunk->state == Chunk::AwaitingGeneration) {
		// GenerateBlocks(chunk);
		chunk->GenerateBlocks();
		chunk->state = Chunk::AwaitingGeometry;
	}
	if (chunk->state == Chunk::AwaitingGeometry) {
		chunk->GenerateVertices();
	}
	AddChunkToQueue(chunk);
}

Chunk* WorldCreator::GetChunkFromQueue() {
	lockguard lock(_inMutex);
	if (_inQueue.empty())
		return nullptr;
	Chunk* chunk = _inQueue.front();
	_inQueue.pop();
	return chunk;
}

void WorldCreator::AddChunkToQueue(Chunk* chunk) {
	lockguard lock(_outMutex);
	_outQueue.push(chunk);
}


void WorldCreator::LoopTest() {
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
		}
	}
}
