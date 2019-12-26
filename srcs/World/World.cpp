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
	if (!chunk and state) {
		GenerateChunk(pos);
		chunk = _chunks[pos];
	}
	chunk->SetActive(state);
}

BlockType World::GetBlock(const glm::ivec3 globalPos) {
	//* Maybe optimize this
	int x, z;
	x = globalPos.x >= 0 ? globalPos.x >> 4 : (globalPos.x + 1) / 16 - 1;
	z = globalPos.z >= 0 ? globalPos.z >> 4 : (globalPos.z + 1) / 16 - 1;
	const glm::ivec2 chunkPos(x, z);
	Chunk* chunk = _chunks[chunkPos];
	if (!chunk) {
		return BlockType::Stone; //! Is this okay?
	}
	x = globalPos.x % 16;
	z = globalPos.z % 16;
	x = x < 0 ? 16 + x : x;
	z = z < 0 ? 16 + z : z;
	return _chunks[chunkPos]->GetBlock(glm::ivec3(x, globalPos.y, z));
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
