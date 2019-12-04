#include "World/World.h"

Shard::Shard(glm::ivec3) {
	_state = true;
	// Generate self here
}

Shard::~Shard() {
	if (_state) {
		// Remove model from renderer
	}
};

void Shard::SetActive(bool state) {
	_state = state;
	if (_state) {
		// Add model to renderer
	} else {
		// Remove model from renderer
	}
}

void Shard::UpdateGeometry() {
	// Remove model from renderer
	// Recalculate the model
	if (_state) {
		// Add model to renderer
	}
}

Block* Shard::GetBlock(glm::ivec3) {
	// Get correct block from block table
	return nullptr;
}

void Shard::SetBlock(glm::ivec3 pos, BLOCK_TYPE type) {
	_blocks[pos.x][pos.y][pos.z] = type;
}