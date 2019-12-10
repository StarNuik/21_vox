#include "World/World.h"

Chunk::Chunk(Game* game, glm::ivec2 pos) {
	_state = true;
	for (int i = 0; i < 16; i++) {
		_shards[i] = new Shard(game, glm::ivec3(pos.x, i, pos.y));
	}
};

Chunk::~Chunk() {
	for (int i = 0; i < 16; i++) {
		delete _shards[i];
	}
};

void Chunk::SetActive(bool state) {
	_state = state;
	for (int i = 0; i < 16; i++) {
		Shard* shard = _shards[i];
		if (!shard)
			continue;
		shard->SetActive(state);
	}
}

BlockType Chunk::GetBlock(glm::ivec3 pos) {
	if (pos.y < 0 || pos.y > 16)
		return BlockType::Air;
	int y = pos.y / 16;
	return _shards[y]->GetBlock(glm::ivec3(pos.x, pos.y % 16, pos.z));
}

void Chunk::SetBlock(glm::ivec3 pos, BlockType type) {
	int y = pos.y / 16;
	_shards[y]->SetBlock(glm::ivec3(pos.x, pos.y % 16, pos.z), type);
}

void Chunk::UpdateGeometry(glm::ivec2 chunkPos) {
	for (int i = 0; i < 16; i++) {
		Shard* shard = _shards[i];
		if (!shard)
			continue;
		shard->UpdateGeometry(glm::ivec3(chunkPos.x * 16, i * 16, chunkPos.y * 16));
	}
}