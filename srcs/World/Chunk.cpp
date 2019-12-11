#include "Types.h"
#include "World/World.h"
#include "World/Shard.h"
#include "Engine/Engine.h"

Chunk::Chunk(Game* game, glm::ivec2 pos) {
	_state = false;
	_game = game;
	_position = pos;
	for (int y = 0; y < 16; y++) {
		_shards[y] = new Shard(game, glm::ivec3(pos.x, y, pos.y));
	}
};

Chunk::~Chunk() {
	for (int y = 0; y < 16; y++) {
		delete _shards[y];
	}
};

void Chunk::Generate() {
	// for (int y = 0; y < 16; y++) {
		// _shards[y]->Generate();
// 
	// }
	World* w = _game->GetWorld();
	MapGeneration* mp = _game->GetGeneration();
	std::unordered_map<glm::ivec2, StoredMapData*> umap = mp->Generation(16, 16, _position);
	int width = (_position.x != 0 ? (16 * _position.x - 16) : 0);
	int height = (_position.y != 0 ? (16 * _position.y - 16) : 0);

	
	// int h = rand() % 30;
	for (int x = 0; x < 16; x++) {
		for (int z = 0; z < 16; z++) {
				// std::cout << width + x << " X|Y " << height + z << std::endl;
				// std::cout << (*mp->umap)[glm::ivec2(width + x, height + z)] << " be" << std::endl;
			int h = (int)floorf((umap)[glm::ivec2(width + x, height + z)]->elevation) + 1;
			// int h = (int)floorf((*mp->umap)[glm::ivec2(width + x, height + z)]->elevation) + 1;
			for (int y = 0; y < 60; y++) {
				w->SetBlock(glm::ivec3(_position.x * 16 + x, y, _position.y * 16 + z), BlockType::Stone);
			}
			for (int y = 60; y < 60 + h; y++) {
				w->SetBlock(glm::ivec3(_position.x * 16 + x, y, _position.y * 16 + z), BlockType::Dirt);
			}
		}
	}
}

void Chunk::SetActive(bool state) {
	_state = state;
	for (int i = 0; i < 16; i++) {
		_shards[i]->SetActive(state);
	}
}

BlockType Chunk::GetBlock(glm::ivec3 pos) {
	int y = pos.y / 16;
	if (pos.y < 0 || pos.y >= 256)
		return BlockType::Air;
	return _shards[y]->GetBlock(glm::ivec3(pos.x, pos.y % 16, pos.z));
}

void Chunk::SetBlock(glm::ivec3 pos, BlockType type) {
	int y = pos.y / 16;
	_shards[y]->SetBlock(glm::ivec3(pos.x, pos.y % 16, pos.z), type);
}

void Chunk::PlayerSetBlock(glm::ivec3 pos, BlockType type) {
	int y = pos.y / 16;
	_shards[y]->SetBlock(glm::ivec3(pos.x, pos.y % 16, pos.z), type);
	_shards[y]->UpdateGeometry();
}
