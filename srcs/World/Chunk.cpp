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

Block* Chunk::GetBlock(glm::ivec3 pos) {
	int y = pos.y / 16;
	return _shards[y]->GetBlock(glm::ivec3(pos.x, pos.y % 16, pos.z));
}

void Chunk::SetBlock(glm::ivec3 pos, BlockType type) {
	int y = pos.y / 16;
	_shards[y]->SetBlock(glm::ivec3(pos.x, pos.y % 16, pos.z), type);
}