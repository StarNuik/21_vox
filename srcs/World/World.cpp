#include "World/World.h"
#include "Engine/Locator.hpp"
#include "Utilities/Time.h"
#include <glm/gtx/hash.hpp>

World::World(Game* game) {
	std::cout << this << std::endl;
	_game = game;
	int64 start = LONG_TIME;
	for (int x = -1; x <= 1; x++)
		for (int z = -1; z <= 1; z++)
			ActivateChunk(glm::ivec2(x, z));
	for (auto pair : _chunks) {
		pair.second->UpdateGeometry(glm::ivec2(pair.first.x, pair.first.y));
	}
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
		this->_chunks[pos] = new Chunk(_game, pos);
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

#include <stdio.h>
BlockType World::GetBlock(glm::ivec3 pos) {
	glm::ivec2 chunkPos = glm::ivec2(pos.x / 16, pos.z / 16);
	Chunk* chunk = this->_chunks[pos];
	// Locator::getLogger()->LogWarning(std::to_string((void*)chunk));
	if (!chunk)
		return BlockType::Air;
	else
		return _chunks[chunkPos]->GetBlock(glm::ivec3(pos.x % 16, pos.y, pos.z % 16));
}

void World::SetBlock(glm::ivec3 pos, BlockType type) {
	// Test if chunk exists
	glm::ivec2 chunkPos(pos.x / 16, pos.z / 16);
	_chunks[chunkPos]->SetBlock(glm::ivec3(pos.x % 16, pos.y, pos.z % 16), type);
}