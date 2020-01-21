#include "Types.h"
#include "World/World.h"
#include "Utilities/Log.h"
#include "Engine/Game.h"
#include "World/Chunk.h"

#include "Utilities/Profiler.h"

World::World(Game* game) {
	_game = game;
}

World::~World() {
	for (auto pair : _chunks) {
		DeactivateChunk(pair.first);
		delete pair.second;
	}
}

void World::GenerateChunk(glm::ivec2 pos) {
	Chunk* chunk = _chunks[pos];
	if (chunk)
		return;
	chunk = new Chunk(_game, pos);
	_chunks[pos] = chunk;
	chunk->Generate();
}

void World::DestroyChunk(glm::ivec2 pos) {
	Chunk* chunk = _chunks[pos];
	if (!chunk)
		return;
	chunk->SetActive(false);
	_chunks[pos] = nullptr;
	delete chunk;
}

void World::ActivateChunk(glm::ivec2 pos) {
	SetChunkState(pos, true);
}

void World::DeactivateChunk(glm::ivec2 pos) {
	SetChunkState(pos, false);
}

void World::SetChunkState(glm::ivec2 pos, bool state) {
	Chunk* chunk = _chunks[pos];
	if (!chunk and !state) {
		return;
	}
	// if (!chunk and state) {
	// 	GenerateChunk(pos);
	// 	chunk = _chunks[pos];
	// }
	chunk->SetActive(state);
}

glm::ivec2 World::Global2Chunk(const glm::ivec3 global) {
	glm::ivec2 chunk;
	chunk.x = global.x >= 0 ? global.x >> 4 : (global.x + 1) / 16 - 1;
	chunk.y = global.z >= 0 ? global.z >> 4 : (global.z + 1) / 16 - 1;
	return chunk;
}

glm::ivec3 World::Global2Local(const glm::ivec3 global) {
	glm::ivec3 local;
	// local.x = global.x % 16;
	// local.z = global.z % 16;
	// local.x = global.x < 0 ? 15 + (global.x + 1) % 16 : local.x;
	// local.z = global.z < 0 ? 15 + (global.z + 1) % 16 : local.z;
	local.x = global.x >= 0 ? global.x % 16 : 15 + (global.x + 1) % 16;
	local.z = global.z >= 0 ? global.z % 16 : 15 + (global.z + 1) % 16;
	local.y = global.y;
	return local;

}

// x
// -16 -15 -14 -13 -12 -11 -10  -9  -8  -7  -6  -5  -4  -3  -2  -1   0   1   2   3   4
// x % 16
//   0 -15 -14 -13 -12 -11 -10  -9  -8  -7  -6  -5  -4  -3  -2  -1   0   1   2   3   4
// (x + 1) % 16
// -15 -14 -13 -12 -11 -10  -9  -8  -7  -6  -5  -4  -3  -2  -1   0   1   2   3   4   5
// 15 - (x + 1) % 16
//   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15   0   1   2   3   4



// res
//   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15   0   1   2   3   4




BlockType World::GetBlock(const glm::ivec3 globalPos) {
	//* Maybe optimize this
	Chunk* chunk = _chunks[Global2Chunk(globalPos)];
	if (!chunk) {
		return BlockType::Stone; //! Is this okay?
	}
	return chunk->GetBlock(Global2Local(globalPos));
}

void World::SetBlock(glm::ivec3 globalPos, BlockType type) {
	Chunk* chunk = _chunks[Global2Chunk(globalPos)];
	if (chunk) {
		chunk->SetBlock(Global2Local(globalPos), type);
	}
}

void World::PlayerSetBlock(glm::ivec3 globalPos, BlockType type) {
	Chunk* chunk = _chunks[Global2Chunk(globalPos)];
	if (chunk) {
		chunk->PlayerSetBlock(Global2Local(globalPos), type);
	}
}
