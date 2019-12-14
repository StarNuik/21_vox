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
			int firstLayerBorder = 40 + block.elevation;
			for (int y = 0; y < firstLayerBorder; y++){
				w->SetBlock(glm::ivec3(_position.x * 16 + x, y, _position.y * 16 + z), block.firstBlockLayer);
			}

			block = mp.Generation(_position, glm::ivec2(x, z)); //second layer generation
			int lastLayerBorder = firstLayerBorder + 40 + block.elevation - 1;
			for (int y = firstLayerBorder; y < lastLayerBorder; y++){
				w->SetBlock(glm::ivec3(_position.x * 16 + x, y, _position.y * 16 + z), block.firstBlockLayer);
			}
			w->SetBlock(glm::ivec3(_position.x * 16 + x, lastLayerBorder, _position.y * 16 + z), block.lastBlockLayer);
			// high.elevation = mp.Generation(MapGeneration::Basic, _position, glm::ivec2(x, z)).elevation;
			// int h = (int)floorf(high.elevation);
			// int firstLayerBorder = h + 40;
			//  h = glm::clamp(h, 0, 256);
			// for (int y = 0; y < firstLayerBorder; y++) {
			// 	w->SetBlock(glm::ivec3(_position.x * 16 + x, y, _position.y * 16 + z), BlockType::Stone);
			// }

			// // if (test == 100)
			// // 	for (int y = firstLayerBorder; y < test; y++)
			// // 		w->SetBlock(glm::ivec3(_position.x * 16 + x, y, _position.y * 16 + z), BlockType::Planks);
			
			// if (high.biom == OCEAN)
			// {;
			// 	h += 10;
			// 	h = glm::clamp(h, 0, 256);
			// 	for (int y = firstLayerBorder; y < 60; y++)
			// 		w->SetBlock(glm::ivec3(_position.x * 16 + x, y, _position.y * 16 + z), BlockType::OreCoal);
			// }
			// else if (high.biom == BEACH)
			// {
			// 	high.elevation = mp.Generation(MapGeneration::Land, _position, glm::ivec2(x,z)).elevation;
			// 	h = (int)floorf(high.elevation);
			// 	h = glm::clamp(h, 0, 256);
			// 	for (int y = firstLayerBorder; y < 60 + h; y++)
			// 		w->SetBlock(glm::ivec3(_position.x * 16 + x, y, _position.y * 16 + z), BlockType::Sand);
			// }
			// else if (high.biom == GRASSLAND)
			// {
			// 	high.elevation = mp.Generation(MapGeneration::Land, _position, glm::ivec2(x, z)).elevation;
			// 	h = (int)floorf(high.elevation);
			// 	h = glm::clamp(h, 0, 256);
			// 	for (int y = firstLayerBorder; y < 60 + h; y++)
			// 		w->SetBlock(glm::ivec3(_position.x * 16 + x, y, _position.y * 16 + z), BlockType::Dirt);
			// 	w->SetBlock(glm::ivec3(_position.x * 16 + x, 60 + h - 1, _position.y * 16 + z), BlockType::Grass);
			// }
			// else if (high.biom == TAIGA)
			// {
			// 	high.elevation = mp.Generation(MapGeneration::HighLand, _position, glm::ivec2(x,z)).elevation;
			// 	h = (int)floorf(high.elevation);
			// 	h = glm::clamp(h, 0, 256);
			// 	for (int y = firstLayerBorder; y < 60 + h; y++) {
			// 		if (y < 60 + h - 5)
			// 			w->SetBlock(glm::ivec3(_position.x * 16 + x, y, _position.y * 16 + z), BlockType::Stone);
			// 		else
			// 			w->SetBlock(glm::ivec3(_position.x * 16 + x, y, _position.y * 16 + z), BlockType::Dirt);
			// 	}
			// }
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
