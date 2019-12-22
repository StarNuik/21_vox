#include "Types.h"
#include "World/World.h"
#include "Utilities/Locator.hpp"
#include "Engine/Game.h"
#include "World/Chunk.h"

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

BlockType World::GetBlock(glm::ivec3 globalPos) {
	//* Maybe optimize this
	glm::ivec2 chunkPos(floorf(globalPos.x / 16.f), floorf(globalPos.z / 16.f));
	Chunk* chunk = this->_chunks[chunkPos];
	if (!chunk) {
		return BlockType::Stone; //! Is this okay?
	} else {
		int x = globalPos.x % 16;
		int z = globalPos.z % 16;
		x = x < 0 ? 16 + x : x;
		z = z < 0 ? 16 + z : z;
		return _chunks[chunkPos]->GetBlock(glm::ivec3(x, globalPos.y, z));
	}
}

void World::SetBlock(glm::ivec3 globalPos, BlockType type) {
	glm::ivec2 chunkPos(floorf(globalPos.x / 16.f), floorf(globalPos.z / 16.f));
	Chunk* chunk = _chunks[chunkPos];
	if (chunk) {
		int x = globalPos.x % 16;
		int z = globalPos.z % 16;
		x = x < 0 ? 16 + x : x;
		z = z < 0 ? 16 + z : z;
		chunk->SetBlock(glm::ivec3(x, globalPos.y, z), type);
	}
}

void World::PlayerSetBlock(glm::ivec3 globalPos, BlockType type) {
	glm::ivec2 chunkPos(globalPos.x / 16, globalPos.z / 16);
	Chunk* chunk = _chunks[chunkPos];
	if (chunk)
		chunk->PlayerSetBlock(glm::ivec3(globalPos.x % 16, globalPos.y, globalPos.z % 16), type);
}

NearestOpaque World::GetNearestOpaque(glm::ivec3 globalPos) {
	NearestOpaque res;
	res.right = GetBlock(globalPos + glm::ivec3(1, 0, 0)) == BlockType::Air;
	res.left = GetBlock(globalPos + glm::ivec3(1, 0, 0)) == BlockType::Air;
	res.top = GetBlock(globalPos + glm::ivec3(1, 0, 0)) == BlockType::Air;
	res.bottom = GetBlock(globalPos + glm::ivec3(1, 0, 0)) == BlockType::Air;
	res.front = GetBlock(globalPos + glm::ivec3(1, 0, 0)) == BlockType::Air;
	res.back = GetBlock(globalPos + glm::ivec3(1, 0, 0)) == BlockType::Air;
	return res;
};