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

void World::DestroyWorld() {
	_wc->inMutex.lock();
	int size = _wc->inQueue.size();
	_wc->inMutex.unlock();
	if (size == 0) {
		for (auto pair : _chunks) {
			delete pair.second;
		}
		_chunks.clear();
	}
}

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

namespace {
int TestNeighbour(std::unordered_map<glm::ivec2, Chunk*>& chunks, glm::ivec2 pos, glm::ivec2 offset) {
	Chunk* chunk = chunks[pos + offset];
	if (!chunk)
		return 0;
	if (chunk->state == Chunk::Complete or chunk->state == Chunk::GenerationComplete)
		return 1;
	return 0;
}
}

void World::AssessChunks(glm::vec3 playerPos) {
	glm::ivec2 playerChunkPos = Global2Chunk(playerPos);

	//* Add to creation or destruction queue
	for (int x = -GENDELETE_RADIUS; x <= GENDELETE_RADIUS; x++)
		for (int z = -GENDELETE_RADIUS; z <= GENDELETE_RADIUS; z++) {
			glm::ivec2 pos = glm::ivec2(x, z) + playerChunkPos;
			Chunk* chunk = _chunks[pos];
			if (chunk and x * x + z * z > GENERATE_SQUARED) {
				_destructionQueue.push_back(chunk);
				_chunks[pos] = nullptr;
				_chunks.erase(pos);
				// delete chunk;
				// _chunks.erase(pos);
			} else if (!chunk and x * x + z * z <= GENERATE_SQUARED) {
				chunk = new Chunk(_game, pos);
				_chunks[pos] = chunk;
				_wc->inMutex.lock();
				_wc->inQueue.push(chunk);
				_wc->inMutex.unlock();
			}
		}
	
	//* Delete queued chunks
	for (int pos = 0; pos < _destructionQueue.size(); pos++) {
		Chunk* chunk = _destructionQueue[pos];
		if (!chunk)
			continue;
		if (chunk->state == Chunk::Complete or chunk->state == Chunk::GenerationComplete) {
			_destructionQueue[pos] = nullptr;
			delete chunk;
		}
	}
	//* Giant costil
	if (_destructionQueue.size() == 0) {
		_destructionQueue.clear();
	} else {
		_destructionQueue.shrink_to_fit();
	}

	//* Update chunks that have neighbours generated
	int count = 0;
	for (int x = -WORLD_RADIUS; x <= WORLD_RADIUS and count < MAX_UPDATES_PER_FRAME; x++)
		for (int z = -WORLD_RADIUS; z <= WORLD_RADIUS and count < MAX_UPDATES_PER_FRAME; z++) {
			glm::ivec2 pos = glm::ivec2(x, z) + playerChunkPos;
			Chunk* chunk = _chunks[pos];
			if (!chunk) {
				continue;
			}
			int neighbours = 0;

			neighbours += TestNeighbour(_chunks, pos, glm::ivec2(1, 0));
			neighbours += TestNeighbour(_chunks, pos, glm::ivec2(-1, 0));
			neighbours += TestNeighbour(_chunks, pos, glm::ivec2(0, 1));
			neighbours += TestNeighbour(_chunks, pos, glm::ivec2(0, -1));

			if (chunk->state == Chunk::GenerationComplete and neighbours == 4) {
				chunk->Update();
				chunk->state = Chunk::Complete;
				count++;
			}
		}
}