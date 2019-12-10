#include "World/World.h"
#include "Engine/Locator.hpp"

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
	chunk->Generate();
	_chunks[pos] = chunk;
}

void World::ActivateChunk(glm::ivec2 pos) {
	Chunk* chunk = _chunks[pos];
	if (!chunk) {
		return;
	}
	chunk->SetActive(true);
}

void World::DeactivateChunk(glm::ivec2 pos) {
	Chunk* chunk = _chunks[pos];
	if (chunk) {
		chunk->SetActive(false);
	}
}

BlockType World::GetBlock(glm::ivec3 globalPos) {
	glm::ivec2 chunkPos(globalPos.x / 16, globalPos.z / 16);
	Chunk* chunk = this->_chunks[chunkPos];
	if (!chunk)
		return BlockType::Air;
	else
		return _chunks[chunkPos]->GetBlock(glm::ivec3(globalPos.x % 16, globalPos.y, globalPos.z % 16));
}

void World::SetBlock(glm::ivec3 globalPos, BlockType type) {
	glm::ivec2 chunkPos(globalPos.x / 16, globalPos.z / 16);
	Chunk* chunk = _chunks[chunkPos];
	if (chunk)
		chunk->SetBlock(glm::ivec3(globalPos.x % 16, globalPos.y, globalPos.z % 16), type);
}

void World::PlayerSetBlock(glm::ivec3 globalPos, BlockType type) {
	glm::ivec2 chunkPos(globalPos.x / 16, globalPos.z / 16);
	Chunk* chunk = _chunks[chunkPos];
	if (chunk)
		chunk->PlayerSetBlock(glm::ivec3(globalPos.x % 16, globalPos.y, globalPos.z % 16), type);
}