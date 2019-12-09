#include "World/World.h"
#include "Engine/Locator.hpp"
#include "Utilities/Time.h"

World::World(Game* game) {
	_game = game;
	int64 start = LONG_TIME;
	for (int x = -1; x <= 1; x++)
		for (int z = -1; z <= 1; z++)
			ActivateChunk(glm::ivec2(x, z));
	int64 end = LONG_TIME;
	Locator::getLogger()->LogSuccess("[World::World]\nGenerated chunk in: " + std::to_string(end - start) + " ms.");
}

World::~World() {
	for (auto pair : _chunks) {
		DeactivateChunk(pair.first);
		delete pair.second;
	}
}

void World::ActivateChunk(glm::ivec2 pos) {
	Chunk* chunk = _chunks[pos];
	if (!chunk) {
		_chunks[pos] = new Chunk(_game, pos);
		chunk = _chunks[pos];
	}
	chunk->SetActive(true);
}

void World::DeactivateChunk(glm::ivec2 pos) {
	Chunk* chunk = _chunks[pos];
	if (chunk) {
		chunk->SetActive(false);
	}
}

Block* World::GetBlock(glm::ivec3 pos) {
	// Test if chunk exists
	glm::ivec2 chunkPos(pos.x / 16, pos.z / 16);
	return _chunks[chunkPos]->GetBlock(glm::ivec3(pos.x % 16, pos.y, pos.z % 16));
}

void World::SetBlock(glm::ivec3 pos, BlockType type) {
	// Test if chunk exists
	glm::ivec2 chunkPos(pos.x / 16, pos.z / 16);
	_chunks[chunkPos]->SetBlock(glm::ivec3(pos.x % 16, pos.y, pos.z % 16), type);
}