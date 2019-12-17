#include "Types.h"
#include "World/World.h"
#include "World/Shard.h"
#include "World/Chunk.h"
#include "Engine/Game.h"
#include "Generation/MapGeneration.h"
#include "Generation/BiomeDefine.h"

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
	// }
	World* w = _game->GetWorld();
	MapGeneration mp = *_game->GetGeneration();
	MapGeneration::StoredMapData block;

	for (int x = 0; x < 16; x++) {
		for (int z = 0; z < 16; z++) {
			
			block = mp.Generation(_position, glm::ivec2(x, z), MapGeneration::Basic); // first layer generation
			int elevation = glm::clamp((int)block.elevation, 0, 255); // max world height == 255
			int firstLayerBorder = 40 + block.elevation;
			for (int y = 0; y < firstLayerBorder; y++){
				w->SetBlock(glm::ivec3(_position.x * 16 + x, y, _position.y * 16 + z), block.firstBlockLayer);
			}

			block = mp.Generation(_position, glm::ivec2(x, z)); //second layer generation
			elevation = glm::clamp((int)block.elevation, 0, 255); // max world height == 255
			int lastLayerBorder = 60 + elevation - 1;
			for (int y = firstLayerBorder; y < lastLayerBorder; y++){
				w->SetBlock(glm::ivec3(_position.x * 16 + x, y, _position.y * 16 + z), block.firstBlockLayer);
			}
			w->SetBlock(glm::ivec3(_position.x * 16 + x, lastLayerBorder, _position.y * 16 + z), block.lastBlockLayer);
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
	if (pos.y < 0 || pos.y >= 256)
		return BlockType::Air;
	int y = pos.y / 16;
	return _shards[y]->GetBlock(glm::ivec3(pos.x, pos.y % 16, pos.z));
}

void Chunk::SetBlock(glm::ivec3 pos, BlockType type) {
	if (pos.y < 0 || pos.y >= 256)
		return;
	int y = pos.y / 16;
	_shards[y]->SetBlock(glm::ivec3(pos.x, pos.y % 16, pos.z), type);
}

void Chunk::PlayerSetBlock(glm::ivec3 pos, BlockType type) {
	int y = pos.y / 16;
	_shards[y]->SetBlock(glm::ivec3(pos.x, pos.y % 16, pos.z), type);
	_shards[y]->UpdateGeometry();
}
