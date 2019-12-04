#include "World/World.h"

World::World(Game* game) {
	_game = game;
	ActivateChunk(glm::ivec2(0));
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

void World::SetBlock(glm::ivec3 pos, BLOCK_TYPE type) {
	// Test if chunk exists
	glm::ivec2 chunkPos(pos.x / 16, pos.z / 16);
	_chunks[chunkPos]->SetBlock(glm::ivec3(pos.x % 16, pos.y, pos.z % 16), type);
}