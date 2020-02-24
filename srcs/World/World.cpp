#include <thread>
#include <mutex>

#include "Types.h"
#include "World/World.h"
#include "Utilities/Log.h"
#include "Engine/Game.h"
#include "World/Chunk.h"
#include "World/Block.h"
#include "World/WorldCreator.h"

#include "Utilities/Profiler.h"

World::World(Game* game) {
	_game = game;
	_wc = game->GetWorldCreator();
}

World::~World() {
	for (auto pair : _chunks) {
		// DeactivateChunk(pair.first);
		delete pair.second;
	}
}

// void World::GenerateChunk(glm::ivec2 pos) {
// 	Chunk* chunk = _chunks[pos];
// 	if (chunk)
// 		return;
// 	chunk = new Chunk(_game, pos);
// 	_chunks[pos] = chunk;
// 	chunk->Generate();
// }

// void World::DestroyChunk(glm::ivec2 pos) {
// 	Chunk* chunk = _chunks[pos];
// 	if (!chunk)
// 		return;
// 	chunk->SetActive(false);
// 	_chunks[pos] = nullptr;
// 	delete chunk;
// }

// void World::ActivateChunk(glm::ivec2 pos) {
// 	SetChunkState(pos, true);
// }

// void World::DeactivateChunk(glm::ivec2 pos) {
// 	SetChunkState(pos, false);
// }

// void World::SetChunkState(glm::ivec2 pos, bool state) {
// 	Chunk* chunk = _chunks[pos];
// 	if (!chunk and !state) {
// 		return;
// 	}
// 	// if (!chunk and state) {
// 	// 	GenerateChunk(pos);
// 	// 	chunk = _chunks[pos];
// 	// }
// 	chunk->SetActive(state);
// }

glm::ivec2 World::Global2Chunk(const glm::ivec3 global) {
	glm::ivec2 chunk;
	chunk.x = global.x >= 0 ? global.x >> 4 : (global.x + 1) / 16 - 1;
	chunk.y = global.z >= 0 ? global.z >> 4 : (global.z + 1) / 16 - 1;
	return chunk;
}

glm::ivec3 World::Global2Local(const glm::ivec3 global) {
	glm::ivec3 local;
	local.x = global.x >= 0 ? global.x % 16 : 15 + (global.x + 1) % 16;
	local.z = global.z >= 0 ? global.z % 16 : 15 + (global.z + 1) % 16;
	local.y = global.y;
	return local;
}

Block World::GetBlock(const glm::vec3 global) {
	return GetBlock(glm::ivec3(glm::floor(global)));
}

Block World::GetBlock(const glm::ivec3 globalPos) {
	//* Maybe optimize this
	Chunk* chunk = _chunks[Global2Chunk(globalPos)];
	if (!chunk) {
		return Block::NoChunk;
	}
	return chunk->GetBlock(Global2Local(globalPos));
}

void World::SetBlock(const glm::vec3 global, Block type) {
	SetBlock(glm::ivec3(glm::floor(global)), type);
}

void World::SetBlock(glm::ivec3 globalPos, Block type) {
	Chunk* chunk = _chunks[Global2Chunk(globalPos)];
	if (chunk) {
		chunk->SetBlock(Global2Local(globalPos), type);
	}
}

void World::PlayerSetBlock(const glm::vec3 global, Block type) {
	PlayerSetBlock(glm::ivec3(glm::floor(global)), type);
}

void World::PlayerSetBlock(glm::ivec3 globalPos, Block type) {
	Chunk* chunk = _chunks[Global2Chunk(globalPos)];
	if (chunk) {
		chunk->PlayerSetBlock(Global2Local(globalPos), type);
	}
}

void World::AssessChunks(glm::vec3 playerPos) {
	glm::ivec2 playerChunkPos = Global2Chunk(playerPos);

	//* Deletion
	for (auto pair : _chunks) {
		Chunk* chunk = pair.second;
		if (chunk == nullptr) {
			_chunks.erase(pair.first);
			continue;
		}
		glm::ivec2 local = pair.first - playerChunkPos;
		if (local.x * local.x + local.y * local.y > RADIUS_SQUARED and chunk->state == Chunk::Completed) {
			delete chunk;
			_chunks.erase(pair.first);
		}
	}

	//* Add to creation queue
	for (int x = -WORLD_RADIUS; x <= WORLD_RADIUS; x++)
		for (int z = -WORLD_RADIUS; z<= WORLD_RADIUS; z++)
			if (x * x + z * z <= RADIUS_SQUARED) {
				glm::ivec2 pos = glm::ivec2(x, z) + playerChunkPos;
				Chunk* chunk = _chunks[pos];
				if (chunk) {
					continue;
				}
				chunk = new Chunk(_game, pos);
				_chunks[pos] = chunk;
				_wc->inMutex.lock();
				_wc->inQueue.push(chunk);
				_wc->inMutex.unlock();
			}
	
	//* Update geometry for those who need it
	_wc->outMutex.lock();
	int size = _wc->outQueue.size();
	_wc->outMutex.unlock();
	for (int count = 0; count < MAX_UPDATES_PER_FRAME and size > 0; count++, size--) {
		_wc->outMutex.lock();
		Chunk* chunk = _wc->outQueue.front();
		_wc->outQueue.pop();
		_wc->outMutex.unlock();
		chunk->Update();
		chunk->state = Chunk::Completed;
		//* Update neighbours
		glm::ivec2 pos = chunk->GetPosition();
		glm::ivec2 offset;
		offset = glm::ivec2(1, 0);
		if (_chunks[pos + offset] and _chunks[pos + offset]->state == Chunk::Completed)
			_chunks[pos + offset]->Update();
		offset = glm::ivec2(-1, 0);
		if (_chunks[pos + offset] and _chunks[pos + offset]->state == Chunk::Completed)
			_chunks[pos + offset]->Update();
		offset = glm::ivec2(0, 1);
		if (_chunks[pos + offset] and _chunks[pos + offset]->state == Chunk::Completed)
			_chunks[pos + offset]->Update();
		offset = glm::ivec2(0, -1);
		if (_chunks[pos + offset] and _chunks[pos + offset]->state == Chunk::Completed)
			_chunks[pos + offset]->Update();
	}
}